/*********************************************************\
 *  File: PLSceneContainer.cpp                           *
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
#include <PLCore/Xml/Xml.h>
#include <IGame/IGame.h>
#include <modstack.h>
#include "PL3dsMaxSceneExport/PLLog.h"
#include "PL3dsMaxSceneExport/PLTools.h"
#include "PL3dsMaxSceneExport/PLScene.h"
#include "PL3dsMaxSceneExport/PLSceneCell.h"
#include "PL3dsMaxSceneExport/PLSceneCamera.h"
#include "PL3dsMaxSceneExport/PLSceneLight.h"
#include "PL3dsMaxSceneExport/PLSceneObject.h"
#include "PL3dsMaxSceneExport/PLSceneCellPortal.h"
#include "PL3dsMaxSceneExport/PLSceneAntiPortal.h"
#include "PL3dsMaxSceneExport/PLSceneSpline.h"
#include "PL3dsMaxSceneExport/PLSceneHelper.h"
#include "PL3dsMaxSceneExport/PLSceneUnknown.h"
#include "PL3dsMaxSceneExport/PLSceneContainer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns a scene node
*/
PLSceneNode *PLSceneContainer::Get(const String &sName)
{
	// Name not empty?
	if (!sName.GetLength() || sName.GetASCII()[0] == '\0')
		return nullptr;

	// Is '.' the first character?
	if (sName.GetASCII()[0] == '.')
		return nullptr; // Error!

	// Is the name 'This' at the beginning?
	if (sName.Compare("This", 0, 4)) {
		// Return this scene node?
		if (sName.GetASCII()[4] == '\0')
			return this;

		// Did a '.' follow?
		if (sName.GetASCII()[4] == '.')
			return Get(sName.GetSubstring(5));
	}

	// Is the name 'Root' at the beginning?
	if (sName.Compare("Root", 0, 4)) {
		// Return root scene node?
		if (sName.GetASCII()[4] == '\0')
			return &GetScene();

		// Did a '.' follow?
		if (sName.GetASCII()[4] == '.')
			return GetScene().Get(sName.GetSubstring(5));
	}

	// 'Parent' at the beginning?
	if (sName.Compare("Parent", 0, 6)) {
		// Return parent scene node?
		if (sName.GetASCII()[6] == '\0')
			return GetContainer();

		// Did a '.' follow?
		if (sName.GetASCII()[6] == '.') {
			// Is there a parent container?
			return GetContainer() ? GetContainer()->Get(sName.GetSubstring(7)) : nullptr;
		}
	}

	// Check for the character '.' within the name
	const char *pszNameT = sName.GetASCII();
	for (unsigned int i=0; *pszNameT!='\0'; i++) {
		if (*pszNameT == '.') {
			// Get the name of the scene container
			String sContainerName = sName.GetSubstring(0, i);

			// Get the scene node and check whether it is a scene container
			PLSceneNode *pSceneNode = Get(sContainerName);
			if (!pSceneNode || (pSceneNode->GetType() != TypeContainer && pSceneNode->GetType() != TypeCell))
				return nullptr; // Error!

			// Change 'into' this scene container
			return static_cast<PLSceneContainer*>(pSceneNode)->Get(sName.GetSubstring(i+1));
		}
		pszNameT++;
	}

	// Search for a scene node with this name
	String sNameLower = sName;
	sNameLower.ToLower(); // Do ONLY use lower case, else the hashing will NOT return the same values!
	std::map<String, PLSceneNode*>::iterator pIterator = m_mapNodes.find(sNameLower);
	return (pIterator == m_mapNodes.end()) ? nullptr : pIterator->second;
}

/**
*  @brief
*    Returns the center of this container
*/
const Point3 &PLSceneContainer::GetWorldSpaceCenter() const
{
	return m_vCenter;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PLSceneContainer::PLSceneContainer(PLSceneContainer *pContainer, const String &sName, EType nType) :
	PLSceneNode(pContainer, nullptr, sName, nType, ""),
	m_bFixedCenter(false),
	m_vCenter(0.0f, 0.0f, 0.0f)
{
	// Initialize the statistics
	memset(&m_sStatistics, 0, sizeof(SStatistics));

	// Set cast shadow flag - if this is not done, nodes within this container will never have active shadows
	AddFlag("CastShadow");
}

/**
*  @brief
*    Destructor
*/
PLSceneContainer::~PLSceneContainer()
{
	// Destroy all scene nodes within this container
	for (std::vector<PLSceneNode*>::size_type i=0; i<m_lstSceneNodes.size(); i++) {
		PLSceneNode *pSceneNode = m_lstSceneNodes[i];
		if (pSceneNode)
			delete pSceneNode;
	}
}

/**
*  @brief
*    Returns a cell with the given name
*/
PLSceneCell *PLSceneContainer::GetCell(const String &sName, IGameNode &cIGameNode)
{
	// Check the given name
	if (!sName.GetLength())
		return nullptr; // Error!

	// First at all, IS there already a cell with this name?
	String sNameLower = sName;
	sNameLower.ToLower(); // Do ONLY use lower case, else the hashing will NOT return the same values!
	std::map<String, PLSceneCell*>::iterator pIterator = m_mapCells.find(sNameLower);
	if (pIterator != m_mapCells.end())
		return pIterator->second;

	// Nope, let's create a cell with this name
	g_pLog->LogFLine(PLLog::Scene, "Cell '%s' is created by the 3ds Max node '%s'", sName.GetASCII(), cIGameNode.GetName());
	PLSceneCell *pCell = new PLSceneCell(*this, sName);
	if (pCell) {
		// Register the new cell
		m_lstSceneNodes.push_back(pCell);
		m_mapCells.insert(std::make_pair(sNameLower, pCell));
		m_mapNodes.insert(std::make_pair(sNameLower, pCell));

		// Update the statistics
		m_sStatistics.nNumOfCells++;
		GetScene().m_sSceneStatistics.nNumOfCells++;
	}

	// Return the created cell
	return pCell;
}

/**
*  @brief
*    Checks and corrects names
*/
void PLSceneContainer::CheckAndCorrectName(String &sName, const TCHAR szMaxNode[], const char szType[]) const
{
	// Check for empty name
	if (!sName.GetLength()) {
		g_pLog->LogFLine(PLLog::Warning, "'%s': There's no %s name!", szMaxNode, szType);

		// Set a dummy name
		sName = "?";

	// Check for spaces/tabs within the name
	} else if (sName.IndexOf(" ") >= 0 || sName.IndexOf("	") >= 0) {
		if (g_SEOptions.bRemoveSpaces) {
			const String sOldName = sName;

			// Remove all spaces
			int i = sName.IndexOf(" ");
			while (i >= 0) {
				sName.Delete(i, 1);
				i = sName.IndexOf(" ");
			}

			// Remove all tabs
			i = sName.IndexOf("	");
			while (i >= 0) {
				sName.Delete(i, 1);
				i = sName.IndexOf("	");
			}

			// Log message
			g_pLog->LogFLine(PLLog::Warning, "'%s': There are spaces/tabs within the %s name '%s', this is NOT recommended! Changed name into '%s' automatically.", szMaxNode, szType, sOldName.GetASCII(), sName.GetASCII());
		} else {
			g_pLog->LogFLine(PLLog::Warning, "'%s': There are spaces/tabs within the %s name '%s', this is NOT recommended!", szMaxNode, szType, sName.GetASCII());
		}
	}
}

/**
*  @brief
*    Writes the scene container into a file
*/
void PLSceneContainer::WriteToFile(XmlElement &cSceneElement, const String &sApplicationDrive, const String &sApplicationDir, const String &sClass)
{
	// Add scene node
	XmlElement *pNodeElement = new XmlElement("Container");
	pNodeElement->SetAttribute("Class", sClass);
	pNodeElement->SetAttribute("Name",  GetName());

	// [TODO] Currently the bounding boxes are not correct
/*	if (!PLTools::IsEmpty(m_cBoundingBox)) {
		const Point3 vMin = m_cBoundingBox.pmin-m_vCenter;
		const Point3 vMax = m_cBoundingBox.pmax-m_vCenter;
		pNodeElement->SetAttribute("AABBMin", String::Format("%f %f %f", vMin.x, vMin.y, vMin.z));
		pNodeElement->SetAttribute("AABBMax", String::Format("%f %f %f", vMax.x, vMax.y, vMax.z));
	}
*/

	// Write position, rotation, scale, bounding box and flags
	WriteToFilePosRotScaleBoxFlags(*pNodeElement);

	// Write flexible variables
	WriteVariables(*pNodeElement);

	// Write modifiers
	WriteModifiers(*pNodeElement, sApplicationDrive, sApplicationDir);

	// Are there any modifiers or scene nodes?
	if (m_lstSceneNodes.size()) {
		// Write down the child nodes
		for (std::vector<PLSceneNode*>::size_type i=0; i<m_lstSceneNodes.size(); i++) {
			PLSceneNode *pSceneNode = m_lstSceneNodes[i];
			if (pSceneNode)
				pSceneNode->WriteToFile(*pNodeElement, sApplicationDrive, sApplicationDir);
		}
	}

	// Link node element
	cSceneElement.LinkEndChild(*pNodeElement);
}


//[-------------------------------------------------------]
//[ Protected virtual functions                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a 3ds Max node to the scene
*/
bool PLSceneContainer::AddIGameNode(IGameNode &cIGameNode)
{
	// Unknown node type by default
	EType nType = TypeUnknown;

	// Is there a '.' within the node name? If yes, replace it by '-'.
	String sName = cIGameNode.GetName();
	int nIndex = sName.IndexOf(".");
	if (nIndex >= 0) {
		g_pLog->LogFLine(PLLog::Warning, "Node name '%s' has '.' within it, '.' is replaced by '-' automatically", cIGameNode.GetName());
		char *pszName = const_cast<char*>(sName.GetASCII()) + nIndex;
		while (*pszName != '\0') {
			if (*pszName == '.')
				*pszName = '-';
			pszName++;
		}
	}
	const char *pszName = sName.GetASCII();

	// Look for 'cell_' (cell_<cell name>_<node name> or cell_<cell name>_<mesh name>_<instance name>)
	String sSceneCellName, sTargetSceneCellName, sSceneNodeName, sMeshName;
	if (!_strnicmp(pszName, "cell_", 5)) {
		// Get the name of the cell
		const char *pszNameT = pszName += 5;

		// Check for '\0'
		if (*pszNameT == '\0') {
			g_pLog->LogFLine(PLLog::Error, "Node name '%s' does not follow the name convention cell_<cell name>_<node name> or cell_<cell name>_<mesh name>_<instance name>. <cell name> is missing!", cIGameNode.GetName());
			sSceneCellName = "?";
			sMeshName	   = "?";
			sSceneNodeName = "?";
		} else {
			// Read the cell name
			while (*pszNameT != '_' && *pszNameT != '\0')
				pszNameT++;
			sSceneCellName.Insert(pszName, 0, pszNameT-pszName);

			// Check for '_'
			if (*pszNameT != '_') {
				g_pLog->LogFLine(PLLog::Error, "Node name '%s' does not follow the name convention cell_<cell name>_<node name> or cell_<cell name>_<mesh name>_<instance name>. <mesh name>/<instance name> is missing!", cIGameNode.GetName());
				sMeshName	   = "?";
				sSceneNodeName = "?";
			} else {
				// Skip '_'
				pszNameT++; 

				// Get the name of the node/mesh
				pszName = pszNameT;
				while (*pszNameT != '_' && *pszNameT != '\0')
					pszNameT++;
				sMeshName.Insert(pszName, 0, pszNameT-pszName);

				// Check for '_'
				if (*pszNameT != '_') {
					// Check for spaces within the mesh name
					CheckAndCorrectName(sMeshName, cIGameNode.GetName(), "mesh");

					// cell_<cell name>_<node name> is used so node name = mesh name
					sSceneNodeName = sMeshName;

					// Construct a 'save' mesh name later if the final scene node name is known...
					sMeshName = "";
				} else {
					// Skip '_'
					pszNameT++; 

					// Get the name of the scene node
					sSceneNodeName = sMeshName+"_";
					pszName = pszNameT;
					while (*pszNameT != '_' && *pszNameT != '\0')
						pszNameT++;
					sSceneNodeName.Insert(pszName, sSceneNodeName.GetLength(), pszNameT-pszName);

					// Is there an instance name? - No log hint because people find this behavior annoying...
				//	if (!(pszNameT-pszName))
				//		g_pLog->LogFLine(PLLog::Hint, "Node name '%s' does not follow the name convention cell_<cell name>_<mesh name>_<instance name>. <instance name> is missing!", cIGameNode.GetName());

					// Check for spaces and tabs within the mesh and node names
					CheckAndCorrectName(sMeshName,      cIGameNode.GetName(), "mesh");
					CheckAndCorrectName(sSceneNodeName, cIGameNode.GetName(), "node");
				}
			}
		}

		// Check for spaces/tabs within the cell name
		CheckAndCorrectName(sSceneCellName, cIGameNode.GetName(), "cell");

	// Look for 'portal_' (portal_<from cell>_<to cell>)
	} else if (!_strnicmp(pszName, "portal_", 7)) {
		// Get the name of the cell, the cell-portal is a scene node of this cell :)
		const char *pszNameT = pszName += 7;

		// Check for '\0'
		if (*pszNameT == '\0') {
			g_pLog->LogFLine(PLLog::Error, "Node name '%s' does not follow the name convention portal_<from cell>_<to cell>. <from cell> is missing!", cIGameNode.GetName());
			sSceneCellName       = "?";
			sTargetSceneCellName = "?";
		} else {
			// Read the cell name
			while (*pszNameT != '_' && *pszNameT != '\0')
				pszNameT++;
			sSceneCellName.Insert(pszName, 0, pszNameT-pszName);

			// Check for '_'
			if (*pszNameT != '_') {
				g_pLog->LogFLine(PLLog::Error, "Node name '%s' does not follow the name convention portal_<from cell>_<to cell>. <to cell> is missing!", cIGameNode.GetName());
				sTargetSceneCellName = "?";
			} else {
				// Skip '_'
				pszNameT++; 

				// Get the name of the target cell
				pszName = pszNameT;
				while (*pszNameT != '_' && *pszNameT != '\0')
					pszNameT++;
				sTargetSceneCellName.Insert(pszName, 0, pszNameT-pszName);
			}
		}

		// Check for spaces/tabs within the names
		CheckAndCorrectName(sSceneCellName,       cIGameNode.GetName(), "cell");
		CheckAndCorrectName(sTargetSceneCellName, cIGameNode.GetName(), "target cell");

		// Get the name of the scene node
		sSceneNodeName = "CellPortalTo_";
		sSceneNodeName += sTargetSceneCellName;

		// Get the 'real' target cell name - in our case, this cell MUST be within the parent container...
		sTargetSceneCellName = "Parent." + sTargetSceneCellName;

		// Set the correct scene node type
		nType = TypeCellPortal;

	// Look for 'antiportal_' (antiportal_<name>)
	} else if (!_strnicmp(pszName, "antiportal_", 11)) {
		// Get the name of the anti-portal)
		sSceneNodeName = String("AntiPortal_") + (pszName += 11);

		// Set the correct scene node type
		nType = TypeAntiPortal;

	// ...
	} else {
		// No log hint because people find this behavior annoying...
	//	g_pLog->LogFLine(PLLog::Hint, "Node name '%s' does not follow the name convention cell_<cell name>_<node name> or cell_<cell name>_<mesh name>_<instance name>. Node is added to the scene root.", cIGameNode.GetName());

		// Set scene node name
		sSceneNodeName = pszName;
	}

	// Check whether the scene cell and node names are valid
	if (sSceneCellName.CompareNoCase("This")) {
		sSceneCellName += '-';
		g_pLog->LogFLine(PLLog::Warning, "'This' is NOT allowed as node name. (3ds Max node '%s') Name is changed into '%s' ", cIGameNode.GetName(), sSceneCellName.GetASCII());
	}
	if (sSceneCellName.CompareNoCase("Root")) {
		sSceneCellName += '-';
		g_pLog->LogFLine(PLLog::Warning, "'Root' is NOT allowed as node name. (3ds Max node '%s') Name is changed into '%s' ", cIGameNode.GetName(), sSceneCellName.GetASCII());
	}
	if (sSceneCellName.CompareNoCase("Parent")) {
		sSceneCellName += '-';
		g_pLog->LogFLine(PLLog::Warning, "'Parent' is NOT allowed as node name. (3ds Max node '%s') Name is changed into '%s' ", cIGameNode.GetName(), sSceneCellName.GetASCII());
	}
	if (sSceneNodeName.CompareNoCase("This")) {
		sSceneNodeName += '-';
		g_pLog->LogFLine(PLLog::Warning, "'This' is NOT allowed as node name. (3ds Max node '%s') Name is changed into '%s' ", cIGameNode.GetName(), sSceneNodeName.GetASCII());
	}
	if (sSceneNodeName.CompareNoCase("Root")) {
		sSceneNodeName += '-';
		g_pLog->LogFLine(PLLog::Warning, "'Root' is NOT allowed as node name. (3ds Max node '%s') Name is changed into '%s' ", cIGameNode.GetName(), sSceneNodeName.GetASCII());
	}
	if (sSceneNodeName.CompareNoCase("Parent")) {
		sSceneNodeName += '-';
		g_pLog->LogFLine(PLLog::Warning, "'Root' is NOT allowed as node name. (3ds Max node '%s') Name is changed into '%s' ", cIGameNode.GetName(), sSceneNodeName.GetASCII());
	}

	// Get cell, this scene node is in
	PLSceneCell *pCell = GetCell(sSceneCellName, cIGameNode);

	// The exporter isn't case sensitive, but compare the 'real' cell names just for sure :)
	if (pCell && pCell->GetName() != sSceneCellName)
		g_pLog->LogFLine(PLLog::Warning, "Node '%s' is within the cell '%s', but '%s' was written -> It's recommended to take care of lower/upper case!", cIGameNode.GetName(), pCell->GetName().GetASCII(), sSceneCellName.GetASCII());

	// Get the container the new scene node is created in. If no cell was found, create the scene node
	// within THIS container.
	PLSceneContainer *pContainer = pCell ? pCell : this;

	// Check whether there's already an scene node with this name, if so, rename it and write a warning
	// into the log
	String sSceneNodeNameLower = sSceneNodeName;
	sSceneNodeNameLower.ToLower(); // Do ONLY use lower case, else the hashing will NOT return the same values!
	std::map<String, PLSceneNode*>::iterator pIterator = pContainer->m_mapNodes.find(sSceneNodeNameLower);
	if (pIterator != pContainer->m_mapNodes.end()) {
		String sNewName;
		int nConflictIndex = 1;

		// Find an unused scene node name
		do {
			sNewName = sSceneNodeName;
			sNewName += "_Conflict_";
			sNewName += PLTools::ToString(nConflictIndex);
			nConflictIndex++;
			sSceneNodeNameLower = sNewName;
			sSceneNodeNameLower.ToLower(); // Do ONLY use lower case, else the hashing will NOT return the same values!
			pIterator = pContainer->m_mapNodes.find(sSceneNodeNameLower);
		} while (pIterator != pContainer->m_mapNodes.end());

		// Write a log message
		g_pLog->LogFLine(PLLog::Warning, "'%s': There's already a scene node with the name '%s' within the container '%s' -> Changed name into '%s'",
			cIGameNode.GetName(), sSceneNodeName.GetASCII(), sSceneCellName.GetASCII(), sNewName.GetASCII());

		// Set the new name
		sSceneNodeName = sNewName;
	}

	// Construct a 'save' mesh name...
	if (!sMeshName.GetLength())
		sMeshName = sSceneCellName.GetLength() ? sSceneCellName + '_' + sSceneNodeName : sSceneNodeName;

	// Are there any children? If yes, we need to create a container for this node containing THIS node AND the children...
	if (cIGameNode.GetChildCount()) {
		PLSceneContainer *pNewContainer = new PLSceneContainer(pContainer, sSceneNodeName);

		// Register the new scene node
		pContainer->m_lstSceneNodes.push_back(pNewContainer);
		sSceneNodeNameLower = sSceneNodeName;
		sSceneNodeNameLower.ToLower(); // Do ONLY use lower case, else the hashing will NOT return the same values!
		pContainer->m_mapNodes.insert(std::make_pair(sSceneNodeNameLower, pNewContainer));

		// The new container becomes the current container
		pContainer = pNewContainer;

		// [TODO] Update the statistics
		// pContainer->m_sStatistics.nNumOfUnknown++;
		// GetScene().m_sSceneStatistics.nNumOfUnknown++;
	}

	// Create the scene node...
	PLSceneNode *pSceneNode = nullptr;
	INode *pMaxNode = cIGameNode.GetMaxNode();
	if (pMaxNode) {
		Object *pMaxObject = pMaxNode->GetObjectRef();
		if (pMaxObject) {
			// Get 'real' 3ds Max object (we really need to do this)
			while (pMaxObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
				pMaxObject = static_cast<IDerivedObject*>(pMaxObject)->GetObjRef();

			// Check the type of the object
			switch (pMaxObject->SuperClassID()) {
				case LIGHT_CLASS_ID:
					// Create the scene node
					pSceneNode = new PLSceneLight(*pContainer, cIGameNode, sSceneNodeName);

					// Update the statistics
					pContainer->m_sStatistics.nNumOfLights++;
					GetScene().m_sSceneStatistics.nNumOfLights++;
					break;

				case GEOMOBJECT_CLASS_ID:
					// Is this a cell-portal?
					if (nType == TypeCellPortal) {
						// Create the scene node
						pSceneNode = new PLSceneCellPortal(*pContainer, cIGameNode, sSceneNodeName, sTargetSceneCellName);

						// Update the number of outgoing cell-portals
						if (pContainer->GetType() == TypeCell) {
							static_cast<PLSceneCell*>(pContainer)->m_lstOutgoingCellPortals.push_back(static_cast<PLSceneCellPortal*>(pSceneNode));

						// ?? There's something totally wrong! ??
						} else {
							g_pLog->LogFLine(PLLog::Warning, "'%s': This cell-portal is within the container '%s', but cell-portals should only be within cells!",
											 cIGameNode.GetName(), sTargetSceneCellName.GetASCII());
						}

						// Update the statistics
						pContainer->m_sStatistics.nNumOfCellPortals++;
						GetScene().m_sSceneStatistics.nNumOfCellPortals++;

					// Is this a anti-portal?
					} else if (nType == TypeAntiPortal) {
						// Create the scene node
						pSceneNode = new PLSceneAntiPortal(*pContainer, cIGameNode, sSceneNodeName);

						// Update the statistics
						pContainer->m_sStatistics.nNumOfAntiPortals++;
						GetScene().m_sSceneStatistics.nNumOfAntiPortals++;

					} else {
						// [TODO] Add rename to mesh node...
						// Create the scene node
						pSceneNode = new PLSceneObject(*pContainer, cIGameNode, sSceneNodeName, GetScene().AddMesh(cIGameNode, sMeshName));

						// Update the statistics
						pContainer->m_sStatistics.nNumOfObjects++;
						GetScene().m_sSceneStatistics.nNumOfObjects++;
					}
					break;

				case SHAPE_CLASS_ID:
					// Create the scene node
					pSceneNode = new PLSceneSpline(*pContainer, cIGameNode, sSceneNodeName);

					// We do not need to update the statistics...
					break;

				case CAMERA_CLASS_ID:
					// Create the scene node
					pSceneNode = new PLSceneCamera(*pContainer, cIGameNode, sSceneNodeName);

					// Update the statistics
					pContainer->m_sStatistics.nNumOfCameras++;
					GetScene().m_sSceneStatistics.nNumOfCameras++;
					break;

				case HELPER_CLASS_ID:
					// Create the scene node
					pSceneNode = new PLSceneHelper(*pContainer, cIGameNode, sSceneNodeName);

					// Update the statistics
					pContainer->m_sStatistics.nNumOfHelpers++;
					GetScene().m_sSceneStatistics.nNumOfHelpers++;
					break;

				default:
					// Create the scene node
					pSceneNode = new PLSceneUnknown(*pContainer, cIGameNode, sSceneNodeName);

					// Update the statistics
					pContainer->m_sStatistics.nNumOfUnknown++;
					GetScene().m_sSceneStatistics.nNumOfUnknown++;
					break;
			}
		}
	}

	// Valid scene node?
	if (pSceneNode) {
		// Are there any children? If yes, we need to create a container for this node containing THIS node AND the children...
		if (cIGameNode.GetChildCount()) {
			// Setup the scene container
			pContainer->m_bFixedCenter = true;
			pContainer->m_vCenter.Set(0.0f, 0.0f, 0.0f);
			pContainer->m_vPos = pSceneNode->m_vPos;
			pContainer->m_vRot = pSceneNode->m_vRot;

			// Setup the scene node 'creating' the scene container
			pSceneNode->m_vPos.Set(0.0f, 0.0f, 0.0f);
			pSceneNode->m_vRot.Set(0.0f, 0.0f, 0.0f);
		}

		// Register the new scene node
		pContainer->m_lstSceneNodes.push_back(pSceneNode);
		sSceneNodeNameLower = sSceneNodeName;
		sSceneNodeNameLower.ToLower(); // Do ONLY use lower case, else the hashing will NOT return the same values!
		pContainer->m_mapNodes.insert(std::make_pair(sSceneNodeNameLower, pSceneNode));

		{ // Add to 3ds Max node to PL node map
			String sKey = String::Format("%19p", cIGameNode.GetMaxNode());
			GetScene().m_mapMaxToPLNodes.insert(std::make_pair(sKey, pSceneNode));
		}

		// Loop through all child nodes
		for (int nNode=0; nNode<cIGameNode.GetChildCount(); nNode++) {
			IGameNode *pIGameNode = cIGameNode.GetNodeChild(nNode);
			if (pIGameNode) {
				g_pLog->LogFLine(PLLog::Scene, "Found 3ds Max child node: %s", pIGameNode->GetName());
				pContainer->AddIGameNode(*pIGameNode);
			}
		}

		// Done
		return true;
	}

	// Error!
	return false;
}

void PLSceneContainer::PostProcess()
{
	// Loop through all scene nodes
	for (std::vector<PLSceneNode*>::size_type i=0; i<m_lstSceneNodes.size(); i++) {
		PLSceneNode *pSceneNode = m_lstSceneNodes[i];
		if (pSceneNode) {
			switch (pSceneNode->GetType()) {
				case TypeContainer:
					static_cast<PLSceneContainer*>(pSceneNode)->PostProcess();
					break;

				case TypeCell:
					static_cast<PLSceneCell*>(pSceneNode)->PostProcess();
					break;
			}

			// Add the bounding box of this scene node the the container one
			const Box3 &cNodeBoundingBox = pSceneNode->GetBoundingBox();
			if (!PLTools::IsEmpty(cNodeBoundingBox)) {
				// Only 'mesh'-nodes are allowed to add their bounding box, other nodes like
				// cameras contribute only their position... else the final bounding box may
				// get hhhuuuuugge...
				if (pSceneNode->GetType() == PLSceneNode::TypeObject ||
					pSceneNode->GetType() == PLSceneNode::TypeCellPortal ||
					pSceneNode->GetType() == PLSceneNode::TypeAntiPortal ||
					pSceneNode->GetType() == PLSceneNode::TypeContainer ||
					pSceneNode->GetType() == PLSceneNode::TypeCell) {
					if (PLTools::IsEmpty(m_cBoundingBox))
						m_cBoundingBox = cNodeBoundingBox;
					else
						m_cBoundingBox += cNodeBoundingBox;
				} else {
					if (PLTools::IsEmpty(m_cBoundingBox))
						m_cBoundingBox.pmin = m_cBoundingBox.pmax = pSceneNode->m_vPos;
					else
						m_cBoundingBox += pSceneNode->m_vPos;
				}
			}
		}
	}

	// Calculate the center of this container
	if (!m_bFixedCenter && !PLTools::IsEmpty(m_cBoundingBox))
		m_vCenter = m_cBoundingBox.Center();
}

void PLSceneContainer::OutputStatistics()
{
	// Get the scene
	const PLScene &cScene = GetScene();

	// Get the total number of scene nodes
	int nTotalNumOfNodes = m_sStatistics.nNumOfCells+m_sStatistics.nNumOfCellPortals+m_sStatistics.nNumOfAntiPortals+
						   m_sStatistics.nNumOfCameras+m_sStatistics.nNumOfLights+m_sStatistics.nNumOfObjects;

	// Write into log
	if (GetType() == TypeCell)
		g_pLog->PrintFLine("Cell '%s':", GetName().GetASCII());
	else
		g_pLog->PrintFLine("Container '%s':", GetName().GetASCII());
	g_pLog->AddSpaces(PLLog::TabSize);
	if (cScene.m_nTotalNumOfNodes)
		g_pLog->PrintFLine("Total number of nodes: %d (%g%%)", nTotalNumOfNodes, (static_cast<float>(nTotalNumOfNodes)/static_cast<float>(cScene.m_nTotalNumOfNodes))*100.0f);
	else
		g_pLog->PrintFLine("Total number of nodes: %d (0%%)", nTotalNumOfNodes);
	if (cScene.m_sSceneStatistics.nNumOfCells)
		g_pLog->PrintFLine("Cells: %d (%g%%)", m_sStatistics.nNumOfCells, (static_cast<float>(m_sStatistics.nNumOfCells)/static_cast<float>(cScene.m_sSceneStatistics.nNumOfCells))*100.0f);
	else
		g_pLog->PrintFLine("Cells: %d (0%%)",  m_sStatistics.nNumOfCells);
	if (cScene.m_sSceneStatistics.nNumOfCameras)
		g_pLog->PrintFLine("Cameras: %d (%g%%)", m_sStatistics.nNumOfCameras, (static_cast<float>(m_sStatistics.nNumOfCameras)/static_cast<float>(cScene.m_sSceneStatistics.nNumOfCameras))*100.0f);
	else
		g_pLog->PrintFLine("Cameras: %d (0%%)",  m_sStatistics.nNumOfCameras);
	if (cScene.m_sSceneStatistics.nNumOfLights)
		g_pLog->PrintFLine("Lights: %d (%g%%)", m_sStatistics.nNumOfLights, (static_cast<float>(m_sStatistics.nNumOfLights)/static_cast<float>(cScene.m_sSceneStatistics.nNumOfLights))*100.0f);
	else
		g_pLog->PrintFLine("Lights: %d (0%%)",  m_sStatistics.nNumOfLights);
	if (cScene.m_sSceneStatistics.nNumOfObjects)
		g_pLog->PrintFLine("Objects: %d (%g%%)", m_sStatistics.nNumOfObjects, (static_cast<float>(m_sStatistics.nNumOfObjects)/static_cast<float>(cScene.m_sSceneStatistics.nNumOfObjects))*100.0f);
	else
		g_pLog->PrintFLine("Objects: %d (0%%)",  m_sStatistics.nNumOfObjects);
	if (cScene.m_sSceneStatistics.nNumOfHelpers)
		g_pLog->PrintFLine("Helpers: %d (%g%%)", m_sStatistics.nNumOfHelpers, (static_cast<float>(m_sStatistics.nNumOfHelpers)/static_cast<float>(cScene.m_sSceneStatistics.nNumOfHelpers))*100.0f);
	else
		g_pLog->PrintFLine("Helpers: %d (0%%)",  m_sStatistics.nNumOfHelpers);
	if (cScene.m_sSceneStatistics.nNumOfUnknown)
		g_pLog->PrintFLine("Unknown: %d (%g%%)", m_sStatistics.nNumOfUnknown, (static_cast<float>(m_sStatistics.nNumOfUnknown)/static_cast<float>(cScene.m_sSceneStatistics.nNumOfUnknown))*100.0f);
	else
		g_pLog->PrintFLine("Unknown: %d (0%%)",  m_sStatistics.nNumOfUnknown);

	// If this is a cell, show some cell-portal information
	if (GetType() == TypeCell) {
		const PLSceneCell &cCell = static_cast<PLSceneCell&>(*this);

		// List of outgoing cell-portals
		std::vector<PLSceneCellPortal*>::size_type nOutgoingCellPortals = cCell.m_lstOutgoingCellPortals.size();
		if (nOutgoingCellPortals) {
			g_pLog->PrintSpaces();
			g_pLog->PrintF("Outgoing cell-portals: %d (%g%%) -> ", nOutgoingCellPortals, static_cast<float>(nOutgoingCellPortals)/static_cast<float>(cScene.m_sSceneStatistics.nNumOfCellPortals)*100.0f);
			for (std::vector<PLSceneCellPortal*>::size_type i=0; i<nOutgoingCellPortals; i++) {
				const PLSceneCellPortal *pCellPortal = cCell.m_lstOutgoingCellPortals[i];
				if (pCellPortal) {
					if (i != 0)
						g_pLog->Print(", ");
					g_pLog->Print(String(pCellPortal->GetMaxNode()->GetName()).GetASCII());
				}
			}
			g_pLog->Print("\n");
		} else {
			g_pLog->PrintLine("Outgoing cell-portals: 0 (0%%)");
		}

		// List of incoming cell-portals
		std::vector<PLSceneCellPortal*>::size_type nIncomingCellPortals = cCell.m_lstIncomingCellPortals.size();
		if (nIncomingCellPortals) {
			g_pLog->PrintSpaces();
			g_pLog->PrintF("Incoming cell-portals: %d (%g%%)", nIncomingCellPortals, static_cast<float>(nIncomingCellPortals)/static_cast<float>(cScene.m_sSceneStatistics.nNumOfCellPortals)*100.0f);
			g_pLog->Print(" -> ");
			for (std::vector<PLSceneCellPortal*>::size_type i=0; i<nIncomingCellPortals; i++) {
				const PLSceneCellPortal *pCellPortal = cCell.m_lstIncomingCellPortals[i];
				if (pCellPortal) {
					if (i != 0)
						g_pLog->Print(", ");
					g_pLog->Print(String(pCellPortal->GetMaxNode()->GetName()).GetASCII());
				}
			}
			g_pLog->Print("\n");
		} else {
			g_pLog->PrintLine("Incoming cell-portals: 0 (0%%)");
		}

		// If the number of outgoing cell-portals is NOT equal to the number of incoming cell-portals, write a warning into the log
		if (nOutgoingCellPortals != nIncomingCellPortals) {
			// Take care of spelling ;-)
			if (nOutgoingCellPortals == 1) {
				g_pLog->LogFLine(PLLog::Warning, "1 cell-portal is going out, but %d cell-portals are coming in - is this indented? (maybe there are any undesired spaces/tabs within your names?)", nIncomingCellPortals);
			} else {
				if (nIncomingCellPortals == 1)
					g_pLog->LogFLine(PLLog::Warning, "%d cell-portals are going out, but 1 cell-portal is coming in - is this indented? (maybe there are any undesired spaces/tabs within your names?)", nOutgoingCellPortals, nIncomingCellPortals);
				else
					g_pLog->LogFLine(PLLog::Warning, "%d cell-portals are going out, but %d cell-portals are coming in - is this indented? (maybe there are any undesired spaces/tabs within your names?)", nOutgoingCellPortals, nIncomingCellPortals);
			}
		}
	}

	// Recursion
	for (std::vector<PLSceneNode*>::size_type i=0; i<m_lstSceneNodes.size(); i++) {
		PLSceneNode *pSceneNode = m_lstSceneNodes[i];
		if (pSceneNode) {
			if (pSceneNode->GetType() == TypeContainer)
				static_cast<PLSceneContainer*>(pSceneNode)->OutputStatistics();
			if (pSceneNode->GetType() == TypeCell)
				static_cast<PLSceneCell*>(pSceneNode)->OutputStatistics();
		}
	}

	// Done
	g_pLog->AddSpaces(-PLLog::TabSize);
}


//[-------------------------------------------------------]
//[ Protected virtual PLSceneNode functions               ]
//[-------------------------------------------------------]
void PLSceneContainer::WriteToFile(XmlElement &cSceneElement, const String &sApplicationDrive, const String &sApplicationDir)
{
	static const String sSceneContainer = "PLScene::SceneContainer";
	WriteToFile(cSceneElement, sApplicationDrive, sApplicationDir, sSceneContainer);
}
