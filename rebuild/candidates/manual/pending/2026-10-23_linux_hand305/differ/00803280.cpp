// CTCTargetingPlayer::UpdateTargetThreatening() -- when the player (in targeting or first-person mode) targets a creature that is not already an enemy willing to attack, and could be threatened, post a "threatened" opinion deed (drawn loaded projectile weapon vs other) and remember the thing; with no target clear the remembered thing
// Grade: DIFFER(548v551) -- flow, calls and pushes line up; retail's frame is 0xc (the creature spill shares the slot later used by the threatened-thing pointer, keys at +0), ours 0x10; the 0x51 lookup keeps the map end in edi and re-forms `lea edi,[ebx+44h]` for 0x46 (ours ecx)
// Leaf: UpdateTargetThreatening
// Retail: Fable.exe (TLC, VC7.1) @ 0x00803280, 551 bytes, plain `ret`.
// Callees: the TC's virtual GetPTargetedThing (slot 33, +0x84), CThing::GetPlayer 0x004C7A30 / GetPosition 0x004C73D0,
//   CPlayer::IsFirstPersonTargeting 0x004874D0, AreThingsEnemies 0x0076D2F0, IsWillingToAttackTarget 0x008648A0, the
//   threat test 0x008645E0 (catalog "CMainGameComponent::BeginInputLoading", propagated; __fastcall creature/threatener
//   + bool), LowerBound 0x0040F020 / const 0x004365B0, CTCCarrying::GetThingInPrimarySlot 0x006A1440,
//   CTCWeapon::IsProjectileWeapon 0x005DAA40 / IsProjectileWeaponLoaded 0x005DA980, CTCHeroOpinionDeedLog::PostDeed
//   0x006C6990, CIntelligentPointer<const CThing>(T*) 0x004A96F0 / operator= 0x004A9760, CBaseIntelligentPointer
//   0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~ 0x00A01C10.
// Layout: engine/CTCTargetingPlayer.h puts PLastThreatenedThing at +0xdc; retail +0xd4 (the two 16-byte PDB vectors are
//   12 bytes in retail, as the landed ClearLockedTarget 0x00802650), so the class is modelled locally. CPlayer +0x20a
//   targeting-mode flag; CThing type byte +0x10, presence bits +0x20 (0x4f, 0x51, 0x46, 0x48), map +0x44.
struct C3DVector { float X, Y, Z; };
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                                     // vtable 0x01238C6C
    CIntelligentPointer() { }
    CIntelligentPointer(T* item);                           // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};
struct CPlayer {
    unsigned char _pad00[0x20a];
    bool          InTargetingMode;                          // +0x20a
    bool IsFirstPersonTargeting();                          // 0x004874D0
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char ThingType;                                // +0x10 (1: creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    CPlayer* GetPlayer() const;                             // 0x004C7A30
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CThingAICreature : CThing { };
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);    // 0x0076D2F0
bool __fastcall IsWillingToAttackTarget(CThingAICreature* creature, CThing* target);   // 0x008648A0
bool __fastcall CanBeThreatened(CThingAICreature* creature, CThing* threatener, bool flag);   // 0x008645E0
struct CTCCarrying { CThing* GetThingInPrimarySlot(); };   // 0x006A1440
struct CTCWeapon {
    bool IsProjectileWeapon();                              // 0x005DAA40
    bool IsProjectileWeaponLoaded();                        // 0x005DA980
};
struct CFaction;
struct CTCHeroOpinionDeedLog {
    long PostDeed(long deed, const C3DVector& position, CThing* subject, CThingAICreature* witness, long postType,
                  bool a, bool b, CFaction* faction, bool c, long d);   // 0x006C6990
};

struct CTCTargetingPlayer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing();                    // slot 33 (+0x84)
    CThing*       ParentThing;                              // +0x04
    unsigned char _pad08[0xd4 - 0x08];
    CIntelligentPointer<const CThing> PLastThreatenedThing; // +0xd4 (PDB +0xdc)
    void UpdateTargetThreatening();
};

void CTCTargetingPlayer::UpdateTargetThreatening()
{
    if (GetPTargetedThing()) {
        if (GetPTargetedThing()->ThingType == 1 &&
            (ParentThing->GetPlayer()->InTargetingMode || ParentThing->GetPlayer()->IsFirstPersonTargeting())) {
            CThingAICreature* creature = (CThingAICreature*)GetPTargetedThing();
            if ((!AreThingsEnemies(GetPTargetedThing(), ParentThing) || !IsWillingToAttackTarget(creature, ParentThing)) &&
                GetPTargetedThing()->HasTC(0x4f) && CanBeThreatened(creature, ParentThing, true)) {
                CThing* parent = ParentThing;
                if (parent->HasTC(0x51)) {
                    CTCHeroOpinionDeedLog* deedLog = parent->GetTC<CTCHeroOpinionDeedLog>(0x51);
                    if (parent->HasTC(0x46)) {
                        CThing* held = ((const CThing*)parent)->GetTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
                        if (held && held->HasTC(0x48)) {
                            CTCWeapon* weapon = ((const CThing*)held)->GetTC<CTCWeapon>(0x48);
                            long deed = (weapon->IsProjectileWeapon() && weapon->IsProjectileWeaponLoaded()) ? 0x29 : 0x28;
                            CThing* owner = ParentThing;
                            deedLog->PostDeed(deed, owner->GetPosition(), GetPTargetedThing(), creature, 1, false, false, 0, false, 0);
                            {
                                CIntelligentPointer<const CThing> threatened(GetPTargetedThing());
                                PLastThreatenedThing = threatened;
                            }
                        }
                    }
                }
            }
        }
    } else {
        CIntelligentPointer<const CThing> none;
        none.SetPItem(0);
        PLastThreatenedThing.SetPItem(none.GetPItem());
    }
}
