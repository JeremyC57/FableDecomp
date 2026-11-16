// CTCLightningOrb::FrameUpdate() -- move the orb along its spline (eased); when it reaches the end, outlives LifeTime or enters the target's bounding sphere it explodes (when the def names an explosion) and the orb thing is killed
// Leaf: ?FrameUpdate@CTCLightningOrb@@UAEXXZ
// Retail: Fable.exe 0x007F24E0. Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCLightningOrb@@UAEXXZ; slot 9 of CTCLightningOrb's vtable 0x01240A2C
//   (rebuild/manifest/rtti-vtables.tsv; engine/CTCLightningOrb.h).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (IS_THING_ALIVE, the GetTC owner); CTCHitLocations::GetBoundingSphere 0x00815420;
//   CThing::GetPos 0x004C73D0; CTCDExplosion::Create(long, const C3DVector&) 0x0080C460 (static __fastcall); CThing::Kill;
//   NBBBMaths::CLinearSpline::GetPointWithParam 0x00A57F70; CTCPhysicsBase SetPosition (virtual) and SetAsMoving 0x006B0130.
// Shape: early `if (!PDef) return;` and the explode/kill and spline-overrun paths each `Kill(true); return;` (VC7.1 cross-jumps
//   the two Kill calls; the early-out joins the final SetAsMoving epilogue as retail); ONE function-scope C3DVector serves as
//   the hit-test sphere centre and the new spline position (same frame slot as retail).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FORCERESEND
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CDefString.h"
#include "engine/CTCLightningNode.h"
#include "engine/CTCLightningOrb.h"
#include "engine/CTCHitLocations.h"
#include "engine/CTCDExplosion.h"

void CTCLightningOrb::FrameUpdate()
{
    if (PDef.IsNull())
        return;
    bool hit = false;
    CTCHitLocations* hitLocations;
    C3DVector centre;
    float radius;
    if (IS_THING_ALIVE(PTarget) && PTarget->GetTC(TCI_HIT_LOCATIONS, &hitLocations)) {
        float range = PDef->ExplosionRange;
        hitLocations->GetBoundingSphere(centre, radius, true);
        if ((centre - ParentThing->GetPos()).GetSquaredMagnitude() < range * range)
            hit = true;
    }
    if ((LifeTime >= 0 && Timer > LifeTime) || hit) {
        const CLightningOrbDef* def = PDef.Get();
        if (def->ExplosionDefIndex > 0)
            CTCDExplosion::Create(def->ExplosionDefIndex, ParentThing->GetPos());
        ParentThing->Kill(true);
        return;
    }
    ++Timer;
    float t = (float)Timer / NumFramesForSplineTraversal;
    t = (t * t + (t + t)) * 0.33333334f;
    if (t > 1.0f) {
        ParentThing->Kill(true);
        return;
    }
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;
    centre = PSpline->GetPointWithParam(t);
    ParentThing->GetPhysicsTC()->SetPosition(centre);
    ParentThing->GetPhysicsTC()->SetAsMoving(true);
}
