#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_n = 0; static void* g_freed[8];
static void my_free(void* p) { g_freed[g_n++] = p; }
struct CRawBuffer { void* m_pData; long m_size; long m_capacity; ~CRawBuffer() { if (m_pData) my_free(m_pData); } };
struct SBufferPair { CRawBuffer m_first; CRawBuffer m_second; };
struct COwner { unsigned char _pad[0x1ac]; SBufferPair m_pairs[3]; ~COwner() { } };
int main()
{
    COwner* o = (COwner*)std::malloc(sizeof(COwner)); std::memset(o, 0, sizeof(COwner));
    o->m_pairs[0].m_first.m_pData = (void*)1; o->m_pairs[0].m_second.m_pData = (void*)2;
    o->m_pairs[1].m_first.m_pData = (void*)0; o->m_pairs[1].m_second.m_pData = (void*)4;
    o->m_pairs[2].m_first.m_pData = (void*)5; o->m_pairs[2].m_second.m_pData = (void*)6;
    o->~COwner();
    bool ok = sizeof(SBufferPair) == 0x18 && g_n == 5 && g_freed[0] == (void*)6 && g_freed[1] == (void*)5 && g_freed[2] == (void*)4
        && g_freed[3] == (void*)2 && g_freed[4] == (void*)1;
    std::free(o);
    if (!ok) { std::printf("006b5d80_TEST FAIL\n"); return 1; }
    std::printf("006b5d80_TEST PASS\n"); return 0;
}