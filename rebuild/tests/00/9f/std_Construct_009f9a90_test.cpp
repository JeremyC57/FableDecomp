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
namespace N009f9a90 {
struct CProfile {
    unsigned char _body[0x48];
    CProfile() { std::memset(this, 0, sizeof(CProfile)); }
    CProfile(const CProfile& other) { std::memcpy(this, &other, sizeof(CProfile)); ++g_extCopies; }
};
struct CChildVector {                                 // retail 12-byte STLport-vector body (void*)
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    CChildVector() { std::memset(this, 0, sizeof(CChildVector)); }
    CChildVector(const CChildVector& other) { std::memcpy(this, &other, sizeof(CChildVector)); ++g_extCopies; }
};
struct CProfileTreeEntry {                       // CProfileManager::CProfileTreeEntry (retail sizeof 0x5C)
    CProfile           Profile;                  // +0x00
    long               ID;                       // +0x48
    CChildVector       Children;                 // +0x4C vector<CProfileTreeEntry*>
    CProfileTreeEntry* Parent;                   // +0x58
};

void __fastcall _Construct(CProfileTreeEntry* p, const CProfileTreeEntry& value)
{
    if (p != 0)
        new (p) CProfileTreeEntry(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N009f9a90;
    CProfileTreeEntry src;
    src.Profile._body[5] = 7;
    src.ID = 42;
    src.Children._M_start = reinterpret_cast<void**>(0x10);
    src.Parent = reinterpret_cast<CProfileTreeEntry*>(0x20);
    CProfileTreeEntry* slot = reinterpret_cast<CProfileTreeEntry*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->Profile._body[5] == src.Profile._body[5])) ok = false;
    if (!(slot->ID == src.ID)) ok = false;
    if (!(slot->Children._M_start == src.Children._M_start)) ok = false;
    if (!(slot->Parent == src.Parent)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("009f9a90_TEST FAIL\n"); return 1; }
    std::printf("009f9a90_TEST PASS\n");
    return 0;
}