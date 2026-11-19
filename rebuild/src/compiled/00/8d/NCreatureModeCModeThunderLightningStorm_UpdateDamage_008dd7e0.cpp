// NCreatureMode::CModeThunderLightningStorm::UpdateDamage() -- damage every live creature in the storm's 2D range that it has not hit yet: electrocute it (adding the effect TC when missing), hit it with the thunder battle def's lightning damage away from the caster (no move-back), raise the hit-by-special-ability event while it lives and remember it in HitThings
// Grade: RELOCATION_MATCH (hand4d 2026-11-19). The parked local model (DIFFER 744v744: InSphere filter guess, hand CThing / def / vector
//   stand-ins) is replaced by a port over the shared headers in FableWin.pdb's statement order (0x02859F70) with Ego_d.pdb's locals.
// Leaf: ?UpdateDamage@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DD7E0, 744 bytes, thiscall, plain `ret`. Name: Ego_r.pdb 0x0081FDA0 / FableWin.pdb 0x02859F70
//   ?UpdateDamage@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ (private; header engine/NCreatureMode_CModeThunderLightningStorm.h).
// Locals: Ego_d.pdb record (VC10 /Od, debug length 2102): function scope lifetime_in_seconds, hit_creatures, lifetime_of_main_beams,
//   p_thunder_battle_def, it; { pcreature { hpParams, ptc_entity_events, ptc_elec } }. lifetime_in_seconds / lifetime_of_main_beams are
//   computed and never read (FableWin 0x0285A19E..0x0285A1E1: Timer / GetConstantFPS, minus the storm def's MainBeamsStartTime); retail
//   drops them as dead code. FableWin opens the hit loop with `xor edx,edx ; je` (0x0285A2AC): the VC6 for-scope idiom.
// Callees: CDefClassBase::GetSubDef<CThunderBattleDef> 0x008E3240, CThing::GetPos 0x004C73D0, CModeBase::GetWorld 0x0073A680,
//   CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, vector<CThingCreatureBase*>, CFilter_Bind2<CThingFilter_IsNot,
//   CThingFilter_In2DRange> > 0x008E3C50, CBaseIntelligentPointer ctor / SetPItem / dtor 0x00A01B10 / 0x00A01B90 / 0x00A01C10, STLport
//   __find<CIntelligentPointer<CThing>*, ..> 0x006C4060, CCharString ctor / dtor 0x0099EBF0 / 0x0099EAE0, CThing::AddTC 0x004C9D60,
//   CTCElectrocutedEffect::Reset 0x007CE7A0, CHitParameters ctor / dtor 0x00789238 / 0x00789358, AddDamageWithAttribute 0x00868CA0,
//   CThing::OnHit (vtable +0x104), the component-map LowerBound 0x0040F020 (inline GetTC<CTCEntityEvents>), operator new 0x00BFEA1A,
//   CEventHitBySpecialAbility ctor 0x00811830, CTCEntityEvents::AddEvent 0x008113C0, vector<CIntelligentPointer<CThing>>::push_back 0x004AD520.
// Inline-budget: FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE -- retail calls the GetSubDef<CThunderBattleDef> template COMDAT out of
//   line (0x008DD803 -> 0x008E3240); the existing shared opt-in keeps the visible body out of line.
// Inferred: the electrocuted-effect test is HasTC(TCI_ELECTROCUTED_EFFECT), not FableWin's GetTC(&ptc_elec): retail tests the presence
//   bit only (`test ah,4`, 0x008DD938) and makes no component-map lookup on that path (GetTC's out-of-line LowerBound would remain);
//   the AddTC result is assigned to ptc_elec and Reset there, as FableWin. The class name reaches AddTC through the implicit
//   const char* -> CCharString conversion (retail builds the temporary before pushing the other arguments, 0x008DD93D; an explicit
//   CCharString(...) argument is built after them).
// Inferred: the hit direction is spelled from two named position references (caster_pos, creature_pos: descriptive names, not in
//   Ego_d's record) and C3DVector(creature_pos - caster_pos per component): retail computes and stores X, Y, Z one at a time
//   (0x008DD980..0x008DD9AB); FableWin's `pcreature->GetPos() - GetParentThing().GetPos()` (operator-, every shared spelling of it and
//   of the C3DVector constructors) leaves all three differences on the x87 stack first (744v744, 19 lines). Same two GetPos calls in
//   the same order (caster first).
// Overlay: engine/CThunderBattleDef.h (new header: Ego_d / Ego_r CThunderBattleDef on CParentDefClassBase, LightningDamage +0x44).
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
    float lifetime_in_seconds = (float)Timer / CMainGameComponent::GetConstantFPS();
    float lifetime_of_main_beams = lifetime_in_seconds - PLightningStormDef->MainBeamsStartTime;
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
                const C3DVector& caster_pos = GetParentThing().GetPos();
                const C3DVector& creature_pos = pcreature->GetPos();
                CHitParameters hpParams(NULL, &GetParentThing(), NULL, pcreature->GetPos(), C3DVector(creature_pos.X - caster_pos.X, creature_pos.Y - caster_pos.Y, creature_pos.Z - caster_pos.Z), true, false, false, false);
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