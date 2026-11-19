// CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(const C2DBoxI& area, const filter&, const finder&) const -- runs the navigation-layer finder over every physical thing in the mapwho cells under the area that is not the ignored thing, has navigation information and whose physics mesh's bounding box contains the filter's point (each thing once per search); true when any thing was visited
// Grade: RELOCATION_MATCH 785v785 (hand4d 2026-11-19, first grade of the port): the old FOREACHINROUGHAREA_BODY copy left the
//   post-node-loop reload order (TLX first in retail); the egod body FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD (hand4c,
//   landed with 0x0066A1E0: Ego_d locals, FableWin order, edge-compare empty test) over ROUGHAREA_EGOD closes it. The filter / finder
//   locals carry Ego_d.pdb names (test_thing; tc_physics; physics, mesh; physics, object, new_layer). True length 785 (ret 0Ch at
//   0x0063FACE; the catalog row says 769).
// Leaf: ??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@VCThingFindNavigationLayerAt@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@ABVCThingFindNavigationLayerAt@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063F7C0, 785 bytes, thiscall `ret 0Ch`. Ego_r.pdb / FableWin.pdb spell it as the Leaf above
//   (const; the finder by const reference). Its one caller is the landed CWorldMap::GetNavigationLayerAt 0x0063E590 (the Bind3
//   branch, `call 0x0063F7C0` at 0x0063E6CC); the Bind2 twin is 0x0063F4B0.
// Callees: CThingSearchTools::GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
//   CThingFilter_IsNot::operator() 0x006A76A0 (out of line), CTCPhysicsBase::HasNavigationLayers (slot 42, +0xA8) /
//   GetPhysicsMeshInfo (slot 93, +0x174), CThingPhysical::CanBlockNavigatingCreature (slot 55, +0xDC),
//   CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (by value), CPhysicsMeshObject::IsPointContainedInBoundingBox 0x00A12890 /
//   GetNavigationLevelAt 0x00A11D10 / ~CPhysicsMeshObject 0x00A127D0, memmove 0x00BFEAE6 (the AreaMaps clear).
// Layout: shared headers (CThingSearchTools AreaMaps +0x0C; NThingMap::CThingMap; CTCMapwho SearchID +0x4C; CThing PhysicsTC +0x60,
//   ThingTypeFlags +0x6C; CFilter_Bind3 const references F1/F2/F3; CThingFilter_IsNot). The two filters and the finder have no shared
//   header: Ego_r.pdb classes CThingFilter_HasNavigationInformation (empty), CThingFilter_OverlappingBoundingBox {C3DVector Pos;
//   float Radius} and CThingFindNavigationLayerAt {C3DVector Pos; long Layer} (struct_layouts_egor.tsv), operator() QBE in Ego_r
//   (??RCThingFilter_HasNavigationInformation@@QBE_NABVCThingPhysical@@@Z, ??RCThingFindNavigationLayerAt@@QBE_NABVCThing@@@Z: the
//   const finder keeps its running Layer in a mutable member), all inline here.
// Shape: the shared egod body FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD (+ FOREACHINROUGHAREA, _CONSTPROC, ROUGHAREA_EGOD),
//   instantiated explicitly. The three functor classes have no shared header (declared here as the landed caller 0x0063E590 does).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CThingPhysical.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind3.h"

class CThingFilter_HasNavigationInformation {
public:
    bool operator()(const CThingPhysical& test_thing) const
    {
        const CTCPhysicsBase* tc_physics = test_thing.GetPhysicsTC();
        if (tc_physics && tc_physics->HasNavigationLayers())
            return test_thing.CanBlockNavigatingCreature();
        return test_thing.CanBlockNavigatingCreature();
    }
};

class CThingFilter_OverlappingBoundingBox {
public:
    CThingFilter_OverlappingBoundingBox(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) {}
    bool operator()(const CThingPhysical& test_thing) const
    {
        const CTCPhysicsBase* physics = test_thing.GetPhysicsTC();
        if (physics->GetPhysicsMeshInfo()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            return mesh.IsPointContainedInBoundingBox(Pos);
        }
        return false;
    }
private:
    C3DVector Pos;                                           // +0x00
    float     Radius;                                        // +0x0c
};

class CThingFindNavigationLayerAt {
public:
    CThingFindNavigationLayerAt(const C3DVector& pos) : Pos(pos), Layer(0) {}
    bool operator()(const CThing& test_thing) const
    {
        const CTCPhysicsBase* physics = test_thing.GetPhysicsTC();
        if (physics && physics->HasNavigationLayers()) {
            CPhysicsMeshObject object = physics->GetPhysicsMeshObject();
            long new_layer = object.GetNavigationLevelAt(Pos);
            if (new_layer > Layer)
                Layer = new_layer;
        }
        return true;
    }
    float GetLayer() const { return (float)Layer; }
private:
    C3DVector    Pos;                                        // +0x00
    mutable long Layer;                                      // +0x0c
};

template bool CThingSearchTools::ForEachInRoughArea<CThingPhysical,
    CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(
    const C2DBoxI& area, const CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>& filter,
    const CThingFindNavigationLayerAt& proc) const;