//
// Created by wj on 22. 7. 21.
//

#include "KMeans.hpp"


namespace camel
{
	KMeans::KMeans()
	{
	}

	KMeans::KMeans(std::vector<Point3> data, float k)
		: mData(std::move(data))
		, mK(k)
	{
	}

	std::vector<Point3> KMeans::GetData() const
	{
		return mData;
	}

	void KMeans::SetData(const std::vector<Point3>& data)
	{
		mData = data;
	}

	void KMeans::SetK(int k)
	{
		mK = k;

		std::cout << "SetK : " << mK << std::endl;
	}

	void KMeans::SetInitialCentroid()
	{
		std::sort(mData.begin(), mData.end(), camelVector::Point3D::DescendingByX);
		float maxX = mData[0].GetX();
		float minX = mData[mData.size() - 1].GetX();

		std::sort(mData.begin(), mData.end(), camelVector::Point3D::DescendingByZ);
		float maxZ = mData[0].GetZ();
		float minZ = mData[mData.size() - 1].GetZ();

		std::cout << "SetInitialCentroid : " << maxX << ", " << minX << ", " << maxZ << ", " << minZ << std::endl;

		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_real_distribution<float> randomX(minX, maxX);
		std::uniform_real_distribution<float> randomZ(minZ, maxZ);

		for (int i = 0; i < mK; i++)
		{
			camel::Point2 centroid = { randomX(generator), randomZ(generator) };
			std::cout << "SetInitialCentroid -> random centroid : " << centroid.GetX() << ", " << centroid.GetZ() << std::endl;
			mCentroids.push_back(centroid);
		}
		std::cout << "SetInitialCentroid -> mCentroids : " << mCentroids.size() << std::endl;
	}

	void KMeans::AssignCentroid()
	{
		for (int i = 0; i < mData.size(); i++)
		{
			std::vector<float> distance;
			for (int j = 0; j < mCentroids.size(); j++)
			{
				distance.push_back(mData[i].DistanceBetweenCentroid(mCentroids[j]));
			}

			int minDistanceIndex = std::min_element(distance.begin(), distance.end()) - distance.begin();
			mData[i].SetCentroid(mCentroids[minDistanceIndex]);
		}
	}

	void KMeans::UpdateCentroid()
	{
		for (int i = 0; i < mCentroids.size(); i++)
		{
			float newCentroidX = 0;
			float newCentroidZ = 0;
			float num = 0;
			for (int j = 0; j < mData.size(); j++)
			{
				if (mCentroids[i].bIsEqual(mData[j].GetCentroid()))
				{
					newCentroidX += mData[j].GetX();
					newCentroidZ += mData[j].GetZ();
					num++;
				}
			}
			mCentroids[i].SetX(newCentroidX / num);
			mCentroids[i].SetZ(newCentroidZ / num);
		}
	}

	void KMeans::Clustering()
	{
		for (int i = 0; i < mK; i++)
		{
			std::vector<camel::Point3> data;

			for (int j = 0; j < mData.size(); j++)
			{
				if (mCentroids[i].bIsEqual(mData[j].GetCentroid()))
				{
					data.push_back(mData[j]);
				}
			}

			Model::Plane model(data);
			mModels.push_back(model);
		}
	}

	void KMeans::Run(int iteration)
	{
		SetInitialCentroid();

		for (int i = 0; i < iteration; i++)
		{
			AssignCentroid();
			UpdateCentroid();
		}
		Clustering();
	}

	void KMeans::FromPCD(const std::string& inputPath)
	{
		mData.reserve(307200);

		std::ifstream fin;
		fin.open(inputPath);
		std::string line;

		if (fin.is_open())
		{
			int num = 1;
			while (!fin.eof())
			{
				getline(fin, line);
				if (num > 10)
				{
					float x, y, z;
					std::istringstream iss(line);
					iss >> x >> y >> z;

					Point3 pointXYZ = {x, y, z};
					mData.push_back(pointXYZ);
				}
				num++;
			}
		}
		fin.close();

		std::cout << "FromPCD : " << mData.size() << std::endl;
	}

	void KMeans::ToPCD(const std::vector<camel::Point3>& data, int num)
	{
		std::cout << "output : " << data.size() << std::endl;
		std::string outputPath = "/home/wj/Desktop/Data/kmeans/output_data/";
		time_t t;
		struct tm* timeinfo;
		time(&t);
		timeinfo = localtime(&t);

		std::string hour, min;

		if (timeinfo->tm_hour < 10) hour = "0" + std::to_string(timeinfo->tm_hour);
		else hour = std::to_string(timeinfo->tm_hour);

		if (timeinfo->tm_min < 10) min = "0" + std::to_string(timeinfo->tm_min);
		else min = std::to_string(timeinfo->tm_min);

		std::string filePath = outputPath + hour + min + "_" + std::to_string(num) + ".pcd";

		std::ofstream fout;
		fout.open(filePath);

		fout << "VERSION" << std::endl;
		fout << "FIELDS x y z" << std::endl;
		fout << "SIZE 4 4 4" << std::endl;
		fout << "TYPE F F F" << std::endl;
		fout << "COUNT 1 1 1" << std::endl;
		fout << "WIDTH 1" << std::endl;
		fout << "HEIGHT " << data.size() << std::endl;
		fout << "VIEWPOINT 0 0 0 1 0 0 0" << std::endl;
		fout << "POINTS " << data.size() << std::endl;
		fout << "DATA ascii" << std::endl;

		for (int i = 0; i < data.size(); i++)
		{
			fout << data[i].GetX() << " " << data[i].GetY() << " " << data[i].GetZ() << "\n";
		}

		fout.close();

	}

	void KMeans::SaveResult()
	{
		for (int i = 0; i < mModels.size(); i++)
		{
			ToPCD(mModels[i].GetData(), i);
		}
	}

}