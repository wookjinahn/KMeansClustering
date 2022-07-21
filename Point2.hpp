//
// Created by wj on 22. 7. 21.
//

#ifndef KMEANSCLUSTERING_POINT2_HPP
#define KMEANSCLUSTERING_POINT2_HPP

#include <camel-euclid/Point2D.h>

namespace camel
{

	class Point2 : public camelVector::Point2D
	{
	public:
		Point2();
		Point2(float x, float z);

		bool bIsEqual(const Point2& other);
	};

}


#endif //KMEANSCLUSTERING_POINT2_HPP
