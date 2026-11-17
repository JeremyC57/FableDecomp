# 2026-11-17 landing nf80: name-fix round 7, NF80 (25 rows: one definition of CBasicString<char>::operator< and NF75's blocked rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF80 took the 17 rows NF75 left blocked (entries q10763, q10920, q10923, q10931, q10939, q10943), four rows that carried a copy of
`operator<`, and six more it claimed while working. For this slice the coordinator let it overlay the root `fable_string.h` (opt-ins only).
Brief, list and records: `naming/2026-11-17_agentNF80/`.

## Rows (27 ported; 25 land, 2 held)
- **`CBasicString<char>::operator<` (0x00429950) has one definition.** `FABLE_RETAIL_CBASICSTRING_LESS_BODY` puts the body in fable_string.h
  as a template member (`__declspec(noinline)` under `_MSC_VER`); the self row adds the explicit instantiation. The held q10763 draft's
  `_DEFINE` form was dropped: its strong copy clashed with every includer's COMDAT copy (LNK2005, shown by a link test). 0x00429950,
  0x009EFB80 and 0x006C31B0 link together.
- **The 13 q10923 binders are on fable_string.h** (the 12 from NF70 plus 0x00686F10, which reloc_bindings.tsv did not show). 0x004B3CE0,
  0x004B4260 and 0x00520D00 lost a second strong copy of operator<. No row defines operator< any more.
- **q10939:** 0x0099B2C0 `CWideString::ToCharString` on fable_string.h (`FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY`); its section-2 line maps
  the stand-in global `g_FableEmptyWideString_0129A8E0` to the string literal it is (the NF65 `g_szCouldNotLoadFromFile` precedent).
- **q10943** 0x005907AB (NUISystem::CDeletion), **q10931** 0x009C8060 (`NGameText::CGroupEntry` ctor), **q10920** 0x00835C80
  (`CThingDeadCreature::Create`, a full port out of the stand-in namespace).
- **Held (`held/`): 0x0049124E and 0x006C37D0.** Both matched with `ConvertToCharString` respelled to `ToCharString` inside their own local
  CWideString models; the right fix is a port onto fable_string.h (queued).

## Entries (APPLIED.tsv): 6 of 6 done
q10923 is done for its listed binders, but 45 other landed rows still call 0x00429950 under a stand-in spelling (NF80 compiled all 166
rows outside its slice: all RELOCATION_MATCH, 121 already right). Each must move onto fable_string.h; queued as one entry.

## Rows and checks
- **Section 1: 13 self renames (20 file moves).** Every STL module matches its siblings' catalog spelling.
- **Headers (12 opt-ins in 8 files; additions only):** fable_string.h (`CBASICSTRING_LESS_BODY`, `CCHARSTRING_FORCEINLINE_LESS`,
  `CWIDESTRING_TOCHARSTRING_BODY`), engine/CTCBase.h (`CTCBASE_REGISTRY_LOOKUP`, `_ALLOCATETC_BODY`, `_ALLOCATETC_DEFINE`), CVectorMap.h,
  CTCInventory.h, CArray.h, GFSizeof.h, NGameText_CDataBank.h and CThingDeadCreature.h. With no opt-in defined the headers are
  token-identical to the repo (NF80 preprocessed 35 includers; `tokid.log`). 10 asserts pinned (CTCBase.h 9, CThingDeadCreature.h 1);
  `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65. NF80's unchanged copies of fable_game.h and fable_retail_banks.h
  (there only for its grading include path) were not copied.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 25 landed rows graded from the repo after landing: 23 RELOCATION_MATCH and 2 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 68 binding lines and added 36; bindings_multi_check is at its baseline (`NullCoord`).

## Open: CTCBase::AllocateTC has no single definition yet
`FABLE_RETAIL_CTCBASE_ALLOCATETC_BODY` gives includers (0x00686FA0 Clone) a shareable inline copy, while the self row 0x00686F10 owns a
strong one through `_DEFINE`; linking those two objects gives LNK2005. Per-object grading passes, so nothing fails today. AllocateTC is not a
template, so the operator< fix does not apply. NF70's CMap `GETSTRONGESTENGINETHEMEAT_BODY` / `_DEFINE` has the same shape. Queued for a
coordinator ruling before any whole-image link (GOTCHAS line added).

## Name fixes queued (`land1117fu`, 21)
NF80's NAME_FIXES.tsv (18: the CCharString tree `_M_insert` / bound rows, the CTCBase registry heap helpers, CreateMenuEntryComponent,
CThingDeadCreature::Create's callees and others), plus the 45-binder line for 0x00429950, the two held ToCharString ports and the
AllocateTC ruling.

## Rule slips
- Two Bash calls exited 1 (a `grep -c` with no match; a `cat` of lock owners while claiming rows).
- One section-2 pairing for 0x00756250 read a stale object file; NF80 regraded and redid it.
- NF80 claimed six rows beyond its list (0x00686F10, 0x004B3CE0, 0x004B4260, 0x00520D00, 0x0049124E, 0x006C37D0), each through the lock.
