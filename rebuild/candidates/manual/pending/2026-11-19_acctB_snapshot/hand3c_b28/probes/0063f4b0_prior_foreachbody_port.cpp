// CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(const C2DBoxI& area, const CFilter_Bind2<...>& filter, const CThingFindNavigationLayerAt& proc) const -- runs the navigation-layer finder over every physical thing in the overlapping mapwho cells that has navigation information and whose physics mesh bounding box contains the point (each thing once per search); true when any call returned true
// Leaf: ??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind2@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@VCThingFindNavigationLayerAt@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_HasNavigationInformation@@VCThingFilter_OverlappingBoundingBox@@@@ABVCThingFindNavigationLayerAt@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063F4B0, 769 bytes, thiscall `ret 0Ch` (Ego_r.pdb / FableWin.pdb spell it as the Leaf: const, the
//   finder by const reference). The Bind3 twin is 0x0063F7C0 (handQ23); the caller is the landed CWorldMap::GetNavigationLayerAt 0x0063E590.
// Shape: the shared body FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_BODY instantiated explicitly; the two filters and the finder
//   (Ego_r.pdb classes, no shared header) inline here, as in the 0x0063F7C0 port.
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
    CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt>(
    const C2DBoxI& area, const CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox>& filter,
    const CThingFindNavigationLayerAt& proc) const;
