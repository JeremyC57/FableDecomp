// Behaviour test for 0078e820: src/0078e820.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCDivineWrath::SetPTarget()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCDivineWrath::SetPTarget() -- clear the spell's target, then take the thing the caster is targeting (interface 8) when it is alive, of a targetable type and the caster is of the right type; Z-targeting follows the caster's player
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078E820, 220 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
// (CIntelligentPointer vtable 0x0123E7D8), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0, the targeting
// component's virtual slot 33 (+0x84, the targeted thing; as CTCCarriedActionUseTrophy 0x007EDEE0), CIntelligentPointer<const
// CThing>::CIntelligentPointer(const CThing*) @ 0x00668B60 (out of line), CThing::GetPlayer @ 0x004C7A10.
// Layout: Ego_r.pdb CTCDivineWrath PTarget +0x71 / IsZTargetting +0x79 sit at retail +0x6d / +0x75 (4 bytes lower; the pointer
// classes are `pack(1)`); retail CThing ThingTypeFlags +0x6c (PDB +0x7c), dead bit +0x91; the player's Z-targeting byte +0x20a
// (named descriptively).
// Shape: two assignments through a converting operator= (`SetPItem(other.GetPItem())`): from an inline CIntelligentPointer(0)
// temporary and, for the target, from an implicit conversion of a `const CThing*` local (`PTarget = target;` builds the
// out-of-line const-thing pointer temporary and re-addresses it with `lea ecx,[esp+8]`; an explicit
// `CIntelligentPointerConstThing(target)` functional cast reuses the ctor's returned this, `mov ecx,eax`); the targeting
// component is the one-ternary GetTC (straight into ecx for the virtual call).
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CPlayer {
    unsigned char _pad00[0x20a];
    bool ZTargeting;                                    // +0x20a
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                       // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTCSelect(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    CPlayer* GetPlayer();                               // 0x004C7A10
};
struct CTCTargetSource {                                // the interface-8 component
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32)
#undef V
    virtual CThing* GetTargetThing();                   // slot 33 (+0x84)
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item);  // 0x00668B60
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
    __forceinline CIntelligentPointerCreature& operator=(const CIntelligentPointerConstThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
#pragma pack(pop)
struct CTCDivineWrath {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x6d - 0x08];
    CIntelligentPointerCreature PTarget;                // +0x6d (PDB +0x71)
    bool IsZTargetting;                                 // +0x75 (PDB +0x79)
    void SetPTarget();
};

void CTCDivineWrath::SetPTarget()
{
    PTarget = CIntelligentPointerCreature(0);
    IsZTargetting = false;
    if (ParentThing->HasTC(8)) {
        const CThing* target = ((CTCTargetSource*)ParentThing->GetTCSelect(8))->GetTargetThing();
        if (target && !(target->Flags91 & 1) && (target->ThingTypeFlags & 8) && (ParentThing->ThingTypeFlags & 0x20)) {
            PTarget = target;
            IsZTargetting = ParentThing->GetPlayer()->ZTargeting;
        }
    }
}

// ---- stubs and driver ----
#include <new>
static CInterfaceMap::SEntry g_entries[2];
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { CHECK(*type == 8); return first; }
static CPlayer g_player;
CPlayer* CThing::GetPlayer() { return &g_player; }
void CTCTargetSource::v00() { } void CTCTargetSource::v01() { } void CTCTargetSource::v02() { } void CTCTargetSource::v03() { } void CTCTargetSource::v04() { } void CTCTargetSource::v05() { } void CTCTargetSource::v06() { } void CTCTargetSource::v07() { } void CTCTargetSource::v08() { } void CTCTargetSource::v09() { } void CTCTargetSource::v10() { } void CTCTargetSource::v11() { } void CTCTargetSource::v12() { } void CTCTargetSource::v13() { } void CTCTargetSource::v14() { } void CTCTargetSource::v15() { } void CTCTargetSource::v16() { } void CTCTargetSource::v17() { } void CTCTargetSource::v18() { } void CTCTargetSource::v19() { } void CTCTargetSource::v20() { } void CTCTargetSource::v21() { } void CTCTargetSource::v22() { } void CTCTargetSource::v23() { } void CTCTargetSource::v24() { } void CTCTargetSource::v25() { } void CTCTargetSource::v26() { } void CTCTargetSource::v27() { } void CTCTargetSource::v28() { } void CTCTargetSource::v29() { } void CTCTargetSource::v30() { } void CTCTargetSource::v31() { } void CTCTargetSource::v32() { }
static CThing* g_target;
CThing* CTCTargetSource::GetTargetThing() { return g_target; }
static int g_baseCtors = 0, g_baseDtors = 0, g_sets = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; ++g_baseCtors; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_baseDtors; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
void CBaseIntelligentPointer::v00() { }
static int g_constCtors = 0;
CIntelligentPointerConstThing::CIntelligentPointerConstThing(const CThing* item) { ++g_constCtors; SetPItem((CThing*)item); }
void CIntelligentPointerConstThing::v00() { }
void CIntelligentPointerCreature::v00() { }
void CTCDivineWrath::v00() { }
static unsigned char g_raw[sizeof(CTCDivineWrath)];
static unsigned char g_buf[sizeof(CTCDivineWrath)];
static CTCTargetSource g_source;
int main()
{
    static CThing parent, target;
    parent.m_presentBits[0] = 1UL << 8; parent.ThingTypeFlags = 0x20;
    g_entries[0].type = 8; g_entries[0].component = (CTCBase*)&g_source;
    parent.m_interfaces.first = &g_entries[0]; parent.m_interfaces.last = &g_entries[1];
    target.ThingTypeFlags = 8; target.Flags91 = 0;
    *(CThing**)(g_raw + 4) = &parent;
    CTCDivineWrath* w = new (g_buf) CTCDivineWrath(*(const CTCDivineWrath*)g_raw);
    CHECK(w->ParentThing == &parent && w->PTarget.m_pItem == 0);
    // 1) a live, targetable target and a caster of the right type: take it, Z-targeting from the caster's player
    g_target = &target; g_player.ZTargeting = true; w->IsZTargetting = false; g_sets = 0; g_constCtors = 0;
    int dtors0 = g_baseDtors;
    w->SetPTarget();
    CHECK(w->PTarget.m_pItem == &target && w->IsZTargetting == true);
    CHECK(g_sets == 4 && g_constCtors == 1 && g_baseDtors - dtors0 == 2);   // (0) temp + assign, const temp + assign; both temps destroyed
    // 2) dead target: cleared and not Z-targeting
    target.Flags91 = 1; g_sets = 0;
    w->SetPTarget();
    CHECK(w->PTarget.m_pItem == 0 && w->IsZTargetting == false && g_sets == 2);
    // 3) target not of a targetable type
    target.Flags91 = 0; target.ThingTypeFlags = 4; w->PTarget.m_pItem = &target;
    w->SetPTarget();
    CHECK(w->PTarget.m_pItem == 0 && w->IsZTargetting == false);
    // 4) caster lacks the type flag
    target.ThingTypeFlags = 8; parent.ThingTypeFlags = 0x10;
    w->SetPTarget();
    CHECK(w->PTarget.m_pItem == 0);
    // 5) no interface 8 on the caster: no lookup at all
    parent.ThingTypeFlags = 0x20; parent.m_presentBits[0] = 0; g_player.ZTargeting = false;
    w->SetPTarget();
    CHECK(w->PTarget.m_pItem == 0 && w->IsZTargetting == false);
    // 6) null target
    parent.m_presentBits[0] = 1UL << 8; g_target = 0;
    w->SetPTarget();
    CHECK(w->PTarget.m_pItem == 0);
    // 7) Z-targeting off on the player: target taken, flag false
    g_target = &target; g_player.ZTargeting = false; w->IsZTargetting = true;
    w->SetPTarget();
    CHECK(w->PTarget.m_pItem == &target && w->IsZTargetting == false);
    if (fails == 0) printf("0078e820_TEST PASS\n");
    return fails;
}
