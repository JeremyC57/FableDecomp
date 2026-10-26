// CCreatureAction_SheatheItemToInventory::Initialise() -- base Initialise, then if the creature has hero ranged attacks (interface type 0x81): cancel aiming, reset the projectile weapon mode and drop player mode 0x22
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F480, length 110 bytes (+ int3 padding).
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// twice), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x81 = bit 1 of the bitset dword at +0x30, `test byte ptr`),
// CTCHeroRangedAttacks::CancelAimingWeapon @ 0x00789FF0, CTCHeroRangedAttacks::ResetProjectileWeaponMode @ 0x00789B30, CThing::GetPlayer @ 0x004C7A30,
// CPlayer::RemoveMode(EPlayerMode) @ 0x00634190.
// Shape: the out-parameter GetInterface as the if condition; the component rides in esi (re-used after `this` is dead) for both of its calls,
// &PCreature in edi. The PlayerMode argument is pushed before the creature/player chain (a chained call evaluates its argument first).
// No usable engine header layout (retail CCreatureActionBase is 0xa8 bytes); modelled like the landed _ForEnflame relative 0x0069F750.
class CTCBase;
class CPlayer { public:
    void RemoveMode(long mode);             // 0x00634190 (EPlayerMode by value)
};
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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
public:
    CPlayer& GetPlayer();   // 0x004C7A30
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCHeroRangedAttacks { public:               // interface type 0x81 component
    void CancelAimingWeapon();              // 0x00789FF0
    void ResetProjectileWeaponMode();       // 0x00789B30
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_SheatheItemToInventory::Initialise()
{
    CCreatureActionBase::Initialise();
    CTCHeroRangedAttacks* pRanged;
    if (PCreature.GetPItem()->GetInterface(&pRanged, 0x81)) {
        pRanged->CancelAimingWeapon();
        pRanged->ResetProjectileWeaponMode();
        PCreature.GetPItem()->GetPlayer().RemoveMode(0x22);
    }
}