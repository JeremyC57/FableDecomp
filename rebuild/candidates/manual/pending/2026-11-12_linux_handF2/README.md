# Chunk F2 (Fable 5.2, rapid fire): six parked rows

6 of 6 matched, in 69 grades and about 64 tool calls, and landed in landing ef. Each was checked by an independent evidence verifier
(EVIDENCE_VERDICTS.md: all six ok -- name proven from two sources, callees and members checked against the disassembly, the test can
fail) and by the coordinator's mechanical checks.
- 0x00406AE0 CUserProfileManager::GetManualSaveDisplayName: the symbol is an implicit CCharString temporary, not a named local
  (a named local gave 344v314); the inline GFIntToWideString body sits in the TU.
- 0x009D5BF0 CBankFileAsync::HandleFailedAllocation: std::set with the STLport insert bodies visible; real FreeBuffer body inline.
- 0x00BE06B0 CEngineStaticMapFileBlockManager::AddRequest: `CFileBlock fileBlock = { 1, lowPriority, data };`.
- 0x00909A00 CCombatSequence_WillForcePushLongRange::IsValid: two separate `return true` statements keep the dead compare alive.
- 0x00B939E0 CEngineSceneGrid::CheckVisibilityResults: both cell lookups through the PDB inline GetCell(x, y). First hand-back had a
  local CBaseClassNonCopyable stub; sent back and fixed onto the shared header.
- 0x00BCFB00 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer: PDB inline accessors; the draw's counts read
  into locals in retail's order.
