// CThingPhysical::InternalModifyHealth(float delta, bool allowDeath) -- apply a health delta: invulnerability cheats, boss phase floor, def minimum, death floor, observer notifications
// Leaf: InternalModifyHealth
// Retail: ?InternalModifyHealth@CThingPhysical@@AAEXM_N@Z (private). Callees: CTCBoss::GetCurrentPhase 0x7AED10, GetMinHealthForPhase 0x7AEE40,
//   TC4 0x57A753 (catalog label AddSuccessfulBlockParry is wrong: hero health observer), TC9 0x7EA270 (label AddExclusiveObserver is wrong: flag setter),
//   SetAsFrameUpdated 0x4C7C40; LowerBound non-const 0x40F020 (TC 4/9), const 0x4365B0 (TC 0xad).
// Layout: retail CThing is 0x10 smaller and CThingPhysical a further 0x8 smaller than the PDB view: MaxHealth/Health/flag bits sit at
//   +0xb0/+0xb4/+0xbc (fld [esi+0xb4], test byte [esi+0xbc]) where the PDB has +0xc8/+0xcc/+0xd4; the shared header's +0xb4 "Scale" does not fit retail.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
    SEntry* LowerBound(const long& type) const;             // 0x004365B0
};
struct CTCBoss {                                            // TC 0xad
    unsigned char _pad00[0x0c];
    void* PPhases;                                          // +0x0c
    unsigned char _pad10[0x11];
    bool Active;                                            // +0x21
    long GetCurrentPhase() const;                           // 0x007AED10
    float GetMinHealthForPhase(long phase) const;           // 0x007AEE40
};
struct CTCHeroHealthObserver {                              // TC 4
    void OnHealthChanged();                                 // 0x0057A753
};
struct CTCObservable {                                      // TC 9
    unsigned char _pad00[0x0c];
    unsigned long Flags;                                    // +0x0c
    void SetFlags(unsigned long flags);                     // 0x007EA270
};
struct CThingPhysicalDef {
    unsigned char _pad00[0x54];
    float MinimumHealth;                                    // +0x54
};
extern bool g_bPlayerInvulnerable;                          // 0x013B86C7
extern bool g_bCreaturesInvulnerable;                       // 0x013B86C8
struct CThingPhysical {
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                               // +0x10 (1 creature, 2 player)
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad50[0x70 - 0x50];
    const CThingPhysicalDef* PBaseDef;                      // +0x70
    unsigned char _pad74[0xb0 - 0x74];
    float MaxHealth;                                        // +0xb0
    float Health;                                           // +0xb4
    unsigned char _padb8[4];
    unsigned char Damageable : 1;                           // +0xbc
    unsigned char Killable : 1;
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void SetAsFrameUpdated();                               // 0x004C7C40
private:
    void InternalModifyHealth(float delta, bool allowDeath);
};

void CThingPhysical::InternalModifyHealth(float delta, bool allowDeath)
{
    if (delta < 0.0f) {
        if (ThingClass == 2 && g_bPlayerInvulnerable)
            return;
        if (ThingClass == 1 && g_bCreaturesInvulnerable)
            return;
        if (!Damageable)
            return;
    }
    Health += delta;
    if (HasTC(0xad)) {
        const CTCBoss* boss = (const CTCBoss*)static_cast<const CThingPhysical*>(this)->GetTC(0xad);
        if (boss->Active && boss->PPhases) {
            float minimum = boss->GetMinHealthForPhase(boss->GetCurrentPhase());
            if (minimum > Health)
                Health = minimum - 1.0f;
        }
    }
    if (Health < PBaseDef->MinimumHealth)
        Health = PBaseDef->MinimumHealth;
    float floor = (allowDeath && Killable && !HasTC(0x84)) ? 0.0f : 1.0f;
    if (floor > Health)
        Health = floor;
    else if (Health > MaxHealth)
        Health = MaxHealth;
    if (ThingClass == 2) {
        if (HasTC(4))
            ((CTCHeroHealthObserver*)GetTC(4))->OnHealthChanged();
    } else if (ThingClass == 1) {
        if (Health <= 0.0001f && HasTC(9)) {
            CTCObservable* observable = (CTCObservable*)GetTC(9);
            if (observable->Flags & 2)
                observable->SetFlags(observable->Flags & ~2);
        }
    }
    SetAsFrameUpdated();
}