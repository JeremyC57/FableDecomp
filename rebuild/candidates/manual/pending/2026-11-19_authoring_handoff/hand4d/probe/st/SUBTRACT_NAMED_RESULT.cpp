// NCreatureMode::CModeThunderLightningStorm::UpdateDamage() -- damage every live creature in the storm's 2D range that it has not hit yet: electrocute it (adding the effect TC when missing), hit it with the thunder battle def's lightning damage away from the caster (no move-back), raise the hit-by-special-ability event while it lives and remember it in HitThings
// Grade: (in progress)
// Leaf: ?UpdateDamage@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_NAMED_RESULT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThunderBattleDef.h"
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CHitParameters.h"
#include "engine/CTCElectrocutedEffect.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventHitBy.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/NCreatureMode_CModeThunderLightningStorm.h"

void NCreatureMode::CModeThunderLightningStorm::UpdateDamage()
{
    CDefPointer<const CThunderBattleDef> p_thunder_battle_def;
    GetParentThing().PeekBaseDef().GetSubDef(&p_thunder_battle_def);
    std::vector<CThingCreatureBase*> hit_creatures;
    GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(
        C2DBoxI(GetParentThing().GetPos().ToC2DCoordI(), (long)StormRadius),
        GFFilter_Bind2(CThingFilter_IsNot(GetParentThing()), CThingFilter_In2DRange(GetParentThing().GetPos(), StormRadius)),
        hit_creatures);
    std::vector<CThingCreatureBase*>::iterator it;
    if (false) {} else for (it = hit_creatures.begin(); it != hit_creatures.end(); ++it) {
        CThingCreatureBase* pcreature = *it;
        if (IS_THING_ALIVE(pcreature)) {
            if (std::find(HitThings.begin(), HitThings.end(), CIntelligentPointer<CThing>(pcreature)) == HitThings.end()) {
                CTCElectrocutedEffect* ptc_elec;
                if (!pcreature->HasTC(TCI_ELECTROCUTED_EFFECT)) {
                    ptc_elec = GFDebugPDynamicCast(pcreature->AddTC("CTCElectrocutedEffect", true, NULL), (CTCElectrocutedEffect*)0);
                    ptc_elec->Reset();
                }
                CHitParameters hpParams(NULL, &GetParentThing(), NULL, pcreature->GetPos(), pcreature->GetPos() - GetParentThing().GetPos(), true, false, false, false);
                hpParams.AddDamageWithAttribute(DAMAGE_LIGHTNING, p_thunder_battle_def->LightningDamage);
                hpParams.SetMoveBackOnHit(false);
                pcreature->OnHit(hpParams);
                CTCEntityEvents* ptc_entity_events;
                if (IS_THING_ALIVE(pcreature) && pcreature->GetTC(&ptc_entity_events))
                    ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hpParams, HERO_ABILITY_LIGHTNING_SPELL));
                HitThings.push_back(CIntelligentPointer<CThing>(pcreature));
            }
        }
    }
}
