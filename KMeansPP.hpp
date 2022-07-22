//
// Created by wj on 22. 7. 23.
//

#ifndef KMEANSCLUSTERING_KMEANSPP_HPP
#define KMEANSCLUSTERING_KMEANSPP_HPP


#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>

#include "Point3.hpp"
#include "Plane.hpp"

namespace camel
{
	class KMeansPP
	{
	public:
		KMeansPP();
		KMeansPP(std::vector<Point3> data, float k);

		std::vector<Point3> GetData() const;
		void SetData(const std::vector<Point3>& data);

		void FromPCD(const std::string& inputPath);
		void ToPCD(const std::vector<camel::Point3>& data, int num);
		void Run();
		void SaveResult();

	private:
		void setInitialCentroid();
		void assignCentroid();
		bool bUpdateCentroid();
		bool bFindNearRate();
		void generateCentroid();
		void clustering();

		std::vector<Point3> mData;

		std::vector<Point2> mCentroids;
		std::vector<Model::Plane> mModels;
	};
}


#endif //KMEANSCLUSTERING_KMEANSPP_HPP
