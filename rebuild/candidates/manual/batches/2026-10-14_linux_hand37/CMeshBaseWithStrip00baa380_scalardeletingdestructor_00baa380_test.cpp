#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static int g_n = 0;
struct CPatchTesselationEdgeStrip { unsigned char _body[4]; ~CPatchTesselationEdgeStrip() { g_seq[g_n++] = 'm'; } };
struct CEngineInternalPrimitiveMeshBase { virtual void v00() { } unsigned char _pad[168]; ~CEngineInternalPrimitiveMeshBase() { g_seq[g_n++] = 'b'; } };
struct CMeshBaseWithStrip_00baa380 { CEngineInternalPrimitiveMeshBase m_base; CPatchTesselationEdgeStrip m_member; ~CMeshBaseWithStrip_00baa380() { } };
static void Destroy_00baa380(CMeshBaseWithStrip_00baa380* o) { delete o; }

int main() {
    CMeshBaseWithStrip_00baa380* o = new CMeshBaseWithStrip_00baa380; Destroy_00baa380(o); if (std::strcmp(g_seq, "mb")) { std::printf("00baa380_TEST FAIL\n"); return 1; }
    std::printf("00baa380_TEST PASS\n"); return 0;
}
