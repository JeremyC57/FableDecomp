// 007dd540 CTCCreatureFlyInCircles::UpdateSwoop
// Leaf: ?UpdateSwoop@CTCCreatureFlyInCircles@@IAEXAAVC3DVector@@0@Z
// Flags: none
#define FABLE_RETAIL_CTCCREATUREFLYINCIRCLES_UPDATESWOOP
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SCALE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCDExplosion.h"
#include "engine/CTCCreatureFlyInCircles.h"
#include "engine/ThingAlive.h"
template<> bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSubDefInfo* index = GetPSubDefInfo(CSpecialEffectsDef::StatGetTypeName());
    if (index) {
        CDefPointer<const CSpecialEffectsDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}
void CTCCreatureFlyInCircles::UpdateSwoop(C3DVector& rPos, C3DVector& rFwd)
{
    C3DVector newPos;
    SwoopProgress += SwoopDelta;
    SwoopSpline.GetSplinePosition(newPos, SwoopProgress);
    rFwd = newPos - rPos;
    rFwd.AccurateNormalise();
    rPos = newPos;
    if (SwoopProgress >= 1.0f || !IS_THING_ALIVE(PSwoopTarget)) {
        Mode = eFly;
        PSwoopTarget = 0;
        return;
    }
    if (SwoopProgress < 0.45f) {
        if (SwoopProgress < 0.3f)
            SwoopSpline.SetControlPoint(1, PSwoopTarget->GetPos() + SwoopTargOffset);
        if (CWorld::GetFrame() - LastExplodeTick > 1) {
            long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("DRAGON_FIRE_TRAIL");
            C3DVector explodePos = newPos + rFwd * 60.0f;
            if (PeekWorldMap().IsPosInMap(explodePos)) {
                explodePos.Z = 10000.0f;
                explodePos.Z = GetWorldMap().GetHeightZBelowPos(explodePos);
                CThing* pExplosion = CTCDExplosion::Create(defIndex, explodePos);
                if (pExplosion) {
                    CTCDExplosion* pTCDExplosion;
                    if (pExplosion->GetTC(&pTCDExplosion))
                        pTCDExplosion->SetThingAsImmune(Creature);
                    LastExplodeTick = CWorld::GetFrame();
                }
            }
            if (!IS_THING_ALIVE(PBurninationEffect)) {
                CDefPointer<const CSpecialEffectsDef> pEffects;
                long effect = 0;
                C3DVector mouthPos;
                CRightHandedSet mouthOri;
                if (!Creature.GetTCGraphicAppearance()->GetDummyObject("MOUTH_DUMMY", 0, 0, mouthPos, mouthOri)) {
                    FABLE_RETAIL_ASSERT(false);
                }
                if (Creature.PeekBaseDef().GetSubDef(&pEffects) && pEffects->GetEffectFromString("BURNINATE", effect)) {
                    PBurninationEffect = CTCDParticleEmitter::Create(effect, mouthPos, false);
                    if (IS_THING_ALIVE(PBurninationEffect)) {
                        CTCDParticleEmitter* pTCDEmitter;
                        if (PBurninationEffect->GetTC(&pTCDEmitter))
                            pTCDEmitter->AttachToThing(Creature, 42, "MOUTH_DUMMY", 0, 0.0f);
                    }
                }
            }
        }
    } else if (IS_THING_ALIVE(PBurninationEffect)) {
        PBurninationEffect->Kill(true);
        PBurninationEffect = 0;
    }
}