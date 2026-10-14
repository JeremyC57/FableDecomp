#include <cstdio>
#include <stddef.h>
static int g_frees = 0, g_rootDtors = 0; static void* g_freed[2];
extern "C" void __cdecl free_stub(void* block) { g_freed[g_frees++ & 1] = block; }
#define free free_stub
struct CFreeableBuffer { void* m_pData; ~CFreeableBuffer() { if (m_pData != 0) free(m_pData); } };
struct CResource_Calls { virtual void v00() { } ~CResource_Calls() { ++g_rootDtors; } };
struct CDefPointeeBase : CResource_Calls { virtual void v00() { } ~CDefPointeeBase() { } };
#pragma pack(push, 1)
struct CUIStateDef : CDefPointeeBase {
    unsigned char _pad0[0x29 - 0x04]; CFreeableBuffer m_buffer0; unsigned char _pad1[0x6c - 0x29 - 4]; CFreeableBuffer m_buffer1;
    virtual void v00() { }
    ~CUIStateDef() { }
};
#pragma pack(pop)
int main()
{
    int a, b;
    {
        CUIStateDef def; def.m_buffer0.m_pData = &a; def.m_buffer1.m_pData = &b;
    }
    if (g_frees != 2 || g_freed[0] != &b || g_freed[1] != &a || g_rootDtors != 1
        || offsetof(CUIStateDef, m_buffer0) != 0x29 || offsetof(CUIStateDef, m_buffer1) != 0x6c) { std::printf("00624dc0_TEST FAIL\n"); return 1; }
    {
        CUIStateDef def; def.m_buffer0.m_pData = 0; def.m_buffer1.m_pData = 0;
    }
    if (g_frees != 2 || g_rootDtors != 2) { std::printf("00624dc0_TEST FAIL\n"); return 1; }
    std::printf("00624dc0_TEST PASS\n"); return 0;
}