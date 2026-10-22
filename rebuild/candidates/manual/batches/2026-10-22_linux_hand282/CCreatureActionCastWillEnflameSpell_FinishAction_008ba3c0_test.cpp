#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
// CCreatureAction_CastWillEnflameSpell::FinishAction() -- base FinishAction; if the item removed for the cast is still alive, clear the primary carry slot (weapons dropped, other things removed, the showable hook run when no longer carried), put the item back in its carry slot (CCarryableDef::CarrySlot), refresh sheathed weapons, invalidate the melee flourish, update the front mode and put the main player in aggressive mode; finally reset the creature's enflame component if it is not active
// Leaf: ?FinishAction@CCreatureAction_CastWillEnflameSpell
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BA3C0, 744 bytes (752 with padding), virtual FinishAction, plain `ret`.
// Callees: CCombatActionBase::FinishAction 0x00859A20; CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C); CVectorMap::LowerBound 0x0040F020
//   (inline GetInterface: 0x46 CTCCarrying, 0x1A showable item, 0x13 CTCInventoryWeapons, 0x30 CTCCombat,
//   0x31 CTCCreatureModeManager, 0x95 enflame); CTCCarrying::IsPrimarySlotEmpty 0x006A1270 / GetThingInPrimarySlot 0x006A1440 /
//   RemoveThing 0x006A19B0 / IsCarryingThing 0x006A1060 / RemoveThingFromPrimarySlot 0x006A1E00 / PutThingInCarrySlot
//   0x006A1F80; DropWeapon 0x00845CE0 (__fastcall); CTCShowableItem::OnShowEnded 0x005D8E50; GetSubDef<CCarryableDef>
//   0x005CC10E; CTCInventoryWeapons::RefreshSheathedWeapons 0x005C9962; CTCCombat::SetMeleeFlourishValid 0x00733E10;
//   CTCCreatureModeManager::UpdateFrontMode 0x0073A840; CThing::GetPlayer 0x004C7A30; CPlayer::IsMainPlayer 0x00487520 /
//   SetAggressiveMode 0x00487EB0; the enflame reset 0x007A44C0.
// Layout: retail CCombatActionBase 0x114 (PCreature +0x08, PItemRemoved +0x2c), Level +0x114; CThing bitset +0x20, map +0x44,
//   def +0x70, dead bit +0x91; CCarryableDef CarrySlot +0x28; the enflame component's active flag +0xd.
// Shape: every component lookup is the out-parameter GetInterface (the carrying and mode lookups unchecked: retail reads the
//   never-written out slot on the miss path; the conditional ones fetch the creature once). The class def is an inline
//   accessor (its GetPItem runs before the &def push) and the item goes to PutThingInCarrySlot through the inline
//   conversion operator (its GetPItem runs before the other pushes). LowerBound / GetSubDef are same-TU noinline bodies so
//   the key and def words are not escaped; the def release folds its null test (DecRefToZero, landed 0x00848500 form).
struct CTCBase;
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CCarryableDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long CarrySlot;                                   // +0x28
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out); // 0x005CC10E
};
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CPlayer {
    bool IsMainPlayer();                              // 0x00487520
    void SetAggressiveMode(bool aggressive);          // 0x00487EB0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    CPlayer* GetPlayer();                             // 0x004C7A30
    CDefClassBase* GetDef() const { return m_pDef; }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    operator CThing*() const { return GetPItem(); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCShowableItem {
    void OnShowEnded();                               // 0x005D8E50
};
struct CTCCarrying;
void __fastcall DropWeapon(CThing* weapon, CTCCarrying* carrying, CThing* owner);   // 0x00845CE0
struct CTCCarrying {
    bool IsPrimarySlotEmpty();                        // 0x006A1270
    CThing* GetThingInPrimarySlot();                  // 0x006A1440
    void RemoveThing(CThing* thing);                  // 0x006A19B0
    bool IsCarryingThing(CThing* thing);              // 0x006A1060
    CThing* RemoveThingFromPrimarySlot(bool unused);  // 0x006A1E00
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};
struct CTCInventoryWeapons {
    void RefreshSheathedWeapons();                    // 0x005C9962
};
struct CTCCombat {
    void SetMeleeFlourishValid(bool valid);           // 0x00733E10
};
struct CTCCreatureModeManager {
    void UpdateFrontMode();                           // 0x0073A840
};
struct CTCEnflame {
    unsigned char _pad[0xd];
    bool Active;                                      // +0xd
    void ResetState();                                // 0x007A44C0
};
struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                      // slot 4: 0x00859A20
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CBaseIntelligentPointer PItemRemoved;             // +0x2c
    unsigned char _pad34[0x114 - 0x34];
};
struct CCreatureAction_CastWillEnflameSpell : CCombatActionBase {
    long Level;                                       // +0x114
    unsigned char _pad118[4];
    virtual void FinishAction();
};

void CCreatureAction_CastWillEnflameSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CIntelligentPointer item;
    item.SetPItem(PItemRemoved.GetPItem());
    if (IS_THING_ALIVE(item)) {
        CTCCarrying* carrying;
        PCreature.GetPItem()->GetInterface(&carrying, 0x46);
        if (!carrying->IsPrimarySlotEmpty()) {
            CThing* thing = carrying->GetThingInPrimarySlot();
            if (thing->HasTC(0x48))
                DropWeapon(thing, carrying, PCreature.GetPItem());
            else
                carrying->RemoveThing(thing);
            CTCShowableItem* showable;
            if (thing->GetInterface(&showable, 0x1a)) {
                if (!carrying->IsCarryingThing(thing))
                    showable->OnShowEnded();
            }
            if (!carrying->IsPrimarySlotEmpty())
                carrying->RemoveThingFromPrimarySlot(false);
        }
        if (carrying->IsCarryingThing(item.GetPItem()))
            carrying->RemoveThing(item.GetPItem());
        CDefPointer<CCarryableDef> carryDef;
        item.GetPItem()->GetDef()->GetSubDef(&carryDef);
        carrying->PutThingInCarrySlot(item, carryDef->CarrySlot, true);
        CTCInventoryWeapons* weapons;
        if (PCreature.GetPItem()->GetInterface(&weapons, 0x13))
            weapons->RefreshSheathedWeapons();
        CTCCombat* combat;
        if (PCreature.GetPItem()->GetInterface(&combat, 0x30))
            combat->SetMeleeFlourishValid(false);
        CTCCreatureModeManager* modes;
        PCreature.GetPItem()->GetInterface(&modes, 0x31);
        modes->UpdateFrontMode();
        if (PCreature.GetPItem()->GetPlayer()->IsMainPlayer())
            PCreature.GetPItem()->GetPlayer()->SetAggressiveMode(true);
    }
    CTCEnflame* enflame;
    if (PCreature.GetPItem()->GetInterface(&enflame, 0x95)) {
        if (!enflame->Active)
            enflame->ResetState();
    }
}

// ---- behaviour test ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)

static int g_log[64]; static int g_nlog;
static void Log(int e) { if (g_nlog < 64) g_log[g_nlog++] = e; }
enum { E_BASEFINISH = 1, E_DROP, E_REMOVE, E_SHOWEND, E_REMOVEPRIMARY, E_PUT, E_REFRESH, E_FLOURISH, E_FRONT, E_AGGRESSIVE, E_RESET };

CCharString::CCharString(const char* text, long length) { (void)length; PStringData = (void*)text; }
CCharString::~CCharString() {}
static int g_defDestroys;
void CCarryableDef::v00() {}
void CCarryableDef::Destroy() { g_defDestroys++; }
static CCarryableDef g_carryDef;
static long g_subIndex = 7; static bool g_hasSub = true;
static CDefPointer<CCarryableDef> s_found, s_none;   // returned by (bitwise) copy: no named-return copy/destroy pair under /Od
CDefPointer<CCarryableDef> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    if (index == 7) { s_found.Object = &g_carryDef; ++g_carryDef.RefCount; return s_found; }
    return s_none;
}
#define DV(n) void CDefClassBase::v##n() {}
DV(00) DV(04) DV(08) DV(0c) DV(10) DV(14) DV(18) DV(1c) DV(20) DV(24) DV(28) DV(2c) DV(30) DV(34)
static const char* g_subName;
long* CDefClassBase::GetSubDefIndex(const CCharString& name) { g_subName = (const char*)name.PStringData; return g_hasSub ? &g_subIndex : 0; }

static CPlayer g_player; static bool g_mainPlayer; static bool g_aggressive; static int g_aggressiveCalls;
bool CPlayer::IsMainPlayer() { return g_mainPlayer; }
void CPlayer::SetAggressiveMode(bool aggressive) { g_aggressive = aggressive; g_aggressiveCalls++; Log(E_AGGRESSIVE); }
CPlayer* CThing::GetPlayer() { return &g_player; }

void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
static int g_ipDtors;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { g_ipDtors++; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CIntelligentPointer::v00() {}

static CTCShowableItem* g_shown;
void CTCShowableItem::OnShowEnded() { g_shown = this; Log(E_SHOWEND); }
static CThing* g_dropW; static CTCCarrying* g_dropC; static CThing* g_dropO;
void __fastcall DropWeapon(CThing* weapon, CTCCarrying* carrying, CThing* owner) { g_dropW = weapon; g_dropC = carrying; g_dropO = owner; Log(E_DROP); }

static bool g_empty[4]; static int g_emptyCalls;
static CThing* g_primary; static CThing* g_removed[4]; static int g_nremoved;
static CThing* g_carried[4]; static int g_ncarried;
static CThing* g_putThing; static long g_putSlot; static bool g_putNow;
static bool g_removePrimaryArg; static int g_removePrimaryCalls;
bool CTCCarrying::IsPrimarySlotEmpty() { return g_empty[g_emptyCalls++]; }
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_primary; }
static bool g_keepCarried;
void CTCCarrying::RemoveThing(CThing* thing)
{
    g_removed[g_nremoved++] = thing; Log(E_REMOVE);
    if (!g_keepCarried) for (int i = 0; i < g_ncarried; i++) if (g_carried[i] == thing) g_carried[i] = 0;
}
bool CTCCarrying::IsCarryingThing(CThing* thing) { for (int i = 0; i < g_ncarried; i++) if (g_carried[i] == thing) return true; return false; }
CThing* CTCCarrying::RemoveThingFromPrimarySlot(bool unused) { g_removePrimaryArg = unused; g_removePrimaryCalls++; Log(E_REMOVEPRIMARY); return 0; }
void CTCCarrying::PutThingInCarrySlot(CThing* thing, long slot, bool immediately) { g_putThing = thing; g_putSlot = slot; g_putNow = immediately; Log(E_PUT); }
void CTCInventoryWeapons::RefreshSheathedWeapons() { Log(E_REFRESH); }
static bool g_flourish = true;
void CTCCombat::SetMeleeFlourishValid(bool valid) { g_flourish = valid; Log(E_FLOURISH); }
void CTCCreatureModeManager::UpdateFrontMode() { Log(E_FRONT); }
void CTCEnflame::ResetState() { Log(E_RESET); }
void CCombatActionBase::v00() {} void CCombatActionBase::v01() {} void CCombatActionBase::v02() {} void CCombatActionBase::v03() {}
void CCombatActionBase::FinishAction() { Log(E_BASEFINISH); }

static void SetTC(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    memset(t->m_presentBits, 0, sizeof t->m_presentBits);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
    for (int i = 0; i < n; i++) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
}
static void Reset() { g_nlog = 0; g_emptyCalls = 0; g_nremoved = 0; g_shown = 0; g_dropW = 0; g_putThing = 0; g_removePrimaryCalls = 0; g_aggressiveCalls = 0; g_ipDtors = 0; g_defDestroys = 0; }
static bool LogIs(const int* e, int n) { if (g_nlog != n) return false; for (int i = 0; i < n; i++) if (g_log[i] != e[i]) return false; return true; }

int main()
{
    static CThing creature, item, weapon, rock;
    static CTCCarrying carrying; static CTCInventoryWeapons weapons; static CTCCombat combat; static CTCCreatureModeManager modes;
    static CTCEnflame enflame; static CTCShowableItem showW, showR;
    static CDefClassBase itemDef; item.m_pDef = &itemDef; itemDef.m_pManager = 0;
    static CDefinitionManager mgr; itemDef.m_pManager = &mgr;
    g_carryDef.CarrySlot = 3;
    CInterfaceMap::SEntry ce[] = { { 0x13, (CTCBase*)&weapons }, { 0x30, (CTCBase*)&combat }, { 0x31, (CTCBase*)&modes },
                                   { 0x46, (CTCBase*)&carrying }, { 0x95, (CTCBase*)&enflame } };
    CInterfaceMap::SEntry we[] = { { 0x1a, (CTCBase*)&showW }, { 0x48, 0 } };
    CInterfaceMap::SEntry re[] = { { 0x1a, (CTCBase*)&showR } };
    SetTC(&weapon, we, 2); SetTC(&rock, re, 1); SetTC(&item, 0, 0);
    static CCreatureAction_CastWillEnflameSpell action; CCreatureAction_CastWillEnflameSpell* a = &action;
    a->PCreature.m_pItem = &creature;

    // no removed item: base finish, then the inactive enflame component is reset
    SetTC(&creature, ce, 5); enflame.Active = false; a->PItemRemoved.m_pItem = 0; Reset();
    a->FinishAction();
    { int e[] = { E_BASEFINISH, E_RESET }; CHECK(LogIs(e, 2)); }
    CHECK(g_ipDtors == 1);
    // dead item, active enflame: nothing but the base finish
    item.m_flags91 = 1; a->PItemRemoved.m_pItem = &item; enflame.Active = true; Reset();
    a->FinishAction();
    { int e[] = { E_BASEFINISH }; CHECK(LogIs(e, 1)); }

    // live item, empty primary slot, item still carried: removed then put back in its def's slot; main player goes aggressive
    item.m_flags91 = 0; g_empty[0] = true; g_carried[0] = &item; g_ncarried = 1; g_mainPlayer = true; g_aggressive = false;
    g_carryDef.RefCount = 1; g_flourish = true; Reset();
    a->FinishAction();
    { int e[] = { E_BASEFINISH, E_REMOVE, E_PUT, E_REFRESH, E_FLOURISH, E_FRONT, E_AGGRESSIVE }; CHECK(LogIs(e, 7)); }
    CHECK(g_removed[0] == &item && g_putThing == &item && g_putSlot == 3 && g_putNow == true && g_flourish == false && g_aggressive == true);
    CHECK(g_carryDef.RefCount == 1 && g_defDestroys == 0 && strcmp(g_subName, "CCarryableDef") == 0 && g_emptyCalls == 1);

    // a weapon in the primary slot: dropped (owner = creature), its showable hook runs, the slot is still busy so it is cleared;
    // item not carried; no inventory-weapons / combat components; not the main player; the def's last reference is dropped
    static CInterfaceMap::SEntry ce2[] = { { 0x31, (CTCBase*)&modes }, { 0x46, (CTCBase*)&carrying } };
    SetTC(&creature, ce2, 2); g_primary = &weapon; g_empty[0] = false; g_empty[1] = false; g_ncarried = 0;
    g_mainPlayer = false; g_carryDef.RefCount = 0; Reset();
    a->FinishAction();
    { int e[] = { E_BASEFINISH, E_DROP, E_SHOWEND, E_REMOVEPRIMARY, E_PUT, E_FRONT }; CHECK(LogIs(e, 6)); }
    CHECK(g_dropW == &weapon && g_dropC == &carrying && g_dropO == &creature && g_shown == &showW && g_removePrimaryArg == false);
    CHECK(g_carryDef.RefCount == 0 && g_defDestroys == 1 && g_aggressiveCalls == 0 && g_emptyCalls == 2);

    // a non-weapon still carried after RemoveThing: no hook; the slot is then empty
    g_primary = &rock; g_empty[0] = false; g_empty[1] = true; g_carried[0] = &item; g_carried[1] = &rock; g_ncarried = 2;
    g_carryDef.RefCount = 4; g_keepCarried = true; Reset();
    a->FinishAction();
    { int e[] = { E_BASEFINISH, E_REMOVE, E_REMOVE, E_PUT, E_FRONT }; CHECK(LogIs(e, 5)); }
    CHECK(g_removed[0] == &rock && g_dropW == 0 && g_shown == 0);
    CHECK(g_removePrimaryCalls == 0 && g_removed[1] == &item && g_carryDef.RefCount == 4);
    s_found.Object = 0;
    if (fails == 0) printf("008ba3c0_TEST PASS\n"); else printf("008ba3c0_TEST FAIL\n");
    return fails;
}
