#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_freed[4]; static long g_nFreed = 0; static void FreeBlock(void* p) { g_freed[g_nFreed++] = p; std::strcat(g_seq, "f"); }
static void* g_destroyed = 0; static void DestroyTexture(void* o) { g_destroyed = o; std::strcat(g_seq, "D"); }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CCountedPointer { void* m_pObject; SCountBlock* m_pCount;
    ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); FreeBlock(m_pCount); } } m_pObject = 0; m_pCount = 0; } };
struct CGraphicFrameData { long m_references; long m_4; CCountedPointer m_texture; unsigned char _pad10[0xa]; unsigned char m_flags;
    void Uninitialise() { std::strcat(g_seq, "U"); } ~CGraphicFrameData() { if (m_flags & 0x10) Uninitialise(); } };
struct CGraphicFrame { CGraphicFrameData* m_pData;
    void UnInitialise_00a00b90() { CGraphicFrameData* data = m_pData; if (data) { if (--data->m_references == 0) { data->~CGraphicFrameData(); FreeBlock(data); } m_pData = 0; } } };

int main() {
    CGraphicFrameData d; SCountBlock c; int tex; std::memset(&d, 0, sizeof d); c.m_references = 1; c.m_pfnDestroy = DestroyTexture; c.m_pObject = &tex;
    CGraphicFrame f; f.m_pData = &d; d.m_references = 2; d.m_flags = 0x10; d.m_texture.m_pObject = &tex; d.m_texture.m_pCount = &c;
    f.UnInitialise_00a00b90(); if (f.m_pData != 0 || d.m_references != 1 || g_seq[0] || g_nFreed != 0) { std::printf("00a00b90_TEST FAIL\n"); return 1; }
    f.m_pData = &d; f.UnInitialise_00a00b90(); if (f.m_pData != 0 || d.m_references != 0 || std::strcmp(g_seq, "UDff") || g_destroyed != &tex || g_freed[0] != &c || g_freed[1] != &d || d.m_texture.m_pCount != 0) { std::printf("00a00b90_TEST FAIL\n"); return 1; }
    f.m_pData = 0; f.UnInitialise_00a00b90(); if (g_nFreed != 2) { std::printf("00a00b90_TEST FAIL\n"); return 1; }
    std::printf("00a00b90_TEST PASS\n"); return 0;
}
