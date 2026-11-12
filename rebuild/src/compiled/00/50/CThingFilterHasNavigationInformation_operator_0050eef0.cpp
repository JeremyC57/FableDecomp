// CThingFilter_HasNavigationInformation::operator()(const CThingPhysical& thing) const -- ask the physics component whether it
// has navigation layers, then return the thing's CanBlockNavigatingCreature() on either path
// Leaf: ??RCThingFilter_HasNavigationInformation@@QBE_NABVCThingPhysical@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050EEF0, 52 bytes, `ret 4`.
// Name (agentNF27, 2026-11-12; was _unattributed::CanBlockNavigatingCreatureFilter on a CThingFilter_0050EEF0 stand-in): Ego_r.pdb
// (0x004E0840) and FableWin.pdb (0x01CDA8E0) ??RCThingFilter_HasNavigationInformation@@QBE_NABVCThingPhysical@@@Z; both PDBs
// pair it as the second filter of CFilter_Bind2<CThingFilter_BoundingBoxOverlapsMap, CThingFilter_HasNavigationInformation>
// (Ego_r ??$GetAllThings@$$CBVCThingPhysical@@...CFilter_Bind2@VCThingFilter_BoundingBoxOverlapsMap@@VCThingFilter_
// HasNavigationInformation@@...), exactly the two retail callers 0x00823738 / 0x0082673B (first filter
// CThingFilter_BoundingBoxOverlapsMap::operator() 0x006A7FB0, then `mov ecx,[filter+4] ; push thing ; call 0x0050EEF0`). The
// same body is expanded inline in CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>
// 0x0063F150. `this` is never read (an empty filter).
// Callees: CTCPhysicsBase::HasNavigationLayers (slot 42, +0xA8) and CThingPhysical::CanBlockNavigatingCreature (slot 55, +0xDC,
// 0x006A4D90), through the shared engine/CThingPhysical.h / engine/CTCPhysicsBase.h retail layouts (PhysicsTC +0x60).
// Shape: both arms end in the same `call [vtbl+0DCh] ; pop esi ; ret 4` -- VC7.1 keeps two explicit returns apart (no tail
// merge); the component pointer is a named local so it loads straight into ecx.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"

class CThingFilter_HasNavigationInformation {
public:
    bool operator()(const CThingPhysical& thing) const;
};

bool CThingFilter_HasNavigationInformation::operator()(const CThingPhysical& thing) const
{
    const CTCPhysicsBase* physics = thing.GetPhysicsTC();
    if (physics && physics->HasNavigationLayers())
        return thing.CanBlockNavigatingCreature();
    return thing.CanBlockNavigatingCreature();
}
