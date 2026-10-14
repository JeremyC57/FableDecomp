#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_destroyed = 0; static void* g_deleted = 0; static void DestroyTexture(void* o) { g_destroyed = o; std::strcat(g_seq, "D"); } static void DeleteBlock(void* p) { g_deleted = p; std::strcat(g_seq, "d"); }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct CCountedPointer { void* m_pObject; SCountBlock* m_pCount; void Reset() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); DeleteBlock(m_pCount); } } m_pCount = 0; m_pObject = 0; } };
struct CGraphicFrameData { unsigned char _pad[8]; CCountedPointer m_texture; unsigned char _pad10[0xa]; unsigned char m_flags; void Uninitialise() { m_flags &= ~0x10; if (m_flags & 4) m_texture.Reset(); } };

int main() {
    CGraphicFrameData d; SCountBlock c; int tex; std::memset(&d, 0, sizeof d); c.m_references = 1; c.m_pfnDestroy = DestroyTexture; c.m_pObject = &tex; d.m_flags = 0x14; d.m_texture.m_pObject = &tex; d.m_texture.m_pCount = &c;
    d.Uninitialise(); if (d.m_flags != 4 || std::strcmp(g_seq, "Dd") || g_destroyed != &tex || g_deleted != &c || d.m_texture.m_pCount != 0 || d.m_texture.m_pObject != 0) { std::printf("009fcba0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; d.m_flags = 0x10; c.m_references = 1; d.m_texture.m_pObject = &tex; d.m_texture.m_pCount = &c; d.Uninitialise(); if (d.m_flags != 0 || g_seq[0] || d.m_texture.m_pCount != &c) { std::printf("009fcba0_TEST FAIL\n"); return 1; }
    d.m_flags = 4; c.m_references = 2; d.Uninitialise(); if (c.m_references != 1 || g_seq[0] || d.m_texture.m_pCount != 0) { std::printf("009fcba0_TEST FAIL\n"); return 1; }
    std::printf("009fcba0_TEST PASS\n"); return 0;
}
