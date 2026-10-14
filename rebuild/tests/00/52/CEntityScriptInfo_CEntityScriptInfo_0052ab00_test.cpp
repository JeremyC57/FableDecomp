#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_strings = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
struct CRetailByteArray { unsigned char* m_pBegin; unsigned char* m_pEnd; unsigned char* m_pCapacity; CRetailByteArray() : m_pBegin(0), m_pEnd(0), m_pCapacity(0) { } };
struct CEntityScriptInfo { CCharString Script; CRetailByteArray SaveData; long UncompressedSize; bool Valid; CEntityScriptInfo() : Script(), SaveData(), UncompressedSize(0), Valid(false) { } };
int main()
{
    unsigned char raw[sizeof(CEntityScriptInfo)]; std::memset(raw, 0xee, sizeof(raw));
    CEntityScriptInfo* e = new (raw) CEntityScriptInfo();
    bool ok = g_strings == 1 && e->SaveData.m_pBegin == 0 && e->SaveData.m_pEnd == 0 && e->SaveData.m_pCapacity == 0 && e->UncompressedSize == 0 && !e->Valid && sizeof(CEntityScriptInfo) == 0x18;
    if (!ok) { std::printf("0052ab00_TEST FAIL\n"); return 1; }
    std::printf("0052ab00_TEST PASS\n"); return 0;
}