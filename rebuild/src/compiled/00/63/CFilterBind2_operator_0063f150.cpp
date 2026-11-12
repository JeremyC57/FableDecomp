// CFilter_Bind2::operator()(const CThingPhysical& thing) const -- (CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, T = CThingPhysical) the thing has navigation information and its physics mesh's bounding box contains the filter point
// Leaf: ??$?RVCThingPhysical@@@?$CFilter_Bind2@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@QBE_NABVCThingPhysical@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063F150, 138 bytes (+ int3 padding), `ret 4`. No direct or absolute references (an
//   unreferenced COMDAT copy); the same test is inlined in the Bind2 ForEachInRoughArea 0x0063F4B0 that CWorld::GetNavigationLayerAt
//   0x0063E590 calls with exactly this filter type; 0x0063F1E0 is the CFilter_Bind3<CThingFilter_IsNot, ...> twin.
// Names (agentNF27, 2026-11-12; was the descriptive CThingFilter_BlockerBoundsContainPoint::operator()): FableWin.pdb carries this exact
//   symbol (and the Bind2 ctor / GFFilter_Bind2 / ForEachInRoughArea<CThingPhysical, this Bind2, CThingFindNavigationLayerAt>); Ego_r.pdb
//   has the same ForEachInRoughArea instance and ??RCThingFilter_HasNavigationInformation@@QBE_NABVCThingPhysical@@@Z. CFilter_Bind2 holds
//   its two filters by reference (engine/CFilter_Bind2.h, Ego_r TPI: F1 +0, F2 +4); F1 (the empty HasNavigationInformation) is never
//   read, F2 (`mov edi,[edi+4]`) is the OverlappingBoundingBox filter whose Pos (+0, FableWin ??0CThingFilter_OverlappingBoundingBox@@QAE@ABVC3DVector@@M@Z:
//   Pos then Radius +0xC) is pushed as the point.
// Callees: CTCPhysicsBase::HasNavigationLayers (slot 42, +0xA8), CThingPhysical::CanBlockNavigatingCreature (slot 55, +0xDC) on both
//   arms, CTCPhysicsBase::GetPhysicsMeshInfo (slot 93, +0x174), CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (hidden 0x40-byte result),
//   CPhysicsMeshObject::IsPointContainedInBoundingBox 0x00A12890, ~CPhysicsMeshObject 0x00A127D0.
// Shape: the two filters' operator()s are inline in the Bind2's `F1(thing) && F2(thing)`; HasNavigationInformation keeps both identical
//   virtual calls (VC7.1 does not merge the arms); OverlappingBoundingBox reads the physics TC again and keeps the mesh object as a named
//   local destroyed at its end.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CFilter_Bind2.h"

class CThingFilter_HasNavigationInformation {
public:
    bool operator()(const CThingPhysical& thing) const
    {
        const CTCPhysicsBase* physics = thing.GetPhysicsTC();
        if (physics && physics->HasNavigationLayers())
            return thing.CanBlockNavigatingCreature();
        return thing.CanBlockNavigatingCreature();
    }
};

class CThingFilter_OverlappingBoundingBox {
public:
    C3DVector Pos;                                                       // +0x0
    float     Radius;                                                    // +0xC
    FABLE_FORCEINLINE bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.GetPhysicsTC();
        if (physics->GetPhysicsMeshInfo() == 0)
            return false;
        CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
        return mesh.IsPointContainedInBoundingBox(Pos);
    }
};

template bool CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>::operator()<CThingPhysical>(const CThingPhysical&) const;
