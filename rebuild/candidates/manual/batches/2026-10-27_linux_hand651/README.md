# Chunk 651 -- 3 of 10 landed (2026-11-01, increased-attempts brief)

Cut 2026-10-31 from the fixable bucket of the re-ranked parked pool, authored 2026-11-01 by one Opus 5.5 agent under
`$S/w22/agent651/CHUNK_BRIEF.md` (owner instruction: at least 12 distinct source-shape probes per row before parking, levers in a
fixed order, probe logs per row in `probe/`). Every row had a prior best DIFFER (`prior/` in the agent dir, from `pending/*/differ/`).
Landed via `coordinator/land_chunk.sh 651` (vland --land 3/3, multi-check 0; the anonymous-namespace binding for 0x0070BF50 set to
the Ego_r hash `?A0x22a65229` per the 2026-11-01 policy and rechecked 3/3):
- 0x006817C0 `CTCCreatureOpinionOfHero::TransformOpinionFromHeroStats` MATCH (3 probes): the catalog's TransformForcedOpinion is the
  twin at 0x00681790 (Ego_r donor order, callers pass GetHeroStatsInOpinionFormat); the real CDefPointer IsNull test, non-null path first.
- 0x0070BF50 anonymous-namespace `IsAnimationStateBaseValid` RELOCATION_MATCH (16 probes): a named `const unsigned long index` local
  per GetType arm lets the identical arms cross-jump; engine/CIVCountedPointeeBase.h, C3DAnimationInfo.h, CMeshDataBank.h.
- 0x00446330 `CGamePlayerInterface::GetInputProcessEvent(CProcessedInput&) const` RELOCATION_MATCH with /GS (14 probes): the landed
  0x00A0D6B0 linked-list scan model (helpers re-read the stored node, list-side RemoveScanInfo, scan info declared after
  bestPriority/found); Ego_r names throughout incl. CInputEvent::SetAsNull 0x00A03BE0, CInputManager::FlagEventAsProcessed 0x009F55C0,
  CMainGameComponent::IsEditorActive 0x004197B0 (vtable slot 9).
Parked in `pending/2026-10-27_linux_hand651/differ/` (STATUS.tsv has residue + probe counts): 00d259a0 CDragon::OnPredicateFail 64v64
(one register, 17 probes + /Og- + QFE), 00e1cae0 MonitorMazePhases 564v568 (handle dtor inlined at 3 exits v 7), 007c7c40
CTCTurncoat::FrameUpdate 744v744 (def pointer esi v ebp), 00f39730 StartComposition 794v796 (0xF kept in ebx), 00918b90
CAIStateGroup_Gaze::Update 802v802 (test-less normal-path def release; was 806v802), 0078a690 CTCHeroRangedAttacks::FrameUpdate
1097v1097 (frame 0x18 v 0x14), 00490494 DrawPreparePrimitivesForEngine 1073v1094 (`drawn` kept in memory). Match rate 3/10 against
1-2/10 for chunks 643-650 at the usual effort; the parked residues are now mostly single-register / allocation ties.
14 NAME_FIXES queued (c651). Labels: ghidra_out/labels_chunk651_2026-10-27.tsv (10). Lessons folded into GOTCHAS (6 lines); review
items in LESSONS.md (landed 0x0094BB20 / 0x00B677D0 / 0x0070C770 debts, the two CLinkedListScanInfo models).
