#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_taskFreed = 0; static int g_taskFrees = 0; static void CoTaskMemFree(void* b) { g_taskFreed = b; ++g_taskFrees; }
struct IUnknown_00cb1cc0 { int m_released; IUnknown_00cb1cc0() : m_released(0) { } virtual void QueryInterface() { } virtual void AddRef() { } virtual void Release() { ++m_released; } };
struct CComHolder_00cb1cc0 { unsigned char _pad[0x3c]; IUnknown_00cb1cc0* m_pInterface; void* m_pBlock; void* m_pTaskMemory;
    void Release() { if (m_pBlock) { CoTaskMemFree(m_pTaskMemory); m_pBlock = 0; m_pTaskMemory = 0; } if (m_pInterface) { m_pInterface->Release(); m_pInterface = 0; } } };
int main() {
    CComHolder_00cb1cc0 h; IUnknown_00cb1cc0 u; int blk = 0, mem = 0; h.m_pInterface = &u; h.m_pBlock = &blk; h.m_pTaskMemory = &mem; h.Release();
    if (g_taskFreed != &mem || h.m_pBlock || h.m_pTaskMemory || u.m_released != 1 || h.m_pInterface) { std::printf("00cb1cc0_TEST FAIL\n"); return 1; }
    h.Release(); if (u.m_released != 1 || g_taskFrees != 1) { std::printf("00cb1cc0_TEST FAIL\n"); return 1; }
    std::printf("00cb1cc0_TEST PASS\n"); return 0;
}
