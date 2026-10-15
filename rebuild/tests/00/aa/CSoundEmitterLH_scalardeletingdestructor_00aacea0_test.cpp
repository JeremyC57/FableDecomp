#include <cstdio>
#include <cstdlib>
// Behaviour test for CSoundEmitterLH scalar deleting destructor (0x00AACEA0): Shutdown, then the array tears
// its elements down and frees its block (when set), then the CBase root.
static int g_order[8]; static int g_n = 0; static int g_freed = 0;
struct CBase { virtual ~CBase() { g_order[g_n++] = 1; } };
struct CEmitterArray {
    void* m_pData; long m_size; long m_capacity;
    void DestroyElements() { g_order[g_n++] = 2; }
    ~CEmitterArray() { DestroyElements(); if (m_pData) { std::free(m_pData); ++g_freed; } }
};
struct CSoundEmitterLH : CBase {
    CEmitterArray m_soundEmitters;
    void Shutdown() { g_order[g_n++] = 3; }
    virtual ~CSoundEmitterLH() { Shutdown(); }
};
int main()
{
    CSoundEmitterLH* s = new CSoundEmitterLH;
    s->m_soundEmitters.m_pData = std::malloc(4);
    CBase* p = s;
    delete p;
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1 || g_freed != 1) { std::printf("00aacea0_TEST FAIL\n"); return 1; }
    g_n = 0;
    s = new CSoundEmitterLH; s->m_soundEmitters.m_pData = 0;
    delete s;
    if (g_n != 3 || g_freed != 1) { std::printf("00aacea0_TEST FAIL\n"); return 1; }
    std::printf("00aacea0_TEST PASS\n");
    return 0;
}