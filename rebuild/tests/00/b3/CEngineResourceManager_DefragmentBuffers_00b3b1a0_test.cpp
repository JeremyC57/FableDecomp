#include <cstdio>
static int g_defrag = 0, g_owner = 0;
class CHandleBasedMemoryPool { public: void Defragment() { ++g_defrag; } };
struct CEngineGlobal_01436ea8 { unsigned char _pad[0x6b0]; void* m_pBuffers; }; static CEngineGlobal_01436ea8 g_eng; static CEngineGlobal_01436ea8* g_pEngine_01436ea8 = &g_eng;
struct CBufferOwner_00bd9b50 { void Defragment() { ++g_owner; } }; static CBufferOwner_00bd9b50 g_bo;
class CEngineResourceManager { public: unsigned char _pad[0x8f0]; CHandleBasedMemoryPool* m_pPoolA; unsigned char _pad8f4[4]; CHandleBasedMemoryPool* m_pPoolB; unsigned char _pad8fc[4]; CHandleBasedMemoryPool* m_pPoolC;
    void DefragmentBuffers() { if (m_pPoolA) m_pPoolA->Defragment(); if (m_pPoolB) m_pPoolB->Defragment(); if (m_pPoolC) m_pPoolC->Defragment(); if (g_pEngine_01436ea8) ((CBufferOwner_00bd9b50*)g_pEngine_01436ea8->m_pBuffers)->Defragment(); } };
int main() {
    CEngineResourceManager* m = (CEngineResourceManager*)new char[sizeof(CEngineResourceManager)]; CHandleBasedMemoryPool p; m->m_pPoolA = &p; m->m_pPoolB = 0; m->m_pPoolC = &p; g_eng.m_pBuffers = &g_bo;
    m->DefragmentBuffers(); if (g_defrag != 2 || g_owner != 1) { std::printf("00b3b1a0_TEST FAIL\n"); return 1; }
    g_pEngine_01436ea8 = 0; m->DefragmentBuffers(); if (g_defrag != 4 || g_owner != 1) { std::printf("00b3b1a0_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00b3b1a0_TEST PASS\n"); return 0;
}