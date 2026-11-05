// CThingFilter_IsNotBeingExclusivelyUsed::operator()(const CThing& thing) const -- the thing has an exclusive-use component and nobody holds it
// Leaf: ??RCThingFilter_IsNotBeingExclusivelyUsed
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8AF0, length 78 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const @ 0x004365B0,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Identity: Ego_r.pdb `??RCThingFilter_IsNotBeingExclusivelyUsed@@QBE_NABVCThing@@@Z` -- the PDB lists it next to
//   HasFreeActionPoint (landed 0x006A8A80); the body reads CTCExclusiveUse::User. The catalog's
//   "CTCInterface_FindInterface" is a descriptive auto label. Callers build the empty filter on the stack
//   (`lea ecx,[esp+17h]`) in CAIStateGroup_Gaze / _Drunk / _MinionReactToCriminal / CTCDummyVillager.
// Layout: CTCExclusiveUse (Ego_r.pdb, 20 bytes) CIntelligentPointer<CThing> User at +0x0c; interface type 0x2f =
//   bit 15 of the presence word +0x24 (`test ch,ch ; jns`); the {kind, component} vector map at +0x44.
// Shape: `return PeekTC(&tc) && !tc->User.GetPItem();` -- the logical-and is materialised int-wide (`mov eax,1` /
//   `xor eax,eax`) like the landed 0x006A8B40; the conversion-temporary key sits in the dead thing-argument slot,
//   the component is loaded into eax for the `lea ecx,[eax+0Ch]`.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCExclusiveUse {
public:
    static ETCInterfaceType StatGetInterfaceType() { return TCI_EXCLUSIVE_USE; }
    unsigned char _pad00[0x0c];                     // +0x00 CTCBase
    CBaseIntelligentPointer User;                   // +0x0c CIntelligentPointer<CThing>
};

class CThingFilter_IsNotBeingExclusivelyUsed {
public:
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsNotBeingExclusivelyUsed::operator()(const CThing& thing) const
{
    const const CTCExclusiveUse* exclusive;
    return thing.PeekTC(&exclusive) && !exclusive->User.GetPItem();
}