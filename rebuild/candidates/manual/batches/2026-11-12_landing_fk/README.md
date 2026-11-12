# 2026-11-12 landing fk -- author workflow wf18 (Opus only): nine of ten rows landed, one in repair

Source: `pending/2026-11-12_linux_handWF18/` (workflow `lane_tools/workflows/author_rows.js`, 66 agents, all Opus; no row needed
Fable). All ten rows byte-matched in about 145 minutes. Landed (all RELOCATION_MATCH, test PASS under VC7.1,
LANDVERIFY_STRICT_HEADERS=1): 0x008A7A70 CGameScriptInterface::TeleportAllFollowersToHeroPosition, 0x00724630
CTCPhysicsStandard::SetPhysicsMeshBankIndex (three author passes; flag /EHsc), 0x009CE5E0 CBankFile::AddEntryMultipleFiles,
0x00627400 CTCQuickAccessMenu::ConstructExpressionCategoryList, 0x007EB6A0 CTCActionUseSearch::OnActionUse, 0x00959F80
CAIStateGroup_HoverKeepInArea::Update, 0x0094F180 CAIStateGroup_BalverineCornered::IsRunnable, 0x00AC53F0
C3DMeshFileEngineMaterialChunk::C3DMeshFileEngineMaterialChunk, 0x007E6450 CTCOracleMinigame::UpdateTints (flag /GS).

**Not landed yet:** 0x00959BF0 CAIStateGroup_HoverWander::Update matches; three verifier findings (raw enum casts where PDB
enumerators exist, a missing override declaration, an inferred virtual placement not marked) are in a repair + two-verifier workflow.

**A wrong row caught by the landing verifier (0x007E6450):** the hand-back graded RELOCATION_MATCH in fg.py and passed both
verifiers, but vland rejected it with RELOC_JUMPTABLE: three of four jump-table entries pointed at the wrong case bodies. The
source had `case n: expression = n`; the retail table at 0x007E6734 maps input 0 -> 0, 1 -> 3, 2 -> 1, 3 -> 2. The switch is now
written in the retail layout order (`case 0: 0; case 2: 1; case 3: 2; case 1: 3`), the test (which had copied the wrong mapping)
is corrected, and the row landed on a second run (both runs are in `logs/land.log`). fg.py normalises jump targets and does not
read jump tables; the workflow's mechanical verifier now runs the landing verifier as a dry run for that reason.
**Coordinator fixes at landing (regraded: match, test PASS):** 0x009CE5E0: the inline CBankCreatorException(const CCharString&)
body now cites where retail expands it (0x009CE62B..0x009CE63E); its NAME_FIXES row about CreateCRC is rewritten as a member fix
inside the landed Equals row, not a rename of 0x009CC750. 0x00959F80: stale C3DVector.h overlay rebased by 3-way merge.
Two landed header comments had `thiscall` rewritten to `fastcall` by the lander's VC7.1 keyword substitution; restored.
**Mechanical checks:** reserved_check 0 (0x00A358F0, 0x00626750, 0x00626E40, 0x0099BA70 are cited or declared only), purity
clean, no local model of a header class in a landed row. Pragmas: `intrinsic(...)`; `auto_inline(off)` around a real same-TU
helper body in 0x00AC53F0.
**Headers** (23 + new engine/CCircle.h; additions under new opt-ins, unused elsewhere, defaults unchanged per hdrcheck2):
CTCSummonSpell.h, CMeshDataBank.h, CTCPhysicsStandard.h, CTCPhysicsBase.h, NGraphicAppearance_CTCGraphicAppearance.h (two rows:
FULLSCALE, REMOVETINT), CPhysicsMesh.h, CBankFile.h, CBankFileEntryUpdateData.h, CTCQuickAccessMenu.h, CTCContainerRewardHero.h,
CPhysicsMeshObject.h, CTCActionUseBase.h, CAIStateGroup_HoverKeepInArea.h, CAIStateGroup_HoverMovementBase.h,
CAIStateGroup_EnemyCreatureBase.h, C3DVector.h (CROSSWITHUP), NEntityEvents_CEventHitBase.h, CIntelligentPointer.h,
CAIStateGroup_BalverineCornered.h, C3DMeshFileEngineMaterialChunk.h, C3DMeshFileMaterialExtChunk.h, C3DMeshFileMaterialChunk.h,
C3DMeshFileMapChunk.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fk): the rows' name fixes (0x004BBFF0, 0x0071A730, 0x009CC750 member fix, 0x0049B760, 0x009D47C0, 0x009D3DF0,
0x00464931, 0x00747030, 0x00812340, 0x0074E500, 0x00ABFDC0, 0x00810F40, 0x008645E0).
