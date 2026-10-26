#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_level = -1;
class CBankFileAsyncData { public: unsigned char _pad[0x10]; long m_byteCount; bool m_finished; bool IsFinished() { return m_finished; } };
struct CTexture_009fd830 { long GetLevelByteLength(long l) { g_level = l; return 99; } };
struct CGraphicFrameData_009fd830 { unsigned char _pad[8]; CTexture_009fd830* m_pTexture; };
struct CFrameData { unsigned char _pad[0x1c]; CGraphicFrameData_009fd830* m_pGraphic; unsigned char _pad20[0x10]; CBankFileAsyncData* m_pAsyncRead;
    long GetUnloadableMemory() { if (m_pAsyncRead) { if (m_pAsyncRead->IsFinished()) return m_pAsyncRead->m_byteCount; return 0; } CGraphicFrameData_009fd830* g = m_pGraphic; if (g && g->m_pTexture) return g->m_pTexture->GetLevelByteLength(0); return 0; } };
int main() {
    CFrameData f; std::memset(&f, 0, sizeof f); CBankFileAsyncData a; a.m_byteCount = 40; a.m_finished = false; CTexture_009fd830 t; CGraphicFrameData_009fd830 g; g.m_pTexture = &t;
    if (f.GetUnloadableMemory() != 0) { std::printf("009fd830_TEST FAIL\n"); return 1; } f.m_pGraphic = &g; if (f.GetUnloadableMemory() != 99 || g_level != 0) { std::printf("009fd830_TEST FAIL\n"); return 1; } g.m_pTexture = 0; if (f.GetUnloadableMemory() != 0) { std::printf("009fd830_TEST FAIL\n"); return 1; }
    f.m_pAsyncRead = &a; if (f.GetUnloadableMemory() != 0) { std::printf("009fd830_TEST FAIL\n"); return 1; } a.m_finished = true; if (f.GetUnloadableMemory() != 40) { std::printf("009fd830_TEST FAIL\n"); return 1; }
    std::printf("009fd830_TEST PASS\n"); return 0;
}