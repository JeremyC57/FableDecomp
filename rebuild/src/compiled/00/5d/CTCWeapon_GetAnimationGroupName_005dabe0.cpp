// CTCWeapon::GetAnimationGroupName() -- the weapon def's animation group name, or an empty name while carried by a creature that cannot use it
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DABE0, 103 bytes (+9 int3). __fastcall, returns const CCharString&.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0; CTCCarryable carrier getter 0x00705170
//          (catalog "CGuiControlListBox::GetCurrentSelectedEntryData" is a byte-twin label); CTCWeapon::IsUsableBy(CThing*)
//          0x005DAB10. Global empty CCharString 0x013BAF5C.
// Shape: interface 0x47 (CTCCarryable, bitset word 2 bit 7 -> `mov cl,[eax+28h] ; test cl,cl ; jns`); a live carrier
//        (Dead bit 0 of +0x91 clear) that fails IsUsableBy returns the global empty string, every other path returns the
//        def's CCharString at +0x34 read straight through the CDefPointer (no add-ref).
// Layout: CTCWeapon ParentThing +0x04, WeaponDef +0x0C; retail CThing bitset +0x20, interface map +0x44, Dead +0x91.
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCharString { public: void* PStringData; };
extern CCharString g_emptyCharString;            // 0x013BAF5C
class CWeaponDef { public:
    unsigned char _pad_0x00[0x34];
    CCharString AnimationGroupName;              // +0x34
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CTCCarryable {
public:
    CThing const* GetPCarriedByThing() const;   // 0x00705170 (Ego_r.pdb ?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ)
private:                   // 0x00705170
};
class CTCWeapon { public:
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[4];
    CDefPointer<CWeaponDef> WeaponDef;           // +0x0c
    bool IsUsableBy(CThing* thing) const;        // 0x005DAB10
    const CCharString& GetAnimationGroupName() const;
};
const CCharString& CTCWeapon::GetAnimationGroupName() const
{
    CTCCarryable* carryable;
    if (ParentThing->GetInterface(&carryable, 0x47)) {
        CThing* carrier = ((CThing*)((CTCCarryable*)carryable)->GetPCarriedByThing());
        if (carrier && !(carrier->Flags91 & 1) && !IsUsableBy(carrier))
            return g_emptyCharString;
    }
    return WeaponDef->AnimationGroupName;
}