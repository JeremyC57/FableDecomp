// CThingCreatureBase::UnsheatheWeapon(long weaponDefIndex, bool useActiveSlot) -- take the weapon of that def out of the carrying component (or create one at the creature's position) and put it in the carryable def's active or secondary-active carry slot
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006648D0, length 233 bytes, `ret 8`.
// Callees: CInterfaceMap::LowerBound @ 0x0040F020 (TC 0x46 = CTCCarrying), CTCCarrying::IsCarryingThing(long) @ 0x006A10A0,
//   CTCCarrying::GetCarriedThing(long) @ 0x006A1120, CTCCarrying::RemoveThing(CThing*) @ 0x006A19B0,
//   CTCCarrying::PutThingInCarrySlot(CThing*, long, bool) @ 0x006A1F80, CThing::GetPos @ 0x004C73D0,
//   CThingObject::Create @ 0x00703210 (`__fastcall`; declared as the landed callers spell it, CreateObject),
//   CDefClassBase::GetSubDef<CCarryableDef> @ 0x005CC10E, CCharString(const char*, long) @ 0x0099EBF0 /
//   ~CCharString @ 0x0099EAE0, the def's virtual Destroy (slot 1).
// Literal: 0x0122D70E = "" (the new object's script name).
// Layout: retail CThing interface bitset +0x20 (TC 0x46 = word +0x28 bit 6), interface map +0x44, PBaseDef +0x70,
//   PlayerNumber +0x90; CCarryableDef ActiveCarrySlot +0x28, SecondaryActiveCarrySlot +0x2c.
// Shape: the carrying component comes from an inline accessor whose result is left unset when the TC is absent (the
//   absent arm reads the never-written local from its dead argument slot); the "" name is an implicit conversion
//   temporary (built before the other arguments); the def out pointer sits in the dead index slot and is released
//   without a null test.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CCarryableDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long ActiveCarrySlot;                       // +0x28
    long SecondaryActiveCarrySlot;              // +0x2c
    long PassiveCarrySlot;                      // +0x30
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<const CCarryableDef>* out);   // 0x005CC10E GetSubDef<CCarryableDef>
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CCarryableDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing;
class CThingObject;
class CTCCarrying { public:
    bool IsCarryingThing(long defIndex);                         // 0x006A10A0
    CThing* GetCarriedThing(long defIndex);                      // 0x006A1120
    void RemoveThing(CThing* thing);                             // 0x006A19B0
    void PutThingInCarrySlot(CThing* thing, long slot, bool flag);   // 0x006A1F80
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad54[0x1c];
    CDefClassBase* PBaseDef;                    // +0x70 CDefPointer<CThingBaseDef const>::Object
    unsigned char _pad74[0x90 - 0x74];
    signed char PlayerNumber;                   // +0x90
    const C3DVector& GetPos() const;       // 0x004C73D0
    __forceinline CCarryableDef* GetCarryableDef()
    {
        CDefPointer<const CCarryableDef> out;
        PBaseDef->GetSubDef(&out);
        return const_cast<CCarryableDef*>(out.Object);
    }
    __forceinline CTCCarrying* GetCarrying()
    {
        CTCCarrying* carrying;
        if (UsingComponents[0x46 >> 5] & (1UL << (0x46 & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(0x46);
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || 0x46 < it->first)
                it = map->end();
            carrying = (CTCCarrying*)it->second;
        }
        return carrying;
    }
};

class CRightHandedSet;
class CThingObject { public:
    static CThingObject* __fastcall Create(long a0, C3DVector const&a1, long a2, CRightHandedSet const*a3, unsigned long a4, CCharString const&a5);   // 0x00703210 (Ego_r.pdb ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z)
};

class CThingCreatureBase : public CThing {
public:
    void UnsheatheWeapon(long weaponDefIndex, bool useActiveSlot);
};

void CThingCreatureBase::UnsheatheWeapon(long weaponDefIndex, bool useActiveSlot)
{
    if (weaponDefIndex <= 0)
        return;
    CTCCarrying* carrying = GetCarrying();
    CCarryableDef* def;
    CThing* weapon;
    if (carrying->IsCarryingThing(weaponDefIndex)) {
        weapon = carrying->GetCarriedThing(weaponDefIndex);
        carrying->RemoveThing(weapon);
    } else {
        weapon = (CThing*)CThingObject::Create(weaponDefIndex, *(const_cast<C3DVector*>(&GetPos())), PlayerNumber, 0, 0, "");
    }
    CDefPointer<const CCarryableDef> out;
    weapon->PBaseDef->GetSubDef(&out);
    def = const_cast<CCarryableDef*>(out.Object);
    carrying->PutThingInCarrySlot(weapon, useActiveSlot ? def->ActiveCarrySlot : def->SecondaryActiveCarrySlot, true);
    if (--def->RefCount == 0)
        def->Destroy();
}