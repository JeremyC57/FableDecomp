// CThingSearchTools::GetAllThingsInRoughArea<const CThingCreatureBase, std::vector<const CThingCreatureBase*>, CFilter_Bind2<CThingFilter_IsCreatureThreatTo, CThingFilter_IsAwareOfThing> >(const C2DBoxI&, const CFilter_Bind2<..>&, std::vector<const CThingCreatureBase*>&) const -- collect every creature in the mapwho cells under the area that is a threat to the given creature and aware of the given thing
// Leaf: ??$GetAllThingsInRoughArea@$$CBVCThingCreatureBase@@V?$vector@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@std@@V?$CFilter_Bind2@VCThingFilter_IsCreatureThreatTo@@VCThingFilter_IsAwareOfThing@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind2@VCThingFilter_IsCreatureThreatTo@@VCThingFilter_IsAwareOfThing@@@@AAV?$vector@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084F850.
// Identity: Ego_r.pdb + FableWin.pdb (entry q11397, NF93 part 3 "claim holds"); the landed caller 0x0084E590 binds this spelling and its
//   TU's visible copy of the template (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD) grades RELOCATION_MATCH at this address.
// NF98 (round 8, q11397): ported off its local model (RetailVector / CThing / CThingCreatureBase / CThingAICreature / C2DBoxI / CMapwhoNode /
//   CThingMapCell / CThingMap / CMapwhoLevelInfo / CLayerInfo / the two filters / Filters / CThingSearchTools_Calls, which emitted a
//   CThingSearchTools_Calls stand-in) onto the shared headers as an explicit instantiation of the header template, with the define /
//   include set of its landed caller 0x0084E590 (the same TU in retail) and that row's two row-local filter classes.
#include <math.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_CAMERAFOCALPOS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATICCAMERAFOCALPOS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETOBSERVEDTHING
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND_WORLD
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
// Include-order: engine/CTCMapwho.h names CTCMapwho in NThingMap::CMapwhoNode before its own class (as the landed 0x0087A7C0).
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"

// No shared headers for these two filters (as in the landed 0x0084E590). CThingFilter_IsCreatureThreatTo (member Creature) and CThingFilter_IsAwareOfThing (member
//   PThingToBeAwareOf): Ego_d.pdb records (one-argument constructor, operator(), the private pointer; out-of-line operator() at the
//   retail addresses of their landed rows). C3DFrustrum: Ego_d.pdb `anonymous-namespace'::C3DFrustrum (Apex, Up, Points, FOV, Focus;
//   C3DFrustrum / IncorporatePoint / AddPoint / UpdateFrustrum / GetFOV / GetFocus / GetApex / GetUp) and Ego_r.pdb's out-of-line members
//   ?..@C3DFrustrum@?A0x49b2e1a3@@ (AddPoint QAEXABVC3DVector@@M@Z, IncorporatePoint the same, GetApex / GetFocus QBEABVC3DVector@@XZ);
//   with the STLport vector Focus sits at +0x28 (retail `fld [esp+7Ch]`, frustum at [esp+54h]). IncorporatePoint is inline here (retail
//   calls AddPoint 0x0084E390 then UpdateFrustrum 0x0084D0E0 at 0x0084E7A8 / 0x0084E7B1); Inferred body: add, then update.
class CThingFilter_IsCreatureThreatTo {
public:
    CThingFilter_IsCreatureThreatTo(const CThingCreatureBase& creature) : Creature(&creature) { }
    bool operator()(const CThingCreatureBase& thing) const;              // 0x006A7600
private:
    const CThingCreatureBase* Creature;
};
class CThingFilter_IsAwareOfThing {
public:
    CThingFilter_IsAwareOfThing(const CThing* thing) : PThingToBeAwareOf(thing) { }
    bool operator()(const CThing& thing) const;                          // 0x006A8630
private:
    const CThing* PThingToBeAwareOf;
};

template void CThingSearchTools::GetAllThingsInRoughArea<const CThingCreatureBase, std::vector<const CThingCreatureBase*>,
    CFilter_Bind2<CThingFilter_IsCreatureThreatTo, CThingFilter_IsAwareOfThing> >(const C2DBoxI&,
    const CFilter_Bind2<CThingFilter_IsCreatureThreatTo, CThingFilter_IsAwareOfThing>&, std::vector<const CThingCreatureBase*>&) const;
