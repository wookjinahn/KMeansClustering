//
// Created by wj on 22. 7. 14.
//

#ifndef CAMEL_PERCEPTION_DEPALLETIZING_PLANE_HPP
#define CAMEL_PERCEPTION_DEPALLETIZING_PLANE_HPP

#include <vector>
#include <cmath>
#include <camel-euclid/Point3D.h>
namespace Model
{
	class Plane
	{
	public:
		Plane();
		Plane(std::vector<camel::Point3>& data);
		Plane(std::vector<camel::Point3>& data, std::vector<float>& parameter);

		std::vector<float> GetParameters() const;
		void SetModelThreshold(float modelThreshold);
		std::vector<camel::Point3> GetData() const;

		void FindParametersWithRandom(const std::vector<camel::Point3>& randomPoints);

		bool bIsInThreshold(const camel::Point3& data);
	private:
		std::vector<float> mParameters;		// 여러개의 plane이 생길 경우도 생각.

		float mModelThreshold = 0.0f;

		std::vector<camel::Point3> mData;
	};
}

#endif //CAMEL_PERCEPTION_DEPALLETIZING_PLANE_HPP
