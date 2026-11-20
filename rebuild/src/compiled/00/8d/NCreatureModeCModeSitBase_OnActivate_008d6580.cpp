// NCreatureMode::CModeSitBase::OnActivate() -- base activation, then seed the idle counter with a random frame count derived from the mode's combination-idle delay
// Leaf: ?OnActivate@CModeSitBase@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6580, 144 bytes, two `ret` tails; CModeSitChair vtable slot 5 (inherited).
// Callees: CModeBase::OnActivate @ 0x0073AAC0 (qualified), virtual GetType (slot 1),
//   CModeBase::GetNoFramesBeforePlayingCombinationIdleAnims(EMode) @ 0x0073A6F0 (as the landed CModeStandBase::AnimUpdate
//   0x00862EC0), _ftol @ 0x00BFEA70 (twice), CModeBase::GetWorld @ 0x0073A680, the world's random seed accessor @ 0x0049D810,
//   GFROR13 @ 0x00497890 (the inlined generator `seed = GFROR13(seed * 0x24A1 + 0x24DF)`).
// Name: FableWin.pdb `?OnActivate@CModeSitBase@NCreatureMode@@UAEXXZ` (the catalog's CModeSitChair::vfunc_5 is inherited).
// Layout: Ego_r.pdb CModeSitBase (0x2c): IdleCounter +0x1c, PlayingIdle +0x20, PRemoveAction +0x24.
// Constants: 0x01238174 = 0.75f, 0x01285670 = -1.5f.
// Name fix (NF124, 2026-11-20, q6453 / q6454): the local CModeBase / CModeSitBase / CWorld models replaced by the shared
//   engine/CTCCreatureModeManager.h (FABLE_RETAIL_CMODEBASE_VIRTUALS + NF124 opt-in _GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS),
//   engine/NCreatureMode_CModeSitFloor.h (NF124 opt-in FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ONACTIVATE) and engine/CWorld.h. Callees now bind
//   the PDB spellings ?OnActivate@CModeBase@CTCCreatureModeManager@@UAEXXZ (0x0073AAC0), ?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ
//   (0x0073A680) and ?GetNoFramesBeforePlayingCombinationIdleAnims@CModeBase@CTCCreatureModeManager@@IAEJW4EMode@NCreatureMode@@@Z (0x0073A6F0);
//   the row's own leaf is now the virtual override (was the non-virtual ?OnActivate@CModeSitBase@NCreatureMode@@QAEXXZ).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ONACTIVATE
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/NCreatureMode.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode_CModeSitFloor.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"

void NCreatureMode::CModeSitBase::OnActivate()
{
    CModeBase::OnActivate();
    float frames = (float)GetNoFramesBeforePlayingCombinationIdleAnims((NCreatureMode::EMode)GetType());
    long minFrames = (long)(frames * 0.75f);
    unsigned long* seed = GetWorld().GetWorldSeed();
    unsigned long range = -minFrames - (long)(frames * -1.5f);
    IdleCounter = GFRandom(range, seed) + minFrames;
}
