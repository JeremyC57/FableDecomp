// CThingFilter_HasBeenDiscovered::operator()(const CThing& thing) const -- a thing with a discovery component reports its Discovered flag; a thing without one counts as discovered
// Leaf: ??RCThingFilter_HasBeenDiscovered
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A84B0, length 63 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0.
// Identity: Ego_r.pdb `??RCThingFilter_HasBeenDiscovered@@QBE_NABVCThing@@@Z`; the landed caller
//   GetPBestThingInRoughArea<CFilter_Bind4<...,unary_negate<CThingFilter_HasBeenDiscovered>>> 0x00937420 names this
//   address, and 0x00937360 (unary_negate::operator()) negates it. The catalog's "ETCInterfaceType_LookupCapability"
//   is a descriptive auto label. The filter is an empty class (`this` unused).
// Layout: CTCDiscovery (Ego_r.pdb, 24 bytes) bool Discovered at +0x0c; interface type 0x40 = bit 0 of the presence
//   word +0x28; the thing's {kind, component} vector map at +0x44 (local model of the CTCCarrying rows).
// Shape: PeekTC<CTCDiscovery> with the conversion-temporary key homed in the dead thing-argument slot (no frame),
//   the component loaded into eax for the byte read; the absent TC returns `mov al,1`.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCDiscovery {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TCI_DISCOVERY; }
    unsigned char _pad00[0x0c];                     // +0x00 CTCBase
    bool Discovered;                                // +0x0c
};

class CThingFilter_HasBeenDiscovered {
public:
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasBeenDiscovered::operator()(const CThing& thing) const
{
    const const CTCDiscovery* discovery;
    if (thing.PeekTC(&discovery))
        return discovery->Discovered;
    return true;
}