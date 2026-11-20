// CAnimateThing::Update(const CAnimateThingUpdate& update) -- advance the combination and delayed combination lists and the main anim set, fade the main alpha by ChangeAlphaBy, update or drop each transition set and share the main set's remaining alpha between the survivors by weight, then roll the delayed animation queue, preload, bump the refresh stamp and mark the graphic appearance changed
// Inline-budget: no flag, no pragma, no forceinline / noinline device. One body-visibility opt-in: FABLE_RETAIL_CANIMSET_ANIMATETHING_UPDATE gives CAnimSet::SetAlpha (declared by the class) its plain `inline` definition -- retail expands it twice here (0x0070E92C, 0x0070EB10) and keeps the out-of-line copy 0x0070BFC0; declaration only: DIFFER(793v881).
// Leaf: ?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070E8B0, 881 bytes (+ int3 padding), thiscall, `ret 4`; speed TU (frameless, `sub esp,20h`).
// Identity: Ego_r.pdb / FableWin.pdb ?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z (the shared header already names 0x0070E8B0);
//   the catalog label CAnimateThing::UpdateFrameLogic was descriptive. Ego_d.pdb: debug length 1493; parameter update; locals
//   trans_alpha_total, multiply_trans_alphas_by, main_alpha, ptc_graphic_appearance, trans_it (list<CAnimSet>::iterator), anim
//   (CAnimateThing::CCompleteAnimation&), trans_anims_update, required_trans_alpha_total, new_trans_alphas; block { trans_it
//   { trans_anim (CAnimSet&) { new_alpha } } }; block { c1 (long) { trans_anim } }.
// Shape (Ego_d.pdb C13 lines 220..292, egod_lines.tsv, VC10 /Od sizes; `this` is a disp32 slot, anim disp8):
//   - 221 (9 B) anim = CurrentAnim; 223 (22 B) the first list through anim (disp8 + imm8), 224 (28 B) DelayedAnim's list through this;
//     226 (12 B) anim.MainAnimSet.Update(update). anim as the CCompleteAnimation reference is what puts `this` in edi (as a CAnimSet&
//     to the main set, or without it: esi/edi swapped, 881v881 34 hunks).
//   - 229 (25 B) GFLimit(main_alpha, 0.0f, 1.0f) (the in-memory clamp), 230 (15 B) SetAlpha (the in-register clamp).
//   - 237 (38 B) one alpha test plus IsValid(); 239 (96 B) erase; 241 (5 B) the else jump; 243..254 the update arm; 250 (31 B) new_alpha,
//     251 (37 B) GFLimit, 253 (12 B) push_back, 254 (9 B) the total.
//   - 262..268 the multiplier if / else; 271 (19 B) trans_it = begin(); 272 (122 B) `for (long c1 = 0; ...; ++c1, ++trans_it)`;
//     276 (41 B) SetAlpha(multiply * new_trans_alphas[c1]) (a walking float iterator instead: esi/edi swapped again).
//   - 279 / 280 pop_front / push_back; 282 PreloadAnims(CurrentAnim); 286 / 288 ptc_graphic_appearance. 270 (226 B) and 287 (207 B)
//     emit nothing in retail (debug-only checks).
//   - Two reads are member reads where Ego_d's sizes suggest accessors: main_alpha = MainAnimSet.CurrentAlpha + ChangeAlphaBy
//     (GetAlpha(): `fld [+2Ch] ; fadd [+8Ch]`, retail loads +8Ch first) and new_alpha's ChangeAlphaBy (GetChangeAlphaBy(): DIFFER(884v881)).
// Callees: UpdateCombinationAnimList 0x0070D390, CAnimSet::Update 0x0070D030, UpdateInternalAlphas 0x0070C2C0, vector<float>::
//   _M_insert_overflow 0x004408A0, free 0x00BFEA14, list<CAnimSet> clear 0x0070B8A0, malloc 0x00BFEA0E, list<CAnimSet>::_M_insert_dispatch
//   0x007119C0, PreloadAnims 0x0070C590, NGraphicAppearance::CTCGraphicAppearance::SetAsChanged 0x004BF9E0. Data: DELTA_NOTIONAL_ZERO
//   0x0129BA3C; 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Overlay (add-only opt-ins): engine/CAnimSet.h FABLE_RETAIL_CANIMSET_ANIMATETHING_UPDATE (HasHadUpdate, the private Update /
//   UpdateInternalAlphas declarations, SetAlpha's inline definition); engine/CAnimateThing.h FABLE_RETAIL_CANIMATETHING_UPDATE_CALLEES
//   (the private UpdateCombinationAnimList / PreloadAnims declarations).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#define FABLE_RETAIL_CANIMATETHING_UPDATE_CALLEES
#define FABLE_RETAIL_CANIMSET_TYPE
#define FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24
#define FABLE_RETAIL_CANIMSET_ANIMATETHING_UPDATE
#include "engine/CAnimateThing.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/GFLimit.h"
#include "engine/GFIsNonZero.h"

void CAnimateThing::Update(const CAnimateThingUpdate& update)
{
    CCompleteAnimation& anim = CurrentAnim;
    UpdateCombinationAnimList(anim.CombinationAnims, update);
    UpdateCombinationAnimList(DelayedAnim.CombinationAnims, update);
    anim.MainAnimSet.Update(update);
    CAnimateThingUpdate trans_anims_update(update.TimePassed, update.DistanceMoved, anim.MainAnimSet.GetPercentageComplete());
    float main_alpha = anim.MainAnimSet.CurrentAlpha + ChangeAlphaBy;
    GFLimit(main_alpha, 0.0f, 1.0f);
    anim.MainAnimSet.SetAlpha(main_alpha);
    float trans_alpha_total = 0.0f;
    std::vector<float> new_trans_alphas;
    for (std::list<CAnimSet>::iterator trans_it = anim.TransitionAnimSets.begin(); trans_it != anim.TransitionAnimSets.end(); ) {
        CAnimSet& trans_anim = *trans_it;
        if (trans_anim.GetAlpha() <= DELTA_NOTIONAL_ZERO || !trans_anim.IsValid()) {
            trans_it = anim.TransitionAnimSets.erase(trans_it);
        } else {
            trans_anim.Update(trans_anims_update);
            if (!trans_anim.HasHadUpdate()) {
                trans_anim.UpdateInternalAlphas();
                trans_anim.SetAsUpdated(true);
            }
            float new_alpha = trans_anim.GetAlpha() - trans_anim.ChangeAlphaBy;
            GFLimit(new_alpha, 0.0f, 1.0f);
            new_trans_alphas.push_back(new_alpha);
            trans_alpha_total += new_alpha;
            ++trans_it;
        }
    }
    float required_trans_alpha_total = 1.0f - main_alpha;
    float multiply_trans_alphas_by;
    if (trans_alpha_total > DELTA_NOTIONAL_ZERO)
        multiply_trans_alphas_by = required_trans_alpha_total / trans_alpha_total;
    else
        multiply_trans_alphas_by = 1.0f;
    std::list<CAnimSet>::iterator trans_it = anim.TransitionAnimSets.begin();
    for (long c1 = 0; trans_it != anim.TransitionAnimSets.end(); ++c1, ++trans_it) {
        CAnimSet& trans_anim = *trans_it;
        trans_anim.SetAlpha(multiply_trans_alphas_by * new_trans_alphas[c1]);
    }
    DelayedAnimQueue.pop_front();
    DelayedAnimQueue.push_back(DelayedAnim);
    PreloadAnims(CurrentAnim);
    ++CurrentAnimRefreshStamp;
    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = ParentThing.GetTCGraphicAppearance();
    ptc_graphic_appearance->SetAsChanged(2);
}