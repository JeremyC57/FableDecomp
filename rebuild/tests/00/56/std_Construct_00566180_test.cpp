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
namespace N00566180 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CWideString {
    void* m_pData;                               // ref-counted wide string body
    CWideString() { std::memset(this, 0, sizeof(CWideString)); }
    CWideString(const CWideString& other) { std::memcpy(this, &other, sizeof(CWideString)); ++g_extCopies; }
};
struct CClanInfo {                               // CTCHeroOnlineScoreboard::CClanInfo (PDB, sizeof 0x14)
    CWideString Name;                            // +0x00
    long        ClanID;                          // +0x04
    CWideString Description;                     // +0x08
    CCharString HomePageURL;                     // +0x0C
    int         NumMembers;                      // +0x10
    CClanInfo() { }
    __forceinline CClanInfo(const CClanInfo& o)   // the implicit copy, spelled out so VC7.1 inlines it into _Construct
        : Name(o.Name), ClanID(o.ClanID), Description(o.Description), HomePageURL(o.HomePageURL), NumMembers(o.NumMembers) { }
};

void __fastcall _Construct(CClanInfo* p, const CClanInfo& value)
{
    if (p != 0)
        new (p) CClanInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00566180;
    CClanInfo src;
    src.Name.m_pData = reinterpret_cast<void*>(17);
    src.ClanID = 77;
    src.Description.m_pData = reinterpret_cast<void*>(18);
    src.HomePageURL.m_pData = reinterpret_cast<void*>(19);
    src.NumMembers = 8;
    CClanInfo* slot = reinterpret_cast<CClanInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 3 && g_links == 0;
    if (!(slot->Name.m_pData == src.Name.m_pData)) ok = false;
    if (!(slot->ClanID == src.ClanID)) ok = false;
    if (!(slot->Description.m_pData == src.Description.m_pData)) ok = false;
    if (!(slot->HomePageURL.m_pData == src.HomePageURL.m_pData)) ok = false;
    if (!(slot->NumMembers == src.NumMembers)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 3 || g_links != 0) ok = false;
    if (!ok) { std::printf("00566180_TEST FAIL\n"); return 1; }
    std::printf("00566180_TEST PASS\n");
    return 0;
}