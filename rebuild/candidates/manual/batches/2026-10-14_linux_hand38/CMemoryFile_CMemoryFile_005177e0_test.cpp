#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_base = 0, g_wide = 0;
struct CBaseClass { virtual void v00() { } CBaseClass() { ++g_base; } };
struct CWideString { void* m_p; CWideString() : m_p(0) { ++g_wide; } };
struct CRetailByteArray { unsigned char* m_pBegin; unsigned char* m_pEnd; unsigned char* m_pCapacity; CRetailByteArray() : m_pBegin(0), m_pEnd(0), m_pCapacity(0) { } };
struct CMemoryFile : CBaseClass { long AccessType; long Position; long Length; CRetailByteArray DataBuffer; bool Opened; CWideString Pathname; virtual void v00() { }
    CMemoryFile() : CBaseClass(), DataBuffer(), Opened(false), Pathname() { } };
int main()
{
    unsigned char raw[sizeof(CMemoryFile)]; std::memset(raw, 0xee, sizeof(raw));
    CMemoryFile* f = new (raw) CMemoryFile();
    bool ok = g_base == 1 && g_wide == 1 && f->DataBuffer.m_pBegin == 0 && f->DataBuffer.m_pEnd == 0 && f->DataBuffer.m_pCapacity == 0 && !f->Opened
        && f->Position == (long)0xeeeeeeee && sizeof(CMemoryFile) == 0x24;
    if (!ok) { std::printf("005177e0_TEST FAIL\n"); return 1; }
    std::printf("005177e0_TEST PASS\n"); return 0;
}
