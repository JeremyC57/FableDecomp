# Landing nf173 (Account A, 2026-11-20): mixed engine / TC names (36 rows)

Agent NF173 (round 10) handed back 44 entries: 21 done, 1 done-already, 7 blocked, 3 deferred, 12 open. Its files are in
`naming/2026-11-20_agentNF173/`. The 12 open entries, q5089 (its NF170 lock is gone) and q6927 go back to the queue unclaimed.

- 36 rows (+ the EH funclet 0x01211DD3, whose source is unchanged and includes its parent 0x00C16E10 by path; the agent's
  recheck_final graded it RELOCATION_MATCH on the new parent, and the funclet include check found 0 broken of 1355).
- Self renames (15): CNavigatorManager::AddNavigationMap 0x00A157C0, CNavQuadTree::GetSmallestNodeSize 0x00A76320, CParticleProgram::
  SetupSimulation 0x00AE0050, ReadBankInfoCount 0x00C16E10, CSegmentFile::Read 0x00C2DA00, CTCWeapon::IsProjectileWeaponAutoLoading
  0x005DA950, CTCLook::FrameUpdate / SetHeadTurnState 0x00753C70 / 0x007517B0, CCountedPointer<CParticleEmitter>::DeleteData 0x00AE91D0,
  the NParticleEngine emitter list / manager trio 0x00AEAF20 / 0x00AEAF80 / 0x00AEB6D0, and CMemoryAllocator::OnFailedAllocation /
  InternalValidateArea / SaveStackMemoryTrackerLog 0x009F1F00 / 0x009F1F40 / 0x009F2080. Nine files moved to their new names.
- Header: engine/CNavigatorManager.h (`orig` equals HEAD) gets FABLE_RETAIL_CNAVIGATORMANAGER_ADDNAVIGATIONMAP_PROCESSREF nested in
  LEVELMAPS, a pointer -> reference signature variant; 0x00A157C0 and 0x00500230 land together on it. Removing the now unused pointer
  form is queued as a header claim.
- In-place respells inside local models the rows had at HEAD (token check clean, objdump: no other symbol names a moved class); no casts
  of stand-ins to real types; no new local model of a class with a shared header.
- Recheck (`logs/recheck.log`): 6 MATCH + 30 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); 0x006818F0 is a
  multi-block TU whose relocations vland cannot pair (RELOC_UNCHECKED, as at HEAD); its respelled callee matches the self row
  0x00701840's Leaf. Split gate 0 created. Includers 173 good, 0 bad. Owner recheck 15 of 15 good.
- tablecheck: nine addresses shrink (0x007446A0 4 -> 1, 0x00A76320 3 -> 1, 0x00AED090 2 -> 1 ...). 0x00446B10 grows 1 -> 2 from
  0x0068EB20's unchanged text (a binding missing from the HEAD table, first recorded here): queued.
- Link (`logs/link_landing.log`): 4 LNK2005 vs 3 at HEAD (`logs/link_control_head.log`; ConvertX / ConvertY and SetHeadTurnState, the
  last respelled QAE -> AAE on both sides). The extra one, ??0CAnimInit, comes from 0x007502D0's pick-any inline ctor against the
  self row 0x005B33F4's NODUPLICATES copy. Landed 0x008EACB0 does the same at HEAD (`logs/link_control_head_animinit.log`: the HEAD
  object against 0x005B33F4 gives the same LNK2005), so it is queued as DEBT, not held.
- Coordinator calls:
  - q6927 (0x009D56C0): the in-place respell grades RELOCATION_MATCH, but its object would define pick-any NBankFileManager COMDATs that
    landed 0x009D06F0 / 0x009A9D60 also define. In-place rule (2) says hold, so it stays at HEAD; the draft is in `naming/.../work/`
    and the entry goes back to the queue as a port onto the shared header.
  - q5111 (0x00C2DA10) waits for 0x00C3AA40's real name (its fields are CBufferedRangeReader's in libpk/CSegmentFile.h): queued.
  - 0x00AE0050's `// Inferred: ... ? true : false` stands (retail normalises the bool with `test al,al ; setne al`; the plain return
    measured DIFFER(21v26)).

## nf173b follow-up (2026-11-20): 0x007502D0 reverted
The advisor's pick-any rule (EXTRA_RULES 2026-11-20) found that 0x007502D0's in-place Set -> Init respell made its object emit the
retail-named `??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z` with a 59-byte body, against the self row 0x005B33F4's 49 bytes (before
nf173 the copy carried a non-retail `long` spelling). The row is back at its pre-nf173 source: RELOCATION_MATCH PASS
(`logs/nf173b_revert_007502d0_recheck.log`), binding table unchanged. q6391 / q6462 go back to the queue (39 of 40 referencers bind
Init; the respell needs the ctor declaration-only or a shared CAnimInit header).
