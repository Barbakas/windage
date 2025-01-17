/* ========================================================================
 * PROJECT: windage Library
 * ========================================================================
 * This work is based on the original windage Library developed by
 *   Woonhyuk Baek
 *   Woontack Woo
 *   U-VR Lab, GIST of Gwangju in Korea.
 *   http://windage.googlecode.com/
 *   http://uvr.gist.ac.kr/
 *
 * Copyright of the derived and new portions of this work
 *     (C) 2009 GIST U-VR Lab.
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact 
 *   Woonhyuk Baek
 *   <windage@live.com>
 *   GIST U-VR Lab.
 *   Department of Information and Communication
 *   Gwangju Institute of Science and Technology
 *   1, Oryong-dong, Buk-gu, Gwangju
 *   South Korea
 * ========================================================================
 ** @author   Woonhyuk Baek
 * ======================================================================== */

#ifndef _CUBE_SENSOR_GROUP_H_
#define _CUBE_SENSOR_GROUP_H_

#include <vector>
#include <cv.h>

#include "base.h"
#include "Utils/wVector.h"
#include "SpatialInteraction/SensorGroup.h"

namespace windage
{
	/**
	 * @brief
	 *		Class for cube sensor group
	 * @author
	 *		windage
	 */
	class DLLEXPORT CubeSensorGroup : public SensorGroup
	{
	protected:
		double cellSize;		///< spatial sensor cell size
		double cellSpacing;		///< spatial sensor cell spacing
		int arragneCount;		///< spatial sensor count at a corner

		void Release();

	public:
		CubeSensorGroup();
		virtual ~CubeSensorGroup();

		void Initialize(int id, Vector3 position = Vector3(), int arrangeCount=5, double arrangeSize=150.0);
		void Initialize(int id, Vector3 position, int arrangeCount, double cellSize, double cellSpacing);

		inline void SetCellSize(double size){this->cellSize = size;};
		inline void SetCellSpacing(double size){this->cellSpacing = size;};
		inline void SetArrangeCount(int count){this->arragneCount = count;};
		inline double GetCellSize(){return this->cellSize;};
		inline double GetCellSpacing(){return this->cellSpacing;};
		inline int GetArrangeCount(){return this->arragneCount;};
	};
}


#endif