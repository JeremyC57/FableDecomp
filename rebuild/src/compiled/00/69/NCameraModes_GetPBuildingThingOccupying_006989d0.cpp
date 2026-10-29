// NCameraModes::GetPBuildingThingOccupying(const CThing& thing) -- the live building (thing type 3) that the thing's component 0x75 points at, or 0
// Leaf: ?GetPBuildingThingOccupying@NCameraModes@@YIPBVCThingBuilding@@ABVCThing@@@Z
//
// Retail 0x006989D0, 83 bytes (+ int3 padding to 96), `ret` (__fastcall: the thing in ecx).
// Callees: the const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (inline GetTC),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Callers: 0x006994CB, 0x0069AD45, 0x0069B6AD (camera modes), 0x0075DB1E.
// Layout (retail, as the landed CThingFilter_IsPotentialCustomer::operator() 0x00799E30 / CTCShopKeeper 0x00799140):
//   CThing thing-type byte +0x10, interface bitset +0x20 (0x75 = dword +0x2C bit 21), interface map +0x44, Dead
//   +0x91 bit 0; component 0x75 holds the building intelligent pointer at +0x0C.
// Shape: `if (thing.HasTC(0x75)) { building = GetTC(0x75)->Building.GetPItem(); if (building && !building->Dead &&
//   building->ThingType == 3) return building; } return 0;` -- one GetPItem call (no IS_THING_ALIVE re-reads); the
//   key temporary is the `push ecx` slot taken before the bitset test.
// Name: Ego_r.pdb `?GetPBuildingThingOccupying@NCameraModes@@YIPBVCThingBuilding@@ABVCThing@@@Z`.
class CThing;
class CThingBuilding;

class CTCBase { public: void* vfptr; CThing* ParentThing; };

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
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

class CThing {
public:
    unsigned char  _pad00[0x10];
    unsigned char  ThingType;                               // +0x10
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  m_presentBits[9];                        // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                            // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                                // +0x91 bit 0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
};

namespace NCameraModes {
const CThingBuilding* __fastcall GetPBuildingThingOccupying(const CThing& thing)
{
    if (thing.HasTC(0x75)) {
        const CThing* building = ((const CTCBuildingOccupant*)thing.GetTC(0x75))->Building.GetPItem();
        if (building && !building->Dead && building->ThingType == 3)
            return (const CThingBuilding*)building;
    }
    return 0;
}
}