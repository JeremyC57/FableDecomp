# NF191 header overlay (2026-11-20)

Every overlay header is the CURRENT repo header (base copy in ../orig/) plus `#ifdef` opt-ins only. Without the opt-in macros each
header's default path is token-identical to the repo header. pin_asserts.py was run after each change.

## engine/CFrontendGameComponent.h
- Base: rebuild/include/engine/CFrontendGameComponent.h (copy in orig/engine/).
- Opt-in `FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CLEARSTRING` (row 0x004944C0, q6283): declares the private
  `void ClearString()` (Ego_r.pdb 0x0049D0E0 + FableWin.pdb 0x01A46A80 ?ClearString@CFrontendGameComponent@@AAEXXZ) and splits the
  private `_body` into `_body_010[0x158]`, `char String[128]` (+0x168), `long StringPos` (+0x1E8), `_body_1ec` (Ego_r.pdb layout), same
  total size. The default path (`private: unsigned char _body[0x1e60 - 0x10]; };`) sits unchanged in the `#else` branch.
- pin_asserts: pinned 0 (the header has no FABLE_STATIC_ASSERT).
- Landed includers (none defines the macro, so none changes): 0x0042EC7C CNewFrontendGameComponent::Run, 0x004959E0
  CFrontendGameComponent::GetProfileNames, 0x00595A03 CFrontEndManager::CanStartAttractMode.

## engine/CThingCreatureBase.h
- Base: rebuild/include/engine/CThingCreatureBase.h (copy in orig/engine/).
- Opt-in `FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_FINISHCURRENTACTION` (row 0x00890C80, q11936), added right after the NF115
  `FLAT_GETPROPERTY` block in the flattened (default) class: the declaration `void FinishCurrentAction();` (FableWin.pdb / Ego_r.pdb
  0x0053D510 ?FinishCurrentAction@CThingCreatureBase@@QAEXXZ, retail 0x00662650), the same declaration the retail branch's
  `FINISHACTION` block has. Declaration only, no layout change.
- pin_asserts: pinned 0 (every assert in the header is already FABLE_STATIC_ASSERT_AT).
- Default path token-identical (the new block only). Landed includers: 601 rows under rebuild/src/compiled; none defines the new macro
  (grep), so none changes.

## engine/CTCBoss.h
- Base: rebuild/include/engine/CTCBoss.h (copy in orig/engine/).
- Opt-in `FABLE_RETAIL_CTCBOSS_INCREMENTSTRIKESSINCEHITHERO` (row 0x007AED30, q12153), after the `STRIKES` block:
  `void IncrementStrikesSinceHitHero();` (Ego_r.pdb 0x007B0680 + FableWin.pdb 0x026D1330). Declaration only, no layout change.
- pin_asserts: pinned 0 (the size assert is FABLE_STATIC_ASSERT_AT(85, ...)).
- Default path token-identical. Landed includers: 34 rows; none defines the new macro, so none changes.

## Checks
- A script strips each NF191 block (for CFrontendGameComponent.h: keeps the `#else` branch) and compares with orig/: identical for
  all three headers.
- Every FABLE_RETAIL_* macro my rows define resolves in rebuild/include or hdr/ (cpp precheck of every row).
- No other running agent (NF189 stlport/fable_map.h, NF190 CHeroSoulsCreatureDef.h / CV_BeardyBaldyScript.h, NF188's landscape
  headers) overlays any of these headers.
