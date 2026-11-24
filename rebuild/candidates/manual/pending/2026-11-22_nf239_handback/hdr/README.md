# NF239 header overlay (2026-11-22)

Two NEW headers, add-only (no existing header line changes; no orig/ base needed, nothing in the repo tree includes them):

- `engine/NPlayerGui_CDrawProjectileTargetingCentre.h`: `NPlayerGui::CDrawProjectileTargetingCentre : public CDrawBase`, no own
  members (Ego_d.pdb TPI 0x3E197, field list 0x3E196: one base, no LF_MEMBER). Used by 0x00437640 CPlayerGui::IsDrawingTargetingCentre.
- `engine/NPlayerGui_CDrawMeleeActionButtonEffects.h`: `NPlayerGui::CDrawMeleeActionButtonEffects : public CDrawBase` with the Ego_d
  member list (TPI 0x3DF4F, field list 0x3DF4E: Effects / FlashTimer / ComboLoadingIconTimer / FlourishCounter / FlourishEffect /
  PMeleeAbilityDef, private) at retail offsets +0x28 .. +0x44 and the inline `SetFlourishEffect(long)` (FableWin public
  ?SetFlourishEffect@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXJ@Z; retail and Ego_r have no out-of-line copy). PMeleeAbilityDef
  is declared only under FABLE_RETAIL_DEF_CLASS_BASE (engine/CDefPointer.h #errors without it). Used by 0x00437650.

Equivalence: no existing header changed, so no includer's preprocessed text changes (`grep -rl` of both new names under
rebuild/src/compiled and rebuild/include: no hits before this hand-back). No FABLE_STATIC_ASSERT added or moved (pin_asserts not needed).
The coordinator copies both files to rebuild/include/engine/ at landing.
