cmake_minimum_required(VERSION 3.2.0)
project(KMeansClustering)

set(CMAKE_CXX_STANDARD 14)

add_executable(KMeansClustering main.cpp KMeansWithK.cpp KMeans.cpp Point3.cpp Point2.cpp Plane.cpp)

find_package(camel-euclid-vector REQUIRED)

target_link_libraries(KMeansClustering camel-euclid-vector)

