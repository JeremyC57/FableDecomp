# 2026-11-13 landing fp -- author workflow wf19 (Opus only): nine of ten rows landed, one not matched

Source: `pending/2026-11-12_linux_handWF19/` (workflow `lane_tools/workflows/author_rows.js`, 64 agents, all Opus). The run was
cut by a container restart with nine rows matched and was resumed from its journal (finished agents replayed from cache).
Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x006B3FF0 CEnvironment::Update, 0x00727D00
CTCRandomAppearanceMorph::OnAppearanceDraw, 0x007BF790 NThingShot::CTCShotDefinitionBase::Init, 0x006BFCD0
CTCBuyableHouse::GetSignPostPos (three author passes), 0x0055D180 CTCPhotojournalCapture::TakeQuickScreenshot (flag /GS),
0x00A8F9E0 C3DPrimitive2::UploadToIndexBuffer, 0x00B206B0
NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::OffsetSplinePositions, 0x007D46D0
CTCTavernGameCoinGolf::FadeOutTableOccupants, 0x0094A3B0 CAIStateGroup_GoToPub::ChooseAction (two passes).

**Not matched:** 0x00D17A80 NScript::CQ_BountyHuntScript::WatchForBoasts, DIFFER(705v785, 4 hunks) after three Opus author passes
(75 grades in the last). The body matches byte for byte; only the destruction of the boast vector at the five early returns
differs: retail inlines each site to a different depth (the inline budget follows a postorder walk of the flow graph), VC7.1
here inlines every site fully and merges the tails. Best source and residue: `pending/2026-11-12_linux_handWF19/differ/`.
This is the fourth row Opus could not match. It is NOT sent to a Fable pass yet: the owner asked (2026-11-13) that the next two
author workflows to finish are not relaunched, so the row waits for a decision.

**Coordinator fixes at landing (rows regraded: match, test PASS):**
- 0x00727D00: the local model nested CMeshMorphSetParts directly in CTCAppearanceMorphBase and typed TextureMorphs as a vector.
  Ego_r nests it in CMeshMorphSet and types it CEngineTextureMorphSet { CArray<CEngineTextureMorph> Morphs }, as the landed sibling
  0x00727C20 models it; respelled (same `lea edx,[eax+4]`).
- 0x007D46D0: CThingFilter_IsHuman::operator() takes `const CThingCreatureBase&` as both PDBs spell it (was `const CThing&`); the
  landed 0x006A7B90 / 0x007D4CE0 spellings are handed back as name fixes.
- 0x006B3FF0: the missing name-fix line for 0x006B5D80 (~CBlendedEnvironmentTheme, both PDBs) added.
**Mechanical checks:** reserved_check 0 (0x006B42F0, 0x0076EFA0, 0x00A22F80 are cited in comments only), purity clean, no local
model of a header class. `#pragma pack(1)` in 0x007D46D0 is on CTCTavernGameCoinBase, which has no shared header.
**Headers** (16; additions under new opt-ins, unused elsewhere, defaults unchanged per hdrcheck2; two re-headed `#if` lines in
CUserProfileManager.h and CDateAndTime.h keep their existing paths token-identical): CEnvironment.h,
CEnvironmentSpellEffectInterpolator.h, CGameDefinitionManager.h, NGraphicAppearance_CTCGraphicAppearance.h (two rows: RANDOMMORPH,
SETFORCERESEND), CTCWeaponTrail.h, CTCObjectAugmentations.h, CTCBuyableHouse.h, CPlayerGui.h, CUserProfileManager.h, CSurface.h
(GETD3DSURFACE), CDateAndTime.h, CAFile.h, C3DPrimitive2.h, CCardinalSpline.h, CThingSearchTools.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fp): the rows' name fixes (0x006B2BD0, 0x006B5D80, 0x006A7B90, 0x006B2CA0, 0x004379E0 and the others in
NAME_FIXES.tsv).
