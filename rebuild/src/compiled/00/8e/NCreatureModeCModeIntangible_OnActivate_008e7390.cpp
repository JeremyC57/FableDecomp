// NCreatureMode::CModeIntangible::OnActivate() -- the creature turns intangible: suspend physics, hide it from mapwho, drop its physics flags, make it un-willable and untargetable; a player creature also suspends its animation and remembers its Damageable bit; record the boss phase and clear the weapon trails
// Leaf: ?OnActivate@CModeIntangible
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E7390, 344 bytes (padded to 352), `ret`; vtable 0x01268514 slot 5.
// Name: FableWin.pdb `?OnActivate@CModeIntangible@NCreatureMode@@UAEXXZ` (slot 5; the mirror of OnDeactivate 0x008E74F0).
// Callees: the physics component's virtual slots 38 (+0x98), 36 (+0x90) and 43 (+0xAC, SetPhysicsFlags 0x007234C0);
//   CModeBase::OnActivate @ 0x0073AAC0; CThing::SetAsDrawable (slot 28, +0x70); CVectorMap::LowerBound @ 0x0040F020 (inlined
//   GetInterface, types 0x3a, 9 and 0x4c) and its const instantiation @ 0x004365B0 (type 0xad, the boss);
//   CTCWillResponse::SetThingAsNotBloodyWillableAtAll @ 0x007E4040 (static `__fastcall`); CThingAnimatable::SetSuspendAnimation
//   @ 0x00833A10; CTCBoss::GetBossPhase @ 0x007AED10 (manifest label CWorld::DrawGetEnvironment is propagated);
//   CTCWeaponTrail::ClearAllWeaponTrails @ 0x0076E150.
// Layout: as OnDeactivate 0x008E74F0 (Ego_r.pdb CModeIntangible; CThingPhysical::Damageable bit 0 of +0xBC).
// Shape: the boss lookup is the landed FrameUpdate (0x008E7600) one-ternary HasTC + GetTC on the const map; the weapon-trail
//   lookup is the same ternary on the non-const map (component straight into ecx, `this` register reused for the thing).
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0 (the const instantiation)
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetFlag85Bit2(bool on);        // slot 36 (+0x90), 0x00723630
    virtual void v37();
    virtual void SetPhysicsActive(bool on);     // slot 38 (+0x98), 0x007235E0
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void SetPhysicsFlags(unsigned long flags);   // slot 43 (+0xAC), 0x007234C0
    virtual void ResetPhysicsFlagsToDef();      // slot 44 (+0xB0), 0x00723A50
};
struct CModeBase;
class CCreatureActionBase;
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
};
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetAsDrawable(bool drawable);  // slot 28 (+0x70), 0x004C8E50
    unsigned char   _pad04[0x0c];
    unsigned char   m_type;                     // +0x10 (1 = player creature)
    unsigned char   _pad11[0x0f];
    unsigned long   m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;               // +0x44
    unsigned char   _pad4c[0x14];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char   _pad64[0x58];
    unsigned char   Damageable : 1;             // +0xBC bit 0 (CThingPhysical)
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
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    void SetSuspendAnimation(bool suspend, CModeBase* mode);                   // 0x00833A10
};
class CTCMapwho { public:                              // interface 0x3a
    unsigned char _pad00[0x50];
    unsigned char InMapwho : 1;                 // +0x50 bit 0
};
class CTCTargeted { public:                            // interface 9
    unsigned char _pad00[0x10];
    bool          Targetable;                   // +0x10
};
class CTCBoss { public:                                // interface 0xad
    long GetBossPhase();                        // 0x007AED10
};
class CTCWeaponTrail { public:                         // interface 0x4c
    void ClearAllWeaponTrails();                // 0x0076E150
};
class CTCWillResponse { public:
    static void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable);   // 0x007E4040
};

struct CModeBase {                              // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;                  // +0x04
    unsigned char WeaponBasedFilter[0x0c];      // +0x08 CAnimFilterOfGroup
    long          FramesBeforeRemovingSelf;     // +0x14
    bool          Active;                       // +0x18
    void OnActivate();                          // 0x0073AAC0
};

namespace NCreatureMode {
struct CModeIntangible : CModeBase {            // vtable 0x01268514
    unsigned char _pad1c[0x14];
    long          Timer;                        // +0x30
    long          BossPhase;                    // +0x34
    bool          WasDamageable;                // +0x38
    CCountedPointer<CCreatureActionBase> PActionOnDeactivate;   // +0x3c
    void OnActivate();
};

void CModeIntangible::OnActivate()
{
    ParentThing->PhysicsTC->SetPhysicsActive(false);
    CModeBase::OnActivate();
    ParentThing->SetAsDrawable(false);
    CTCMapwho* mapwho;
    if (ParentThing->GetInterface(&mapwho, 0x3a))
        mapwho->InMapwho = 0;
    ParentThing->PhysicsTC->SetFlag85Bit2(false);
    ParentThing->PhysicsTC->SetPhysicsFlags(0x20);
    CTCWillResponse::SetThingAsNotBloodyWillableAtAll(ParentThing, true);
    CTCTargeted* targeted;
    if (ParentThing->GetInterface(&targeted, 9))
        targeted->Targetable = false;
    CThing* thing = ParentThing;
    if (thing->m_type == 1) {
        thing->SetSuspendAnimation(true, this);
        WasDamageable = thing->Damageable;
    }
    if (((const CThing*)ParentThing)->HasTC(0xad))
        BossPhase = ((const CThing*)ParentThing)->GetTC<CTCBoss>(0xad)->GetBossPhase();
    if (ParentThing->HasTC(0x4c))
        ParentThing->GetTC<CTCWeaponTrail>(0x4c)->ClearAllWeaponTrails();
}
}