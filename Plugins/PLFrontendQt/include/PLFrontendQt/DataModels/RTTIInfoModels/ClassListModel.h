/*********************************************************\
 *  File: ClassListModel.h                               *
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


#ifndef __PLFRONTENDQT_PERMODULECLASSLISTMODEL_H__
#define __PLFRONTENDQT_PERMODULECLASSLISTMODEL_H__


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLFrontendQt/DataModels/TreeModelBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Module;
	class Class;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {
namespace DataModels {
namespace RTTIInfoModels {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    RTTI class list data model
*/
class PLFRONTENDQT_API ClassListModel : public TreeModelBase {


	//[-------------------------------------------------------]
	//[ Qt definitions (MOC)                                  ]
	//[-------------------------------------------------------]
	Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
				// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum ClassListModelRoles {
			ClassListNameRole = Qt::UserRole+1,
			ClassListDescriptionRole,
			ClassListItemType,
			ClassNameWithoutNamespaceRole,
			ClassBaseClassRole,
			ModuleVendorRole,
			ModuleLicenseRole,
			ModuleFilenameRole
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		explicit ClassListModel(bool hierarchical = false, QObject *parent = nullptr);
		virtual ~ClassListModel();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		void SetupModel(bool hierarchical);
		void CreateClassItemsFromModule(const PLCore::Module &moduleItem, TreeItemBase *parent);
		void CreateHierarchicalClassItems(const PLCore::Class &baseClass, TreeItemBase *parent);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // SceneRendererDataModel
} // DataModels
} // PLFrontendQt


#endif // __PLFRONTENDQT_PERMODULECLASSLISTMODEL_H__
