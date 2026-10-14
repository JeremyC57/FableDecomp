#include <cstdio>

static int g_dtors = 0;
struct WaypointData { int x; ~WaypointData() { ++g_dtors; } };
struct CWaypointDataOwner_00c17910 { WaypointData* m_pWaypointData;
    void ReleaseWaypointData() { if (m_pWaypointData) { delete m_pWaypointData; m_pWaypointData = 0; } } };
int main()
{
    CWaypointDataOwner_00c17910 o; o.m_pWaypointData = new WaypointData;
    o.ReleaseWaypointData(); if (g_dtors != 1 || o.m_pWaypointData != 0) { std::printf("00c17910_TEST FAIL\n"); return 1; }
    o.ReleaseWaypointData(); if (g_dtors != 1) { std::printf("00c17910_TEST FAIL\n"); return 1; }
    std::printf("00c17910_TEST PASS\n"); return 0;
}
