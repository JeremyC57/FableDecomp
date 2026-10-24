#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N0052b4e0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CArrayByte {                                 // retail 12-byte STLport-vector body (unsigned char)
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    CArrayByte() { std::memset(this, 0, sizeof(CArrayByte)); }
    CArrayByte(const CArrayByte& other) { std::memcpy(this, &other, sizeof(CArrayByte)); ++g_extCopies; }
};
struct CEntityScriptInfo {                       // retail sizeof 0x18 (PDB 0x1C with a 16-byte CArray)
    CCharString Script;                          // +0x00
    CArrayByte  SaveData;                        // +0x04 CArray<unsigned char>
    long        UncompressedSize;                // +0x10
    bool        Valid;                           // +0x14
};

void __fastcall _Construct(CEntityScriptInfo* p, const CEntityScriptInfo& value)
{
    if (p != 0)
        new (p) CEntityScriptInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N0052b4e0;
    CEntityScriptInfo src;
    src.Script.m_pData = reinterpret_cast<void*>(17);
    src.SaveData._M_start = reinterpret_cast<unsigned char*>(0x22);
    src.UncompressedSize = 300;
    src.Valid = true;
    CEntityScriptInfo* slot = reinterpret_cast<CEntityScriptInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->Script.m_pData == src.Script.m_pData)) ok = false;
    if (!(slot->SaveData._M_start == src.SaveData._M_start)) ok = false;
    if (!(slot->UncompressedSize == src.UncompressedSize)) ok = false;
    if (!(slot->Valid == src.Valid)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("0052b4e0_TEST FAIL\n"); return 1; }
    std::printf("0052b4e0_TEST PASS\n");
    return 0;
}