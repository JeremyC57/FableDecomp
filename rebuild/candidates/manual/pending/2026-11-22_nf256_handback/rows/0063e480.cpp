// CWorldMap::GetHeightZBelowPos(const C3DVector& pos) const -- the highest physics-mesh surface between pos and the
// ground below it: starts from the ground height, gathers every thing with a physics mesh in a 4x4 box round the position
// and casts the segment pos -> (pos.X, pos.Y, ground) against each thing's physics mesh, keeping the highest intersection Z
// Leaf: ?GetHeightZBelowPos@CWorldMap@@QBEMABVC3DVector@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// NF256 (2026-11-22, CODEXP27 L28): ported off the row-local CWorld / CMainGameComponent / CThingSearchTools / CPhysicsMeshObject /
//   CTCPhysicsBase / C2DBoxI / vector models and the same-TU strong stub onto the shared headers, on the define block of the
//   0x0049C490 self row (the GetAllThingsInRoughArea<const CThing, vector<const CThing*>, CThingFilter_HasPhysicsMesh> body, noinline
//   as in 0x0079A890: FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_NOINLINE_PASS27). The filter is CThingFilter_HasPhysicsMesh: retail
//   calls 0x0049C490, the address whose landed self row and binder 0x0049BE90 are that instantiation.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E480, 272 bytes, `ret 4`. PDB ?GetHeightZBelowPos@CWorldMap@@QBEMABVC3DVector@@@Z.
// Callees: GetGroundSizeZAt 0x0063A300, __ftol2 0x00BFEA70 (x2), CThingSearchTools::GetAllThingsInRoughArea<const CThing,
//   vector<const CThing*>, CThingFilter_HasPhysicsMesh> 0x0049C490 (`ret 0Ch`), CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850,
//   CPhysicsMeshObject::GetPointIntersectionFromTo 0x00A12450 (both PDBs: by reference), ~CPhysicsMeshObject 0x00A127D0,
//   free thunk 0x00BFEA14.
// Layout: CMainGameComponent::Get() 0x013B86A0 -> +0x24 world -> +0x20 thing search tools; CThing::PhysicsTC +0x60.
//   No CWorldMap data members are read.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_NOINLINE_PASS27
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#include "stlport/fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasPhysicsMesh.h"

float CWorldMap::GetHeightZBelowPos(const C3DVector& pos) const
{
    float ground = GetGroundSizeZAt(pos);
    float best = ground;
    std::vector<const CThing*> things;
    long x = (long)pos.X;
    long y = (long)pos.Y;
    CMainGameComponent::Get()->GetWorld().GetThingSearchTools().GetAllThingsInRoughArea<const CThing>(C2DBoxI(x - 2, y - 2, x + 2, y + 2), CThingFilter_HasPhysicsMesh(), things);
    for (std::vector<const CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
        const CTCPhysicsBase* physics = (*it)->GetPhysicsTC();
        C3DVector below(pos.X, pos.Y, ground);
        CMeshIntersectionData data;
        bool hit = physics->GetPhysicsMeshObject().GetPointIntersectionFromTo(pos, below, data);
        if (hit && data.IntersectionPosition.Z > best)
            best = data.IntersectionPosition.Z;
    }
    return best;
}
