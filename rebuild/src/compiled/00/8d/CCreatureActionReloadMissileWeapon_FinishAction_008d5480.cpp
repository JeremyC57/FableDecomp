// CCreatureAction_ReloadMissileWeapon::FinishAction() -- take the live ammo thing out of the creature's carry slots, kill it, clear the pointer and mark the weapon in the primary slot loaded; then the base FinishAction
// Leaf: FinishAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D5480, 283 bytes (288 with padding), virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PAmmo +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020
//   (inlined lookups: 0x46 CTCCarrying, bit 6 of +0x28; 0x48 CTCWeapon, bit 8 of +0x28), CTCCarrying::RemoveThing 0x006A19B0,
//   CThing::Kill(bool) 0x004C9B80, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10
//   (CIntelligentPointer vtable 0x0125A858), CTCCarrying::GetThingInPrimarySlot 0x006A1440,
//   CTCWeapon::SetProjectileWeaponLoaded(bool, CThing*) 0x005DAC50, CCreatureActionBase::FinishAction 0x00694430.
// Layout: retail CCreatureActionBase is 0xa8 bytes: the ammo pointer is this class's first member; CThing +0x91 dead bit.
// Shape: IS_THING_ALIVE + the out-parameter GetInterface (carrying in edi across the calls); the pointer clear is the
//   landed "assign through a stack CIntelligentPointer" shape (0x008C99C0: default-construct, SetPItem(0), copy the item
//   into the member, destroy) in its own block, so the lookup keys share the stack pointer's words (8-byte frame); the
//   weapon lookup is the pointer-returning form so the component loads straight into ecx.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    void Kill(bool immediately);                      // 0x004C9B80
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
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
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0125A858
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCCarrying { public:                                  // interface type 0x46
    void RemoveThing(CThing* thing);                  // 0x006A19B0
    CThing* GetThingInPrimarySlot();                  // 0x006A1440
};
class CTCWeapon { public:                                    // interface type 0x48
    void SetProjectileWeaponLoaded(bool loaded, CThing* projectile);   // 0x005DAC50
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                      // slot 4: 0x00694430
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_ReloadMissileWeapon : public CCreatureActionBase { public:
    CBaseIntelligentPointer PAmmo;                    // +0xa8
    virtual void FinishAction();
};

void CCreatureAction_ReloadMissileWeapon::FinishAction()
{
    CTCCarrying* carrying;
    if (IS_THING_ALIVE(PAmmo) && PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
        carrying->RemoveThing(PAmmo.GetPItem());
        PAmmo.GetPItem()->Kill(true);
        {
            CIntelligentPointer none;
            none.SetPItem(0);
            PAmmo.SetPItem(none.GetPItem());
        }
        CThing* weaponThing = carrying->GetThingInPrimarySlot();
        if (weaponThing && !(weaponThing->m_flags91 & 1) && weaponThing->HasTC(0x48))
            ((CTCWeapon*)weaponThing->FindTC(0x48))->SetProjectileWeaponLoaded(true, 0);
    }
    CCreatureActionBase::FinishAction();
}