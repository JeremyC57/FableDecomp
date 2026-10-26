#include <cstdio>
static int g_flush = 0, g_reload = 0; static long g_reloadFlags = 0;
struct CDeviceOwner_009f8200 { void Flush() { ++g_flush; } }; static CDeviceOwner_009f8200 g_devOwner;
struct SDeviceRecord_013ca618 { unsigned char _pad[0x6c]; CDeviceOwner_009f8200* m_pOwner; }; static SDeviceRecord_013ca618 g_rec; static SDeviceRecord_013ca618* GetDeviceRecord_009a4ec0() { g_rec.m_pOwner = &g_devOwner; return &g_rec; }
class CGraphicDataBank { public: void Reload(long f) { ++g_reload; g_reloadFlags = f; } };
struct SRenderRequest_00b3b7c0 { long m_a; long m_kind; };
class CEngineResourceManager { public: unsigned char _pad[0x954]; CGraphicDataBank* m_pDataBank; void Render(SRenderRequest_00b3b7c0* r) { if (r->m_kind == 0x40000000) { GetDeviceRecord_009a4ec0()->m_pOwner->Flush(); m_pDataBank->Reload(1); } } };
int main() {
    CEngineResourceManager* m = (CEngineResourceManager*)new char[sizeof(CEngineResourceManager)]; CGraphicDataBank bank; m->m_pDataBank = &bank; SRenderRequest_00b3b7c0 req = { 0, 1 };
    m->Render(&req); if (g_flush || g_reload) { std::printf("00b3b7c0_TEST FAIL\n"); return 1; }
    req.m_kind = 0x40000000; m->Render(&req); if (g_flush != 1 || g_reload != 1 || g_reloadFlags != 1) { std::printf("00b3b7c0_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00b3b7c0_TEST PASS\n"); return 0;
}