// CTCTurncoat::FrameUpdate() -- for a turncoatable creature: while turncoated (alive parent, turncoatedness at its resistance) count the turncoat time down and on expiry restart it, play the deactivate effect, clear the turncoatedness, kill the turncoated effect and play the release sound; otherwise let the turncoatedness recover and grow / shrink the stage-effect list to the current stage
// FITTED: `IS_THING_ALIVE(fx.Get())` in the kill loop (retail tests one GetPItem result, 0x007C7EEB, then calls GetPItem again for Kill); cur_stage computed before max_stage (gives the PDef temporary retail's ebp, 0x007C7C5A); the implicit const char* sound / helper-name arguments (temporaries built before the pushes, 0x007C7D20 / 0x007C7E62).
// Leaf: ?FrameUpdate@CTCTurncoat@@UAEXXZ
// Inline-budget: existing shared declaration-only opt-in FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE -- retail calls vector<CIntelligentPointer<CThing> >::push_back 0x004AD520 (0x007C7EB5); the inline STLport body expands here (794 v 744 bytes without it). IS_THING_ALIVE<CIntelligentPointer<CThing> > stays out of line (0x007C7CCD) with no device.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C7C40, 744 bytes, plain `ret`, 8-aligned frame.
// Ego_d.pdb (tc_turncoat.obj): parent (CThingCreatureBase&) at function scope; { ptc_sound_player }; { new_cur_stage, interp, cur_stage,
//   stage_proportion, max_stage { i { fx (CThing*) { ptc_emitter } } } { num_effects_to_kill { i { fx (CIntelligentPointer<CThing>&) } } } }.
// Same-TU bodies (Ego_d order, after the row), each graded at its own address with this file: IsTurncoatable 0x007C7870 MATCH,
//   IsTurncoated 0x007C7890 MATCH (both inlined here).
// Callees: CThing::GetPos 0x004C73D0, CTCSpecialAbilities::CreateParticleEffect 0x0072AB70, IS_THING_ALIVE<CIntelligentPointer<CThing> >
//   0x004CC340 (out of line), CBaseIntelligentPointer 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CThing::Kill 0x004C9B80, LowerBound 0x0040F020, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, CTCDParticleEmitter::AttachToThing
//   0x006E0BE0, vector<CIntelligentPointer<CThing> >::push_back 0x004AD520 (out of line: existing FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE),
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, __ftol2.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCTURNCOAT
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTurncoatDef.h"
#include "engine/CTCTurncoat.h"
#include "engine/ThingAlive.h"

void CTCTurncoat::FrameUpdate()
{
    if (!IsTurncoatable())
        return;
    CThingCreatureBase& parent = static_cast<CThingCreatureBase&>(GetParentThing());
    if (IS_THING_ALIVE(&parent) && IsTurncoated())
    {
        if (--WillTurncoatTimer > 0)
            return;
        CurrentRecoveryTimer = 0;
        WillTurncoatTimer = TurncoatTimeLast;
        if (PDef->TurncoatDeactivateEffect)
            CTCSpecialAbilities::CreateParticleEffect(PDef->TurncoatDeactivateEffect, parent.GetPos());
        CurrentTurncoatedness = 0.0f;
        if (IS_THING_ALIVE(TurncoatedEffect))
            TurncoatedEffect->Kill(true);
        CTCSoundPlayer* ptc_sound_player;
        if (GetParentThing().GetTC(TCI_SOUND_PLAYER, &ptc_sound_player))
            ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_TURNCOAT_RELEASE", true, false, false, -1);
    }
    else
    {
        if (CurrentRecoveryTimer == 0)
        {
            CurrentTurncoatedness -= ResistanceRecoveryPerFrame;
            if (CurrentTurncoatedness < 0.0f)
                CurrentTurncoatedness = 0.0f;
        }
        else
            --CurrentRecoveryTimer;
        long cur_stage = TurncoatStageEffects.size();
        long max_stage = PDef->TurncoatStageEffects.size();
        float stage_proportion = CurrentTurncoatedness / CreatureTurncoatResistance;
        float interp = stage_proportion / (1.0f - 1.0f / max_stage);
        if (interp < 0.0f)
            interp = 0.0f;
        else if (interp > 1.0f)
            interp = 1.0f;
        long new_cur_stage = (long)(interp * max_stage);
        if (new_cur_stage > cur_stage)
        {
            for (long i = cur_stage; i < new_cur_stage; ++i)
            {
                CThing* fx = CTCSpecialAbilities::CreateParticleEffect(PDef->TurncoatStageEffects[i], parent.GetPos());
                if (IS_THING_ALIVE(fx))
                {
                    CTCDParticleEmitter* ptc_emitter;
                    fx->GetTC(TCI_PARTICLE_EMITTER, &ptc_emitter);
                    ptc_emitter->AttachToThing(parent, 8, "", 0, 0.0f);
                }
                TurncoatStageEffects.push_back(CIntelligentPointer<CThing>(fx));
            }
        }
        else if (new_cur_stage < cur_stage)
        {
            long num_effects_to_kill = cur_stage - new_cur_stage;
            for (long i = 0; i < num_effects_to_kill; ++i)
            {
                CIntelligentPointer<CThing>& fx = TurncoatStageEffects.back();
                if (IS_THING_ALIVE(fx.Get()))
                    fx->Kill(true);
                TurncoatStageEffects.pop_back();
            }
        }
    }
}

bool CTCTurncoat::IsTurncoatable() const
{
    return TurncoatOverride && PDef->Turncoatable;
}

bool CTCTurncoat::IsTurncoated() const
{
    return IsTurncoatable() && CurrentTurncoatedness >= CreatureTurncoatResistance;
}
