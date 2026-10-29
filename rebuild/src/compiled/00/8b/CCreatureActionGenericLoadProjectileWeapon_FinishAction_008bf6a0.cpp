// CCreatureAction_GenericLoadProjectileWeapon::FinishAction() -- if the creature carries a live weapon with a weapon component (0x48), mark its projectile as loaded; then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BF6A0, length 145 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (x2),
//          CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::SetProjectileWeaponLoaded(bool, CThing*)
//          @ 0x005DAC50, CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: the carrying component (0x46) comes from the bool-returning out-parameter GetInterface template; the weapon
//   component (0x48) is a HasTC test (flags dword loaded into ecx, `test ch,1`) plus a pointer-returning lookup whose
//   result goes straight into ecx for the call (the out-parameter form parks it in eax first); the carried thing's alive test is inline on the raw pointer (null, then dead bit +0x91);
//   everything joins on the base FinishAction call.
// Layout: CCombatActionBase (retail 0x114 bytes) with PCreature at +8; nothing else of the class is touched.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    unsigned long HasTC(long interfaceType) const   // the masked presence word (a bool return narrows to shr/and)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* FindTC(long interfaceType)   // the lookup alone, result straight into a register
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
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

class CTCCarrying { public:                        // interface type 0x46 component
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

class CTCWeapon { public:                          // interface type 0x48 component
    void SetProjectileWeaponLoaded(bool loaded, CThing* projectile);   // 0x005DAC50
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
    protected: virtual void FinishAction(); public:            // 0x00859A20
};

class CCreatureAction_GenericLoadProjectileWeapon : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_GenericLoadProjectileWeapon::FinishAction()
{
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon != 0 && !(weapon->m_flags91 & 1)) {
            if (weapon->HasTC(0x48))
                ((CTCWeapon*)weapon->FindTC(0x48))->SetProjectileWeaponLoaded(true, 0);
        }
    }
    CCombatActionBase::FinishAction();
}