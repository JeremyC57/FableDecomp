// CFilter_Bind3::operator()(const CThingPhysical& thing) const -- (CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, T = CThingPhysical) the thing is not the excluded one, has navigation information and its physics mesh's bounding box contains the filter point
// Leaf: ??$?RVCThingPhysical@@@?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@QBE_NABVCThingPhysical@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063F1E0, 150 bytes, `ret 4`. No direct or absolute references (an unreferenced COMDAT
//   copy, like the Bind2 0x0063F150 beside it); the same body is inlined in the Bind3 ForEachInRoughArea 0x0063F7C0 (`mov ebx,[edi+8] ;
//   push ebx`) that CWorld::GetNavigationLayerAt 0x0063E590 calls with exactly this filter type.
// Names (agentNF27, 2026-11-12; was the descriptive CThingFilter_IsNotAndBlockerBoundsContainPoint::operator(), whose notes read +8 as a
//   by-value member): FableWin.pdb carries this exact symbol (and the Bind3 ctor / GFFilter_Bind3 / ForEachInRoughArea<CThingPhysical,
//   this Bind3, CThingFindNavigationLayerAt>, which Ego_r.pdb also has). CFilter_Bind3 holds its three filters by reference
//   (engine/CFilter_Bind3.h, Ego_r TPI: F1 +0, F2 +4, F3 +8): F1 is passed to CThingFilter_IsNot::operator() 0x006A76A0 (`mov ecx,[edi]`),
//   F2 (the empty HasNavigationInformation) is never read, and F3 is a POINTER (`mov edi,[edi+8]` at 0x0063F229) to the
//   OverlappingBoundingBox filter whose Pos (+0) is pushed as the point.
// Callees: CThingFilter_IsNot::operator() 0x006A76A0 (engine/CThingFilter_IsNot.h), CTCPhysicsBase::HasNavigationLayers (slot 42,
//   +0xA8), CThingPhysical::CanBlockNavigatingCreature (slot 55, +0xDC) on both arms, CTCPhysicsBase::GetPhysicsMeshInfo (slot 93,
//   +0x174), CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::IsPointContainedInBoundingBox 0x00A12890,
//   ~CPhysicsMeshObject 0x00A127D0.
// Shape: as 0x0063F150 (HasNavigationInformation inline, both virtual arms kept; OverlappingBoundingBox inline with the mesh object a
//   named local), after the out-of-line IsNot test.
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
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind3.h"

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

template bool CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>::operator()<CThingPhysical>(const CThingPhysical&) const;
