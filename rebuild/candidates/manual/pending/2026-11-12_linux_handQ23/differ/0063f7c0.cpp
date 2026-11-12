// CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(const C2DBoxI& area, const filter&, const finder&) const -- runs the navigation-layer finder over every physical thing in the mapwho cells under the area that is not the ignored thing, has navigation information and whose physics mesh's bounding box contains the filter's point (each thing once per search); true when any thing was visited
// Grade: DIFFER(785v785) -- Q23 PORTED onto the shared headers (2 grades; overlay FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_BODY + CThingPhysical STATTYPEFLAGS + CPhysicsMeshObject NAVIGATION, hdr/README.md). One residue, the one Q19 found on the GetAllThingsInRoughArea twin 0x008780D0: after the node loop retail reloads edx = cells.TLX [esp+40h] first, then esi = BRX, edi = BRY, ebx = map; ours reloads BRX, BRY, map, TLX. 20 toy (ungraded) spellings: cells declared before the map loop moves TLX ahead of map (BRX, BRY, TLX, map) but nothing moves TLX ahead of BRX/BRY (ctor / Initialise / field order / while / explicit guards / map pointer / layer outside).
// Leaf: ??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@VCThingFindNavigationLayerAt@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsNot@@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@ABVCThingFindNavigationLayerAt@@@Z
// Flags: none
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
// Shape: the shared body FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_BODY (the GetAllThingsInRoughArea scan with the PDB
//   `const Proc&` signature), instantiated explicitly.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_BODY
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
    CThingFilter_OverlappingBoundingBox(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) {}
    bool operator()(const CThingPhysical& thing) const
    {
        const CTCPhysicsBase* physics = thing.GetPhysicsTC();
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
    bool operator()(const CThing& thing) const
    {
        const CTCPhysicsBase* physics = thing.GetPhysicsTC();
        if (physics && physics->HasNavigationLayers()) {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            long layer = mesh.GetNavigationLevelAt(Pos);
            if (layer > Layer)
                Layer = layer;
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
