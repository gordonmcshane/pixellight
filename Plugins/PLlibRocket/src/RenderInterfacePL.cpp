/*********************************************************\
 *  File: RenderInterfacePL.cpp                          *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Texture/TextureManager.h>
#include "PLlibRocket/RenderInterfacePL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLlibRocket {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
RenderInterfacePL::~RenderInterfacePL()
{
}

/**
*  @brief
*    Returns the used renderer context
*/
RendererContext &RenderInterfacePL::GetRendererContext() const
{
	return *m_pRendererContext;
}


//[-------------------------------------------------------]
//[ Public virtual RenderInterfacePL functions            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Configures PixelLight's rendering system for rendering Rocket
*/
bool RenderInterfacePL::ConfigureRenderSystem()
{
	// Get the used renderer
	Renderer &cRenderer = GetRendererContext().GetRenderer();

	// Reset all render states to default
	cRenderer.GetRendererContext().GetEffectManager().Use();

	// No back face culling, please
	cRenderer.SetRenderState(RenderState::CullMode, Cull::None);

	// Disable depth-buffering; all of the geometry is already depth-sorted
	cRenderer.SetRenderState(RenderState::ZEnable,		false);	// Disable z buffer test
	cRenderer.SetRenderState(RenderState::ZWriteEnable,	false);	// Disable z buffer writing

	// Enable simple alpha blending (blend functions are already correct by default)
	cRenderer.SetRenderState(RenderState::BlendEnable, true);

	// Enable writing to all four channels
	cRenderer.SetColorMask();

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual Rocket::Core::RenderInterface functions ]
//[-------------------------------------------------------]
void RenderInterfacePL::RenderGeometry(Rocket::Core::Vertex* ROCKET_UNUSED(vertices), int ROCKET_UNUSED(num_vertices), int* ROCKET_UNUSED(indices), int ROCKET_UNUSED(num_indices), Rocket::Core::TextureHandle ROCKET_UNUSED(texture), const Rocket::Core::Vector2f& ROCKET_UNUSED(translation))
{
	// We've chosen to not support non-compiled geometry in the PixelLight renderer
}

Rocket::Core::CompiledGeometryHandle RenderInterfacePL::CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture)
{
	// Create the compiled geometry instance
	RocketPLCompiledGeometry *pRocketPLCompiledGeometry = new RocketPLCompiledGeometry();

	{ // Create the vertex buffer
		VertexBuffer *pVertexBuffer = pRocketPLCompiledGeometry->pVertexBuffer = m_pRendererContext->GetRenderer().CreateVertexBuffer();

		// Setup the vertex attribute layout
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float2);
		pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Color,    0, VertexBuffer::RGBA);

		// Allocate the vertex buffer
		pVertexBuffer->Allocate(num_vertices, Usage::Static);

		// Setup the vertex buffer data
		if (pVertexBuffer->Lock(Lock::WriteOnly)) {
			// Loop through all given vertices
			Rocket::Core::Vertex *pRocketVertex = vertices;
			for (int i=0; i<num_vertices; i++, pRocketVertex++) {
				// Position
				float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::Position));
				pfVertex[0] = pRocketVertex->position.x;
				pfVertex[1] = pRocketVertex->position.y;

				// Texture coordinate
				pfVertex = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::TexCoord));
				pfVertex[0] = pRocketVertex->tex_coord.x;
				pfVertex[1] = pRocketVertex->tex_coord.y;

				// Color
				pVertexBuffer->SetColor(i, Color4(pRocketVertex->colour.red, pRocketVertex->colour.green, pRocketVertex->colour.blue, pRocketVertex->colour.alpha));
			}

			// Unlock the vertex buffer
			pVertexBuffer->Unlock();
		}
	}

	{ // Create the index buffer
		IndexBuffer *pIndexBuffer = pRocketPLCompiledGeometry->pIndexBuffer = m_pRendererContext->GetRenderer().CreateIndexBuffer();

		// Allocate the index buffer
		pIndexBuffer->SetElementTypeByMaximumIndex(num_vertices);
		pIndexBuffer->Allocate(num_indices, Usage::Static);

		// Setup the index buffer data
		if (pIndexBuffer->Lock(Lock::WriteOnly)) {
			// Copy over the indices
			for (int i=0; i<num_indices; i++)
				pIndexBuffer->SetData(i, indices[i]);

			// Unlock the index buffer
			pIndexBuffer->Unlock();
		}
	}

	// Set the texture handler
	pRocketPLCompiledGeometry->pTextureHandler = texture ? reinterpret_cast<TextureHandler*>(texture) : nullptr;

	// Return the created compiled geometry instance
	return reinterpret_cast<Rocket::Core::CompiledGeometryHandle>(pRocketPLCompiledGeometry);
}

void RenderInterfacePL::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
	// Get the compiled geometry instance
	RocketPLCompiledGeometry *pRocketPLCompiledGeometry = reinterpret_cast<RocketPLCompiledGeometry*>(geometry);

	// Destroy the stuff
	delete pRocketPLCompiledGeometry->pVertexBuffer;
	delete pRocketPLCompiledGeometry->pIndexBuffer;
	delete pRocketPLCompiledGeometry;
}

void RenderInterfacePL::EnableScissorRegion(bool enable)
{
	m_bScissorRegion = enable;
	m_pRendererContext->GetRenderer().SetRenderState(RenderState::ScissorTestEnable, m_bScissorRegion);
	if (m_bScissorRegion) {
		if (!m_bScissorRegionInitialized)
			m_cScissorRegion = m_pRendererContext->GetRenderer().GetViewport();
		m_pRendererContext->GetRenderer().SetScissorRect(&m_cScissorRegion);
	}
}

void RenderInterfacePL::SetScissorRegion(int x, int y, int width, int height)
{
	m_cScissorRegion.vMin.SetXY(static_cast<float>(x), static_cast<float>(y));
	m_cScissorRegion.vMax.SetXY(static_cast<float>(x + width), static_cast<float>(y + height));
	if (m_bScissorRegion)
		m_pRendererContext->GetRenderer().SetScissorRect(&m_cScissorRegion);
}

bool RenderInterfacePL::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	// Load the texture
	Texture *pTexture = m_pRendererContext->GetTextureManager().LoadResource(String::FromUTF8(source.CString()));
	if (pTexture && pTexture->GetTextureBuffer() && pTexture->GetTextureBuffer()->GetType() == TextureBuffer::TypeTextureBuffer2D) {
		{ // Get the texture dimension
			TextureBuffer2D *pTextureBuffer2D = static_cast<TextureBuffer2D*>(pTexture->GetTextureBuffer());
			texture_dimensions.x = pTextureBuffer2D->GetSize().x;
			texture_dimensions.y = pTextureBuffer2D->GetSize().y;
		}

		// Create a PL texture handler and return it
		TextureHandler *pTextureHandler = new TextureHandler();
		pTextureHandler->SetResource(pTexture);
		texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(pTextureHandler);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

bool RenderInterfacePL::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	// Generate an (hopefully) unique texture name
	static uint32 nUniqueTextureID = 0;
	static const String sTextureID = "PLlibRocket::RenderInterfacePL::GenerateTexture_";
	const String sTextureName = sTextureID + (nUniqueTextureID++);

	// Get the texture manager instance
	TextureManager &cTextureManager = m_pRendererContext->GetTextureManager();

	// Is there already such a texture?
	Texture *pTexture = cTextureManager.GetByName(sTextureName);
	if (!pTexture) {
		// Create the texture right now!
		pTexture = cTextureManager.Create(sTextureName);
		if (pTexture) {
			// Create the image by just sharing the image data provided by libRocket
			Image cImage = Image::CreateImageAndShareData(DataByte, ColorRGBA, Vector3i(source_dimensions.x, source_dimensions.y, 1), CompressionNone, const_cast<Rocket::Core::byte*>(source));	// In here, the image will not manipulate the provided data, so the const cast is ok

			// Create the 2D texture buffer
			pTexture->SetTextureBuffer(reinterpret_cast<TextureBuffer*>(m_pRendererContext->GetRenderer().CreateTextureBuffer2D(cImage)));
		}
	}

	// Create a PL texture handler and return it
	if (pTexture) {
		TextureHandler *pTextureHandler = new TextureHandler();
		pTextureHandler->SetResource(pTexture);
		texture_handle = reinterpret_cast<Rocket::Core::TextureHandle>(pTextureHandler);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

void RenderInterfacePL::ReleaseTexture(Rocket::Core::TextureHandle texture)
{
	// Get PL texture handler
	TextureHandler *pTextureHandler = reinterpret_cast<TextureHandler*>(texture);
	if (pTextureHandler) {
		// Destroy the texture
		Texture *pTexture = pTextureHandler->GetResource();
		if (pTexture)
			pTexture->Delete();

		// Destroy the PL texture handler
		delete pTextureHandler;
	}
}

float RenderInterfacePL::GetHorizontalTexelOffset()
{
	return -m_pRendererContext->GetRenderer().GetTexelToPixelOffset().x;
}

float RenderInterfacePL::GetVerticalTexelOffset()
{
	return -m_pRendererContext->GetRenderer().GetTexelToPixelOffset().y;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
RenderInterfacePL::RenderInterfacePL() :
	m_pRendererContext(nullptr),
	m_bScissorRegion(false)
{
	// No implementation because the default constructor is never used
}

/**
*  @brief
*    Copy constructor
*/
RenderInterfacePL::RenderInterfacePL(const RenderInterfacePL &cSource) :
	m_pRendererContext(nullptr),
	m_bScissorRegion(false)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Constructor
*/
RenderInterfacePL::RenderInterfacePL(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext),
	m_bScissorRegionInitialized(false),
	m_bScissorRegion(false)
{
}

/**
*  @brief
*    Copy operator
*/
RenderInterfacePL &RenderInterfacePL::operator =(const RenderInterfacePL &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLlibRocket
