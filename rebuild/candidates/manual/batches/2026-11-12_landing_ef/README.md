# 2026-11-12 landing ef -- chunk F2: six rows matched by one Fable rapid-fire agent

Source: `pending/2026-11-12_linux_handF2/`. Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x00406AE0 CUserProfileManager::GetManualSaveDisplayName, 0x009D5BF0 CBankFileAsync::HandleFailedAllocation, 0x00BE06B0
CEngineStaticMapFileBlockManager::AddRequest, 0x00909A00 CCombatSequence_WillForcePushLongRange::IsValid, 0x00B939E0
CEngineSceneGrid::CheckVisibilityResults, 0x00BCFB00 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer.

Verification before landing: a workflow of six independent Opus verifiers (evidence lens) passed all six with no issues
(`EVIDENCE_VERDICTS.md`); the coordinator ran the mechanical checks (reserved_check 0, purity, no local model of a header class,
14 overlay headers additions-only with hdrcheck2 default=same and no existing opt-in changed).
Retail-volatile note: 0x009D5BF0's test models `volatile bool Finished / AllowRestart`, as engine/CBankFileAsyncData.h does from the PDB.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011ef): 0x009D5A90 is CBankFileAsync::RestartAbortedReads (landed RestartReads); 0x00BE0810 is the implicit
std::pair<const long, CFileBlock> destructor (landed CUnprovenRecord_00be0810; layout evidence only); landed 0x009098E0 models
CTCAICreatureWillPowerIndicator::PoweredUp as CTCWillPower::Active; landed 0x00406610 GetManualSaveFileName is declared non-const
(the PDB says const).
