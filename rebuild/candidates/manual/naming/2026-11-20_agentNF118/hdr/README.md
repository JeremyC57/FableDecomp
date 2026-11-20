# NF118 header overlay (FABLE_HDR_OVERLAY=<agent dir>/hdr)

Every change is an `#ifdef` opt-in with a member-specific macro name; with the macro undefined each header is token-identical to
its base copy in `../orig/` (the repo header at the start of this round).

## engine/CEnvironmentSpellEffectInterface.h (base: rebuild/include/engine/CEnvironmentSpellEffectInterface.h)
- `FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_FRAMEUPDATE`: public declaration `void FrameUpdate();` (Ego_r.pdb / FableWin.pdb
  ?FrameUpdate@CEnvironmentSpellEffectInterface@@QAEXXZ, retail 0x00885010, landed). Used by row 0x006B32A0
  CEnvironment::UpdateInterpolation (q4757). Placed after the RENDERFRAME block, before `private:`.
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT).
- Landed includers (direct or through CEnvironmentSpellEffectInterpolator.h / NScreenEffect_CScreenEffectPriorityStack.h): none
  defines the new macro, so the opt-in changes none of them (the default path is token-identical).
- NF115 / NF116 overlays do not carry this header.

## engine/CAIBrain.h (base: rebuild/include/engine/CAIBrain.h)
- `FABLE_RETAIL_CAIBRAIN_GETCURRENTSTATEGROUPSCRIPTPRIORITY`: public declaration `EScriptAIPriority GetCurrentStateGroupScriptPriority() const;`
  (Ego_r.pdb / FableWin.pdb ?GetCurrentStateGroupScriptPriority@CAIBrain@@QBE?AW4EScriptAIPriority@@XZ, retail 0x0088B760, landed), right
  after the INITCREATUREPOSITION block. Needs an EScriptAIPriority declaration first (CThingAICreature.h SCRIPTPRIORITY). Used by row
  0x006D5AD0 (q5047).
- pin_asserts.py: pinned 0 (the FABLE_STATIC_ASSERT_AT lines already carry pinned ids).
- Landed includers: 281 rows in rebuild/src/compiled include CAIBrain.h; none defines the new macro, so none changes (default path token-identical).

## engine/CTCSpecialAbilities.h (base: rebuild/include/engine/CTCSpecialAbilities.h)
Three opt-ins in the FABLE_RETAIL_CTCSPECIALABILITIES_FULL branch, right after the CreateParticleEffect line (public):
- `FABLE_RETAIL_CTCSPECIALABILITIES_ISCHARGINGUPABILITY`: `bool IsChargingUpAbility() const;` (Ego_r / FableWin
  ?IsChargingUpAbility@CTCSpecialAbilities@@QBE_NXZ, retail 0x0072A860). Rows 0x0072A860 (self), 0x0068F2A0, 0x00690030.
- `FABLE_RETAIL_CTCSPECIALABILITIES_GETCURRENTABILITYSTRAFEANIM`: `CCharString GetCurrentAbilityStrafeAnim() const;` (retail 0x0072B870).
  Row 0x00690030.
- `FABLE_RETAIL_CTCSPECIALABILITIES_GETCURRENTABILITYBEINGUSED`: inline `EHeroAbility GetCurrentAbilityBeingUsed() const { return CurrentAbility; }`
  (FableWin inline). Row 0x00690030.
- Left alone: FABLE_RETAIL_CTCSPECIALABILITIES_STRAFE_CALLS_PASS24 (non-FULL branch) still declares the out-of-line
  `GetAbilityMutexLock() const` for the pending pass24 draft 0x008E9B70; that draft's 0x0072A860 call should become IsChargingUpAbility
  (needs header claim; see NOTES).
- pin_asserts.py: pinned 0.
- Landed includers: 47 rows; none defines the new macros, so none changes (default path token-identical).
