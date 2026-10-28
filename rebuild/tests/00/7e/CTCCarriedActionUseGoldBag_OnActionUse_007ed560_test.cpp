// Behaviour test for 007ed560: src/007ed560.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCCarriedActionUseGoldBag::OnActionUse (vfunc_28)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCarriedActionUseGoldBag::OnActionUse(CThingAICreature* user) -- a creature with gold uses a carried gold bag on a targeted creature (type 2): give it the default gift (50) or the receiver's own asking amount; then report the use to the bag's interface-0x1A component
// Leaf: vfunc_28
// Retail 0x007ED560, 368 bytes, `ret 4` (vtable slot 10, +0x28; catalogued as vfunc_28).
// Callees: the base use hook 0x007EAB90, CVectorMap::LowerBound const 0x004365B0 (interfaces 8, 4, 0x80) and 0x0040F020
// (0x1A), CTCTargetingBase::IsTargetedThingOfType 0x007F5A70 and slots 38 (+0x98) / 33 (+0x84), the default gold gift
// 0x007898F0 (returns 50), the receiver's 0x80 component gift amount 0x00789900, CCreatureAction_GiveMoneyToThing ctor
// 0x007ED450 / dtor 0x007ED420, CThingCreatureBase::SetCurrentAction 0x006644F0, the 0x1A component's use hook 0x005D8E50.
// Layout: the landed CTCCarriedActionUseGive::vfunc_28 0x007ED170 model (copied); hero stats gold +0x3C.
// Shape: HasTC + const ternary GetTC lookups on the user (the map pointer stays in esi for the stats lookup); the action is
// a temporary passed straight to SetCurrentAction; the 0x1A report runs on every path.
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
class CTCBase;
class CThing;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
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

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTCConst(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CCreatureActionBase { public:                                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    ~CCreatureActionBase();                                 // 0x00693EF0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
};
class CThingCreatureBase;
class CCreatureAction_GiveMoneyToThing : public CCreatureActionBase { public:
    unsigned char _body[0xb4 - 0xa8];
    CCreatureAction_GiveMoneyToThing(CThingCreatureBase* giver, CThing* receiver, long amount);   // 0x007ED450
    ~CCreatureAction_GiveMoneyToThing();                    // 0x007ED420
};
class CThingAICreature : public CThing { public:
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0 (CThingCreatureBase)
};
class CTCTargetingBase { public:                                   // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTargetedThing();                     // slot 33 (+0x84)
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual bool CanInteractWithTarget(long type);          // slot 38 (+0x98)
    bool IsTargetedThingOfType(long type);                  // 0x007F5A70
};
struct CTCInterface1A {                                     // interface 0x1a component of the carried thing
    void OnUseFailed();                                     // 0x005D8E50
};
class CTCCarriedActionUseBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThingAICreature* user);                 // 0x007EAB90 (empty base implementation)
};
long GetDefaultGoldGift();                                 // 0x007898F0
class CTCHeroStats { public:                                       // interface 4
    unsigned char _pad00[0x3c];
    long          Gold;                                     // +0x3c
};
struct CTCGoldReceiver {                                    // interface 0x80
    long GetGoldGiftAmount();                               // 0x00789900
};
struct CTCCarriedActionUseGoldBag : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user);
};

void CTCCarriedActionUseGoldBag::vfunc_28(CThingAICreature* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    if (user->HasTC(8)) {
        CTCTargetingBase* targeting = user->GetTCConst<CTCTargetingBase>(8);
        CTCHeroStats* stats;
        if (targeting->IsTargetedThingOfType(2) && targeting->CanInteractWithTarget(2)
            && user->GetInterface(&stats, 4) && stats->Gold > 0) {
            long amount = GetDefaultGoldGift();
            CThing* receiver = targeting->GetTargetedThing();
            if (receiver->HasTC(0x80))
                amount = receiver->GetTCConst<CTCGoldReceiver>(0x80)->GetGoldGiftAmount();
            if (amount > 0)
                user->SetCurrentAction(&CCreatureAction_GiveMoneyToThing((CThingCreatureBase*)user,
                                                                         targeting->GetTargetedThing(), amount));
        }
    }
    if (ParentThing->HasTC(0x1a))
        ParentThing->GetTC<CTCInterface1A>(0x1a)->OnUseFailed();
}

// ---- stubs ----
void CCreatureActionBase::v00() {}
CCreatureActionBase::~CCreatureActionBase() {}
static int g_ctors, g_dtors; static void* g_ctorThis; static void* g_dtorThis; static CThingCreatureBase* g_giver; static CThing* g_recv; static long g_amount;
CCreatureAction_GiveMoneyToThing::CCreatureAction_GiveMoneyToThing(CThingCreatureBase* giver, CThing* receiver, long amount)
{ ++g_ctors; g_ctorThis = this; g_giver = giver; g_recv = receiver; g_amount = amount; }
CCreatureAction_GiveMoneyToThing::~CCreatureAction_GiveMoneyToThing() { ++g_dtors; g_dtorThis = this; }
static int g_sets; static CCreatureActionBase* g_setAction; static CThingAICreature* g_setThis; static int g_setDtorsBefore;
bool CThingAICreature::SetCurrentAction(CCreatureActionBase* action) { ++g_sets; g_setAction = action; g_setThis = this; g_setDtorsBefore = g_dtors; return true; }
void CTCTargetingBase::v00() {}
void CTCTargetingBase::v01() {}
void CTCTargetingBase::v02() {}
void CTCTargetingBase::v03() {}
void CTCTargetingBase::v04() {}
void CTCTargetingBase::v05() {}
void CTCTargetingBase::v06() {}
void CTCTargetingBase::v07() {}
void CTCTargetingBase::v08() {}
void CTCTargetingBase::v09() {}
void CTCTargetingBase::v10() {}
void CTCTargetingBase::v11() {}
void CTCTargetingBase::v12() {}
void CTCTargetingBase::v13() {}
void CTCTargetingBase::v14() {}
void CTCTargetingBase::v15() {}
void CTCTargetingBase::v16() {}
void CTCTargetingBase::v17() {}
void CTCTargetingBase::v18() {}
void CTCTargetingBase::v19() {}
void CTCTargetingBase::v20() {}
void CTCTargetingBase::v21() {}
void CTCTargetingBase::v22() {}
void CTCTargetingBase::v23() {}
void CTCTargetingBase::v24() {}
void CTCTargetingBase::v25() {}
void CTCTargetingBase::v26() {}
void CTCTargetingBase::v27() {}
void CTCTargetingBase::v28() {}
void CTCTargetingBase::v29() {}
void CTCTargetingBase::v30() {}
void CTCTargetingBase::v31() {}
void CTCTargetingBase::v32() {}
void CTCTargetingBase::v34() {}
void CTCTargetingBase::v35() {}
void CTCTargetingBase::v36() {}
void CTCTargetingBase::v37() {}
static CThing* g_target; static int g_getTargets; static bool g_canInteract; static long g_canType; static bool g_isType; static long g_isTypeArg;
CThing* CTCTargetingBase::GetTargetedThing() { ++g_getTargets; return g_target; }
bool CTCTargetingBase::CanInteractWithTarget(long type) { g_canType = type; return g_canInteract; }
bool CTCTargetingBase::IsTargetedThingOfType(long type) { g_isTypeArg = type; return g_isType; }
static int g_failed; static CTCInterface1A* g_failedThis;
void CTCInterface1A::OnUseFailed() { ++g_failed; g_failedThis = this; }
void CTCCarriedActionUseBase::v00() {}
static int g_base; static CThingAICreature* g_baseUser; static int g_order;
void CTCCarriedActionUseBase::OnUseBase(CThingAICreature* user) { ++g_base; g_baseUser = user; g_order = g_failed; }
long GetDefaultGoldGift() { return 50; }
static long g_askAmount; static CTCGoldReceiver* g_askThis;
long CTCGoldReceiver::GetGoldGiftAmount() { g_askThis = this; return g_askAmount; }
static void Reset()
{
    g_ctors = g_dtors = 0; g_ctorThis = g_dtorThis = 0; g_giver = 0; g_recv = 0; g_amount = 0; g_sets = 0; g_setAction = 0; g_setThis = 0;
    g_getTargets = 0; g_canType = 0; g_isTypeArg = 0; g_failed = 0; g_failedThis = 0; g_base = 0; g_baseUser = 0; g_order = -1; g_askThis = 0;
}
static long g_buf[3][sizeof(CThing) / 4 + 1];
static CThing* NewThing(int i) { memset(g_buf[i], 0, sizeof g_buf[i]); return (CThing*)g_buf[i]; }
static void SetInterfaces(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    memset(t->m_presentBits, 0, sizeof t->m_presentBits);
    for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
}
int main()
{
    CThingAICreature* user = (CThingAICreature*)NewThing(0); CThing* bag = NewThing(1); CThing* recv = NewThing(2);
    static CTCTargetingBase targeting; static CTCHeroStats stats; static CTCGoldReceiver asker; static CTCInterface1A report;
    stats.Gold = 10;
    CInterfaceMap::SEntry ue[] = { { 2, 0 }, { 4, (CTCBase*)&stats }, { 8, (CTCBase*)&targeting }, { 0x40, 0 } }; SetInterfaces(user, ue, 4);
    CInterfaceMap::SEntry be[] = { { 3, 0 }, { 0x1a, (CTCBase*)&report } }; SetInterfaces(bag, be, 2);
    CInterfaceMap::SEntry re[] = { { 1, 0 } }; SetInterfaces(recv, re, 1);
    CTCCarriedActionUseGoldBag gb; gb.ParentThing = bag;
    g_target = recv; g_isType = true; g_canInteract = true;
    // default gift of 50 to a plain receiver; the 0x1A component is told afterwards
    Reset();
    gb.vfunc_28(user);
    CHECK(g_base == 1 && g_baseUser == user && g_order == 0);
    CHECK(g_isTypeArg == 2 && g_canType == 2);
    CHECK(g_ctors == 1 && g_giver == (CThingCreatureBase*)user && g_recv == recv && g_amount == 50);
    CHECK(g_sets == 1 && g_setThis == user && g_setAction == (CCreatureActionBase*)g_ctorThis && g_setDtorsBefore == 0);
    CHECK(g_dtors == 1 && g_dtorThis == g_ctorThis);
    CHECK(g_failed == 1 && g_failedThis == &report);
    // receiver with its own asking amount
    CInterfaceMap::SEntry re2[] = { { 1, 0 }, { 0x80, (CTCBase*)&asker } }; SetInterfaces(recv, re2, 2);
    Reset(); g_askAmount = 30;
    gb.vfunc_28(user);
    CHECK(g_askThis == &asker && g_ctors == 1 && g_amount == 30 && g_sets == 1 && g_failed == 1);
    // asking nothing: no action
    Reset(); g_askAmount = 0;
    gb.vfunc_28(user);
    CHECK(g_askThis == &asker && g_ctors == 0 && g_sets == 0 && g_failed == 1);
    // no gold: no action
    Reset(); g_askAmount = 30; stats.Gold = 0;
    gb.vfunc_28(user);
    CHECK(g_ctors == 0 && g_sets == 0 && g_askThis == 0 && g_failed == 1);
    stats.Gold = 10;
    // cannot interact / wrong target type: no action
    Reset(); g_canInteract = false;
    gb.vfunc_28(user);
    CHECK(g_ctors == 0 && g_failed == 1 && g_canType == 2);
    Reset(); g_canInteract = true; g_isType = false;
    gb.vfunc_28(user);
    CHECK(g_ctors == 0 && g_canType == 0 && g_failed == 1);
    g_isType = true;
    // user without stats interface: no action
    CInterfaceMap::SEntry ue2[] = { { 2, 0 }, { 8, (CTCBase*)&targeting } }; SetInterfaces(user, ue2, 2);
    Reset();
    gb.vfunc_28(user);
    CHECK(g_ctors == 0 && g_canType == 2 && g_failed == 1);
    // user without targeting, bag without 0x1A: nothing but the base hook
    CInterfaceMap::SEntry ue3[] = { { 4, (CTCBase*)&stats } }; SetInterfaces(user, ue3, 1);
    CInterfaceMap::SEntry be2[] = { { 3, 0 } }; SetInterfaces(bag, be2, 1);
    Reset();
    gb.vfunc_28(user);
    CHECK(g_base == 1 && g_isTypeArg == 0 && g_ctors == 0 && g_failed == 0);
    if (fails == 0) printf("007ed560_TEST PASS\n"); else printf("007ed560_TEST FAIL\n");
    return fails;
}