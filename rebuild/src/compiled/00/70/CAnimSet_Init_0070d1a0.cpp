// CAnimSet::Init(CAnimateThing& animator, const CAnimInit& init, float alpha) -- reset the set from an anim init: one anim and one blend alpha (init.Alpha, init.Alpha), the current world frame, the percentage pair, alpha (current = last), the speed / flag / priority fields and the looping / finished / forwards / updated bits
// Leaf: ?Init@CAnimSet@@AAEXAAVCAnimateThing@@ABVCAnimInit@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D1A0, length 329 bytes, thiscall `ret 0Ch`.
// Identity (NF125 round 10): Ego_r.pdb ?Init@CAnimSet@@AAEXAAVCAnimateThing@@ABVCAnimInit@@M@Z (private; engine/CAnimSet.h declares it);
//   both landed callers 0x0070D580 SetMainAnim and 0x0070D780 AddCombinationAnim bind that spelling and pass (*this, init, alpha).
//   Was a free-standing local CAnimSet / vector / CAnimSetInitData model defining ?Init@CAnimSet@@QAEXPAVCAnimateThing@@ABUCAnimSetInitData@@M@Z.
// Callees: memmove @ 0x00BFEAE6 (vector<short>::clear's trivial copy), vector<short>::_M_insert_overflow @ 0x00710980 and
//   vector<CTransitionBlendAlpha>::_M_insert_overflow @ 0x00710F70 (push_back's overflow path, the STLport bodies visible but out of line:
//   FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), CWorld::GetFrame @ 0x0049D870 (static).
// Shape: the statement order of the landed body (and of the same-TU copy in 0x0070D780): each vector is clear()ed then push_back'd;
//   `CurrentAlpha = LastAlpha = alpha;` before the zeroed change fields gives retail's store order.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include "fable_string.h"
#include "engine/CWorld.h"                             // CWorld::GetFrame 0x0049D870 (static)
#include "engine/CAnimSet.h"

void CAnimSet::Init(CAnimateThing& animator, const CAnimInit& init, float alpha)
{
    PAnimator = &animator;
    Anims.clear();
    Anims.push_back((short)init.Anim);
    InternalAlphas.clear();
    InternalAlphas.push_back(CTransitionBlendAlpha(init.Alpha, init.Alpha));
    FrameInternalAlphasUpdated = CWorld::GetFrame();
    CurrentPercentageComplete = init.Frame.Current;
    LastPercentageComplete = init.Frame.Last;
    CurrentAlpha = LastAlpha = alpha;
    LastUpdateChange = 0.0f;
    ChangeAlphaBy = 0.0f;
    Type = init.Type;
    UpdateMode = init.UpdateMode;
    Looping = init.Looping;
    Speed = init.Speed;
    Finished = false;
    Forwards = true;
    Flags = init.Flags;
    TypeFlags = init.TypeFlags;
    HadUpdate = false;
    Priority = init.Priority;
}
