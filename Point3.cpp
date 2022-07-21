//
// Created by wj on 22. 7. 21.
//

#include "Point3.hpp"

namespace camel
{
	Point3::Point3()
			: camelVector::Point3D()
	{
	}

	Point3::Point3(float x, float y, float z)
			: camelVector::Point3D(x, y, z)
	{
	}

	float Point3::DistanceBetween(const Point3& other)
	{
		return std::sqrt((GetX() - other.GetX()) * (GetX() - other.GetX()) + (GetY() - other.GetY()) * (GetY() - other.GetY()) + (GetZ() - other.GetZ()) * (GetZ() - other.GetZ()));
	}

	float Point3::DistanceBetweenCentroid(const Point2& other)
	{
		return std::sqrt((GetX() - other.GetX()) * (GetX() - other.GetX()) + (GetZ() - other.GetZ()) * (GetZ() - other.GetZ()));
	}

	void Point3::SetCentroid(const Point2& centroid)
	{
		mCentroid = centroid;
	}

	Point2 Point3::GetCentroid() const
	{
		return mCentroid;
	}
}