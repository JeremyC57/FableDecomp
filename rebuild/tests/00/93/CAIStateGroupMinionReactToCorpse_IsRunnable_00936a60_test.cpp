// Behaviour test for 00936a60: src/00936a60.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CAIStateGroup_MinionReactToCorpse::IsRunnable()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_MinionReactToCorpse::IsRunnable() -- find the nearest dead creature, latch it, and react when it is
// still around, its corpse component (0x40) is not yet handled, it was an ally, and the creature can see its spot.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00936A60, length 304 bytes (+ int3 padding).
// Callees: the physics-position accessor @ 0x006A4D00, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestLeafThing<CThingFilter_None, CThingCompare_Nearest, CThingDeadCreature> @ 0x00936950,
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (vtable 0x0128B080), CVectorMap::LowerBound @ 0x0040F020
// (0x40 = bit 0 of +0x28, entry form), AreThingsAllies @ 0x0076D4F0 (__fastcall), CThing::GetPosition @ 0x004C73D0,
// the creature's look test (const C3DVector&, bool) @ 0x00665260 (manifest label propagated).
// Shape: the dead creature is zeroed up front and filled through the out slot; the empty CThingFilter_None is one
// frame byte; the final call's bool is returned directly.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char m_flags92;                    // +0x92
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const       // std::bitset::test: `shr ; and cl,1` (kept as a byte when reused)
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the bitset::test form: the test's bool is materialised (`shr ; and dl,1 ; mov [esp+x],dl`) and returned
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
    // the ternary form (landed 0x00954690): the end/hit choice is ONE ternary, so the component lands straight in
    // the register of its use (ecx for a call on it); the miss path reads the never-written slot
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    // the KidCleanup form (landed 0x00915190): `if (mask) *pp = Lookup();` with an entry-value lookup
    __forceinline CTCBase* LookupValue(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap::SEntry* p = m_interfaces.LowerBound(&type);
        if (p == m_interfaces.last) p = m_interfaces.last;
        else if (p->type > interfaceType) p = m_interfaces.last;
        return p->component;
    }
    template<class T> __forceinline void GetInterfaceK(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            *ppOut = (T*)LookupValue(interfaceType);
    }
    // GetInterface<T>() returning the component: hit = the entry's component, miss = the never-written local
    template<class T> __forceinline T* PeekInterface(long interfaceType)
    {
        T* component;
        if (HasTC(interfaceType))
            component = (T*)FindEntry(interfaceType)->component;
        return component;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DVector { public: float x, y, z; };
class CThingPhysical : public CThing { public:
    const C3DVector& GetPhysicsPosition();      // 0x006A4D00
};
struct CThingPos : CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CThingCreatureLook : CThing {
    bool CanSeePosition(const C3DVector& pos, bool checkLineOfSight);  // 0x00665260
};
bool __fastcall AreThingsAllies(CThing* a, CThing* b);   // 0x0076D4F0
class CThingFilter_None { public:
    CThingFilter_None() { }
};
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
class CThingDeadCreature;
class CThingSearchTools { public:
    bool GetPBestLeafThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare,
        CThingDeadCreature** out);              // 0x00936950
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the search GetPBestLeafThing runs (returns the best leaf thing or 0)
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestLeafThing @ 0x00936950 (compiled in retail's TU): clears the out pointer, stores the best
// thing found, reads the comparator through its const reference; no argument escapes.
__declspec(noinline) bool CThingSearchTools::GetPBestLeafThing(const CThingFilter_None& filter, const CThingCompare_Nearest& compare, CThingDeadCreature** out)
{
    *out = 0;
    long best = NearSink_s7_0(this, *(const long*)&compare);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)
struct CTCCorpse {                              // interface 0x40
    unsigned char _pad[0x0c];
    bool m_handled;                             // +0x0c
};
struct CIntelligentPointerDead : CBaseIntelligentPointer {   // CIntelligentPointer<CThingDeadCreature> (vtable 0x0128B080)
    virtual void v00();
    CIntelligentPointerDead(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerDead& operator=(const CIntelligentPointerDead& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CAIStateGroup_MinionReactToCorpse : CAIStateGroupBase {
    CIntelligentPointerDead PCorpse;            // +0x14
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionReactToCorpse::IsRunnable()
{
    CThingDeadCreature* corpse = 0;
    if (PeekThingSearchTools()->GetPBestLeafThing(CThingFilter_None(),
            CThingCompare_Nearest(((CThingPhysical*)PBrain->PCreature)->GetPhysicsPosition()), &corpse)) {
        PCorpse = CIntelligentPointerDead((CThing*)corpse);
        if (IS_THING_ALIVE(PCorpse)) {
            CThing* dead = PCorpse.GetPItem();
            CTCCorpse* state;
            if (dead->HasTC(0x40) && (dead->GetInterfaceV(&state, 0x40), !state->m_handled)) {
                CThing* creature = PBrain->PCreature;
                if (AreThingsAllies(creature, PCorpse.GetPItem())) {
                    CThingCreatureLook* looker = (CThingCreatureLook*)PBrain->PCreature;
                    return looker->CanSeePosition(((CThingPos*)PCorpse.GetPItem())->GetPosition(), true);
                }
            }
        }
    }
    return false;
}

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
void CAIStateGroupBase::v01() {}
void CAIStateGroupBase::v02() {}
void CAIStateGroupBase::v03() {}
void CAIStateGroupBase::v04() {}
void CAIStateGroupBase::v05() {}
void CAIStateGroupBase::v06() {}
void CAIStateGroupBase::v07() {}
void CAIStateGroupBase::v08() {}
void CAIStateGroupBase::v13() {}
void CAIStateGroupBase::v15() {}
void CAIStateGroupBase::v16() {}
void CAIStateGroupBase::v17() {}
void CIntelligentPointerDead::v00() {}
static long g_best = 0; static int g_search = 0, g_set = 0, g_ctor = 0, g_dtor = 0, g_allies = 0, g_see = 0, g_lb = 0; static bool g_alliesResult = true, g_seeResult = true;
static CThingSearchTools g_tools; static C3DVector g_phys = { 1, 2, 3 }, g_pos = { 4, 5, 6 }; static const C3DVector* g_seenPos = 0; static bool g_los = false;
long NearSink_s7_0(const void* self, long seen) { ++g_search; CHECK(self == &g_tools); return g_best; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CBaseIntelligentPointer::CBaseIntelligentPointer() { ++g_ctor; m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtor; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { ++g_set; m_pItem = item; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { ++g_lb; SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
const C3DVector& CThingPhysical::GetPhysicsPosition() { return g_phys; }
const C3DVector& CThingPos::GetPosition() { return g_pos; }
bool CThingCreatureLook::CanSeePosition(const C3DVector& pos, bool los) { ++g_see; g_seenPos = &pos; g_los = los; return g_seeResult; }
bool __fastcall AreThingsAllies(CThing* a, CThing* b) { ++g_allies; return g_alliesResult; }
CAIStateGroupBase::~CAIStateGroupBase() {}
void CAIStateGroupBase::Cleanup() {} void CAIStateGroupBase::OnReplace() {} bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() {} long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
CThingSearchTools* CAIStateGroup_MinionReactToCorpse::PeekThingSearchTools() { return &g_tools; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() { static unsigned long s; return &s; }
unsigned long __fastcall GFROR13(unsigned long v) { return v; }
long __fastcall GFGetCurrentFrame() { return 0; }
int main()
{
    static CThing creatureRaw; static CAIBrain brain; brain.PCreature = &creatureRaw;
    static CThing corpse; static CTCCorpse corpseTC; corpseTC.m_handled = false;
    CInterfaceMap::SEntry entries[1] = { { 0x40, (CTCBase*)&corpseTC } };
    corpse.m_interfaces.first = entries; corpse.m_interfaces.last = entries + 1; corpse.m_presentBits[2] = 1UL << 0;   // 0x40 >> 5 = 2, bit 0
    static unsigned char raw[sizeof(CAIStateGroup_MinionReactToCorpse)]; CAIStateGroup_MinionReactToCorpse& g = *(CAIStateGroup_MinionReactToCorpse*)raw;
    g.PBrain = &brain; g.PCorpse.m_pItem = 0;
    g_best = 0; CHECK(g.CAIStateGroup_MinionReactToCorpse::IsRunnable() == false && g_search == 1 && g_set == 0);
    g_best = (long)&corpse; CHECK(g.CAIStateGroup_MinionReactToCorpse::IsRunnable() == true); CHECK(g.PCorpse.m_pItem == &corpse && g_allies == 1 && g_see == 1 && g_seenPos == &g_pos && g_los);
    corpse.m_flags91 = 1; CHECK(g.CAIStateGroup_MinionReactToCorpse::IsRunnable() == false && g_allies == 1);   // dead flag: not alive
    corpse.m_flags91 = 0; corpseTC.m_handled = true; CHECK(g.CAIStateGroup_MinionReactToCorpse::IsRunnable() == false && g_allies == 1);
    corpseTC.m_handled = false; g_alliesResult = false; CHECK(g.CAIStateGroup_MinionReactToCorpse::IsRunnable() == false && g_allies == 2 && g_see == 1);
    g_alliesResult = true; g_seeResult = false; CHECK(g.CAIStateGroup_MinionReactToCorpse::IsRunnable() == false && g_see == 2);
    corpse.m_presentBits[2] = 0; g_seeResult = true; CHECK(g.CAIStateGroup_MinionReactToCorpse::IsRunnable() == false && g_see == 2);   // no corpse interface
    CHECK(g_ctor == g_dtor);   // every temporary pointer destroyed
    if (fails == 0) printf("00936a60_TEST PASS\n"); else printf("00936a60_TEST FAIL\n");
    return fails;
}