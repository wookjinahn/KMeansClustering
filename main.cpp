#include <iostream>
#include <chrono>

#include "KMeansWithK.hpp"
#include "KMeans.hpp"


int main()
{
	const auto startTime = std::chrono::high_resolution_clock::now();
	camel::KMeans kmeans;
	kmeans.FromPCD("/home/wj/Desktop/Data/kmeans/test4_ideal.pcd");
//	kmeans.SetK(4);

	kmeans.Run();
	kmeans.SaveResult();

	const auto stopTime = std::chrono::high_resolution_clock::now();

	const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
	std::cout << elapsedTime.count() << " us." << std::endl;
}
