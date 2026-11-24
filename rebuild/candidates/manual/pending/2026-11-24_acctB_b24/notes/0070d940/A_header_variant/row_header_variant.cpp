// CAnimateThing::CalcInterpolatedMeshAnim(const CCompleteAnimation& anim, const CDelayedAnimation& delayed, float alpha) const -- build the blend state of a complete animation at an interpolation alpha: the main set, cross-faded from its transition sets (a chain of transition states weighted by their interpolated alphas, skipped beyond the animation LOD distance), plus every loaded combination anim of the current and the delayed animation, each list stable-sorted by priority
// Grade: RELOCATION_MATCH at stock flags (B24C10; raw diff 0, every emitted COMDAT copy EQ) -- ONLY with the Inferred operand below: ESCALATED, not an author hand-back
// Forms (B24C9, NOTES.md): `GetMeshBank().PeekAnimation(Anims[0])` call sites (object chain evaluated before the argument: retail's
//   eax/ecx/edx rotation, c2prio --rotation); the reserve as an if/else of two calls (c2 tail-merges them; a ternary argument pushes first).
// Owner decision (2026-11-24): "yes use it and mark inferred" -- the always-true operand below lands as an Inferred construct.
// Inferred: a compile-time-true second operand on the combination-anims test (`NGlobalConsole::EnableCombinationAnimations && true`;
//   emits no bytes). Required by retail's inline split: 0x0070DF89 CALLS vector<CAnimSetIt>::_M_insert_overflow 0x00711080 in the
//   first push loop while the second loop (0x0070E1xx) expands it. VC7.1 decides per site from c2's static frequency (score
//   (c/f+6c)/7 < 74, c = 73): the first site needs f <= 0.91, i.e. the combination block at <= 0.365; a plain `if` gives 0.5 (score 68,
//   expanded), an `&&` 0.25 (score 79, called; c2 trace inl/a1.txt). Window (a1 trace): block f in (0.173, 0.365] -- one `&&` level
//   exactly (graded: `&& true && true` DIFFER(2379v2395)). Ruled out: loop shapes (B24C7), an empty() guard (+4
//   instructions, B24C9), an early return (c10/b1: 72 hunks), compile order (inl/f150: forced budget path rejects both sites),
//   /d2inlT67 (breaks 0x00711A50), `#pragma inline_depth(1|2)` around the first push loop (no effect). A nested `if (true)` block
//   also matches (c10/n1g, n2g) but renumbers the local static (?BJ@ / ?BK@ v ?BH@ = 23, the number in Ego_r's atexit name -- a
//   different build, so a hint, not proof); the `&&` operand keeps 23.
//   The leading form `true && NGlobalConsole::EnableCombinationAnimations` matches as well (c10/esc2.cpp). The original operand's
//   wording is unknown (a release-constant build switch is the likeliest); see ESCALATE.md.
// Leaf: ?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z
// Inline-split (B24C10): no flag. The first combination pass's overflow site scores 68 at f 1.25 (cost 73 = c1 52 + ext 21; c2
//   trace inl/v18.txt decision 294) and needs f <= 0.91 to be called; the second pass's (f 31.25) is expanded like retail. Retail's
//   combination block must therefore be colder than this text's 0.5 (an `&&` operand that compiles out gives 0.25 and matches:
//   ESCALATE.md). /d2inlT67 is wrong for this TU (it calls the 0x00711A50 copy's inner __uninitialized_fill_n, which retail expands);
//   compile order is ruled out (inl/f150.txt: the forced budget path rejects both sites).
//   FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_FORCEINLINE (existing repo opt-in) with the converting operator= forced under it
//   (CONVERT_ASSIGN): retail expands `transition = transitions.back()` (score 144) and calls ~vector (113) in the same block;
//   without either opt-in the row grades worse (B24C9 c9/d1..d3: 797 / 778 / 795 instructions v 785).
// Pick-any: FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL (the /Gr TU's __destroy_aux: the ~vector<CIVCountedPointer<TS> > copy = 0x00711760)
//   and FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26 with rebuild_abi.h / fable_compiler.h ahead of the STLport header (the
//   vector<CIVCountedPointer<TS> >::_M_insert_overflow copy = 0x00711A50); both existing repo opt-ins. c9/pk.py: every copy EQ; the
//   14-byte ~vector<CAnimSetIt> copy has no own retail address (the _$E2 thunk inlines it) and equals retail's folded POD-vector dtors.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070D940, 2395 bytes, thiscall `ret 10h` (hidden result + 3 arguments), frame 0x3C, no EH.
//   Ego_r.pdb ?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z
//   (private const; Ego_r neighbours CalcMeshAnims / GetProgressFromAlpha); the function-static sorted_combination_anims 0x013BAC20
//   (guard 0x013BAC2C, atexit thunk _$E2 0x01229630) is named in both PDBs.
// Callees: CThing::GetPos 0x004C73D0, CWorld::GetGameLODCentre (slot 8), GFWorld3DDistanceSmallerThan 0x006614F0, CBaseClass ctor
//   0x0099A2D0 / dtor 0x0099A2E0, operator new 0x00BFEA1A (3 inline C3DAnimationTransitionState constructions, IVRefCount 1),
//   CAnimSet::GetMeshAnim 0x0070C900 (x6), C3DAnimationTransitionState::SetStartState 0x00710560 / SetEndState 0x007105A0 (x2 each),
//   GetProgressFromAlpha 0x0070BE40, C3DAnimationBlendState::SetBaseAnimation 0x00A3C2D0 (x2) / AddCombinationBlend 0x00A3C310 (x2),
//   vector<..TransitionState..>::push_back 0x00711C50 / overflow 0x00711A50 / ~vector 0x00711760, vector<CAnimSetIt>::reserve
//   0x00710B00 / _M_insert_overflow 0x00711080, STLport __stable_sort_aux 0x00711B80 (__fastcall, empty comparator),
//   CAnimateThing::GetMeshBank (inline, out of line 0x0070C0D0), CMeshDataBank::PeekAnimation 0x00A242C0 (x4), C3DAnimationInfo::IsLoaded 0x00A23F50 / TouchAnimation 0x00A26A50 (x2 each),
//   CAnimSet::GetInterpolatedTime 0x0070C800 (x2), malloc / free / atexit.
// Globals: g_primitiveDrawDistanceScale 0x013B8774, the bools 0x01380BDA (animation LOD; name descriptive) and NGlobalConsole::EnableCombinationAnimations 0x013756F7,
//   CMainGameComponent::PMainGameComponent 0x013B86A0, DELTA_NOTIONAL_ZERO 0x0129BA3C, 1.0f / 24.0f literals.
// Headers: engine/CAnimateThing.h (CALCCACHEDANIMS, PROGRESSFROMALPHA), CAnimSet.h (MESHANIM), C3DAnimationTransitionState.h
//   (SETTERS), C3DAnimationStateAlpha.h (INDEX_CTOR), C3DAnimationBlendState.h (ADDCOMBINATIONBLEND), CAnimateThing_GetMeshBank.h
//   (GETMESHBANK); overlay $W/hdr (README.md).
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
#define FABLE_RETAIL_C3DANIMATIONSTATE_ORIGINAL_CTORS_PASS23
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
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
#define FABLE_RETAIL_CANIMSET_MESHANIM
#define FABLE_RETAIL_CANIMSET_GETMESHANIM_INLINE_BODY
#define FABLE_RETAIL_CANIMATETHING_CALCCACHEDANIMS
#define FABLE_RETAIL_CANIMATETHING_PROGRESSFROMALPHA
#define FABLE_RETAIL_CANIMATETHING_GETMESHBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_UNINIT_COPY_FALSE_FASTCALL_PASS26
#define FABLE_RETAIL_STLP_STABLE_SORT_AUX_DECL
#define FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER_DECL
#define FABLE_RETAIL_CANIMSETITPRIORITYLESSTHAN
#include "rebuild_abi.h"
#include "fable_compiler.h"
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
#include "engine/CAnimateThing_GetMeshBank.h"

extern float g_primitiveDrawDistanceScale;                          // 0x013B8774
extern bool g_AnimationLOD;                                         // 0x01380BDA (name descriptive)
namespace NGlobalConsole { extern bool EnableCombinationAnimations; } // 0x013756F7 (name from ONE source: Ego_d.pdb publics
                                                                    //   ?EnableCombinationAnimations@NGlobalConsole@@3_NA, no address; placed by its neighbours 0x013756F4 / F5,
                                                                    //   bound NGlobalConsole::DrawProximityFadeOutMeshes / CombatParticleEffects -- an open doubt, see NOTES)
typedef std::list<CAnimSet>::const_iterator CAnimSetIt;

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
    if (NGlobalConsole::EnableCombinationAnimations && true) {
        static std::vector<CAnimSetIt> sorted_combination_anims;
        sorted_combination_anims.clear();
        if (anim.CombinationAnims.size() > delayed.CombinationAnims.size())
            sorted_combination_anims.reserve(anim.CombinationAnims.size());
        else
            sorted_combination_anims.reserve(delayed.CombinationAnims.size());
        for (CAnimSetIt it = anim.CombinationAnims.begin(); it != anim.CombinationAnims.end(); ++it)
            sorted_combination_anims.push_back(it);
        std::stable_sort(sorted_combination_anims.begin(), sorted_combination_anims.end(), CAnimSetItPriorityLessThan());
        for (const CAnimSetIt* set = sorted_combination_anims.begin(); set != sorted_combination_anims.end(); ++set) {
            const CAnimSet& combination = **set;
            if (GetMeshBank().PeekAnimation(combination.Anims[0])->IsLoaded()) {
                GetMeshBank().PeekAnimation(combination.Anims[0])->TouchAnimation();
                state.AddCombinationBlend(combination.GetFirstMeshAnim(alpha));
            }
        }
        sorted_combination_anims.clear();
        for (CAnimSetIt it2 = delayed.CombinationAnims.begin(); it2 != delayed.CombinationAnims.end(); ++it2)
            sorted_combination_anims.push_back(it2);
        std::stable_sort(sorted_combination_anims.begin(), sorted_combination_anims.end(), CAnimSetItPriorityLessThan());
        for (const CAnimSetIt* set2 = sorted_combination_anims.begin(); set2 != sorted_combination_anims.end(); ++set2) {
            const CAnimSet& combination = **set2;
            if (GetMeshBank().PeekAnimation(combination.Anims[0])->IsLoaded()) {
                GetMeshBank().PeekAnimation(combination.Anims[0])->TouchAnimation();
                state.AddCombinationBlend(combination.GetFirstMeshAnim(alpha));
            }
        }
    }
    return state;
}
