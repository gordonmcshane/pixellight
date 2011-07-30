/*********************************************************\
 *  File: SNMPhysicsCharacter.h                          *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
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


#ifndef __PLENGINE_CONTROLLER_SNMPHYSICSCHARACTER_H__
#define __PLENGINE_CONTROLLER_SNMPHYSICSCHARACTER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLMath/Vector3.h>
#include <PLPhysics/SceneNodeModifiers/SNMPhysics.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLPhysics {
	class Body;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Basic PL physics character scene node modifier class
*
*  @remarks
*    This class will access the PLPhysics::SNMPhysicsBody scene node modifier and take over
*    control of the physics body. So, this modifier makes only sense if there's a
*    PLPhysics::SNMPhysicsBody, too.
*
*  @note
*    - Primary intended for rapid prototyping
*/
class SNMPhysicsCharacter : public PLPhysics::SNMPhysics {


	//[-------------------------------------------------------]
	//[ Public definition                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node modifier flags (PLScene::SceneNodeModifier flags extension)
		*/
		enum EFlags {
			YMovement = 1<<2	/**< Perform movement also on the y axis, else movement is just performed on the x/z plane */
		};
		pl_enum(EFlags)
			pl_enum_base(SNMPhysics::EFlags)
			pl_enum_value(YMovement, "Perform movement also on the y axis, else movement is just performed on the x/z plane")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNMPhysicsCharacter, "PLEngine", PLPhysics::SNMPhysics, "Basic PL physics character scene node modifier class")
		// Attributes
		pl_attribute(Speed,					float,					2.0f,	ReadWrite,	DirectValue,	"Speed",										"Min='0.0'")
		pl_attribute(Acceleration,			float,					20.0f,	ReadWrite,	DirectValue,	"Acceleration",									"Min='0.0'")
		pl_attribute(MaxAcceleration,		float,					30.0f,	ReadWrite,	DirectValue,	"Maximum acceleration",							"Min='0.0'")
		pl_attribute(JumpForce,				float,					4.0f,	ReadWrite,	DirectValue,	"Jump force, if 0, jumping is not allowed",		"Min='0.0'")
		pl_attribute(JumpReadyTime,			float,					0.5f,	ReadWrite,	DirectValue,	"Time to past until we can jump again",			"Min='0.0'")
		pl_attribute(JumpGroundDistance,	float,					0.05f,	ReadWrite,	DirectValue,	"Minimum ground distance required for jumping",	"Min='0.001'")
			// Overwritten PLScene::SceneNodeModifier attributes
		pl_attribute(Flags,					pl_flag_type(EFlags),	0,		ReadWrite,	GetSet,			"Flags",										"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PL_API SNMPhysicsCharacter(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNMPhysicsCharacter();

		/**
		*  @brief
		*    Gets the running mode
		*
		*  @return
		*    'true' if the character runs, else 'false'
		*/
		PL_API bool IsRunning() const;

		/**
		*  @brief
		*    Sets the running mode
		*
		*  @param[in] bRun
		*    Did the character run?
		*/
		PL_API void SetRunning(bool bRun = false);

		/**
		*  @brief
		*    Gets the creeping mode
		*
		*  @return
		*    'true' if the character creeps, else 'false'
		*/
		PL_API bool IsCreeping() const;

		/**
		*  @brief
		*    Sets the creeping mode
		*
		*  @param[in] bCreep
		*    Did the character creep?
		*/
		PL_API void SetCreeping(bool bCreep = false);

		/**
		*  @brief
		*    Gets the movement vector
		*
		*  @return
		*    The movement vector
		*
		*  @note
		*    - The movement vector should be normalized
		*/
		PL_API const PLMath::Vector3 &GetMovement() const;

		/**
		*  @brief
		*    Sets the movement vector
		*
		*  @param[in] vMovement
		*    The new movement vector, if the length is >1, the vector is used in a normalized form inside OnUpdate()
		*
		*  @see
		*    - GetMovement()
		*/
		PL_API void SetMovement(const PLMath::Vector3 &vMovement);

		/**
		*  @brief
		*    Performs a jump
		*
		*  @return
		*    'true' if all went fine an the jump is performed, else 'false'
		*    (maybe the character does not stand on the ground)
		*/
		PL_API bool Jump();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Returns the PL physics body of the scene node
		*
		*  @return
		*    The PL physics body of the scene node, a null pointer if there's no such body
		*/
		PL_API PLPhysics::Body *GetPhysicsBody() const;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PL_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node modifier needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			m_bRun;				/**< If true, velocity in the callback is scaled up */
		bool			m_bCreep;			/**< If true, velocity in the callback is scaled down */
		PLMath::Vector3 m_vMovement;		/**< Current movement vector */
		float			m_fJumpReadyTimer;	/**< If this timer is 0, we are allowed to jump */
		bool			m_bJump;			/**< If true, let the character jump */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PL


#endif // __PLENGINE_CONTROLLER_SNMPHYSICSCHARACTER_H__