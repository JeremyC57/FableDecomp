// CAnimSet::UpdateDistanceMoved(float distance) -- advance a distance-driven anim set: each animation's speed is its movement
// distance per cycle / 100; the change is distance / (alpha-weighted speed total * the animator's thing scale), 0.1 when no animation
// moves and 0 when the total is negligible; the completion pair is then moved on (UpdatePercentageCompleteTo), looping sets wrap
// back into [0,1] and others clamp to [0,1] and finish at 1.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070CE10, length 495 bytes, __fastcall, ret 4.
// Callees: std::vector<float>::vector(size_type, const float&, const allocator&) 0x004516E7 (COMDAT), CMeshDataBank::PeekAnimation
//   0x00A242C0, free 0x00BFEA14 (via the inline ~vector).
// Globals: CMainGameComponent::PMainGameComponent 0x013B86A0 (+0x24 World -> +0x3c PMainMeshBank), DELTA_NOTIONAL_ZERO 0x0129BA3C;
//   literals 0.01f 0x01230A5C, 0.1f 0x0122EE58, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Names: Ego_r.pdb ?UpdateDistanceMoved@CAnimSet@@AAEXM@Z (0x00576690), FableWin.pdb 0x01E9BFF0; inline helpers PeekAnimInfo
//   (Ego_r 0x005758B0), UpdatePercentageCompleteTo (Ego_r 0x00575250; FableWin debug copy 0x01E9BA00 is just Last = Current;
//   Current = p, 14 instructions -- the wrap/clamp is this function's own), C3DAnimationInfo::GetMovementDistance (FableWin
//   0x01D8C5A0), CAnimateThing::GetThingScale (FableWin).
// Shape: the fill ctor's body is visible to the TU but called out of line (FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE): VC7.1 then
//   knows `speeds` does not escape, keeps its _M_start in ebp through the PeekAnimation calls and the inline dtor, and parks the
//   empty allocator temporary on the `distance` argument slot (frame 0x10). The new percentage (inline parameter) is spilled to
//   the same slot and re-read by the wrap branches.
// Leaf: ?UpdateDistanceMoved@CAnimSet@@AAEXM@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DANIMATIONINFO
#define FABLE_RETAIL_C3DANIMATIONINFO_MOVEMENT
#define FABLE_RETAIL_CANIMSET_UPDATEDISTANCE
#define FABLE_RETAIL_CANIMATETHING_GETTHINGSCALE
#define FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#include "stlport/fable_containers.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB

inline const C3DAnimationInfo& CAnimSet::PeekAnimInfo(short anim) const
{
    return *CMainGameComponent::Get()->GetWorld().GetMeshBank()->PeekAnimation(anim);
}

inline void CAnimSet::UpdatePercentageCompleteTo(float percentage)
{
    LastPercentageComplete = CurrentPercentageComplete;
    CurrentPercentageComplete = percentage;
}

void CAnimSet::UpdateDistanceMoved(float distance)
{
    std::vector<float> speeds(Anims.size(), 0.0f);
    bool noMovement = true;
    for (unsigned int i = 0; i < Anims.size(); i++) {
        if (Anims[i]) {
            speeds[i] = PeekAnimInfo(Anims[i]).GetMovementDistance() * 0.01f;
            if (speeds[i] > 0.0f)
                noMovement = false;
        }
    }
    float change;
    if (noMovement) {
        change = 0.1f;
    } else {
        float total = 0.0f;
        for (unsigned int i = 0; i < Anims.size(); i++) {
            if (Anims[i])
                total += InternalAlphas[i].CurrentAlpha * speeds[i];
        }
        if (total > DELTA_NOTIONAL_ZERO)
            change = distance / (total * PAnimator->GetThingScale());
        else
            change = 0.0f;
    }
    LastUpdateChange = change;
    UpdatePercentageCompleteTo(change + CurrentPercentageComplete);
    if (Looping) {
        if (LastPercentageComplete > 1.0f) {
            LastPercentageComplete -= 1.0f;
            CurrentPercentageComplete -= 1.0f;
        } else if (LastPercentageComplete < 0.0f) {
            LastPercentageComplete += 1.0f;
            CurrentPercentageComplete += 1.0f;
        }
    } else {
        float& current = CurrentPercentageComplete;
        if (current < 0.0f)
            current = 0.0f;
        else if (current > 1.0f)
            current = 1.0f;
        if (1.0f - DELTA_NOTIONAL_ZERO <= current)
            Finished = true;
    }
}