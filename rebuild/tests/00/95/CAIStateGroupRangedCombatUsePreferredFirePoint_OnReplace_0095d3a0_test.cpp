// Behaviour test for 0095d3a0: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
#define snprintf _snprintf
#else
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace() -- release the claimed fire point (its 0xAC
// component's user cleared, the pointer reset), then, for a live creature, deregister it from its combat manager
// when it was a registered combatant and drop the ranged-combat mode (0x19).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D3A0, length 285 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PFirePoint x3), CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface: 0xAC = the fire-point component bit 12 of +0x34, unconditional; 0x31 = CTCCreatureModeManager
// bit 17 of +0x24, tested), the fire point's SetUser(CThing*) @ 0x0087C390 (manifest "CVillageTaskCarryCrate::
// AssignCreatureToTask" is propagated), CBaseIntelligentPointer ctor/SetPItem/dtor (null temporary, vtable
// 0x01238C6C), the creature's registered-combatant test @ 0x00662410 (manifest "C3DAnimationBlendState::
// AddCombinationBlend" is propagated), CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::
// DeregisterCombatant @ 0x006ED9C0, CTCCreatureModeManager::RemoveMode @ 0x0073ABA0.
// Engine header names (rebuild/include/engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h; flattened base).
// Shape: the fire-point lookup is not tested, so the absent arm calls SetUser through the never-written local (it reads
// the dead key frame word straight into ecx, as retail); the call is spelled in BOTH arms, hit arm on the lookup
// expression over a named thing local, so VC7.1 cross-jumps them with the component already in ecx (a joined pointer
// copies it through eax, +2 bytes). The mode-manager lookup IS tested (out-parameter GetInterface).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

struct CThing {
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CAIStateGroupBase {
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
struct CThingCreatureBase;
struct CTCFirePoint {                           // interface 0xAC
    void SetUser(CThing* user);                 // 0x0087C390
};
struct CCombatManager {
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
};
struct CThingCreatureBase : CThing {
    bool IsRegisteredCombatant();               // 0x00662410
    CCombatManager* GetCombatManager();         // 0x00661ED0
};
struct CTCCreatureModeManager {                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

struct CAIStateGroup_RangedCombatUsePreferredFirePoint : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CIntelligentPointerThing PFirePoint;        // +0x1c CIntelligentPointer<CThing>
    long TimeOutCounter;                        // +0x24
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    virtual void OnReplace();
};

void CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace()
{
    if (IS_THING_ALIVE(PFirePoint)) {
        CTCFirePoint* point;
        CThing* thing = PFirePoint.GetPItem();
        if (thing->HasTC(0xac))
            ((CTCFirePoint*)thing->FindEntry(0xac)->component)->SetUser(0);
        else
            point->SetUser(0);
        PFirePoint = CIntelligentPointerThing(0);
    }
    CThingCreatureBase* creature = GetCreature();
    if (creature && !(creature->m_flags91 & 1)) {
        if (creature->IsRegisteredCombatant())
            GetCreature()->GetCombatManager()->DeregisterCombatant(GetCreature());
        CTCCreatureModeManager* modes;
        if (GetCreature()->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x19);
    }
}

// ---- stubs ----
static char g_log[4096];
static void logf(const char* fmt, long a = 0, long b = 0, long c = 0) { char t[256]; snprintf(t, sizeof t, fmt, a, b, c); strcat(g_log, t); strcat(g_log, ";"); }
CAIStateGroupBase::~CAIStateGroupBase() { }
#define V(n) void CAIStateGroupBase::n() { }
V(v01) V(v02) V(v03) V(v04) V(v05) V(v06) V(v07) V(v08) V(Cleanup) V(OnReplace) V(Update) V(v13) V(v15) V(v16) V(v17)
#undef V
bool CAIStateGroupBase::IsRunnable() { return false; }
long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return 0; }
long __fastcall GFGetCurrentFrame() { return 0; }
unsigned long __fastcall GFROR13(unsigned long value) { return value; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; logf("ctor"); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { logf("dtor"); }
void CBaseIntelligentPointer::SetPItem(CThing* item) { logf("SetPItem(%ld)", (long)(item != 0)); m_pItem = item; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static CTCFirePoint g_firePoint; static CTCCreatureModeManager g_modes; static CCombatManager g_manager;
static bool g_registered; static CThingCreatureBase* g_creature;
// the miss path of the fire-point lookup calls through a never-written local: the stub does not touch `this`
void CTCFirePoint::SetUser(CThing* user) { logf("SetUser(%ld,%ld)", (long)(this == &g_firePoint), (long)(user == 0)); }
void CCombatManager::DeregisterCombatant(CThingCreatureBase* creature) { logf("Deregister(%ld,%ld)", (long)(this == &g_manager), (long)(creature == g_creature)); }
bool CThingCreatureBase::IsRegisteredCombatant() { logf("IsRegisteredCombatant"); return g_registered; }
CCombatManager* CThingCreatureBase::GetCombatManager() { logf("GetCombatManager"); return &g_manager; }
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { logf("RemoveMode(%ld,%ld)", (long)(this == &g_modes), (long)mode); }

int main()
{
    static unsigned char creatureBytes[0x100]; memset(creatureBytes, 0, sizeof creatureBytes);
    static unsigned char pointBytes[0x100]; memset(pointBytes, 0, sizeof pointBytes);
    g_creature = (CThingCreatureBase*)creatureBytes; CThing* point = (CThing*)pointBytes;
    static CInterfaceMap::SEntry creatureEntries[3] = { {3, (CTCBase*)0x11}, {0x31, (CTCBase*)&g_modes}, {0, (CTCBase*)0} };
    static CInterfaceMap::SEntry pointEntries[3] = { {0x40, (CTCBase*)0x22}, {0xac, (CTCBase*)&g_firePoint}, {0, (CTCBase*)0} };
    g_creature->m_interfaces.first = creatureEntries; g_creature->m_interfaces.last = creatureEntries + 2;
    point->m_interfaces.first = pointEntries; point->m_interfaces.last = pointEntries + 2;
    static CAIBrain brain; memset(&brain, 0, sizeof brain);
    // the group has no default constructor (its CIntelligentPointer members take an item): zero-filled storage, the method called statically
    CAIStateGroup_RangedCombatUsePreferredFirePoint* group = (CAIStateGroup_RangedCombatUsePreferredFirePoint*)calloc(1, sizeof(*group));
    group->PBrain = &brain;

    // 1. no fire point, no creature: nothing happens
    group->PFirePoint.m_pItem = 0; brain.PCreature = 0; g_log[0] = 0;
    group->CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace();
    CHECK(strcmp(g_log, "") == 0);

    // 2. live fire point with its 0xAC component, registered live creature with a mode manager
    group->PFirePoint.m_pItem = point; point->m_presentBits[5] = 1UL << 12;
    brain.PCreature = g_creature; g_creature->m_presentBits[1] = 1UL << 17; g_registered = true; g_log[0] = 0;
    group->CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace();
    CHECK(strcmp(g_log, "SetUser(1,1);ctor;SetPItem(0);SetPItem(0);dtor;IsRegisteredCombatant;GetCombatManager;Deregister(1,1);RemoveMode(1,25);") == 0);
    CHECK(group->PFirePoint.m_pItem == 0);

    // 3. dead fire point (+0x91 bit 0): left alone; unregistered creature without a mode manager: nothing more
    group->PFirePoint.m_pItem = point; point->m_flags91 = 1; g_registered = false; g_creature->m_presentBits[1] = 0; g_log[0] = 0;
    group->CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace();
    CHECK(strcmp(g_log, "IsRegisteredCombatant;") == 0);
    CHECK(group->PFirePoint.m_pItem == point);

    // 4. dead creature: skipped entirely; live fire point released
    point->m_flags91 = 0; g_creature->m_flags91 = 1; g_log[0] = 0;
    group->CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace();
    CHECK(strcmp(g_log, "SetUser(1,1);ctor;SetPItem(0);SetPItem(0);dtor;") == 0);

    // 5. fire point without the 0xAC component: retail still calls SetUser (through the never-written local; the stub ignores `this`)
    group->PFirePoint.m_pItem = point; point->m_presentBits[5] = 0; g_creature->m_flags91 = 0; g_log[0] = 0;
    group->CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace();
    CHECK(strncmp(g_log, "SetUser(", 8) == 0);
    CHECK(strstr(g_log, ",1);ctor;SetPItem(0);SetPItem(0);dtor;IsRegisteredCombatant;") != 0);

    // 6. lower-bound end-slot pick: the 0xAC bit set with no 0xAC entry
    static CInterfaceMap::SEntry pointEntries2[2] = { {0x40, (CTCBase*)0x22}, {0, (CTCBase*)&g_firePoint} };
    point->m_interfaces.first = pointEntries2; point->m_interfaces.last = pointEntries2 + 1;
    group->PFirePoint.m_pItem = point; point->m_presentBits[5] = 1UL << 12; g_log[0] = 0;
    group->CAIStateGroup_RangedCombatUsePreferredFirePoint::OnReplace();
    CHECK(strncmp(g_log, "SetUser(1,1);", 13) == 0);

    if (fails == 0) printf("0095d3a0_TEST PASS\n"); else printf("0095d3a0_TEST FAIL\n");
    return fails != 0;
}