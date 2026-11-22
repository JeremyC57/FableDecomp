// NThingHelpers::CreateEffectOnHit(const CThing& thing_hit, const CHitParameters& hit_params, const CWorldMap& world_map, float delta_health) -- spawn the hit particle effect: melee hits on creatures whose def does not use the actual hit position move it to 0.75 x the physics radius towards the striker (direction swung to the striker's last action hand), then, when in the map and within the def's render fade distance of the game LOD centre, create the material's blended (region-weighted) or random sword-hit effect and hand it the DIRECTION vector
// Leaf: ?CreateEffectOnHit@NThingHelpers@@YIXABVCThing@@ABVCHitParameters@@ABVCWorldMap@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00839420, 1308 bytes, __fastcall (thing_hit ecx, hit_params edx; world_map, delta_health on the stack), `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?CreateEffectOnHit@NThingHelpers@@YIXABVCThing@@ABVCHitParameters@@ABVCWorldMap@@M@Z; the landed
//   CMaterialDef::GetRandomSwordHitParticleEffect 0x008EE600 names this caller. Text: Ego_d.pdb's parameters, locals, blocks and line table
//   (lines 86..214); every local carries its Ego_d name and type.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, NCombatActionHelpers::IsToCreateCombatParticleEffects 0x008F7860,
//   CHitParameters::IsMeleeStrike 0x00868860, CDefClassBase::GetSubDef<CCreatureDef> 0x00485815, CThing::GetPos 0x004C73D0,
//   C3DVector::AccurateNormalise 0x00A14440, CWorldMap::IsPosInMap 0x004FC600 / GetLoadedRegion 0x004FB150 / GetRegion 0x004FC180,
//   CMainGameComponent::PeekDefinitionManager 0x00415FBC, CDefString::operator CCharString 0x00415DD0,
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CMaterialDef::GetBlendedSwordHitEffect 0x008EE790 /
//   GetRandomBerserkedSwordHitParticleEffect 0x008EE720 / GetRandomSwordHitParticleEffect 0x008EE600, CTCDParticleEmitter::Create (5-argument)
//   0x006E11F0 / Create (3-argument) 0x006E0880, IS_THING_ALIVE<const CThing*> 0x00436380 / IS_THING_ALIVE<CThing*> 0x0040DCE0,
//   CThing::GetTCDriver<CTCDParticleEmitter> 0x004ACB50, CCRC::Calc 0x004014A0, NParticleEngine::CParticleUpdateParameters<unsigned long,
//   unsigned long> ctor 0x006E24D0 / dtor 0x006E2500, CArray<pair<unsigned long, C3DVector> >::push_back 0x0076B120,
//   CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0; CWorld slot 8 GetGameLODCentre, CDefClassBase slot 10 GetInstantiationName.
//   Data: NGlobalConsole::CombatParticleEffects 0x013756F5, DELTA_NOTIONAL_ZERO 0x0129BA3C, CMainGameComponent::PMainGameComponent
//   0x013B86A0; literals 0.75f, -1.0f, "DIRECTION" 0x01272FC0.
// Headers: new opt-ins FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS (engine/CMaterialDef.h: the three PDB declarations) and
//   FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCREATUREDEF_SIZE_BODY (engine/CDefinitionManager.h; scaffolding: gives this TU's pick-any
//   GetSubDef<CCreatureDef> the owner 0x00485815's size / frame-pointer body; no effect on this function's bytes). Existing opt-ins that
//   decide bytes here: FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (the visible GetSubDef body: pcreature_def is released in place,
//   0x0083968F, and takes the frame word below pmaterial_def's) and FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT + FABLE_RETAIL_ASSERT_DEAD_ARM
//   (the debug cast's one ASSERT kept as the release dead arm: it emits no byte, but its symbols move the field symbols of the
//   `point_of_hit - lod_centre` temporary past c2's 16-bit sort-hash wrap, which orders the squared-magnitude products X, Z, Y as retail
//   has them at 0x00839736; GOTCHAS 466 mechanism, measured with the c2 probes: NOTES.md).
// Line 87 is Ego_d's one statement. Retail's dead zero store to the local's frame word (0x00839430 `mov dword ptr [esp+20h],0`, written
//   again at 0x00839443) is what VC7.1 emits for this text; the draft's separate `pthing_hitting = 0;` statement changes no byte
//   (measured, NOTES.md) and is gone.
// Inferred: the release dead arm of GFDebugPDynamicCast's debug ASSERT (FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT; GFDebugPDynamicCast.h
//   itself marks it Inferred: one assert, argument fitted). The cast is Ego_d's (line 129 = 13 B, a two-argument fastcall call); the cast
//   alone leaves the products Z, Y, X (DIFFER(1308v1308), 6 lines); with the arm they are retail's X, Z, Y.
// Open Ego_d deviations, ruling needed (NOTES.md): line 208 (Ego_d 40 B = NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "DIRECTION"),
//   ours 42 B: c2 does not expand that shared helper here, and the Ego_d spelling builds 1306 B with a call retail does not have, so
//   the row spells the helper's body as the landed siblings do); line 207 (Ego_d 481 B = the GetTCDriver call plus one expanded debug
//   check; the release statement is the call, the debug text is not recovered).
// Ego_d sizes that need a header claim, no byte here: lines 151 / 166 (Ego_d 77 / 52 B = these texts when CDefPointer::IsNull() / Get()
//   are declared throw(): measured 77 / 52 with VC10 /Od, 97 / 72 with today's header).
// `GetTypeFlags() & 8` is the creature type bit as the landed rows spell it (no named enumerator in the shared headers).
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCREATUREDEF_SIZE_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CHITPARAMETERS_EFFECTONHIT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_LASTHANDEDNESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/GFWorldDistance.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCDriverBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CMaterialDef.h"
#include "engine/CRegionDef.h"
#include "engine/CRegion.h"
#include "engine/CHitParameters.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"
#include "engine/CCRC.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CTCDParticleEmitter.h"

namespace NGlobalConsole { extern bool CombatParticleEffects; }    // 0x013756F5
namespace NCombatActionHelpers { bool __fastcall IsToCreateCombatParticleEffects(const CThing& thing, const CThing* hitter); }   // 0x008F7860
namespace NThingHelpers {
void __fastcall CreateEffectOnHit(const CThing& thing_hit, const CHitParameters& hit_params, const CWorldMap& world_map, float delta_health)
{
    const CThing* pthing_hitting = hit_params.GetPThingHitting();
    if (!NCombatActionHelpers::IsToCreateCombatParticleEffects(thing_hit, pthing_hitting))
    {
        return;
    }

    C3DVector point_of_hit = hit_params.GetHitPosition();

    if (!NGlobalConsole::CombatParticleEffects)
    {
        return;
    }
    if (!hit_params.IsCreateParticleEffectOnHit())
    {
        return;
    }

    C3DVector attack_direction = hit_params.GetHitDirection();
    CDefPointer<const CMaterialDef> pmaterial_def = hit_params.GetPHitMaterialDef();

    if (IS_THING_ALIVE(pthing_hitting) && (thing_hit.GetTypeFlags() & 8) && hit_params.IsMeleeStrike())
    {
        CDefPointer<const CCreatureDef> pcreature_def;
        if (thing_hit.GetPBaseDef()->GetSubDef(&pcreature_def) && !pcreature_def->UseActualHitPosForHitEffects)
        {
            C3DVector line_out = pthing_hitting->GetPos() - thing_hit.GetPos();
            line_out.Z = 0.0f;
            if (line_out.IsNonZero())
            {
                line_out.AccurateNormalise();

                C3DVector new_point_of_hit = thing_hit.GetPos() + line_out * thing_hit.GetPhysicsTC()->GetRadius() * 0.75f;
                new_point_of_hit.Z = point_of_hit.Z;

                C3DVector new_direction_of_hit = attack_direction;
                if (pthing_hitting->GetTypeFlags() & 8)
                {
                    C3DVector perp = line_out.CrossWithUp();
                    const CThingCreatureBase* phitter = GFDebugPDynamicCast(pthing_hitting, (const CThingCreatureBase*)0);
                    if (phitter->GetLastActionHandedness() == 0)
                    {
                        new_direction_of_hit = perp * -1.0f;
                    }
                    else if (phitter->GetLastActionHandedness() == 1)
                    {
                        new_direction_of_hit = perp * 1.0f;
                    }
                }

                point_of_hit = new_point_of_hit;
                attack_direction = new_direction_of_hit;
            }
        }
    }

    if (!world_map.IsPosInMap(point_of_hit))
    {
        return;
    }

    if (!thing_hit.GetPBaseDef().IsNull())
    {
        C3DVector lod_centre = CMainGameComponent::Get()->PeekWorld().GetGameLODCentre();
        if ((point_of_hit - lod_centre).GetSquaredMagnitude() > GFSqr(thing_hit.GetPBaseDef()->RenderFadeDistance))
        {
            return;
        }
    }

    if (!pmaterial_def.IsNull())
    {
        const CGameDefinitionManager& def_man = CMainGameComponent::Get()->PeekDefinitionManager();
        const CRegion& region = world_map.GetRegion(world_map.GetLoadedRegion());
        const CRegionDef* pregion_def = region.GetPRegionDef().Get();
        long region_def_index = 0;
        if (pregion_def)
        {
            region_def_index = def_man.GetDefGlobalIndexFromName(pregion_def->GetInstantiationName());
        }

        CThing* pthing_emitter = 0;
        long from_effect_id = 0;
        long to_effect_id = 0;
        float interpolate = 0.0f;

        if (pmaterial_def->GetBlendedSwordHitEffect(region_def_index, delta_health, from_effect_id, to_effect_id, interpolate))
        {
            if (from_effect_id && to_effect_id)
            {
                pthing_emitter = CTCDParticleEmitter::Create(from_effect_id, to_effect_id, interpolate, point_of_hit, false);
            }
        }
        else
        {
            if (IS_THING_ALIVE(pthing_hitting) && pthing_hitting->HasTC(TCI_BERSERK))
            {
                from_effect_id = pmaterial_def->GetRandomBerserkedSwordHitParticleEffect();
            }
            else
            {
                from_effect_id = pmaterial_def->GetRandomSwordHitParticleEffect();
            }
            if (from_effect_id > 0)
            {
                pthing_emitter = CTCDParticleEmitter::Create(from_effect_id, point_of_hit, false);
            }
        }

        if (IS_THING_ALIVE(pthing_emitter))
        {
            CTCDParticleEmitter* tcd_particle_emitter = 0;
            pthing_emitter->GetTCDriver(&tcd_particle_emitter);
            std::pair<unsigned long, C3DVector> parameter(CCRC::Calc(0, "DIRECTION", 9), attack_direction);
            NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> params;
            params.VectorParams.push_back(parameter);
            tcd_particle_emitter->SetEmitterUpdateParameters(params);
        }
    }
}
}