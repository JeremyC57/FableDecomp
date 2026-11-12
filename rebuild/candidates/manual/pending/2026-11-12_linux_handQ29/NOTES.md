# chunkQ29 notes

Matched (RELOCATION_MATCH, strict headers, tests PASS under VC7.1 and g++):
- 0x007CC710 CTCMultiArrow::ReleaseShot -- port onto engine/CTCMultiArrow.h (VSLOTS2 for FinalCleanup, VSLOTS3 for the members,
  overlay RELEASESHOT) + std::vector + CThing.h; the first port grade was already 180v180 (only ebx/edi swapped); the
  CIntelligentPointer COPY_ORIGINAL_CHAIN + GET_ORIGINAL_CHAIN spellings fixed the swap.
- 0x009D6350 CBankFileAsyncData::Abort -- port onto CBankFileAsyncData.h / CBankFileAsync.h (CBANKFILEASYNC_DATA), CCriticalSectionLock.h,
  CMovableResourceMemoryPool.h (+FREERESOURCE overlay), fable_map.h TREE_ERASE + TREE_ERASE_KEY (set::erase(key) out of line
  0x009D7F40). Callees renamed: 0x0098EC90 = CThreadedFile::RemoveObject (was "HasReadFinished" in the landed dtor), 0x00A62ED0 =
  CMovableResourceMemoryPool::FreeResource. Local `class CThreadedFile` (no shared header exists; one member).
  Shape: `result = true` in both arms, then one `if (result)` block (a single `result = true` stored bl into Aborted).

In progress (left at DIFFER under the one-row rule's budget clause):
- 0x00EB3BA0 -- identity fixed: NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero (only caller 0x00EB23B2 in
  CMurderTwistSheriffGuard::Main; index at +0x30 = guard ID in Ego_r; PDB order after DoSayingThankYouToHero 0x00EB3740). Ported onto
  CV_MurderTwistFinalScript.h (+CGameScriptInterfaceBase.h, CENTITYSCRIPTBASE_ORIGINAL_MEMBERS: Game is a reference in both PDBs).
  186v187: only the index temp's register at the tail. 42 grades + 5 probes; the full list of ideas tried is on line 3 of
  differ/00eb3ba0.cpp. Lead: writing `seh_Guards[ID]` twice and giving c2 IsValid's real body (noinline) produces retail's exact
  `mov edx,[esi+30h] ; mov eax,[ecx+7Ch] ; shl edx,4 ; lea r,[eax+edx]` (CSE across the call) but also lets c2 keep ecx across
  IsValid (179v187); retail evidently had the mod/ref but not the register summary.
- The landed CMurderTwistSheriff::AttackHero 0x00EB1EC0 still uses local CV_MurderTwistFinalScript / handle models (strict mode
  would reject); the shared-header port is shown in differ/00eb3ba0.cpp.

Untouched (not opened under the one-row-at-a-time rule): 0x00BD2A90, 0x00958B20, 0x00465C54, 0x00722740, 0x009FC950.
