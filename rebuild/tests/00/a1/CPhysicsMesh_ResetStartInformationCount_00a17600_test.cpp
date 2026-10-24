// Behaviour test for CPhysicsMesh::ResetStartInformationCount (0x00A17600): every field of the static
// CPhysicsInfomationOutputData block is zero afterwards, and nothing next to it is touched.
#include <cstdio>
#include <cstring>

class CPhysicsInfomationOutputData {
public:
    bool UsingSegmentation;
    long TotalSphereIntersectionCalls;
    long TotalPointIntersectionCalls;
    long TotalSuccessfulSphereBoundingBoxTests;
    long TotalSuccessfulSphereBoundingSphereTests;
    long TotalSuccessfulSphereIntersectionTests;
    long TotalSuccessfulPointBoundingBoxTests;
    long TotalSuccessfulPointBoundingSphereTests;
    long TotalSuccessfulPointIntersectionTests;
    long TotalNoSphereTrianglesTested;
    long TotalNoPointTrianglesTested;
    float TotalTime;
};

class CPhysicsMesh {
public:
    static void ResetStartInformationCount();
    static CPhysicsInfomationOutputData Information;
};

CPhysicsInfomationOutputData CPhysicsMesh::Information;
static CPhysicsInfomationOutputData g_neighbour;

void CPhysicsMesh::ResetStartInformationCount()
{
    std::memset(&Information, 0, sizeof(CPhysicsInfomationOutputData));
}

int main()
{
    int ok = 1;
    CPhysicsInfomationOutputData& d = CPhysicsMesh::Information;
    d.UsingSegmentation = true;
    d.TotalSphereIntersectionCalls = 1; d.TotalPointIntersectionCalls = 2;
    d.TotalSuccessfulSphereBoundingBoxTests = 3; d.TotalSuccessfulSphereBoundingSphereTests = 4;
    d.TotalSuccessfulSphereIntersectionTests = 5; d.TotalSuccessfulPointBoundingBoxTests = 6;
    d.TotalSuccessfulPointBoundingSphereTests = 7; d.TotalSuccessfulPointIntersectionTests = 8;
    d.TotalNoSphereTrianglesTested = 9; d.TotalNoPointTrianglesTested = 10; d.TotalTime = 2.5f;
    g_neighbour.TotalNoPointTrianglesTested = 77;

    CPhysicsMesh::ResetStartInformationCount();
    if (d.UsingSegmentation || d.TotalSphereIntersectionCalls || d.TotalPointIntersectionCalls) ok = 0;
    if (d.TotalSuccessfulSphereBoundingBoxTests || d.TotalSuccessfulSphereBoundingSphereTests) ok = 0;
    if (d.TotalSuccessfulSphereIntersectionTests || d.TotalSuccessfulPointBoundingBoxTests) ok = 0;
    if (d.TotalSuccessfulPointBoundingSphereTests || d.TotalSuccessfulPointIntersectionTests) ok = 0;
    if (d.TotalNoSphereTrianglesTested || d.TotalNoPointTrianglesTested) ok = 0;
    if (d.TotalTime > 0.0001f || d.TotalTime < -0.0001f) ok = 0;
    if (g_neighbour.TotalNoPointTrianglesTested != 77) ok = 0;

    if (!ok) { std::printf("00a17600_TEST FAIL\n"); return 1; }
    std::printf("00a17600_TEST PASS\n");
    return 0;
}