// 00821850 CMap::InitCachedVolumes
// Leaf: ?InitCachedVolumes@CMap@@IAEXXZ
// Flags: /d2inlT120
#define FABLE_RETAIL_MAP_CACHED_VOLUMES_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_CVECTORMAP_SORT_INLINE
#define FABLE_RETAIL_CCONTAINMENTVOLUME
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVVOLUMES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GETMAPBOX
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL_STATTYPEFLAGS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/CContainmentVolume.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_BoundingBoxOverlapsMap.h"
#include "engine/CThingFilter_HasNavigationInformation.h"
// Inline-budget: the collector at 008266E0 expands the pointer append and
// calls the overflow member only on the full-capacity branch.
template<> FABLE_FORCEINLINE void std::vector<const CThingPhysical*>::push_back(const CThingPhysical* const& x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        std::_Construct(this->_M_finish, x);
        ++this->_M_finish;
    } else {
        _M_insert_overflow(this->_M_finish, x, _IsPODType(), 1, true);
    }
}
#include "engine/CThingSearchTools.h"
#include "engine/CMap.h"

void CMap::InitCachedVolumes()
{
    CachedVolumes.reset(new CCachedVolumes);
    const C2DBoxI& area=GetWorld()->GetWorldMap().GetMapBox(GetWorldMapNumber());
    CVectorMap<long,CCountedPointer<CContainmentVolume> > volumes;
    std::vector<const CThingPhysical*> things;
    GetWorld()->PeekThingSearchTools().GetAllThings<const CThingPhysical>(
        GFFilter_Bind2(CThingFilter_BoundingBoxOverlapsMap(GetWorld()->GetWorldMap(),GetWorldMapNumber()),CThingFilter_HasNavigationInformation()),
        things);
    long thingssize=things.size();
    for(long c1=0;c1<thingssize;++c1) {
        const CThingPhysical* thing=things[c1];
        if(thing->GetPhysicsTC()->HasPhysicsMeshObject()) {
            CPhysicsMeshObject physics_object=thing->GetPhysicsTC()->GetPhysicsMeshObject();
            long old_size=volumes.size();
            physics_object.GetNavigationLevelVolumes(volumes);
        }
    }
    volumes.Sort();
    CCachedVolumes& containment_volumes=*CachedVolumes;
    containment_volumes.InitialiseNavigationContainmentVolumes(volumes);
}