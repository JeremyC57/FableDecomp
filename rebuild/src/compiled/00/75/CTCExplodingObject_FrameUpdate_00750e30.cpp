// CTCExplodingObject::FrameUpdate() [0x00750E30] -- tick an armed countdown, detect nearby creatures, wobble and maintain a proximity particle effect.
// Grade: RELOCATION_MATCH (retail_asm),501bytes; required called helpers618/36bytes also exact.
// Flags: none
// Leaf: ?FrameUpdate@CTCExplodingObject@@UAEXXZ
// Retail: Fable.exe(TLC)00750E30..00751025,501bytes,plain ret; CTCExplodingObject vtable slot9.
// Retail-asm: 0x00750E77..0x00750E93 bare `fistp dword ptr [ebp-4]` plus integer-bit ceiling fixup, shared GFFloatToLongCeil.
// Plain/intrinsic ladder: history/00750e30/rounding_ladder records30C++ and10intrinsic forms; none emits bare dword fistp.
#define FABLE_RETAIL_CTCMAPWHO_PASS22_SHARED_MAPNODE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_CTCPHYSICSBASE_CALCULATERADIUS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_PASS22_FIRST_THISEND
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_PASS22_FIRST_PEEKCELL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCEXPLODINGOBJECT_PASS22
#define FABLE_RETAIL_CTCWOBBLE_PASS22
#define FABLE_RETAIL_EXPLODING_OBJECT_DEF
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCExplodingObject.h"
#include "engine/CExplodingObjectDef.h"
#include "engine/CTCWobble.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
#include "engine/GFFloatToLongCeil.h"
inline bool CThingFilter_OverlappingCircle::operator()(const CThing& thing) const
{
    const CTCPhysicsBase* physics=thing.GetPhysicsTC();
    if(!physics)return false;
    C3DVector difference=physics->GetMapwhoPosition()-SpherePos;
    float radius=physics->Radius+Radius;
    float squared=difference.Y*difference.Y;
    squared+=difference.X*difference.X;
    return squared<radius*radius;
}
void CTCExplodingObject::FrameUpdate()
{
    if(CountdownTimer>0) {
        --CountdownTimer;
        if(CountdownTimer==0) { Explode();return; }
    }
    if(TriggeredOnCreatureProximity) {
        long radius=GFFloatToLongCeil(TriggerRadius+4.0f);
        C2DBoxI area(GetParentThing().GetPos().ToC2DCoordI(),radius);
        {
        CThingCreatureBase* creature;
        if(GetThingSearchTools().GetPFirstThingInRoughArea(area,CThingFilter_OverlappingCircle(GetParentThing().GetPos(),TriggerRadius),&creature)) {
            Explode();return;
        }
        if(GetThingSearchTools().GetPFirstThingInRoughArea(area,CThingFilter_OverlappingCircle(GetParentThing().GetPos(),TriggerRadius+4.0f),&creature)) {
            if(!GetParentThing().HasTC(TCI_WOBBLE)) {
                CTCWobble* wobble=static_cast<CTCWobble*>(GetParentThing().AddTC("CTCWobble",false,0));
                wobble->SetWobbleParameters(0.5f,1.0f/90.0f);
            }
            if(PDef->ProximityParticleEffect>0 && !IS_THING_ALIVE(PProximityParticle))
                PProximityParticle=CTCDParticleEmitter::Create(PDef->ProximityParticleEffect,GetParentThing().GetPos(),false);
        } else if(IS_THING_ALIVE(PProximityParticle)) {
            PProximityParticle->Kill(true);
        }
        }
    }
}