#include <UnitTest++.h>
#include <Eigen/Dense>
#include <cmath>
#include "Compute4FingeredGrasps.h"
#include "PositionsNormalsFile.h"
#include "OBJFile.h"
#include "ObjectSurfacePoints.h"
#include "SamplingPoints.h"

namespace
{

inline void test_uniformAxis(ObjectSurfacePoints &osp, int npointsPerAxis, double halfAngle, int nUniqueSol)
{
    std::vector<Eigen::Vector3d> sammplePoints = SamplingPoints::uniformAxis(osp.minAABB, osp.maxAABB, npointsPerAxis);

    std::vector<std::vector<Grasp> > sol;
    Compute4FingeredGrasps::compute4FingeredGrasps(sol, osp.surfacePoints, sammplePoints, halfAngle);
    std::vector<Grasp> uniqueSol;
    Compute4FingeredGrasps::uniqueSol(uniqueSol, sol);
    /*
    std::vector<std::vector<Grasp> > sol_naive;
    Compute4FingeredGrasps::compute4FingeredGrasps_naive(sol_naive, osp.surfacePoints, sammplePoints, halfAngle);
    std::vector<Grasp> uniqueSol_naive;
    Compute4FingeredGrasps::uniqueSol(uniqueSol_naive, sol_naive);
    CHECK_EQUAL(uniqueSol_naive.size(), uniqueSol.size());
    */
    CHECK_EQUAL(nUniqueSol, uniqueSol.size());
}

TEST(compute4FingeredGrasps_cow500_16PointsPerAxis)
{
    char filename[1000] = "meshes/spectralMesh/cow500.txt";
    int npointsPerAxis = 16;
    double halfAngle = 10.d;
    int nUniqueSol = 35255;

    PositionsNormalsFile obj(filename);
    ObjectSurfacePoints osp(obj);
    test_uniformAxis(osp, npointsPerAxis, halfAngle, nUniqueSol);
}

TEST(compute4FingeredGrasps_CatLying_800_8PointsPerAxis)
{
    char filename[1000] = "meshes/KIT/CatLying_800.obj";
    int npointsPerAxis = 8;
    double halfAngle = 10.d;
    int nUniqueSol = 126939;

    OBJFile obj(filename);
    ObjectSurfacePoints osp(obj);
    test_uniformAxis(osp, npointsPerAxis, halfAngle, nUniqueSol);
}


//isPointInCone

TEST(isPointInCone_true0)
{
    double angle = 10.0/180.0*M_PI;
    Eigen::Vector3d point(1,0,0);
    Eigen::Vector3d normal(1,-1,0);
    Eigen::Vector3d position(0,1,0);
    const bool Result = Compute4FingeredGrasps::isPointInCone(point, SurfacePoint(normal, position), angle);
    CHECK_EQUAL(true, Result);
}

TEST(isPointInCone_true1)
{
    double angle = 10.0/180.0*M_PI;
    Eigen::Vector3d point(1,0,0);
    Eigen::Vector3d normal(1.000005,-1.00005,0);
    Eigen::Vector3d position(0,1,0);
    const bool Result = Compute4FingeredGrasps::isPointInCone(point, SurfacePoint(normal, position), angle);
    CHECK_EQUAL(true, Result);
}

TEST(isPointInCone_false0)
{
    double angle = 10.0/180.0*M_PI;
    Eigen::Vector3d point(1,0,0);
    Eigen::Vector3d normal(1.500005,-1.00005,0);
    Eigen::Vector3d position(0,1,0);
    const bool Result = Compute4FingeredGrasps::isPointInCone(point, SurfacePoint(normal, position), angle);
    CHECK_EQUAL(false, Result);
}

TEST(isPointInConeDoubleside_true0)
{
    double angle = 10.0/180.0*M_PI;
    Eigen::Vector3d point(1,0,0);
    Eigen::Vector3d normal(1.000005,-1.00005,0);
    Eigen::Vector3d position(0,1,0);
    const bool Result = Compute4FingeredGrasps::isPointInConeDoubleside(point, SurfacePoint(normal, position), angle);
    CHECK_EQUAL(true, Result);
}

TEST(isPointInConeDoubleside_true1)
{
    double angle = 10.0/180.0*M_PI;
    Eigen::Vector3d point(0,1,0);
    Eigen::Vector3d normal(-1.000005,1.00005,0);
    Eigen::Vector3d position(1,0,0);
    const bool Result = Compute4FingeredGrasps::isPointInConeDoubleside(point, SurfacePoint(normal, position), angle);
    CHECK_EQUAL(true, Result);
}

TEST(isPointInConeDoubleside_false0)
{
    double angle = 10.0/180.0*M_PI;
    Eigen::Vector3d point(1,0,0);
    Eigen::Vector3d normal(1.500005,-1.00005,0);
    Eigen::Vector3d position(0,1,0);
    const bool Result = Compute4FingeredGrasps::isPointInConeDoubleside(point, SurfacePoint(normal, position), angle);
    CHECK_EQUAL(false, Result);
}



}
