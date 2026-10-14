#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_order = 0, g_free = 0, g_ptr = 0, g_str = 0, g_del = 0; static void* g_freed = 0; static void* g_deleted = 0;
static void my_free(void* p) { g_free = ++g_order; g_freed = p; }
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pLink; ~CBaseIntelligentPointer() { g_ptr = ++g_order; } };
struct CCharString { void* m_p; ~CCharString() { g_str = ++g_order; } };
struct CNamedBufferOwner { long m_0; CCharString m_name; long m_8, m_c, m_10; CBaseIntelligentPointer m_pTarget; void* m_pBuffer;
    ~CNamedBufferOwner() { if (m_pBuffer) my_free(m_pBuffer); }
    static void operator delete(void* p) { g_del = ++g_order; g_deleted = p; } };
static void __fastcall DestroyNamedBufferOwner(CNamedBufferOwner* object) { delete object; }
int main()
{
    DestroyNamedBufferOwner(0);
    bool ok = g_order == 0;
    unsigned char raw[sizeof(CNamedBufferOwner)]; std::memset(raw, 0, sizeof(raw));
    CNamedBufferOwner* o = (CNamedBufferOwner*)raw; o->m_pBuffer = (void*)0x1234;
    DestroyNamedBufferOwner(o);
    ok = ok && g_free == 1 && g_freed == (void*)0x1234 && g_ptr == 2 && g_str == 3 && g_del == 4 && g_deleted == (void*)raw;
    if (!ok) { std::printf("0085b090_TEST FAIL\n"); return 1; }
    std::printf("0085b090_TEST PASS\n"); return 0;
}
