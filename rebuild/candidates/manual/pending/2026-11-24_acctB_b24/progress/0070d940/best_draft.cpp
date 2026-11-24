// CAnimateThing::CalcInterpolatedMeshAnim(const CCompleteAnimation& anim, const CDelayedAnimation& delayed, float alpha) const -- build the blend state of a complete animation at an interpolation alpha: the main set, cross-faded from its transition sets (a chain of transition states weighted by their interpolated alphas, skipped beyond the animation LOD distance), plus every loaded combination anim of the current and the delayed animation, each list stable-sorted by priority
// Grade: DIFFER(2482v2395) 127 hunks with /d2inlT67 (stock DIFFER(2610v2395) 122) -- call sequence = retail; residue: ours keeps 0 in ebp through the transition loop and `end` in memory where retail keeps `end` in ebx, and our scope-end releases keep their null stores (retail drops them)
// Inline-budget: /d2inlT67 -- vector<CAnimSetIt>::_M_insert_overflow at the first combination pass scores 68 (cost 73 at f 1.25), retail calls it (0x0070DF89) and expands the second pass's; our copy of 0x00711080 is instruction-equal to retail's, no loop shape tried makes the site colder; every other decision unchanged (max expanded score 66). NOTES.md B24C7.
// Leaf: ?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070D940, 2395 bytes, thiscall `ret 10h` (hidden result + 3 arguments), frame 0x3C, no EH.
//   Ego_r.pdb ?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z
//   (private const; Ego_r neighbours CalcMeshAnims / GetProgressFromAlpha); the function-static sorted_combination_anims 0x013BAC20
//   (guard 0x013BAC2C, atexit thunk _$E2 0x01229630) is named in both PDBs.
// Callees: CThing::GetPos 0x004C73D0, CWorld::GetGameLODCentre (slot 8), GFWorld3DDistanceSmallerThan 0x006614F0, CBaseClass ctor
//   0x0099A2D0 / dtor 0x0099A2E0, operator new 0x00BFEA1A (3 inline C3DAnimationTransitionState constructions, IVRefCount 1),
//   CAnimSet::GetMeshAnim 0x0070C900 (x6), C3DAnimationTransitionState::SetStartState 0x00710560 / SetEndState 0x007105A0 (x2 each),
//   GetProgressFromAlpha 0x0070BE40, C3DAnimationBlendState::SetBaseAnimation 0x00A3C2D0 (x2) / AddCombinationBlend 0x00A3C310 (x2),
//   vector<..TransitionState..>::push_back 0x00711C50 / overflow 0x00711A50 / ~vector 0x00711760, vector<CAnimSetIt>::reserve
//   0x00710B00 / _M_insert_overflow 0x00711080, STLport __stable_sort_aux 0x00711B80 (__fastcall, comparator carrying alpha),
//   CMeshDataBank::PeekAnimation 0x00A242C0 (x4), C3DAnimationInfo::IsLoaded 0x00A23F50 / TouchAnimation 0x00A26A50 (x2 each),
//   CAnimSet::GetInterpolatedTime 0x0070C800 (x2), malloc / free / atexit.
// Globals: g_primitiveDrawDistanceScale 0x013B8774, the bools 0x01380BDA (animation LOD) and 0x013756F7 (combination anims; both
//   names descriptive), CMainGameComponent::PMainGameComponent 0x013B86A0, DELTA_NOTIONAL_ZERO 0x0129BA3C, 1.0f / 24.0f literals.
//   C3DAnimationState index / time + Alpha); C3DAnimationTransitionState 0x14 (vtable 0x01265144). Row-local models (no CAnimateThing
// Headers: engine/CAnimateThing.h (CALCCACHEDANIMS, PROGRESSFROMALPHA), CAnimSet.h (MESHANIM), C3DAnimationTransitionState.h
//   (SETTERS), C3DAnimationStateAlpha.h (INDEX_CTOR), C3DAnimationBlendState.h (ADDCOMBINATIONBLEND); overlay $W/hdr (README.md).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
#define FABLE_RETAIL_C3DANIMATIONSTATE
#define FABLE_RETAIL_C3DANIMATIONSTATEALPHA
#define FABLE_RETAIL_C3DANIMATIONSTATEALPHA_INDEX_CTOR
#define FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE
#define FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE_INTERPOLATE
#define FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE_SETTERS
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_CTOR_BODY_PASS26
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_GENERATED_SPECIALS_PASS23
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_ADDCOMBINATIONBLEND
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DANIMATIONINFO
#define FABLE_RETAIL_C3DANIMATIONINFO_ISLOADED
#define FABLE_RETAIL_CANIMSET_UPDATEDISTANCE
#define FABLE_RETAIL_CANIMSET_MESHANIM
#define FABLE_RETAIL_CANIMATETHING_CALCCACHEDANIMS
#define FABLE_RETAIL_CANIMATETHING_PROGRESSFROMALPHA
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_STABLE_SORT_AUX_DECL
#define FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER_DECL
#define FABLE_RETAIL_CANIMSETITPRIORITYLESSTHAN
#include "stlport/fable_containers.h"
#include "engine/C3DAnimationTransitionState.h"
#include "engine/C3DAnimationStateAlpha.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/CAnimSet.h"
#include "engine/CAnimateThing.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "stlport/fable_algorithm.h"
#include "engine/GFWorldDistance.h"
#include "engine/CAnimSetItPriorityLessThan.h"

extern float g_primitiveDrawDistanceScale;                          // 0x013B8774
extern bool g_AnimationLOD;                                         // 0x01380BDA (name descriptive)
extern bool g_DrawCombinationAnims;                                 // 0x013756F7 (name descriptive)
typedef std::list<CAnimSet>::const_iterator CAnimSetIt;

inline const C3DAnimationInfo& CAnimSet::PeekAnimInfo(short anim) const
{
    return *CMainGameComponent::Get()->GetWorld().GetMeshBank()->PeekAnimation(anim);
}

C3DAnimationBlendState CAnimateThing::CalcInterpolatedMeshAnim(const CCompleteAnimation& anim, const CDelayedAnimation& delayed, float alpha) const
{
    bool beyond_lod = false;
    if (ParentThing.HasTC(TCI_PHYSICS)) {
        float distance = (g_primitiveDrawDistanceScale + 1.0f) * 24.0f;
        beyond_lod = g_AnimationLOD && !GFWorld3DDistanceSmallerThan(CMainGameComponent::Get()->GetWorld().GetGameLODCentre(), ParentThing.GetPos(), distance);
    }
    C3DAnimationBlendState state;
    if (anim.MainAnimSet.IsValid()) {
        if (!beyond_lod && !anim.TransitionAnimSets.empty()) {
            CIVCountedPointer<C3DAnimationStateBase> transition;
            if (anim.TransitionAnimSets.size() > 1) {
                CIVCountedPointer<C3DAnimationTransitionState> first(new C3DAnimationTransitionState);
                std::list<CAnimSet>::const_iterator it = anim.TransitionAnimSets.begin();
                CIVCountedPointer<C3DAnimationStateBase> start = it->GetMeshAnim(alpha);
                float total_alpha = it->GetInterpolatedAlpha(alpha);
                ++it;
                CIVCountedPointer<C3DAnimationStateBase> end = it->GetMeshAnim(alpha);
                total_alpha += it->GetInterpolatedAlpha(alpha);
                first->SetStartState(start);
                first->SetEndState(end);
                first->SetProgress(GetProgressFromAlpha(it->GetInterpolatedAlpha(alpha), total_alpha));
                std::vector<CIVCountedPointer<C3DAnimationTransitionState> > transitions;
                transitions.push_back(first);
                ++it;
                while (it != anim.TransitionAnimSets.end()) {
                    CIVCountedPointer<C3DAnimationTransitionState> next(new C3DAnimationTransitionState);
                    end = it->GetMeshAnim(alpha);
                    next->SetStartState(transitions.back());
                    next->SetEndState(end);
                    total_alpha += it->GetInterpolatedAlpha(alpha);
                    next->SetProgress(GetProgressFromAlpha(it->GetInterpolatedAlpha(alpha), total_alpha));
                    transitions.push_back(next);
                    ++it;
                }
                transition = transitions.back();
            } else {
                transition = anim.TransitionAnimSets.begin()->GetMeshAnim(alpha);
            }
            CIVCountedPointer<C3DAnimationTransitionState> main(new C3DAnimationTransitionState);
            main->SetStartState(transition);
            main->SetEndState(anim.MainAnimSet.GetMeshAnim(alpha));
            main->SetProgress(anim.MainAnimSet.GetInterpolatedAlpha(alpha));
            state.SetBaseAnimation(main);
        } else {
            state.SetBaseAnimation(anim.MainAnimSet.GetMeshAnim(alpha));
        }
    }
    if (g_DrawCombinationAnims) {
        static std::vector<CAnimSetIt> sorted_combination_anims;
        sorted_combination_anims.clear();
        sorted_combination_anims.reserve(anim.CombinationAnims.size() > delayed.CombinationAnims.size() ? anim.CombinationAnims.size() : delayed.CombinationAnims.size());
        for (CAnimSetIt it = anim.CombinationAnims.begin(); it != anim.CombinationAnims.end(); ++it)
            sorted_combination_anims.push_back(it);
        std::stable_sort(sorted_combination_anims.begin(), sorted_combination_anims.end(), CAnimSetItPriorityLessThan(alpha));
        for (const CAnimSetIt* set = sorted_combination_anims.begin(); set != sorted_combination_anims.end(); ++set) {
            if ((*set)->PeekAnimInfo((*set)->Anims[0]).IsLoaded()) {
                (*set)->PeekAnimInfo((*set)->Anims[0]).TouchAnimation();
                state.AddCombinationBlend((*set)->GetFirstMeshAnim(alpha));
            }
        }
        sorted_combination_anims.clear();
        for (CAnimSetIt it2 = delayed.CombinationAnims.begin(); it2 != delayed.CombinationAnims.end(); ++it2)
            sorted_combination_anims.push_back(it2);
        std::stable_sort(sorted_combination_anims.begin(), sorted_combination_anims.end(), CAnimSetItPriorityLessThan(alpha));
        for (const CAnimSetIt* set2 = sorted_combination_anims.begin(); set2 != sorted_combination_anims.end(); ++set2) {
            if ((*set2)->PeekAnimInfo((*set2)->Anims[0]).IsLoaded()) {
                (*set2)->PeekAnimInfo((*set2)->Anims[0]).TouchAnimation();
                state.AddCombinationBlend((*set2)->GetFirstMeshAnim(alpha));
            }
        }
    }
    return state;
}
