// NCameraModes::CTools::GetStandardThings(std::list<const CThing*>& things, const C3DVector& origin, float radius) const -- refill the
// list with the things of types 3 and 5 that overlap the sphere, searched over the sphere's integer bounding square.
// Leaf: ?GetStandardThings@CTools@NCameraModes@@QBEXAAV?$list@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@ABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00877720, 277 bytes (`ret 0Ch`).
// Naming: FableWin.pdb / Ego_r.pdb ?GetStandardThings@CTools@NCameraModes@@QBEX... (public const; list<const CThing*>&, const C3DVector&, float).
// Callees: free 0x00BFEA14 (STLport list clear, inline), __ftol2 0x00BFEA70 (x3), the out-of-line
//   CThingSearchTools::GetAllThingsInRoughArea<CThing, std::list<const CThing*>, CFilter_Bind2<CThingFilter_IsOfType,
//   CThingFilter_OverlappingSphere> > instance 0x00878430 (its only two call sites are here), twice.
// Shape: the sibling GetThingsOfType<CThingBuilding> 0x00878390 idiom: one C2DBoxI(origin.ToC2DCoordI(), (long)radius) for both
//   searches; each search binds a fresh IsOfType(type) + OverlappingSphere(origin, radius) pair (origin is re-read after the first
//   call; the type word is homed in the dead `things` argument slot). The type words 3 and 5 are retail's immediates
//   (0x008777C6 / 0x0087780F); no named enumerator for them is in the shared headers.
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_STANDARDTHINGS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_WORLD
#define FABLE_RETAIL_CGAMECAMERAMANAGER_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGBUILDING_TYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CThingFilter_IsOfType.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CFilter_Bind2.h"
void NCameraModes::CTools::GetStandardThings(std::list<const CThing*>& things,const C3DVector& origin,float radius)const
{
 things.clear();
 C2DBoxI area(origin.ToC2DCoordI(),(long)radius);
 GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(area,GFFilter_Bind2(CThingFilter_IsOfType(3),CThingFilter_OverlappingSphere(origin,radius)),things);
 GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(area,GFFilter_Bind2(CThingFilter_IsOfType(5),CThingFilter_OverlappingSphere(origin,radius)),things);
}