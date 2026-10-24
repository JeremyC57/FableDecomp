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
namespace N005661c0 {
struct CWideString {
    void* m_pData;                               // ref-counted wide string body
    CWideString() { std::memset(this, 0, sizeof(CWideString)); }
    CWideString(const CWideString& other) { std::memcpy(this, &other, sizeof(CWideString)); ++g_extCopies; }
};
struct CClanMember {                             // CTCHeroOnlineScoreboard::CClanMember (PDB, sizeof 0xC)
    CWideString Name;                            // +0x00
    long        MemberID;                        // +0x04
    CWideString Description;                     // +0x08
};

void __fastcall _Construct(CClanMember* p, const CClanMember& value)
{
    if (p != 0)
        new (p) CClanMember(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N005661c0;
    CClanMember src;
    src.Name.m_pData = reinterpret_cast<void*>(17);
    src.MemberID = 78;
    src.Description.m_pData = reinterpret_cast<void*>(18);
    CClanMember* slot = reinterpret_cast<CClanMember*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->Name.m_pData == src.Name.m_pData)) ok = false;
    if (!(slot->MemberID == src.MemberID)) ok = false;
    if (!(slot->Description.m_pData == src.Description.m_pData)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("005661c0_TEST FAIL\n"); return 1; }
    std::printf("005661c0_TEST PASS\n");
    return 0;
}