/*********************************************************\
 *  File: PLDynVarNativNumericTypeTreeItem.h             *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


#ifndef __PLFRONTENDQT_PLDYNVARNATIVNUMERICTYPETREEITEM_H__
#define __PLFRONTENDQT_PLDYNVARNATIVNUMERICTYPETREEITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLFrontendQt/DataModels/PLTreeItems/PLDynVarStringTreeItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Dynamic variable tree item: for native numeric types, float, double, signed/unsigned integer (8, 16, 32, 64 bit)
*/
class PLFRONTENDQT_API PLDynVarNativNumericTypeTreeItem : public PLDynVarStringTreeItem {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		explicit PLDynVarNativNumericTypeTreeItem(PLCore::DynVar *dynVar, TreeItemBase *parent = nullptr);
		virtual QVariant data(const int column, const int role) override;
		bool setData(const int column, const QVariant &value, const int role);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		QVariant getBaseTypeData();
		bool setBaseTypeData(const QVariant &value);
		static PLDynVarTreeItemTypes::DynVarTreeItemTypes GetNumericTypeFromDynVar(const PLCore::DynVar *dynVar);

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int m_basisTypeId;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // DataModels
} // PLFrontendQt


#endif // __PLFRONTENDQT_PLDYNVARNATIVNUMERICTYPETREEITEM_H__
