// CAnimateThing::CalcCachedAnims() const -- recompute the cached start (0) and end (1) blend states from the current animation and the head of the delayed-animation queue, then give both the parent thing's skeletal morph when it has one
// Grade: DIFFER(618v618) -- frame slots only: retail puts the second blend-state temporary (0x0070E327) and the IsNull temporary (0x0070E3C6) in the skeletal temporaries' slot [esp+14h]; ours shares [esp+0Ch] with the first (frame-packer weight tie, see NOTES)
// Leaf: ?CalcCachedAnims@CAnimateThing@@ABEXXZ
// Retail: Fable.exe (TLC, VC7.1) 0x0070E2A0, 618 bytes, thiscall `ret`.
// Names: Ego_r.pdb ?CalcCachedAnims@CAnimateThing@@ABEXXZ (private const; Ego_d this = const CAnimateThing*, function-scope local
//   ptc_appearance_morph (const CTCSkeletalMorph*); Ego_d's `{ long c1 }` block has no retail code: a debug-only block, FableWin /Od 277i
//   v ours 138i). Callees: CalcInterpolatedMeshAnim 0x0070D940 (Ego_r), PeekTC<const CTCSkeletalMorph> through CVectorMap::LowerBound
//   const 0x004365B0 (TCI_SKELETAL_MORPH 120), CTCSkeletalMorph::GetPSkeletalMorph const 0x00786870 (Ego_r), C3DAnimationBlendState::
//   SetSkeletalMorph 0x00A3C350 (Ego_r), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC.
// Headers (hdr/): new engine/CTCSkeletalMorph.h; opt-ins FABLE_RETAIL_CANIMATETHING_CALCCACHEDANIMS, FABLE_RETAIL_C3DANIMATIONBLENDSTATE_
//   SETSKELETALMORPH, FABLE_RETAIL_STLP_LIST_CONST_FRONT; existing CIVCOUNTEDPOINTER_RELEASE_ISNULL + CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
//   (FITTED: the temporaries' release `mov edi,[eax+4] ; mov ecx,eax` order 0x0070E2F5; fwsize 17i SAME count as FableWin).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_GENERATED_SPECIALS_PASS23
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SETSKELETALMORPH
#define FABLE_RETAIL_CANIMATETHING_CALCCACHEDANIMS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CONST_FRONT
#include "fable_string.h"
#include "engine/CAnimateThing.h"
#include "engine/CThing.h"
#include "engine/CTCSkeletalMorph.h"

void CAnimateThing::CalcCachedAnims() const
{
    const CTCSkeletalMorph* ptc_appearance_morph;
    StartAnimState = CalcInterpolatedMeshAnim(CurrentAnim, DelayedAnimQueue.front(), 0.0f);
    EndAnimState = CalcInterpolatedMeshAnim(CurrentAnim, DelayedAnimQueue.front(), 1.0f);
    if (GetParentThing().PeekTC(&ptc_appearance_morph)) {
        if (!ptc_appearance_morph->GetPSkeletalMorph().IsNull()) {
            StartAnimState.SetSkeletalMorph(ptc_appearance_morph->GetPSkeletalMorph());
            EndAnimState.SetSkeletalMorph(ptc_appearance_morph->GetPSkeletalMorph());
        }
    }
}
