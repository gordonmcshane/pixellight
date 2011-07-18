/*********************************************************\
 *  File: Color3.h                                       *
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


#ifndef __PLGRAPHICS_COLOR3_H__
#define __PLGRAPHICS_COLOR3_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGeneral/String/String.h>
#include "PLGraphics/PLGraphics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Color4;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    RGB color
*/
class Color3 {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		// Special
		PLGRAPHICS_API static const Color3 Null;	/**< -1.0, -1.0, -1.0 */
		// HTML 4/VGA
		PLGRAPHICS_API static const Color3 Black;	/**< Black (sRGB="0 0 0", Hex="#000000") */
		PLGRAPHICS_API static const Color3 White;	/**< White (sRGB="255 255 255", Hex="#FFFFFF") */
		PLGRAPHICS_API static const Color3 Red;		/**< Red (sRGB="255 0 0", Hex="#FF0000") */
		PLGRAPHICS_API static const Color3 Yellow;	/**< Yellow (sRGB="255 255 0", Hex="#FFFF00") */
		PLGRAPHICS_API static const Color3 Green;	/**< Green (sRGB="0 128 0", Hex="#008000") */
		PLGRAPHICS_API static const Color3 Aqua;	/**< Aqua, also called cyan or blue-green (sRGB="0 255 255", Hex="#00FFFF") */
		PLGRAPHICS_API static const Color3 Blue;	/**< Blue (sRGB="0 0 255", Hex="#0000FF") */
		PLGRAPHICS_API static const Color3 Fuchsia;	/**< Fuchsia, also called full magenta, bright pink or vivid pink (sRGB="255 0 255", Hex="#FF00FF") */
		PLGRAPHICS_API static const Color3 Maroon;	/**< Maroon, dark brownish-red color (sRGB="128 0 0", Hex="#800000") */
		PLGRAPHICS_API static const Color3 Olive;	/**< Olive, dark green/brown color (sRGB="128 128 0", Hex="#808000") */
		PLGRAPHICS_API static const Color3 Navy;	/**< Navy, also called navy blue, very dark shade of the color blue (sRGB="0 0 128", Hex="#000080") */
		PLGRAPHICS_API static const Color3 Purple;	/**< Purple, color intermediate to red and blue (sRGB="128 0 128", Hex="#800080") */
		PLGRAPHICS_API static const Color3 Teal;	/**< Teal, also called teal blue, greenish dark-blue color (sRGB="0 128 128", Hex="#008080") */
		PLGRAPHICS_API static const Color3 Gray;	/**< Gray (sRGB="128 128 128", Hex="#808080") */
		PLGRAPHICS_API static const Color3 Silver;	/**< Silver (sRGB="192 192 192", Hex="#C0C0C0") */
		PLGRAPHICS_API static const Color3 Lime;	/**< Lime, actually corresponds to the green primary of an RGB display (sRGB="0 255 0", Hex="#00FF00") */


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		union {
			float fColor[3];
			struct {
				float r, g, b;
			};
		};


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Maps a RGB floating point channels (0.0f to 1.0f range) to PLGeneral::uint32
		*
		*  @param[in] fR
		*    Red color component (0.0-1.0)
		*  @param[in] fG
		*    Green color component (0.0-1.0)
		*  @param[in] fB
		*    Blue color component (0.0-1.0)
		*
		*  @return
		*    The color as PLGeneral::uint32
		*/
		PLGRAPHICS_API static PLGeneral::uint32 ToUInt32(float fR, float fG, float fB);

		/**
		*  @brief
		*    Returns the red color component (float, 0.0-1.0) from a PLGeneral::uint32 RGB color
		*
		*  @param[in] nColor
		*    Color as PLGeneral::uint32
		*
		*  @return
		*    The red color component (0.0-1.0)
		*/
		PLGRAPHICS_API static float RedFromUInt32(PLGeneral::uint32 nColor);

		/**
		*  @brief
		*    Returns the green color component (float, 0.0-1.0) from a PLGeneral::uint32 RGB color
		*
		*  @param[in] nColor
		*    Color as PLGeneral::uint32
		*
		*  @return
		*    The green color component (0.0-1.0)
		*/
		PLGRAPHICS_API static float GreenFromUInt32(PLGeneral::uint32 nColor);

		/**
		*  @brief
		*    Returns the blue color component (float, 0.0-1.0) from a PLGeneral::uint32 RGB color
		*
		*  @param[in] nColor
		*    Color as PLGeneral::uint32
		*
		*  @return
		*    The blue color component (0.0-1.0)
		*/
		PLGRAPHICS_API static float BlueFromUInt32(PLGeneral::uint32 nColor);

		/**
		*  @brief
		*    Swaps the RB components in the given buffer with 3 components (BGR <-> RGB)
		*
		*  @param[in, out] nBuffer
		*    Reference to the buffer (RGB or BGR)
		*  @param[in]      nNumOfPixels
		*    Buffer size (width*height => number of pixels)
		*
		*  @note
		*    - This function is internally assembler optimizes for maximum performance
		*/
		PLGRAPHICS_API static void SwapRB(PLGeneral::uint8 nBuffer[], PLGeneral::uint32 nNumOfPixels);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor (all components are set to 0.0)
		*/
		PLGRAPHICS_API Color3();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] fR
		*    Red color component (0.0-1.0)
		*  @param[in] fG
		*    Green color component (0.0-1.0)
		*  @param[in] fB
		*    Blue color component (0.0-1.0)
		*/
		PLGRAPHICS_API Color3(float fR, float fG, float fB);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nR
		*    Red color component (0-255)
		*  @param[in] nG
		*    Green color component (0-255)
		*  @param[in] nB
		*    Blue color component (0-255)
		*/
		PLGRAPHICS_API Color3(PLGeneral::uint8 nR, PLGeneral::uint8 nG, PLGeneral::uint8 nB);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] fColor
		*    Float array holding the color, MUST have 3 elements! (0.0-1.0)
		*/
		PLGRAPHICS_API Color3(const float fColor[]);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nColor
		*    PLGeneral::uint8 array holding the color, MUST have 3 elements! (0-255)
		*/
		PLGRAPHICS_API Color3(const PLGeneral::uint8 nColor[]);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] fColor
		*    Value for all three color components (0.0-1.0)
		*/
		PLGRAPHICS_API Color3(float fColor);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nColor
		*    Value for all three color components (0-255)
		*/
		PLGRAPHICS_API Color3(PLGeneral::uint8 nColor);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source color
		*/
		PLGRAPHICS_API Color3(const Color3 &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLGRAPHICS_API ~Color3();

		/**
		*  @brief
		*    Maps this RGB floating point channels (0.0f to 1.0f range) to PLGeneral::uint32
		*
		*  @return
		*    This color as PLGeneral::uint32
		*/
		PLGRAPHICS_API PLGeneral::uint32 ToUInt32() const;

		/**
		*  @brief
		*    Maps from a PLGeneral::uint32 to RGB floating point channels (0.0f to 1.0f range)
		*
		*  @param[in] nColor
		*    Color as PLGeneral::uint32
		*/
		PLGRAPHICS_API void FromUInt32(PLGeneral::uint32 nColor);

		/**
		*  @brief
		*    Get the color component as float values
		*
		*  @param[out] fR
		*    Will receive the red color component (0.0-1.0)
		*  @param[out] fG
		*    Will receive the green color component (0.0-1.0)
		*  @param[out] fB
		*    Will receive the blue color component (0.0-1.0)
		*/
		PLGRAPHICS_API void GetRGB(float &fR, float &fG, float &fB) const;

		/**
		*  @brief
		*    Get the color component as integer values
		*
		*  @param[out] nR
		*    Will receive the red color component (0-255)
		*  @param[out] nG
		*    Will receive the green color component (0-255)
		*  @param[out] nB
		*    Will receive the blue color component (0-255)
		*/
		PLGRAPHICS_API void GetRGB(PLGeneral::uint8 &nR, PLGeneral::uint8 &nG, PLGeneral::uint8 &nB) const;

		/**
		*  @brief
		*    Set the color components by using float values
		*
		*  @param[in] fR
		*    Red color component (0.0-1.0)
		*  @param[in] fG
		*    Green color component (0.0-1.0)
		*  @param[in] fB
		*    Blue color component (0.0-1.0)
		*/
		PLGRAPHICS_API void SetRGB(float fR, float fG, float fB);

		/**
		*  @brief
		*    Set the color components by using integer values
		*
		*  @param[in] nR
		*    Red color component (0-255)
		*  @param[in] nG
		*    Green color component (0-255)
		*  @param[in] nB
		*    Blue color component (0-255)
		*/
		PLGRAPHICS_API void SetRGB(PLGeneral::uint8 nR, PLGeneral::uint8 nG, PLGeneral::uint8 nB);

		/**
		*  @brief
		*    Get the color component as array of float values
		*
		*  @param[out] fColor
		*    Reference to float array receiving the color values, MUST have 3 elements! (0.0-1.0)
		*/
		PLGRAPHICS_API void GetRGB(float fColor[]) const;

		/**
		*  @brief
		*    Get the color component as array of integer values
		*
		*  @param[out] nColor
		*    Reference to PLGeneral::uint8 array receiving the color values, MUST have 3 elements! (0-255)
		*/
		PLGRAPHICS_API void GetRGB(PLGeneral::uint8 nColor[]) const;

		/**
		*  @brief
		*    Set the color components by using a reference to float values
		*
		*  @param[in] fColor
		*    Reference to float array containing the color value, MUST have 3 elements! (0.0-1.0)
		*/
		PLGRAPHICS_API void SetRGB(const float fColor[]);

		/**
		*  @brief
		*    Set the color components by using a reference to integer values
		*
		*  @param[in] nColor
		*    Reference to PLGeneral::uint8 array containing the color value, MUST have 3 elements! (0-255)
		*/
		PLGRAPHICS_API void SetRGB(const PLGeneral::uint8 nColor[]);

		/**
		*  @brief
		*    Set all color component to the same float value
		*
		*  @param[in] fValue
		*    Value for all three color component (0.0-1.0)
		*/
		PLGRAPHICS_API void SetRGB(float fValue);

		/**
		*  @brief
		*    Set all color component to the same integer value
		*
		*  @param[in] nColor
		*    Value for all three color component (0-255)
		*/
		PLGRAPHICS_API void SetRGB(PLGeneral::uint8 nColor);

		/**
		*  @brief
		*    Get the red color value as float
		*
		*  @return
		*    Red value (0.0-1.0)
		*/
		PLGRAPHICS_API float GetR() const;

		/**
		*  @brief
		*    Get the red color value as integer
		*
		*  @return
		*    Red value (0-255)
		*/
		PLGRAPHICS_API PLGeneral::uint8 GetRInt() const;

		/**
		*  @brief
		*    Set the red color component using a float value
		*
		*  @param[in] fR
		*    Red value (0.0-1.0)
		*/
		PLGRAPHICS_API void SetR(float fR);

		/**
		*  @brief
		*    Set the red color component using an integer value
		*
		*  @param[in] nR
		*    Red value (0-255)
		*/
		PLGRAPHICS_API void SetR(PLGeneral::uint8 nR);

		/**
		*  @brief
		*    Get the green color value as float
		*
		*  @return
		*    Green value (0.0-1.0)
		*/
		PLGRAPHICS_API float GetG() const;

		/**
		*  @brief
		*    Get the green color value as integer
		*
		*  @return
		*    Green value (0-255)
		*/
		PLGRAPHICS_API PLGeneral::uint8 GetGInt() const;

		/**
		*  @brief
		*    Set the green color component using a float value
		*
		*  @param[in] fG
		*    Green value (0.0-1.0)
		*/
		PLGRAPHICS_API void SetG(float fG);

		/**
		*  @brief
		*    Set the green color component using an integer value
		*
		*  @param[in] nG
		*    Green value (0-255)
		*/
		PLGRAPHICS_API void SetG(PLGeneral::uint8 nG);

		/**
		*  @brief
		*    Get the blue color value as float
		*
		*  @return
		*    Blue value (0.0-1.0)
		*/
		PLGRAPHICS_API float GetB() const;

		/**
		*  @brief
		*    Get the blue color value as integer
		*
		*  @return
		*    Blue value (0-255)
		*/
		PLGRAPHICS_API PLGeneral::uint8 GetBInt() const;

		/**
		*  @brief
		*    Set the blue color component using a float value
		*
		*  @param[in] fB
		*    Blue value (0.0-1.0)
		*/
		PLGRAPHICS_API void SetB(float fB);

		/**
		*  @brief
		*    Set the blue color component using an integer value
		*
		*  @param[in] nB
		*    Blue value (0-255)
		*/
		PLGRAPHICS_API void SetB(PLGeneral::uint8 nB);

		/**
		*  @brief
		*    Returns if the color is valid
		*
		*  @return
		*    'true' if the color is valid, else 'false'
		*/
		PLGRAPHICS_API bool IsValid() const;

		/**
		*  @brief
		*    Clamps the color values between 0.0 and 1.0
		*/
		PLGRAPHICS_API void Saturate();

		/**
		*  @brief
		*    Get luminance of color as float
		*
		*  @return
		*    Luminance value (0.0-1.0)
		*
		*  @note
		*    - "human eye formula" (red*0.299 + green*0.587 + blue*0.114) is used
		*/
		PLGRAPHICS_API float GetLuminance() const;

		/**
		*  @brief
		*    Get luminance of color as integer
		*
		*  @return
		*    Luminance (0-255)
		*
		*  @see
		*    - GetLuminance()
		*/
		PLGRAPHICS_API PLGeneral::uint8 GetLuminanceInt() const;

		/**
		*  @brief
		*    Get luminance as color (results in a grayscale color)
		*
		*  @return
		*    Luminance color (grayscale)
		*
		*  @see
		*    - GetLuminance()
		*/
		PLGRAPHICS_API Color3 GetLuminanceColor() const;


		//[-------------------------------------------------------]
		//[ Assignment operators                                  ]
		//[-------------------------------------------------------]
		PLGRAPHICS_API Color3 &operator =(const Color3 &cC);
		PLGRAPHICS_API Color3 &operator =(const Color4 &cC);
		PLGRAPHICS_API Color3 &operator =(const float fC[]);
		PLGRAPHICS_API Color3 &operator =(float fD);
		PLGRAPHICS_API operator float *();
		PLGRAPHICS_API operator const float *() const;

		//[-------------------------------------------------------]
		//[ Comparison                                            ]
		//[-------------------------------------------------------]
		PLGRAPHICS_API bool operator ==(const Color3 &cC) const;
		PLGRAPHICS_API bool operator !=(const Color3 &cC) const;
		PLGRAPHICS_API bool operator ==(const Color4 &cC) const;
		PLGRAPHICS_API bool operator !=(const Color4 &cC) const;
		PLGRAPHICS_API bool operator ==(float f) const;
		PLGRAPHICS_API bool operator !=(float f) const;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		PLGRAPHICS_API Color3  operator +(const Color3 &cC) const;
		PLGRAPHICS_API Color3  operator +(float fN) const;
		PLGRAPHICS_API Color3 &operator +=(const Color3 &cC);
		PLGRAPHICS_API Color3 &operator +=(float fN);
		PLGRAPHICS_API Color3  operator -() const;
		PLGRAPHICS_API Color3  operator -(const Color3 &cC) const;
		PLGRAPHICS_API Color3  operator -(float fN) const;
		PLGRAPHICS_API Color3 &operator -=(const Color3 &cC);
		PLGRAPHICS_API Color3 &operator -=(float fN);
		PLGRAPHICS_API Color3  operator *(const Color3 &cC) const;
		PLGRAPHICS_API Color3  operator *(float fS) const;
		PLGRAPHICS_API Color3 &operator *=(const Color3 &cC);
		PLGRAPHICS_API Color3 &operator *=(float fS);
		PLGRAPHICS_API Color3  operator /(const Color3 &cC) const;
		PLGRAPHICS_API Color3  operator /(float fS) const;
		PLGRAPHICS_API Color3 &operator /=(const Color3 &cC);
		PLGRAPHICS_API Color3 &operator /=(float fS);
		PLGRAPHICS_API float  &operator [](int nIndex);

		/**
		*  @brief
		*    To string
		*
		*  @return
		*    String with the data
		*/
		PLGRAPHICS_API PLGeneral::String ToString() const;

		/**
		*  @brief
		*    From string
		*
		*  @param[in] sString
		*    String with the data
		*/
		PLGRAPHICS_API bool FromString(const PLGeneral::String &sString);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLGraphics/Color/TypeColor3.inl"


#endif // __PLGRAPHICS_COLOR3_H__