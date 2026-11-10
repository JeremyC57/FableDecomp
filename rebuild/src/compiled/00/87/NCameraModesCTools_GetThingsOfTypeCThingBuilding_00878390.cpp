// NCameraModes::CTools::GetThingsOfType<CThingBuilding> -- collect nearby buildings of a type.
// Leaf: ??$GetThingsOfType@VCThingBuilding@@@CTools@NCameraModes@@QBEXAAV?$list@PBVCThingBuilding@@V?$allocator@PBVCThingBuilding@@@std@@@std@@JABVC3DVector@@M@Z
// Retail: Fable.exe 0x00878390,160 bytes.
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS
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
class CThingBuilding;
template<class T>
void NCameraModes::CTools::GetThingsOfType(std::list<const T*>& things,long thing_type,const C3DVector& origin,float radius)const
{
 C2DBoxI area(origin.ToC2DCoordI(),(long)radius);
 GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<T>(area,GFFilter_Bind2(CThingFilter_IsOfType(thing_type),CThingFilter_OverlappingSphere(origin,radius)),things);
}
template void NCameraModes::CTools::GetThingsOfType<CThingBuilding>(std::list<const CThingBuilding*>&,long,const C3DVector&,float)const;