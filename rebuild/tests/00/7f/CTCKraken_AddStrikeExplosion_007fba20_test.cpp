// Behaviour test for 007fba20: src/007fba20.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCKraken::AddStrikeExplosion(CThing* explosion)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCKraken::AddStrikeExplosion(CThing* explosion) -- remembers a tentacle-strike explosion in the explosion list and tells its driver to leave the kraken and every living tentacle unharmed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FBA20, length 224 bytes (`ret 4`, + int3 padding); speed TU (no frame pointer).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem @ 0x00A01B50 and
// ~CBaseIntelligentPointer @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), malloc @ 0x00BFEA0E (the STLport list node),
// CExplosionDriver::AddExcludedThing(CThing*) @ 0x0080CBF0.
// Layout: retail CTCKraken ParentThing +0x04, ExplosionList +0x30 and Tentacles +0x44 (PDB +0x30/+0x4C: 4-byte list heads; node
// {next, prev, CIntelligentPointer}, 16 bytes). CThing: components bitset +0x20, PTCDriver +0x5C (PDB +0x6C), Dead = +0x91 bit 0.
// Shape: the explosion is wrapped in a local intelligent pointer (base ctor, vtable, SetPItem) that push_back copies into the
// malloc'd node (the placement-new null test, GetPItem on the source) and links before the header; the driver is the thing's
// driver comes from the landed GetTCDriver out-pointer template (0x00750DD0 shape: interface 0x41 -> the cached driver, the bitset
// test folds to `test [thing+28h],2`) and is not tested: the uninitialised out word shares the dead `explosion` argument slot, so
// the absent arm reads `[esp+1Ch]` and `this` stays in ebp (a plain value-returning getter caches the argument in ebp instead);
// the tentacle walk re-reads the header every pass and skips empty or dead pointers (two GetPItem calls per live tentacle).
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);    // 0x00BFEA0E
inline void* __cdecl operator new(size_t, void* place) { return place; }
extern "C" int  __cdecl puts(const char* text);    // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct CPresenceBits {                             // std::bitset<274>
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
class CThing;
struct CExplosionDriver {                           // the explosion thing's driver component (interface 0x41)
    void AddExcludedThing(CThing* thing);          // 0x0080CBF0
};
class CThing { public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                  // +0x20
    unsigned char _pad_0x44[0x5c - 0x44];
    CExplosionDriver* PTCDriver;                    // +0x5c
    unsigned char _pad_0x60[0x91 - 0x60];
    unsigned char Dead : 1;                         // +0x91 bit 0
    bool GetTCDriver(CExplosionDriver** out)        // the landed 0x00750DD0 template: interface 0x41 present -> its cached driver
    {
        if ((UsingComponents.words[2] & 2) != 0) {
            *out = PTCDriver;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CThing* item);                    // 0x00A01B90
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SThingPointerNode {                          // STLport _List_node<CIntelligentPointer<CThing>> (16 bytes)
    SThingPointerNode*  _M_next;
    SThingPointerNode*  _M_prev;
    CIntelligentPointer _M_data;
};
struct CThingPointerList {                          // std::list<CIntelligentPointer<CThing>>: the header node
    SThingPointerNode* _M_node;
    SThingPointerNode* begin() const { return _M_node->_M_next; }
    SThingPointerNode* end() const { return _M_node; }
    SThingPointerNode* insert(SThingPointerNode* position, const CIntelligentPointer& value)
    {
        SThingPointerNode* node = (SThingPointerNode*)malloc(sizeof(SThingPointerNode));
        new (&node->_M_data) CIntelligentPointer(value);
        SThingPointerNode* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
        return node;
    }
    void push_back(const CIntelligentPointer& value) { insert(end(), value); }
};

class CTCKraken { public:
    unsigned char     _pad00[0x04];
    CThing*           ParentThing;                  // +0x04
    unsigned char     _pad08[0x30 - 0x08];
    CThingPointerList ExplosionList;                // +0x30
    unsigned char     _pad34[0x44 - 0x34];
    CThingPointerList Tentacles;                    // +0x44
    void AddStrikeExplosion(CThing* explosion);
};
void CTCKraken::AddStrikeExplosion(CThing* explosion)
{
    ExplosionList.push_back(CIntelligentPointer(explosion));
    CExplosionDriver* driver;
    explosion->GetTCDriver(&driver);
    driver->AddExcludedThing(ParentThing);
    for (SThingPointerNode* it = Tentacles.begin(); it != Tentacles.end(); it = it->_M_next) {
        CIntelligentPointer& tentacle = it->_M_data;
        if (tentacle.GetPItem() && !tentacle.GetPItem()->Dead)
            driver->AddExcludedThing(tentacle.GetPItem());
    }
}

// ---- stubs and driver ----
extern "C" void* __cdecl calloc(size_t count, size_t size);
static int g_ctors = 0, g_dtors = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() { PItem = 0; ++g_ctors; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { PItem = item; }
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointer::v00() { }
static CThing* g_excluded[8]; static CExplosionDriver* g_by[8]; static int g_nExcluded = 0;
void CExplosionDriver::AddExcludedThing(CThing* thing) { g_by[g_nExcluded] = this; g_excluded[g_nExcluded++] = thing; }
static SThingPointerNode* NewNode() { return (SThingPointerNode*)calloc(1, sizeof(SThingPointerNode)); }
static CThing* NewThing() { return (CThing*)calloc(1, 0xa0); }
int main()
{
    CTCKraken* k = (CTCKraken*)calloc(1, sizeof(CTCKraken));
    CThing* parent = NewThing(); k->ParentThing = parent;
    SThingPointerNode& eh = *NewNode(); eh._M_next = &eh; eh._M_prev = &eh; k->ExplosionList._M_node = &eh;
    CThing* t1 = NewThing(); CThing* t2 = NewThing(); CThing* t3 = NewThing();
    t2->Dead = 1;
    SThingPointerNode& th = *NewNode(); SThingPointerNode& n1 = *NewNode(); SThingPointerNode& n2 = *NewNode();
    SThingPointerNode& n3 = *NewNode(); SThingPointerNode& n4 = *NewNode();
    new (&n1._M_data) CIntelligentPointer(t1); new (&n2._M_data) CIntelligentPointer(t2);
    new (&n3._M_data) CIntelligentPointer((CThing*)0); new (&n4._M_data) CIntelligentPointer(t3);
    th._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &n4; n4._M_next = &th;
    th._M_prev = &n4; n4._M_prev = &n3; n3._M_prev = &n2; n2._M_prev = &n1; n1._M_prev = &th;
    k->Tentacles._M_node = &th;
    CThing* explosion = NewThing();
    CExplosionDriver* driver = (CExplosionDriver*)calloc(1, 16);
    explosion->UsingComponents.words[2] = 2; explosion->PTCDriver = driver;
    int before = g_ctors;
    k->AddStrikeExplosion(explosion);
    CHECK(eh._M_next != &eh && eh._M_next == eh._M_prev);
    CHECK(eh._M_next->_M_data.PItem == explosion && eh._M_next->_M_next == &eh && eh._M_next->_M_prev == &eh);
    CHECK(g_ctors - before == 2 && g_dtors == 1);   // the local and the node copy; the local destroyed
    CHECK(g_nExcluded == 3);
    CHECK(g_excluded[0] == parent && g_excluded[1] == t1 && g_excluded[2] == t3);
    CHECK(g_by[0] == driver && g_by[1] == driver && g_by[2] == driver);
    CThing* explosion2 = NewThing(); explosion2->UsingComponents.words[2] = 2; explosion2->PTCDriver = driver;
    k->AddStrikeExplosion(explosion2);   // appended at the back
    CHECK(eh._M_prev->_M_data.PItem == explosion2 && eh._M_next->_M_data.PItem == explosion && eh._M_next->_M_next == eh._M_prev);
    CHECK(g_nExcluded == 6);
    if (fails == 0) printf("007fba20_TEST PASS\n");
    return fails;
}
