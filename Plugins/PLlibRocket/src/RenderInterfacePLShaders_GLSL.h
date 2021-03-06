/*********************************************************\
 *  File: RenderInterfacePLShaders_GLSL.h                *
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


// GLSL (OpenGL 2.0 ("#version 110") and OpenGL ES 2.0 ("#version 100")) vertex shader source code, "#version" is added by hand (the depreciated "attribute" and "varying" instead of "in" and "out" are used because we want to stay compatible to OpenGL ES 2.0)
static const PLCore::String sVertexShaderSourceCodeGLSL = "\
// Attributes\n\
attribute mediump vec2 VertexPosition;		// Window space vertex position input\n\
attribute lowp    vec2 VertexTexCoord;		// Vertex texture coordinate input\n\
varying   lowp    vec2 VertexTexCoordVS;	// Vertex texture coordinate output\n\
attribute lowp    vec4 VertexColor;			// Vertex color input\n\
varying   lowp    vec4 VertexColorVS;		// Vertex color output\n\
\n\
// Uniforms\n\
uniform mediump vec2 Translation;	// Window space translation\n\
uniform mediump vec2 WindowSizeInv;	// 1/(window size)\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	gl_Position    = vec4(VertexPosition + Translation, 0.0, 1.0);\n\
	gl_Position.xy *= WindowSizeInv;		// Change into 0..1 range\n\
	gl_Position.y   = 1.0 - gl_Position.y;	// Flip y axis\n\
	gl_Position.xy  = gl_Position.xy*2.0 - 1.0;	// Change into -1..1 range\n\
\n\
	// Pass through the vertex texture coordinate\n\
	VertexTexCoordVS = VertexTexCoord;\n\
\n\
	// Pass through the vertex color\n\
	VertexColorVS = VertexColor;\n\
}";


// GLSL (OpenGL 2.0 ("#version 110") and OpenGL ES 2.0 ("#version 100")) fragment shader source code, "#version" is added by hand (the depreciated "attribute" and "varying" instead of "in" and "out" are used because we want to stay compatible to OpenGL ES 2.0)
static const PLCore::String sFragmentShaderSourceCodeGLSL = "\
// Attributes\n\
varying lowp vec2 VertexTexCoordVS;	// Interpolated vertex texture coordinate input from vertex shader\n\
varying lowp vec4 VertexColorVS;	// Interpolated vertex color input from vertex shader\n\
\n\
// Uniforms\n\
uniform lowp sampler2D TextureMap;	// Texture map\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Fragment color = fetched interpolated texel color\n\
	gl_FragColor = texture2D(TextureMap, VertexTexCoordVS)*VertexColorVS;\n\
\n\
	// Perform an alpha test to discard fragments - Reject pixels with an alpha of <=0\n\
	if (gl_FragColor.a <= 0.0)\n\
		discard; // Throw the fragment away and don't draw it!\n\
}";
