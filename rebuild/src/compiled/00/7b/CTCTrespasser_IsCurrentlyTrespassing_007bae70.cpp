// CTCTrespasser::IsCurrentlyTrespassing() -- true when the owner is inside a live building (through its interface-117 component) whose trespassable component says it is being trespassed in
// Leaf: IsCurrentlyTrespassing
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe @ 0x007BAE70, 138 bytes.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (two inlined GetTC lookups: interface 117 = bit 21
//          of the bitset word at CThing+0x2c, interface 189 = bit 29 of the word at +0x34), CBaseIntelligentPointer::GetPItem
//          0x00A01B50.
// Layout: retail CThing: bitset +0x20, interface map +0x44, Dead flag +0x91 bit 0; the interface-117 component holds the
//         building as a CIntelligentPointer<CThing> at +0x0c; Ego_r.pdb CTCTrespassable: Trespassable +0x0c,
//         IsBeingTrespassedIn +0x0d.
// Shape: nested ifs ending in one shared `xor al,al` exit; the first lookup is HasTC (`if (bits & mask) return true;
//        return false;`) + a __forceinline GetTC, the second the __forceinline out-pointer GetInterface (a returning GetTC
//        there loads the component into edx instead of eax); the key lives in the `push ecx` slot.
class CTCBase;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;

struct CTCBuildingLocation {                    // the owner's interface-117 component (class name unknown)
    unsigned char _pad_0x00[0xc];
    CBaseIntelligentPointer PBuilding;          // +0x0c
};
class CTCTrespassable { public:
    unsigned char _pad_0x00[0xc];
    bool Trespassable;                          // +0x0c
    bool IsBeingTrespassedIn;                   // +0x0d
};

class CTCTrespasser { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool IsCurrentlyTrespassing();
};

bool CTCTrespasser::IsCurrentlyTrespassing()
{
    CThing* thing = ParentThing;
    if (thing->HasTC(TCI_VOLUME_CONTAINMENT_TRACKER)) {
        CTCBuildingLocation* location = (CTCBuildingLocation*)((CTCBuildingLocation*)&thing->GetTC(TCI_VOLUME_CONTAINMENT_TRACKER));
        CThing* building = ((CThing*)location->PBuilding.GetPItem());
        CTCTrespassable* trespassable;
        if (building && !building->IsDead() && building->GetTC(TCI_TRESPASSABLE, &trespassable))
            return trespassable->IsBeingTrespassedIn;
    }
    return false;
}