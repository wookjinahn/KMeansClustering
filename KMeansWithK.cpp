//
// Created by wj on 22. 7. 21.
//

#include "KMeansWithK.hpp"


namespace camel
{
	KMeansWithK::KMeansWithK()
	{
	}

	KMeansWithK::KMeansWithK(std::vector<Point3> data, float k)
		: mData(std::move(data))
		, mK(k)
	{
	}

	std::vector<Point3> KMeansWithK::GetData() const
	{
		return mData;
	}

	void KMeansWithK::SetData(const std::vector<Point3>& data)
	{
		mData = data;
	}

	void KMeansWithK::SetK(int k)
	{
		mK = k;

		std::cout << "SetK : " << mK << std::endl;
	}

	void KMeansWithK::SetInitialCentroid()
	{
		std::sort(mData.begin(), mData.end(), camelVector::Point3D::DescendingByX);
		float maxX = mData[0].GetX();
		float minX = mData[mData.size() - 1].GetX();

		std::sort(mData.begin(), mData.end(), camelVector::Point3D::DescendingByZ);
		float maxZ = mData[0].GetZ();
		float minZ = mData[mData.size() - 1].GetZ();

		std::cout << "setInitialCentroid : " << maxX << ", " << minX << ", " << maxZ << ", " << minZ << std::endl;

		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_real_distribution<float> randomX(minX, maxX);
		std::uniform_real_distribution<float> randomZ(minZ, maxZ);

		for (int i = 0; i < mK; i++)
		{
			camel::Point2 centroid = { randomX(generator), randomZ(generator) };
			std::cout << "setInitialCentroid -> random centroid : " << centroid.GetX() << ", " << centroid.GetZ() << std::endl;
			mCentroids.push_back(centroid);
		}
		std::cout << "setInitialCentroid -> mCentroids : " << mCentroids.size() << std::endl;
	}

	void KMeansWithK::AssignCentroid()
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

	bool KMeansWithK::bUpdateCentroid()
	{
		// =========================================
		// ===== !!! NaN이 생기는 경우도 있음 !!! =====
		// =========================================
		std::vector<bool> checkUpdates;
		for (int cenIndex = 0; cenIndex < mCentroids.size(); cenIndex++)
		{
			bool checkUpdate = true;
			float newCentroidX = 0;
			float newCentroidZ = 0;
			float num = 0;

			for (int DataIndex = 0; DataIndex < mData.size(); DataIndex++)
			{
				// for문 돌면서 centroid가 같은 것들을 골라 값을 업데이트하기 위해 다 더해
				if (mCentroids[cenIndex].bIsEqual(mData[DataIndex].GetCentroid()))
				{
					newCentroidX += mData[DataIndex].GetX();
					newCentroidZ += mData[DataIndex].GetZ();
					num++;
				}
			}

			if (num != 0)
			{
				float updateX = newCentroidX / num;
				float updateZ = newCentroidZ / num;

				if (mCentroids[cenIndex].GetX() == updateX && mCentroids[cenIndex].GetZ() == updateZ)
				{
					checkUpdate = false;
					checkUpdates.push_back(checkUpdate);
					continue;
				}

				mCentroids[cenIndex].SetX(updateX);
				mCentroids[cenIndex].SetZ(updateZ);

				checkUpdates.push_back(checkUpdate);
			}
		}

		bool returnCheck = false;
		for (int i = 0; i < checkUpdates.size(); i++)
		{
			returnCheck = returnCheck || checkUpdates[i];
		}
		return returnCheck;
	}

	void KMeansWithK::Clustering()
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

	void KMeansWithK::Run( )
	{
		SetInitialCentroid();

//		for (int i = 0; i < iteration; i++)
//		{
//			AssignCentroid();
//			bUpdateCentroid();
//		}

		AssignCentroid();
		while (bUpdateCentroid())
		{
			std::cout << "iterate" << std::endl;
			AssignCentroid();
		}
		Clustering();
	}

	void KMeansWithK::FromPCD(const std::string& inputPath)
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

	void KMeansWithK::ToPCD(const std::vector<camel::Point3>& data, int num)
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

	void KMeansWithK::SaveResult()
	{
		for (int i = 0; i < mModels.size(); i++)
		{
			ToPCD(mModels[i].GetData(), i);
		}
	}

}