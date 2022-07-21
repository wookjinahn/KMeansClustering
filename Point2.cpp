//
// Created by wj on 22. 7. 21.
//

#include "Point2.hpp"

namespace camel
{
	Point2::Point2()
			: camelVector::Point2D()
	{
	}

	Point2::Point2(float x, float y)
			: camelVector::Point2D(x, y)
	{
	}

	bool Point2::bIsEqual(const Point2& other)
	{
		if (GetX() == other.GetX() && GetZ() == other.GetZ())
		{
			return true;
		}
		return false;
	}
}