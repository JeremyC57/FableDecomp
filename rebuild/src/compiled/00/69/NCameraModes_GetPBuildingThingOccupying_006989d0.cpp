// NCameraModes::GetPBuildingThingOccupying(const CThing& thing) -- the live building (thing type 3) that the thing's component 0x75 points at, or 0
// Leaf: ?GetPBuildingThingOccupying@NCameraModes@@YIPBVCThingBuilding@@ABVCThing@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
//
// Retail 0x006989D0, 83 bytes (+ int3 padding to 96), `ret` (__fastcall: the thing in ecx).
// Callees: the const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (inline GetTC),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Callers: 0x006994CB, 0x0069AD45, 0x0069B6AD (camera modes), 0x0075DB1E.
// Layout (retail, as the landed CThingFilter_IsPotentialCustomer::operator() 0x00799E30 / CTCShopKeeper 0x00799140):
//   CThing thing-type byte +0x10, interface bitset +0x20 (0x75 = dword +0x2C bit 21), interface map +0x44, Dead
//   +0x91 bit 0; component 0x75 holds the building intelligent pointer at +0x0C.
// Shape: `if (thing.HasTC(0x75)) { building = GetTC(0x75)->Building.GetPItem(); if (building && !building->IsDead() &&
//   building->GetType() == 3) return building; } return 0;` -- one GetPItem call (no IS_THING_ALIVE re-reads); the
//   key temporary is the `push ecx` slot taken before the bitset test.
// Name: Ego_r.pdb `?GetPBuildingThingOccupying@NCameraModes@@YIPBVCThingBuilding@@ABVCThing@@@Z`.
class CThing;
class CThingBuilding;

class CTCBase { public: void* vfptr; CThing* ParentThing; };

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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

struct CTCBuildingOccupant : CTCBase {                      // component 0x75 (descriptive)
    unsigned char           _pad08[4];
    CIntelligentPointer<CThing> Building;                       // +0x0C
};

namespace NCameraModes {
const CThingBuilding* __fastcall GetPBuildingThingOccupying(const CThing& thing)
{
    if (thing.HasTC(TCI_VOLUME_CONTAINMENT_TRACKER)) {
        const CThing* building = ((const CTCBuildingOccupant*)((const CTCBuildingOccupant*)&thing.GetTC(TCI_VOLUME_CONTAINMENT_TRACKER)))->Building.GetPItem();
        if (building && !building->IsDead() && building->GetType() == 3)
            return (const CThingBuilding*)building;
    }
    return 0;
}
}