// CCreatureAction_UnsheatheItemFromInventory::Initialise() -- base Initialise; if the creature has the hero ranged-attacks component (interface type 0x81) cancel its aiming and reset its projectile mode, then take the creature's player out of mode 0x22
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F910, length 110 bytes.
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8, twice), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x81 = bit 1 of the
// bitset dword at +0x30, `test byte ptr [eax+30h],2`), CTCHeroRangedAttacks::CancelAimingWeapon @ 0x00789FF0,
// CTCHeroRangedAttacks::ResetProjectileWeaponMode @ 0x00789B30, CThing::GetPlayer @ 0x004C7A30, CPlayer::RemoveMode(EPlayerMode)
// @ 0x00634190.
// Shape: the &PCreature address stays in edi (named by the compiler, not the source); the component rides in esi for its two
// calls; the player chain pushes 0x22 before evaluating its object expression (natural VC argument-first order). Retail
// CCreatureActionBase is 0xa8 bytes (the PDB build is larger), so `this` is modelled locally.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CPlayer {
    void RemoveMode(long mode);             // 0x00634190 (EPlayerMode)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    CPlayer* GetPlayer();                   // 0x004C7A30
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
struct CTCHeroRangedAttacks {
    void CancelAimingWeapon();              // 0x00789FF0
    void ResetProjectileWeaponMode();       // 0x00789B30
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    void Initialise();                      // 0x00694D10
};

struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    void Initialise();
};

void CCreatureAction_UnsheatheItemFromInventory::Initialise()
{
    CCreatureActionBase::Initialise();
    CTCHeroRangedAttacks* ranged;
    if (PCreature.GetPItem()->GetInterface(&ranged, 0x81)) {
        ranged->CancelAimingWeapon();
        ranged->ResetProjectileWeaponMode();
        PCreature.GetPItem()->GetPlayer()->RemoveMode(0x22);
    }
}
