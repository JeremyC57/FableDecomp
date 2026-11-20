# Landing nf166 (Account A, 2026-11-20): game component / world / animation data names (32 rows; 0x00A99610 held)

Agent NF166 (round 10) handed back 31 entries (16 done, 10 done-already, 3 blocked, 2 set aside for Account B's lock on 0x009FDE80,
returned to the queue); its files are in `naming/2026-11-20_agentNF166/`.

- 32 rows land. ConstantFPS: 14 stand-in rows moved onto the PDB spelling that 416 rows already bound. Tests of 0x0066FAB0 and
  0x00697710 were rewritten after a LINK_FAIL, keeping their PassPatterns. 0x0066FAB0's test adds a test-only
  `CConstantFPSTestAccess : CMainGameComponent` to set the protected static.
- Self renames (8): CFrontendGameComponent::GetMaxVisibleProfileNumber / WorldNumber / SaveNumber, CGameTimeManager::SetTimeOfDay
  0x006BB810 (was CThingDrawInformation::SetBarPercentage; two PDBs plus two callers; the file moves), CGameScriptInterface::
  FailAllActiveQuests / SetQuestAsFailed, C3DAnimationSequence::ReadCompressedFromFile, CParticleProgram::Update 0x00AE34D0.
  The last was CSortedMapOwner_00ae34d0::SortAndProcess. Its body calls a private CParticleProgram member, the other no-argument
  members are ruled out, both PDBs hold Update, and FableWin's span is 0x40 as retail. Coordinator: accepted on that evidence.
- Headers (add-only declaration opt-ins, `orig` equals HEAD): CParticleProgram.h (UPDATE), CQuestManager.h (FAILALLACTIVEQUESTS),
  CGameTimeManager.h (SETTIMEOFDAY).
- Casts disclosed by the agent, all present at HEAD and now feeding PDB spellings: 0x00B82100 (BlendState bytes ->
  C3DAnimationBlendState*, this -> CParticleContext*), 0x0088FDC0 (a stand-in field retyped CGameTimeManager*), 0x004163B7 (the
  local CMainGameComponent model's display pointer retyped CDisplayEngine*), 0x00892FD0 (a const-dropping cast kept for the
  0x006E71F0 call). None is new; their ports belong with the shared-header waves.
- Held: 0x00A99610 (q4207). Respelled, its same-TU strong copy defines FindFirstObject, which the landed owner 0x00A99110 also
  defines strongly: a new LNK2005 (`logs/link_landing_with_00a99610.log`), not at HEAD (`logs/link_control_head.log`). The row
  is restored from HEAD and regraded (RELOCATION_MATCH, `logs/recheck_00a99610_restored.log`). Its bindings equal HEAD's.
  The respelled row is kept in `naming/2026-11-20_agentNF166/held/`.
- Recheck (`logs/recheck.log`, all 33 before the hold): 3 MATCH + 30 RELOCATION_MATCH, every test PASS. rebind multi-check 1
  (baseline); split gate 0 created; includers 238 good, 0 bad. tablecheck: no address gains a spelling; 12 shrink (0x01375550
  12 -> 3; 0x00A99110 goes back to its HEAD count after the hold). The leaf warning on 0x00417001 (Leaf line MAE, object QAE)
  is pre-existing and queued in NAME_FIXES.
- Link (`logs/link_landing.log`, the landed objects with 0x00A99610 at HEAD): 1 LNK2005, ??0CGameEvent, the same at HEAD.
- Queued: the 0x00A99610 respell after its copy goes pick-any, the four ConstantFPS rows, 0x0065A5BB's regrade, and a note that
  the July rows behind q4940 / q11948 may move without Account B's row.
