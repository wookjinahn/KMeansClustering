//
// Created by wj on 22. 7. 21.
//

#ifndef KMEANSCLUSTERING_POINT3_HPP
#define KMEANSCLUSTERING_POINT3_HPP

#include <iostream>
#include <cmath>
#include <random>
#include <camel-euclid/Point3D.hpp>

#include "Point2.hpp"

const double PI = 3.14159265359;
const double D2R = PI / 180;
const double R2D = 180 / PI;

namespace camel
{

	class Point3 : public camelVector::Point3D
	{
	public:
		Point3();
		Point3(float x, float y, float z);

		float DistanceBetween(const Point3& other);
		float DistanceBetweenCentroid(const Point2& other);

		void SetCentroid(const Point2& centroid);
		Point2 GetCentroid() const;

	private:
		Point2 mCentroid;
	};

}

#endif //KMEANSCLUSTERING_POINT3_HPP
