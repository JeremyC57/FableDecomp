// CCreatureAction_FireMissileWeapon::ResetCreatureUseProjectileWeaponState() -- for a live creature, release any missile still held, then cancel the hero's weapon aiming (ranged-attacks component 0x81)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D58D0, length 134 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CCreatureAction_FireMissileWeapon::ReleaseMissile
//          (CThing**, CThing**, long*) @ 0x008D5790, CVectorMap::LowerBound @ 0x0040F020,
//          CTCHeroRangedAttacks::CancelAimingWeapon @ 0x00789FF0.
// Shape: PCreature's address is taken once (esi); IS_THING_ALIVE inlined; ReleaseMissile gets the addresses of three
//   block-scoped locals (only the first is zeroed, after the pushes); the lookup key then reuses the dead first
//   local's frame word; the bool-returning out-parameter GetInterface for bit 0x81 (`test byte ptr [eax+30h],2`)
//   loads the component straight into ecx for the call.
// Layout: CCreatureActionBase (retail 0xa8 bytes) with PCreature at +8; nothing else of the class is touched.
class CTCBase;

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
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCHeroRangedAttacks { public:               // interface type 0x81 component
    void CancelAimingWeapon();              // 0x00789FF0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
};

class CCreatureAction_FireMissileWeapon : public CCreatureActionBase { public:
    void ReleaseMissile(CThing** missile, CThing** target, long* globalIndex);   // 0x008D5790
    void ResetCreatureUseProjectileWeaponState();
};

void CCreatureAction_FireMissileWeapon::ResetCreatureUseProjectileWeaponState()
{
    if (IS_THING_ALIVE(PCreature)) {
        {
            CThing* missile = 0;
            CThing* target;
            long globalIndex;
            ReleaseMissile(&missile, &target, &globalIndex);
        }
        CTCHeroRangedAttacks* ranged;
        if (PCreature.GetPItem()->GetInterface(&ranged, 0x81))
            ranged->CancelAimingWeapon();
    }
}