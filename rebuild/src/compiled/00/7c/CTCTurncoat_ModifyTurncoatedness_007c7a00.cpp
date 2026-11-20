// CTCTurncoat::ModifyTurncoatedness(float) -- while below its resistance, add to the creature's turncoatedness (clamped to [0, resistance], restarting the recovery delay); on reaching the resistance play the activate effect, attach the turncoated effect, kill the stage effects and play the turncoat chant
// Leaf: ?ModifyTurncoatedness@CTCTurncoat@@QAEXM@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C7A00, 572 bytes (+ int3 padding), `ret 4`.
// Callees: CThing::GetPos() const 0x004C73D0; CreateParticleEffect(long, const C3DVector*) 0x0072AB70 (`__fastcall`);
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / ~CBaseIntelligentPointer 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem
//   0x00A01B90; IS_THING_ALIVE<CIntelligentPointer<...> > 0x004CC340 (`__fastcall`, out of line); LowerBound 0x0040F020
//   (interfaces 0x4B particle emitter, 0x10 sound player); CTCDParticleEmitter::AttachToThing 0x006E0BE0; CThing::Kill(bool)
//   0x004C9B80; vector<CIntelligentPointer<CThing> >::erase(first, last) 0x004AC0F0; CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00;
//   CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0.
// Layout (retail CTCTurncoat = Ego_r.pdb, the stage-effect vector 12 bytes, as the landed ctor 0x007C78C0):
//   CurrentTurncoatedness +0x10, PDef +0x14, TurncoatedEffect +0x18, TimeDelayBeforeResistanceRecoveryFrames +0x30,
//   CurrentRecoveryTimer +0x34, TurncoatStageEffects +0x3c, CreatureTurncoatResistance +0x48; CTurncoatDef TurncoatedEffect
//   +0x28, TurncoatActivateEffect +0x2c.
// NF200 (2026-11-20, q4492): port onto the shared headers in the shape of the landed sibling 0x007C7C40 CTCTurncoat::FrameUpdate
//   (engine/CTCTurncoat.h FABLE_RETAIL_CTCTURNCOAT, CThing.h, CIntelligentPointer.h, CTCSpecialAbilities.h, CTCSoundPlayer.h,
//   CTCDParticleEmitter.h, CTurncoatDef.h, ThingAlive.h). The row-local CCharString / CThing / CBaseIntelligentPointer /
//   CIntelligentPointer / CEffectVector / CTCDParticleEmitter / CTCSoundPlayer / CTurncoatDef / CDefPointer / CTCTurncoat models are gone:
//   0x004CC340 binds ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z (FableWin.pdb), and
//   the stage vector's erase 0x004AC0F0 the STLport vector<CIntelligentPointer<CThing> >::erase (declaration only:
//   FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, as retail calls it).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCTURNCOAT
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
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

void CTCTurncoat::ModifyTurncoatedness(float amount)
{
    if (CurrentTurncoatedness >= CreatureTurncoatResistance)
        return;
    CurrentRecoveryTimer = TimeDelayBeforeResistanceRecoveryFrames;
    CurrentTurncoatedness += amount;
    if (CurrentTurncoatedness > CreatureTurncoatResistance)
        CurrentTurncoatedness = CreatureTurncoatResistance;
    else if (CurrentTurncoatedness < 0.0f)
        CurrentTurncoatedness = 0.0f;
    if (CurrentTurncoatedness >= CreatureTurncoatResistance) {
        if (PDef->TurncoatActivateEffect)
            CTCSpecialAbilities::CreateParticleEffect(PDef->TurncoatActivateEffect, GetParentThing().GetPos());
        if (!IS_THING_ALIVE(TurncoatedEffect)) {
            long effect = PDef->TurncoatedEffect;
            if (effect) {
                TurncoatedEffect = CTCSpecialAbilities::CreateParticleEffect(effect, GetParentThing().GetPos());
                if (IS_THING_ALIVE(TurncoatedEffect)) {
                    CTCDParticleEmitter* particleEmitter;
                    TurncoatedEffect->GetTC(TCI_PARTICLE_EMITTER, &particleEmitter);
                    particleEmitter->AttachToThing(GetParentThing(), 8, "", 0, 0.0f);
                }
            }
        }
        for (long i = 0; i < (long)TurncoatStageEffects.size(); ++i) {
            if (IS_THING_ALIVE(TurncoatStageEffects[i]))
                TurncoatStageEffects[i]->Kill(true);
        }
        TurncoatStageEffects.clear();
        CTCSoundPlayer* ptc_sound_player;
        if (GetParentThing().GetTC(TCI_SOUND_PLAYER, &ptc_sound_player))
            ptc_sound_player->PlayAttachedCriteria("HERO_ABILITY_TURNCOAT_CHANT", true, false, false, -1);
    }
}
