#include <UnitTest++.h>
#include "OBJFile.h"
#include "Flann3D.h"
#include "NaiveNN3D.h"
#include "Timer.h"

namespace
{

TEST(Flann3D_NaiveNN3D)
{
    Timer tmr;

    OBJFile obj("test/meshes/KIT/Amicelli_800.obj");
    //int nPoints = 400;
    //int nFacets = 798;
    tmr.reset();
    Flann3D flann3D(obj.vertexs);
    printf("flann3D(obj.vertexs) : %lf\n", tmr.elapsed());
    NaiveNN3D naiveNN3D(obj.vertexs);

    Eigen::Vector3d p(0,0,0);
    std::vector<Eigen::Vector3d> queries;
    queries.push_back(Eigen::Vector3d(p.x(), p.y(), p.z()));
    p = obj.vertexs[0] + Eigen::Vector3d(1,1,1);
    queries.push_back(Eigen::Vector3d(p.x(), p.y(), p.z()));
    p = obj.vertexs[0] + Eigen::Vector3d(-1,-1,-1);
    queries.push_back(Eigen::Vector3d(p.x(), p.y(), p.z()));
    p = obj.vertexs[obj.vertexs.size()-1] + Eigen::Vector3d(1,1,1);
    queries.push_back(Eigen::Vector3d(p.x(), p.y(), p.z()));
    p = obj.vertexs[obj.vertexs.size()-1] + Eigen::Vector3d(-1,-1,-1);
    queries.push_back(Eigen::Vector3d(p.x(), p.y(), p.z()));
    p = obj.vertexs[obj.vertexs.size()/2] + Eigen::Vector3d(1,1,1);
    queries.push_back(Eigen::Vector3d(p.x(), p.y(), p.z()));
    p = obj.vertexs[obj.vertexs.size()/2] + Eigen::Vector3d(-1,-1,-1);
    queries.push_back(Eigen::Vector3d(p.x(), p.y(), p.z()));
    std::vector<Eigen::Vector3d> queries2(queries);
    for(int i=0 ; i<50 ;++i){
        queries.insert(queries.end(), queries2.begin(), queries2.end());
    }

    std::vector< std::vector< std::pair<Eigen::Vector3d, double> > > outFlann3D, outNaiveNN3D;
    unsigned int knn = 3;
    tmr.reset();
    flann3D.knnSearch(queries, outFlann3D, knn);
    printf("flann3D.knnSearch : %lf\n", tmr.elapsed());
    tmr.reset();
    naiveNN3D.knnSearch(queries, outNaiveNN3D, knn);
    printf("naiveNN3D.knnSearch : %lf\n", tmr.elapsed());
    CHECK_EQUAL(queries.size(), outFlann3D.size());
    CHECK_EQUAL(queries.size(), outNaiveNN3D.size());
    for(unsigned int i=0 ; i<queries.size() ; ++i){
        for(unsigned int j=0 ; j<knn ; ++j){
            CHECK_EQUAL(outFlann3D[i][j].first, outNaiveNN3D[i][j].first);
            CHECK_CLOSE(outFlann3D[i][j].second, outNaiveNN3D[i][j].second, 0.0000001);
        }
    }
}

}
