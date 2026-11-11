// CTCActionUseAcceptBoast::OnActionUse(CThing& user) [Ego_r.pdb; was vfunc_28(CThingCreatureBase*)] -- run the base slot, then tell the parent thing's boasting-position component (interface 0x71) that the creature accepted
// Leaf: OnActionUse
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EBBC0, length 96 bytes. Slot 28 of the
// CTCActionUseAcceptBoast vtable 0x0123F674 (rtti-vtables.tsv).
// Callees: CTCActionUseBase's slot 28 @ 0x007EAB90 (an empty `ret 4`, called non-virtually; manifest "$E2"),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0x71
// = bit 17 of the bitset dword at +0x2c), CTCBoastingPosition::OnBoastAccepted(CThingCreatureBase*) @
// 0x00782390.
//
// Shape: the creature argument is held in edi across the base call; then the inlined GetInterface
// template (landed 0x00949320) with an UNINITIALISED component local -- the key temporary and the
// never-written local both live in the argument's own (dead) stack slot (`lea eax,[esp+0Ch]` / the miss
// path's `mov eax,[esp+0Ch]`), so there is no frame of its own.
// Header note: engine/CTCBoastingPosition.h holds the layout only; the callee is modelled as
// CTCBoastingPosition_Calls.
class CTCBase;
class CThingCreatureBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCBoastingPosition_Calls {                    // interface type 0x71 component
    void OnBoastAccepted(CThingCreatureBase* creature);   // 0x00782390
};

class CTCActionUseBase { public:
    // +0x00 vptr (OnActionUse is virtual)
    CThing* ParentThing;                        // +0x04 (CTCBase)
    virtual void OnActionUse(CThing& user);   // 0x007EAB90 Ego_r.pdb ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z (empty `ret 4`, RTTI slot 28; was vfunc_28(CThingCreatureBase*))
};

struct CTCActionUseAcceptBoast : CTCActionUseBase {
    virtual void OnActionUse(CThing& user);   // Ego_r.pdb (UAE; RTTI slot 28, was vfunc_28(CThingCreatureBase*))
};

void CTCActionUseAcceptBoast::OnActionUse(CThing& thing)
{
    CThingCreatureBase* creature = (CThingCreatureBase*)&thing;
    CTCActionUseBase::OnActionUse(thing);
    CTCBoastingPosition_Calls* position;
    ParentThing->GetTC(TCI_BOASTING_POSITION, &position);
    position->OnBoastAccepted(creature);
}