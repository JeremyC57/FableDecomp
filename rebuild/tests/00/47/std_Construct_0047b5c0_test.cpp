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
namespace N0047b5c0 {
struct CWideString {
    void* m_pData;                               // ref-counted wide string body
    CWideString() { std::memset(this, 0, sizeof(CWideString)); }
    CWideString(const CWideString& other) { std::memcpy(this, &other, sizeof(CWideString)); ++g_extCopies; }
};
struct CHeroLogBookEntry {                       // PDB sizeof 0xC
    CWideString Name;                            // +0x00
    CWideString AbbreviatedName;                 // +0x04
    CWideString Content;                         // +0x08
};

void __fastcall _Construct(CHeroLogBookEntry* p, const CHeroLogBookEntry& value)
{
    if (p != 0)
        new (p) CHeroLogBookEntry(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N0047b5c0;
    CHeroLogBookEntry src;
    src.Name.m_pData = reinterpret_cast<void*>(17);
    src.AbbreviatedName.m_pData = reinterpret_cast<void*>(18);
    src.Content.m_pData = reinterpret_cast<void*>(19);
    CHeroLogBookEntry* slot = reinterpret_cast<CHeroLogBookEntry*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 3 && g_links == 0;
    if (!(slot->Name.m_pData == src.Name.m_pData)) ok = false;
    if (!(slot->AbbreviatedName.m_pData == src.AbbreviatedName.m_pData)) ok = false;
    if (!(slot->Content.m_pData == src.Content.m_pData)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 3 || g_links != 0) ok = false;
    if (!ok) { std::printf("0047b5c0_TEST FAIL\n"); return 1; }
    std::printf("0047b5c0_TEST PASS\n");
    return 0;
}