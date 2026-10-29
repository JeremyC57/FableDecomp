// CTCMultiStrike::FinalCleanup() -- find the parent's weapon (the inventory's active melee weapon, else the thing carried in its primary slot) and take the multi-strike effect off it
// Leaf: ?FinalCleanup@CTCMultiStrike@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CCD70, 137 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?FinalCleanup@CTCMultiStrike@@AAEXXZ (private void()); its only references are the
//   CTCMultiStrike vtable thunks (vtable 0x0126EE0C) 0x007CCE00 (slot 14, OnKill) and 0x007CCE10 (slot 2, OnRemove) that
//   tail-jump here (the ability-TC pattern of the landed CTCWillLightning::FinalCleanup 0x007A0950). Label correction: the
//   landed 0x007CCE10 is catalogued as FinalCleanup (it is OnRemove), 0x007CCE00 as vfunc_14 (OnKill).
// Callees: the const CVectorMap LowerBound 0x004365B0 (inline const GetInterface: 0x13 CTCInventoryWeapons, 0x46
//   CTCCarrying -- interface ids from their GetInterfaceType slots), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCCarrying::GetThingInPrimarySlot() const 0x006A1440 (Ego_r.pdb), CTCMultiStrike::RemoveEffectOnWeapon(CThing*)
//   0x007CC9E0 (Ego_r.pdb).
// Layout: CTCInventoryWeapons PActiveMeleeWeapon +0x134 (Ego_r +0x14C); CThing bitset +0x20, interface map +0x44.
// Shape: the lookups go through a const parent (the const LowerBound copy); a missing weapon interface falls to the
//   carrying lookup, and both arms meet at the null test before the call.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CTCInventoryWeapons {                         // interface 0x13
public:
    unsigned char _pad00[0x134];
    CIntelligentPointer<CThing> PActiveMeleeWeapon; // +0x134
};
class CTCCarrying {                                 // interface 0x46
public:
    CThing* GetThingInPrimarySlot() const;          // 0x006A1440
};

class CTCMultiStrike {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    void RemoveEffectOnWeapon(CThing* weapon);      // 0x007CC9E0
private:
    void FinalCleanup();
};

void CTCMultiStrike::FinalCleanup()
{
    const CThing* parent = ParentThing;
    CThing* weapon = 0;
    const CTCInventoryWeapons* weapons;
    const CTCCarrying* carrying;
    if (parent->GetInterface(&weapons, 0x13))
        weapon = weapons->PActiveMeleeWeapon.GetPItem();
    else if (parent->GetInterface(&carrying, 0x46))
        weapon = carrying->GetThingInPrimarySlot();
    if (weapon)
        RemoveEffectOnWeapon(weapon);
}