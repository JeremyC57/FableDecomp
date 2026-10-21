// Behaviour test for 0091db50: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CAIStateGroup_EnemyCreatureBase::GetEnemy()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __int64 long long
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_EnemyCreatureBase::GetEnemy() -- the creature's current combat enemy when it is usable: not
// Grade: RELOCATION_MATCH (advisor) -- real noinline LowerBound body, single-exit `result`, and BOTH lookups spelled on
// `PBrain->PCreature` (no named creature local): the pure same-TU LowerBound lets VC7.1 CSE the creature load across the
// first call, but the second `+0x44` is a fresh expression, so retail's `lea edi,[esi+44h]` / `mov edi,[edi+4]` and
// `add esi,44h` / `mov esi,[esi+4]` come out instead of one map pointer kept live in edi. A named creature local used for
// both lookups CSEs the map address (ternary pick, template / impure LowerBound bodies did not change that).
// incapacitated, and either the targeting component's own target passing the group's lock test or a thing the
// creature may engage.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091DB50, length 228 bytes.
// Callees: CVectorMap::LowerBound @ 0x004365B0 (inlined GetInterface: type 8, bit 8 of +0x20; type 0x34, bit 20
// of +0x24), the type-8 component's virtual slot 29 (+0x74) GetEnemy, IsIncapacitated(CThing*) @ 0x0085F0C0
// (__fastcall), the type-0x34 component's target GetPItem thunk @ 0x007446A0 (+0xc; the manifest's _Cons_val label
// is propagated), this->virtual slot 21 (+0x54), the component's flag tests @ 0x00661600 (+0x2d || +0x31; label
// CTCFadeOutAndIn::IsFading propagated) / @ 0x00643B3B (float +0x18 compare), and the __fastcall engage test
// @ 0x008645E0 (creature, thing, 0; label CMainGameComponent::BeginInputLoading propagated).
// Shape: two inlined GetInterface lookups on the same creature (the second one advances esi to the map in
// place), nested ifs with two `mov eax,esi` hit returns and the shared null exit; the lock test is the ternary
// `UsesTargetLock() ? IsLockedA() : IsLockedB()` on the 0x34 component.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
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
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20();
    virtual bool UsesTargetLock();              // slot 21 (+0x54)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_HasExtraData {
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

struct CMessageEvent;
struct CMessageEventManager;
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

struct CTCCombatTarget {                        // interface type 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};

struct CTCTargeting {                           // interface type 0x34
    CThing* GetPTarget();                       // 0x007446A0 (pointer at +0xc)
    bool IsLockedFast();                        // 0x00661600 (+0x2d || +0x31)
    bool IsLockedSlow();                        // 0x00643B3B (float +0x18 compare)
};

bool __fastcall IsIncapacitated(CThing* thing);                       // 0x0085F0C0
bool __fastcall CanEngageThing(CThing* creature, CThing* thing, long flags);   // 0x008645E0

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CThing* GetEnemy();
};

CThing* CAIStateGroup_EnemyCreatureBase::GetEnemy()
{
    CThing* result = 0;
    CTCCombatTarget* combat;
    if (PBrain->PCreature->GetInterface(&combat, 8)) {
        CTCTargeting* targeting;
        if (PBrain->PCreature->GetInterface(&targeting, 0x34)) {
            CThing* enemy = combat->GetEnemy();
            if (enemy && !IsIncapacitated(enemy)) {
                if (enemy == targeting->GetPTarget()) {
                    if (UsesTargetLock() ? targeting->IsLockedFast() : targeting->IsLockedSlow())
                        result = enemy;
                } else if (CanEngageThing(PBrain->PCreature, enemy, 0))
                    result = enemy;
            }
        }
    }
    return result;
}

// ---- stubs ----
static char g_log[2048];
static void logf(const char* fmt, long a = 0, long b = 0) { char t[256]; _snprintf(t, sizeof t, fmt, a, b); strcat(g_log, t); strcat(g_log, ";"); }
CAIStateGroupBase::~CAIStateGroupBase() { }
#define V(n) void CAIStateGroupBase::n() { }
V(v01) V(v02) V(v03) V(v04) V(v05) V(v06) V(v07) V(v08) V(Cleanup) V(OnReplace) V(Update) V(v13) V(v15) V(v16) V(v17) V(v18) V(v19) V(v20)
#undef V
bool CAIStateGroupBase::IsRunnable() { return false; }
long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
static bool g_usesLock;
bool CAIStateGroupBase::UsesTargetLock() { logf("UsesTargetLock"); return g_usesLock; }
CWorld* CAIStateGroupBase::GetPWorld() { return 0; }
CWorld* CAIStateGroupBaseW::GetPThingWorld() { return 0; }
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
#define V(n) void CTCCombatTarget::n() { }
V(v00) V(v01) V(v02) V(v03) V(v04) V(v05) V(v06) V(v07) V(v08) V(v09) V(v10) V(v11) V(v12) V(v13) V(v14) V(v15) V(v16) V(v17) V(v18) V(v19) V(v20) V(v21) V(v22) V(v23) V(v24) V(v25) V(v26) V(v27) V(v28)
#undef V
static CThing* g_combatEnemy;
CThing* CTCCombatTarget::GetEnemy() { logf("Combat::GetEnemy"); return g_combatEnemy; }
static CThing* g_target; static bool g_lockedFast, g_lockedSlow, g_incapacitated, g_canEngage;
struct CTCTargetingImpl { unsigned char pad[0xc]; CThing* target; };
CThing* CTCTargeting::GetPTarget() { logf("GetPTarget"); return ((CTCTargetingImpl*)this)->target; }
bool CTCTargeting::IsLockedFast() { logf("IsLockedFast"); return g_lockedFast; }
bool CTCTargeting::IsLockedSlow() { logf("IsLockedSlow"); return g_lockedSlow; }
bool __fastcall IsIncapacitated(CThing* thing) { logf("IsIncapacitated(%ld)", (long)(thing == g_combatEnemy)); return g_incapacitated; }
static CThing* g_creature;
bool __fastcall CanEngageThing(CThing* creature, CThing* thing, long flags) { logf("CanEngage(%ld,%ld)", (long)(creature == g_creature && thing == g_combatEnemy), flags); return g_canEngage; }

int main()
{
    static unsigned char creatureBytes[0x100]; memset(creatureBytes, 0, sizeof creatureBytes);
    CThing* creature = (CThing*)creatureBytes; g_creature = creature;
    static CTCCombatTarget combat;
    static CTCTargetingImpl targetingImpl; memset(&targetingImpl, 0, sizeof targetingImpl);
    CTCTargeting* targeting = (CTCTargeting*)&targetingImpl;
    static CInterfaceMap::SEntry entries[5] = { {3, (CTCBase*)0x11}, {8, (CTCBase*)&combat}, {0x34, (CTCBase*)targeting}, {0x40, (CTCBase*)0x22}, {0, (CTCBase*)0} };
    creature->m_interfaces.first = entries; creature->m_interfaces.last = entries + 4;
    static CAIBrain brain; memset(&brain, 0, sizeof brain); brain.PCreature = creature;
    CAIStateGroup_EnemyCreatureBase* group = (CAIStateGroup_EnemyCreatureBase*)calloc(1, sizeof(CAIStateGroup_EnemyCreatureBase));
    new (group) CAIStateGroup_EnemyCreatureBase();
    group->PBrain = &brain;
    static unsigned char enemyBytes[0x100]; CThing* enemy = (CThing*)enemyBytes;

    // 1. no combat-target component: null, nothing called
    g_log[0] = 0;
    CHECK(group->GetEnemy() == 0);
    CHECK(strcmp(g_log, "") == 0);

    // 2. type 8 present, type 0x34 absent: null, no virtual call
    creature->m_presentBits[0] = 1UL << 8; g_log[0] = 0;
    CHECK(group->GetEnemy() == 0);
    CHECK(strcmp(g_log, "") == 0);

    // 3. both present, no enemy: null
    creature->m_presentBits[1] = 1UL << 20; g_combatEnemy = 0; g_log[0] = 0;
    CHECK(group->GetEnemy() == 0);
    CHECK(strcmp(g_log, "Combat::GetEnemy;") == 0);

    // 4. enemy incapacitated: null
    g_combatEnemy = enemy; g_incapacitated = true; g_log[0] = 0;
    CHECK(group->GetEnemy() == 0);
    CHECK(strcmp(g_log, "Combat::GetEnemy;IsIncapacitated(1);") == 0);

    // 5. enemy is the targeting target, lock test through the group's UsesTargetLock (fast / slow variants)
    g_incapacitated = false; targetingImpl.target = enemy;
    g_usesLock = true; g_lockedFast = true; g_log[0] = 0;
    CHECK(group->GetEnemy() == enemy);
    CHECK(strcmp(g_log, "Combat::GetEnemy;IsIncapacitated(1);GetPTarget;UsesTargetLock;IsLockedFast;") == 0);
    g_lockedFast = false; g_log[0] = 0;
    CHECK(group->GetEnemy() == 0);
    g_usesLock = false; g_lockedSlow = true; g_log[0] = 0;
    CHECK(group->GetEnemy() == enemy);
    CHECK(strcmp(g_log, "Combat::GetEnemy;IsIncapacitated(1);GetPTarget;UsesTargetLock;IsLockedSlow;") == 0);
    g_lockedSlow = false; g_log[0] = 0;
    CHECK(group->GetEnemy() == 0);

    // 6. enemy is not the target: the engage test on the brain's creature decides
    targetingImpl.target = 0; g_canEngage = true; g_log[0] = 0;
    CHECK(group->GetEnemy() == enemy);
    CHECK(strcmp(g_log, "Combat::GetEnemy;IsIncapacitated(1);GetPTarget;CanEngage(1,0);") == 0);
    g_canEngage = false; g_log[0] = 0;
    CHECK(group->GetEnemy() == 0);

    // 7. the lookup is a lower bound on the sorted map: a missing 0x34 entry with its bit set reads the end slot (null)
    static CInterfaceMap::SEntry entries2[3] = { {8, (CTCBase*)&combat}, {0x40, (CTCBase*)0x22}, {0, (CTCBase*)0} };
    creature->m_interfaces.first = entries2; creature->m_interfaces.last = entries2 + 2;
    g_combatEnemy = 0; g_log[0] = 0;
    CHECK(group->GetEnemy() == 0);

    if (fails == 0) printf("0091db50_TEST PASS\n"); else printf("0091db50_TEST FAIL\n");
    return fails != 0;
}