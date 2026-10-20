#ifndef _MSC_VER
#define __forceinline inline
#endif
struct CTCBase;
struct CThing;
struct CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerConstThing() { SetPItem(0); }
    CIntelligentPointerConstThing(const CThing* item);              // 0x00668B60
    __forceinline CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
#pragma pack(pop)

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

struct CPlayer {
    unsigned char _pad00[0x20a];
    bool ZTargeting;                                // +0x20A
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                   // +0x6C (bit 3 creature, bit 5 player creature)
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    CPlayer* GetPlayer();                           // 0x004C7A10

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

struct CTCTargeting {                               // interface type 8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetZTarget();                   // slot 33 (+0x84)
};

struct CTCUnholyPower {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x6d - 0x08];
    CIntelligentPointerConstThing PTarget;          // +0x6D CIntelligentPointer<CThingCreatureBase>
    bool IsZTargetting;                             // +0x75
    void SetPTarget();
};

void CTCUnholyPower::SetPTarget()
{
    PTarget = CIntelligentPointerConstThing();
    IsZTargetting = false;
    if (ParentThing->HasTC(8)) {
        CThing* target = ((CTCTargeting*)ParentThing->GetTC(8))->GetZTarget();
        if (target && !(target->m_flags91 & 1) && (target->ThingTypeFlags & 8)
            && (ParentThing->ThingTypeFlags & 0x20)) {
            {
                CIntelligentPointerConstThing newTarget(target);
                PTarget = newTarget;
            }
            IsZTargetting = ParentThing->GetPlayer()->ZTargeting;
        }
    }
}

// ---- test stubs for the retail callees ----
#include <cstdio>
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerConstThing::v00() {}
CIntelligentPointerConstThing::CIntelligentPointerConstThing(const CThing* item) { SetPItem((CBaseObject*)item); }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static CPlayer g_player;
CPlayer* CThing::GetPlayer() { return &g_player; }
static CThing* g_zTarget;
struct CTestTargeting : CTCTargeting { virtual CThing* GetZTarget() { return g_zTarget; } };
void CTCTargeting::v00() {} void CTCTargeting::v01() {} void CTCTargeting::v02() {} void CTCTargeting::v03() {}
void CTCTargeting::v04() {} void CTCTargeting::v05() {} void CTCTargeting::v06() {} void CTCTargeting::v07() {}
void CTCTargeting::v08() {} void CTCTargeting::v09() {} void CTCTargeting::v10() {} void CTCTargeting::v11() {}
void CTCTargeting::v12() {} void CTCTargeting::v13() {} void CTCTargeting::v14() {} void CTCTargeting::v15() {}
void CTCTargeting::v16() {} void CTCTargeting::v17() {} void CTCTargeting::v18() {} void CTCTargeting::v19() {}
void CTCTargeting::v20() {} void CTCTargeting::v21() {} void CTCTargeting::v22() {} void CTCTargeting::v23() {}
void CTCTargeting::v24() {} void CTCTargeting::v25() {} void CTCTargeting::v26() {} void CTCTargeting::v27() {}
void CTCTargeting::v28() {} void CTCTargeting::v29() {} void CTCTargeting::v30() {} void CTCTargeting::v31() {}
void CTCTargeting::v32() {} CThing* CTCTargeting::GetZTarget() { return 0; }
void CTCUnholyPower::v00() {}

static void MakeThing(CThing& t, unsigned long typeFlags)
{
    for (unsigned i = 0; i < sizeof(t); ++i) ((unsigned char*)&t)[i] = 0;
    t.ThingTypeFlags = typeFlags;
}

int main()
{
    int ok = 1;
    CTestTargeting targeting;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 3; entries[0].component = 0;
    entries[1].type = 8; entries[1].component = (CTCBase*)(CTCTargeting*)&targeting;
    CThing owner; MakeThing(owner, 0x20 | 8);
    owner.m_presentBits[0] = 1u << 8;
    owner.m_interfaces.first = entries; owner.m_interfaces.last = entries + 2;
    CThing creature; MakeThing(creature, 8);
    g_player.ZTargeting = true;

    CTCUnholyPower power;
    power.ParentThing = &owner;
    power.IsZTargetting = false;
    // 1: live creature target, player owner -> taken, z-target flag copied
    g_zTarget = &creature;
    power.SetPTarget();
    if (power.PTarget.GetPItem() != &creature || power.IsZTargetting != true) ok = 0;
    // 2: dead target -> cleared
    creature.m_flags91 = 1;
    power.SetPTarget();
    if (power.PTarget.GetPItem() != 0 || power.IsZTargetting != false) ok = 0;
    creature.m_flags91 = 0;
    // 3: owner not the player creature -> cleared
    power.SetPTarget(); if (power.PTarget.GetPItem() != &creature) ok = 0;
    owner.ThingTypeFlags = 8;
    power.SetPTarget();
    if (power.PTarget.GetPItem() != 0) ok = 0;
    owner.ThingTypeFlags = 0x28;
    // 4: target not a creature
    creature.ThingTypeFlags = 1;
    power.SetPTarget();
    if (power.PTarget.GetPItem() != 0) ok = 0;
    creature.ThingTypeFlags = 8;
    // 5: no targeting component -> cleared
    owner.m_presentBits[0] = 0;
    power.SetPTarget();
    if (power.PTarget.GetPItem() != 0 || power.IsZTargetting) ok = 0;
    // 6: no z-target
    owner.m_presentBits[0] = 1u << 8; g_zTarget = 0;
    power.SetPTarget();
    if (power.PTarget.GetPItem() != 0) ok = 0;
    if (!ok) { std::printf("00793d20_TEST FAIL\n"); return 1; }
    std::printf("00793d20_TEST PASS\n");
    return 0;
}