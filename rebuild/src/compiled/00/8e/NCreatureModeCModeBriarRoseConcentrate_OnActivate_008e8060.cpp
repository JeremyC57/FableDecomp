// NCreatureMode::CModeBriarRoseConcentrate::OnActivate() -- charge Drain Life, place imposters, and attach the charge effects.
// Leaf: ?OnActivate@CModeBriarRoseConcentrate@NCreatureMode@@UAEXXZ
// Flags: none
// The marker vector grows here. Imposters uses the separately emitted intelligent-pointer push_back.
// Retail: 0x008E8060, 1784 bytes. Both donor PDBs and the mode vtable at 0x0126855C identify the override.
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHINGMARKER_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR

#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_ACTIVATE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSECONCENTRATEINTO
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCABSORBHITS_ADD_PASS28
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/CMainGameComponent.h"
#include "engine/NCreatureMode_CModeBriarRoseConcentrate.h"
#include "engine/CWorld.h"
#include "engine/CThingMarker.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CBriarRoseDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CTCAbsorbHits.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_BriarRoseConcentrateInto.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"


#include "engine/CDefClassBase_GetSubDef.h"
namespace std { template<> void vector<CIntelligentPointer<CThing> >::push_back(const CIntelligentPointer<CThing>&); }
void NCreatureMode::CModeBriarRoseConcentrate::OnActivate()
{
    CModeStandBase::OnActivate();
    float seconds_to_charge = 15.0f;
    if (GetParentThing().GetPBaseDef()->GetSubDef(&PDef))
        seconds_to_charge = PDef->SecondsToChargeDrainLife;
    Timer = (long)(CMainGameComponent::GetConstantFPS() * seconds_to_charge);

    if (GetParentThing().GetType() == CThingAICreature::StatGetType()) {
        CThingAICreature* p_ai_creature = static_cast<CThingAICreature*>(&GetParentThing());
        p_ai_creature->SuspendAI(true, this);
    }

    if (!Imposter) {
        std::vector<CThing*> markers;
        GetWorld().PeekThingSearchTools().GetAllThings<CThingMarker>(CThingFilter_HasScriptName("TeleportMarker"), markers);

        if (!markers.empty()) {
            long num_markers = markers.size();
            Imposters.reserve(num_markers);
            long briar_marker_index = GFRandom(num_markers, GetWorld().GetWorldSeed());
            GetParentThing().GetPhysicsTC()->ResetPosition(markers[briar_marker_index]->GetPos());

            long imposter_def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_RIVAL_HERO_BRIAR_ROSE_IMPOSTER");
            CThingAICreatureInit init;
            init.ScriptName = "BRIAR_ROSE_IMPOSTER";

            C3DVector arena_centre(0.0f, 0.0f, 0.0f);
            for (long c1 = 0; c1 != num_markers; ++c1) {
                CThing* pthing = markers[c1];
                const C3DVector& pos = pthing->GetPos();
                arena_centre += pos;
                if (c1 != briar_marker_index) {
                    CThingAICreature* pimposter = CThingAICreature::Create(imposter_def_index, pos, 4, init);
                    if (IS_THING_ALIVE(pimposter)) {
                        if (!PDef.IsNull() && PDef->NumberOfHitsToKillImposters > 1) {
                            long num_absorb_hits = PDef->NumberOfHitsToKillImposters - 1;
                            CTCBase* ptc_base = pimposter->AddTC("CTCAbsorbHits", true, 0);
                            if (ptc_base) {
                                CTCAbsorbHits* ptc_absorb = static_cast<CTCAbsorbHits*>(ptc_base);
                                ptc_absorb->AddHitsRemaining(num_absorb_hits);
                            }
                        }
                        pimposter->SetCurrentAction(CCreatureAction_BriarRoseConcentrateInto(*pimposter, *PTarget, true));
                        Imposters.push_back(CIntelligentPointer<CThing>(pimposter));
                    }
                }
            }
            arena_centre /= num_markers;

            {
                C3DVector line_to_centre = arena_centre - GetParentThing().GetPos();
                line_to_centre.Z = 0.0f;
                line_to_centre.AccurateNormalise();
                GetParentThing().GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), line_to_centre));
            }

            for (std::vector<CIntelligentPointer<CThing> >::iterator it = Imposters.begin(); it != Imposters.end(); ++it) {
                CThing* pimposter = it->Get();
                C3DVector line_to_centre = arena_centre - pimposter->GetPos();
                line_to_centre.Z = 0.0f;
                line_to_centre.AccurateNormalise();
                pimposter->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), line_to_centre));
            }
        }
    }

    CDefPointer<const CSpecialEffectsDef> pdef;
    if (GetParentThing().GetPBaseDef()->GetSubDef(&pdef)) {
        long fx_index = 0;
        if (pdef->GetEffectFromString("DRAIN_CHARGE_BODY", fx_index)) {
            CThing* pparticle = CTCDParticleEmitter::Create(fx_index, GetParentThing().GetPos(), false);
            if (IS_THING_ALIVE(pparticle)) {
                CTCDParticleEmitter* ptc_particle;
                pparticle->GetTCDriver(&ptc_particle);
                ptc_particle->AttachToThing(GetParentThing(), 8, "", 0, 0.0f);
                PBodyEffect = CIntelligentPointer<CThing>(pparticle);
            }
        }
        if (pdef->GetEffectFromString("DRAIN_CHARGE_SWORD", fx_index)) {
            CThing* pparticle = CTCDParticleEmitter::Create(fx_index, GetParentThing().GetPos(), false);
            if (IS_THING_ALIVE(pparticle)) {
                CTCDParticleEmitter* ptc_particle;
                pparticle->GetTCDriver(&ptc_particle);
                const CTCCarrying* ptc_carrying;
                if (GetParentThing().PeekTC(&ptc_carrying)) {
                    CThing* psword = ptc_carrying->GetThingInPrimarySlot();
                    if (IS_THING_ALIVE(psword))
                        ptc_particle->AttachToThing(*psword, 10, "WEAPON_FOCUS_01", 0, 0.0f);
                }
                PSwordEffect = CIntelligentPointer<CThing>(pparticle);
            }
        }
    }
}