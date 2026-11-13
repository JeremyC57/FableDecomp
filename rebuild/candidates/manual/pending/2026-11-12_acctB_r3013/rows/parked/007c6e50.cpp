// CTCTurncoatSpell::FrameUpdate() -- per frame: tick the will-button timer (a dead target releases the button); RELEASED removes the spell, JUST_RELEASED also unlocks the ability mutex and finishes the caster's action; while the button is held: test the target against TurncoatRange[LevelOfSpell], start the out-of-range effect / stop or restart the turncoating effects on a range change, drain whole points of the accumulated stamina cost, absorb a turncoated target (sound, originator, release) or, while the target lives with health and the caster has stamina and is in range, raise its turncoatedness by whole points of accumulated resistance damage and every XPFrameCounter frames give the caster the turncoat ability's stat experience; always make a noise
// Grade: DIFFER(1804v1806) -- one allocation decision in the XP block: retail loads the caster into a codegen temp (`mov edx,[esp+14h]` hoisted above `mov [esi+90h],ecx`, then `mov eax,[edx+2Ch]` / `mov esi,edx`, 0x007C741C..0x007C7432), ours gives the caster's split piece esi (`mov esi,[esp+14h]` after the store, +2 bytes) and the temp rotation after it shifts by one slot (eax/ecx/edx at 0x007C7434..0x007C749E)
// Leaf: ?FrameUpdate@CTCTurncoatSpell@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C6E50, 1806 bytes (`sub esp,64h` .. `ret` at 0x007C755D; FinalCleanup follows at 0x007C7560).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCTurncoatSpell@@UAEXXZ (Ego_r donor order StopTurncoatingEffects
//   0x007C5B70 / FrameUpdate 0x007C5C30 / FinalCleanup 0x007C6420 = retail 0x007C6DA0 / 0x007C6E50 / 0x007C7560); slot 9 of the
//   CTCTurncoatSpell vtable 0x0126E5C4 (CTCBase slot 9 FrameUpdate).
// Callees: CWillButtonHelperTimer::FrameUpdate 0x008EDDD0; CBaseIntelligentPointer::GetPItem 0x00A01B50 / ~ 0x00A01C10;
//   IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 and <CIntelligentPointer<CThingCreatureBase> > 0x006961F0 (out of line at the
//   later sites; expanded at the two first PTarget checks); CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760
//   (out of line); CVectorMap::LowerBound 0x0040F020 (the expanded GetTC); CThing::RemoveTC 0x004C9840, GetPos 0x004C73D0;
//   CThingCreatureBase::FinishCurrentAction 0x00662650, MakeNoise 0x00662110; GetSubDef<CSpecialEffectsDef> 0x00669580;
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30; CTCSpecialAbilities::CreateParticleEffect 0x0072AB70 / FadeOrKillEffect(CThing*)
//   0x0072AC40 (both static __fastcall); CTCDParticleEmitter::AttachToThing 0x006E0BE0; CTCTurncoatSpell::StopTurncoatingEffects
//   0x007C6DA0 / StartTurncoatingEffects 0x007C6AA0; CTCHeroStats::ModifyStaminaBy 0x0057B1DB; CTCTurncoat::IsTurncoated 0x007C7890 /
//   ModifyTurncoatedness 0x007C7A00 / SetTurncoatOriginator 0x007C76D0 (this TU's out-of-line COMDAT); CGameDefinitionManager::Get
//   0x0044C6B0 + CDefinitionManager::GetDef<CHeroAbilityDef>(const CCharString&, CDefPointer*) 0x00792350; CTCHeroExperience::
//   AddTrainableStatExperience 0x006D8300; CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00; CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Data: "HERO_TURNCOATING_OUT_OF_RANGE_EFFECT" 0x0126E6C8, "" 0x0122D70E, "HERO_ABILITY_WILL_TURNCOAT" 0x0126E6AC,
//   "HERO_ABILITY_TURNCOAT_ABSORB" 0x0126E68C, "SIMPLE_NOISE" 0x01268A5C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Shape: `bool in_range = squared distance <= range * range;` (the late `mov byte [esp+13h],1` and `test ah,1 ; je`); the else-if chain
//   of the three range transitions (the third arm re-tests InRange, 0x007C7274); two textual copies of the out-of-range effect start
//   (cleanup-flag bits 1 / 2 of [esp+18h]); the implicit CThing* -> CIntelligentPointer conversion for the effect (`lea edx,[esp+40h]`
//   re-read, 0x007C7081); `if (!turncoat->IsTurncoated()) { if (dead || health <= 0 || stamina <= 0) release; else if (InRange) ... }
//   else { absorb; release; }` (the absorb arm last, falling into the shared release store 0x007C7528); GetEffectFromString's body
//   visible (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY: the effect tables' priority drops to -13, below `turncoat`'s -12, so
//   turncoat gets ebp as in retail and the PTarget address temp splits into ebx / lea ecx / edi: grade 3 went 0.926 -> 0.950); the
//   by-name GetDef body visible (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY: the def kept in esi, no release null test).
// Residue (measured with c2prio, tools/c2p): the caster's XP-block split piece (2 refs: the HasTC word load, spill 2, and the map copy)
//   is coloured esi (copy bonus with the map); retail's matches a skipped piece whose load is a codegen temp (rotation ecx -> edx -> eax,
//   which our build shows at the same lines). Invariant under 14 c2prio-checked spellings (XP declaration position, `!--`, split
//   decrement, explicit HasTC + GetTC(type), GetTC(type, &p), `= 0` init, CThing& / CThing* views, nested / chained condition
//   forms, no MakeNoise, ParentThing at the stats / sound lookups); a spill-0 what-if on that piece alone did not move it either.
// Chunk 3013 (pass 1, 2 grades): the c2 split pass, not the spill test, decides it. Measured (tools/c2p/c2prio_b.py, first/last
//   block of each piece): the XP piece #73 spans the counter-store block (165) and the map block (167); forcing its spill to 0 at the
//   driver check (0x10782a5e) only defers it and the recomputed spill (1) colours it again. Writing MakeNoise on ParentThing keeps
//   `this` live through the XP block and then NO XP piece is formed (the HasTC / map refs fall into the memory piece): the XP code
//   becomes retail's `mov edx,[esp+14h]` hoisted / `mov eax,[edx+2Ch]` / copy (grade 2: 1805v1806, ratio 0.971), but map / experience
//   move to edi / ebp and MakeNoise reads [esi+4] (retail [esp+18h]), so that spelling is not retail. ParentThing at the XP lookup
//   (grade 1) re-reads [esi+4]. No `parent` local at all: no candidate, every use from [esi+4].
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_TURNCOAT
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_STAMINA
#define FABLE_RETAIL_CTCHEROSTATS_MODIFYSTAMINA
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCTURNCOAT
#define FABLE_RETAIL_CTCTURNCOATSPELL
#define FABLE_RETAIL_CWILLBUTTONHELPERTIMER_STATE_INLINE
#define FABLE_RETAIL_CWILLBUTTONHELPERTIMER_SETSTATE_INLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/CTCTurncoat.h"
#include "engine/CSpecialAbilitiesTurncoatSpellDef.h"
#include "engine/CTCTurncoatSpell.h"

void CTCTurncoatSpell::FrameUpdate()
{
    Timer.FrameUpdate();
    CThingCreatureBase* parent = (CThingCreatureBase*)ParentThing;
    CWillButtonHelperTimer::EButtonHeldState state = Timer.GetCurrentButtonHeldState();
    if (!IS_THING_ALIVE(PTarget))
        Timer.SetCurrentButtonHeldState(CWillButtonHelperTimer::JUST_RELEASED_DELAYED);
    switch (state) {
    case CWillButtonHelperTimer::BEING_HELD_DOWN:
    {
        if (!IS_THING_ALIVE(PTarget))
            return;
        C3DVector my_pos = parent->GetPos();
        C3DVector target_pos = PTarget->GetPos();
        C3DVector difference = my_pos - target_pos;
        float range = PDef->TurncoatRange[LevelOfSpell];
        bool in_range = difference.GetSquaredMagnitude() <= range * range;
        if (!InRange && !in_range && !IS_THING_ALIVE(TurncoatingEffectOnMeOutOfRange)) {
            CDefPointer<const CSpecialEffectsDef> effects;
            long effect = 0;
            if (PDef->GetSubDef(&effects) && effects->GetEffectFromString("HERO_TURNCOATING_OUT_OF_RANGE_EFFECT", effect)) {
                TurncoatingEffectOnMeOutOfRange = CTCSpecialAbilities::CreateParticleEffect(effect, ParentThing->GetPos());
                if (IS_THING_ALIVE(TurncoatingEffectOnMeOutOfRange)) {
                    CTCDParticleEmitter* emitter;
                    TurncoatingEffectOnMeOutOfRange->GetTC(&emitter);
                    emitter->AttachToThing(*ParentThing, 8, "", 0, 0.0f);
                }
            }
        }
        else if (InRange && !in_range) {
            StopTurncoatingEffects();
            CDefPointer<const CSpecialEffectsDef> effects;
            long effect = 0;
            if (PDef->GetSubDef(&effects) && effects->GetEffectFromString("HERO_TURNCOATING_OUT_OF_RANGE_EFFECT", effect)) {
                TurncoatingEffectOnMeOutOfRange = CTCSpecialAbilities::CreateParticleEffect(effect, ParentThing->GetPos());
                if (IS_THING_ALIVE(TurncoatingEffectOnMeOutOfRange)) {
                    CTCDParticleEmitter* emitter;
                    TurncoatingEffectOnMeOutOfRange->GetTC(&emitter);
                    emitter->AttachToThing(*ParentThing, 8, "", 0, 0.0f);
                }
            }
        }
        else if (!InRange && in_range) {
            if (IS_THING_ALIVE(TurncoatingEffectOnMeOutOfRange))
                CTCSpecialAbilities::FadeOrKillEffect(TurncoatingEffectOnMeOutOfRange.Get());
            StartTurncoatingEffects();
        }
        InRange = in_range;
        CTCHeroStats* hero_stats;
        parent->GetTC(&hero_stats);
        AccModifyStamina += PerFrameStaminaModify;
        long stamina_modify = StaminaModify;
        while (AccModifyStamina > 1.0f) {
            AccModifyStamina -= 1.0f;
            ++stamina_modify;
        }
        hero_stats->ModifyStaminaBy(-stamina_modify);
        CTCTurncoat* turncoat;
        PTarget->GetTC(&turncoat);
        if (!turncoat->IsTurncoated()) {
            if (!IS_THING_ALIVE(PTarget) || PTarget->GetHealth() <= 0.0f || hero_stats->GetStamina() <= 0)
                Timer.SetCurrentButtonHeldState(CWillButtonHelperTimer::JUST_RELEASED_DELAYED);
            else if (InRange) {
                AccModifyResistance += PerFrameResistanceModify;
                long resistance_modify = ResistanceModify;
                while (AccModifyResistance > 1.0f) {
                    AccModifyResistance -= 1.0f;
                    ++resistance_modify;
                }
                turncoat->ModifyTurncoatedness((float)resistance_modify);
                if (--CurXPFrameCounter == 0) {
                    CurXPFrameCounter = XPFrameCounter;
                    CTCHeroExperience* experience;
                    if (parent->GetTC(&experience)) {
                        CDefPointer<const CHeroAbilityDef> ability_def;
                        CGameDefinitionManager::Get().GetDef("HERO_ABILITY_WILL_TURNCOAT", &ability_def);
                        experience->AddTrainableStatExperience(ability_def->Stat, ability_def->StatExperienceGainedOnUse);
                    }
                }
            }
        }
        else {
            CTCSoundPlayer* sound;
            if (parent->GetTC(&sound))
                sound->PlayAttachedCriteria("HERO_ABILITY_TURNCOAT_ABSORB", true, false, false, -1);
            turncoat->SetTurncoatOriginator(parent);
            Timer.SetCurrentButtonHeldState(CWillButtonHelperTimer::JUST_RELEASED_DELAYED);
        }
        parent->MakeNoise("SIMPLE_NOISE");
        break;
    }
    case CWillButtonHelperTimer::JUST_RELEASED:
    {
        CTCSpecialAbilities* abilities;
        if (ParentThing->GetTC(&abilities))
            abilities->UnLockAbilitiyMutex();
        parent->FinishCurrentAction();
        parent->RemoveTC(TCI_TURNCOAT_SPELL);
        return;
    }
    case CWillButtonHelperTimer::RELEASED:
        parent->RemoveTC(TCI_TURNCOAT_SPELL);
        return;
    }
}
