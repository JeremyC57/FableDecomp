# 2026-11-17 landing nf82: name-fix round 7, NF82 (63 rows: 43 of the 45 remaining operator< binders and the CCharString tree rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF82 took the 20 entries queued at the nf80 landing (q10989..q11008) and the 45 landed rows that still called
`CBasicString<char>::operator<` (0x00429950) under a stand-in spelling. Brief, list and records: `naming/2026-11-17_agentNF82/`.

## Rows (STATUS.tsv, 78 = 77 locked + 0x004BB270, claimed through the lock): 63 ported, 9 not-a-binder, 6 blocked
- **operator< binders (q11007): 43 of 45 ported** onto fable_string.h; each now calls 0x00429950 as `??M?$CBasicString@D@@QBE_NABV0@@Z`.
  The two left are 0x009D2BA0 (the map's value type has no second source) and 0x00A2AC30 (`CMeshDataBank::CLipSyncEntry` is in no header).
  Most were STLport sort / heap / tree helpers; they now emit the STLport templates through new opt-ins in stlport/fable_containers.h.
- **Element types corrected** (retail has no identical-code folding, so each address is one instance): the 0x009B47F0..0x009B5C70 family
  is `pair<CCharString, unsigned long>` (CDefinitionManager::CheckDependencies' sort; the old 0x009B5800 row said `long`); 0x00A403F0 /
  0x00A408E0 are `pair<CCharString, CSoundBankLH*>` (retail RTTI name; reached from CSoundManagerLH::NewAtmos). The quest-persistence setters
  0x004B4E30 / 0x004B4F30 are Insert then Sort (both members in FableWin.pdb).
- **CCharString tree rows:** `_M_insert` / `_M_lower_bound` / `_M_upper_bound` / `equal_range` / `erase` of a dozen `map<CCharString, ...>` and
  `set<CCharString>` instances, most of them catalogued as Dinkumware `std::_Tree<...,float,...>::_Lbound` copies before.
- **q11008:** 0x006C37D0 `CSkeletalMorphResourceManager::PreloadBoneConfigs` is on fable_string.h's ToCharString body (NF80's in-place draft
  dropped). 0x0049124E is blocked (about a dozen local models).
- `NParticleEngine::CRegisteredPSCInfo` has no shared header; rows 0x00AF2F10 / 0x00AF3290 declare it with its Ego_r.pdb members.
- **Blocked:** 0x0049124E, 0x004DBF08 / 0x004DD860 (the two forwarders' identity can not be proven), 0x00663F50 (about twenty local
  models), 0x009D2BA0, 0x00A2AC30.

## Entries (APPLIED.tsv, 20): 14 done, 2 done-already (q10996, q11006), 4 blocked (q11001, q11005, q11007 (2 rows left), q11008 (1 left))

## Rows and checks
- **Section 1: 48 self renames (80 file moves).** STL modules match their siblings' catalog spellings.
- **Headers (opt-ins in 5 files; fable_string.h needed no change):** engine/CSymbolMap.h (`_CSYMBOLMAP_NAMESUSED`), stlport/fable_containers.h
  (`_STLP_DISTANCE_INPUT_NOINLINE`, `_MAKE_HEAP_BODY`, `_UNGUARDED_LINEAR_INSERT_BODY`, `_INSERTION_SORT_BODY`, `_UNGUARDED_INSERTION_SORT_AUX_BODY`,
  `_PARTIAL_SORT_BODY` with `_PARTIAL_SORT_INLINE_MAKE_HEAP`, `_SORT_COPY_BACKWARD_NOINLINE`), engine/CVectorMap.h
  (`_CVECTORMAP_CONST_LOWERBOUND_COUNT_FIRST`), engine/CThingDeadCreature.h (`_SETANIMATIONFILTER_OUTOFLINE`), engine/CQuestManager.h
  (`_PERSISTINFO_SETTERS`). With no opt-in defined each header preprocesses to the repo text for 13 sampled includers and the 55 landed rows
  that define a parent macro (`tokid.log`). `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 63 rows graded from the repo after landing: 62 RELOCATION_MATCH and 1 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 53 binding lines and added 36; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 42)
NF82's NAME_FIXES.tsv (41: outside rows binding slice addresses under stand-in or wrong-element spellings, e.g. 0x00A41260 / 0x00A41960 model
`unsigned long` where the element is `CSoundBankLH*`), plus one question: the count-first const `CVectorMap::LowerBound` body also matches the
landed 0x004365B0, so it may be the original text for every instance.

## Rule slips (NF82 `work/slips.txt`)
- One Bash call exited 1; one heredoc edit failed inside a call that exited 0 (one extra grade).
- 0x004B9540 (4th compile) and 0x004B4F30 (7th form) went past the three-failed-forms limit and were kept ported (within the 15-grade budget).
- Probing an old row after grading a new one overwrote its object (two regrades); two grade calls ran without a file (nothing compiled).
- One opt-in first reused NF68's macro name in another scope; renamed (the row's preprocessed text is unchanged).
- Generated families were graded in batches and STATUS.tsv written per family, not per row.
