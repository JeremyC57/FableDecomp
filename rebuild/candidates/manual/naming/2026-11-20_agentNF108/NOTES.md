# NF108 notes (2026-11-19 / 20, PRIORITY reconciliation slice 2 + NF104's 10 open entries)

Two runs: the first NF108 run stopped mid-slice (container restart) with 13 entries done, 1 blocked; this run re-graded every row in
rows/ first (work/rc.log: 24 rows, all MATCH / RELOCATION_MATCH + PASS) and continued. Counts below come from APPLIED.tsv / STATUS.tsv.

**Entries (40): 20 done, 0 done-already, 1 rejected, 3 blocked, 0 deferred, 16 open.**
- done: q4185 q4250 q4258 q4265 q4406 q4434 q4448 q4487 q4648 q4701 q4702 q4738 q4759 q4760 q5046 q5048 q5099 q5150 q6236 q6324
- rejected: q4376 (REJECTED.tsv; probable spelling in NAME_FIXES.tsv)
- blocked: q4453 (1 Leaf-less July row left, 00be7d70), q4526 (1 Leaf-less row left, 006a4b80; 007032f0 not attempted), q4528 (00931010
  after 3 failed forms, first run)
- open: q4449 q4524 q4525 q4744 q4769 q4975 q5080 q5205 q6210 q6212 q6230 not started; q4447 q4452 q4454 q4645 q5002 analysed only
  (0 grades): their APPLIED notes carry the findings (rows left and what each needs) for the next pass. q4452: 115 of 120 REFS rows
  already bind the claim; 5 rows left.

**Rows: 52 ported (rows/), 12 not-a-binder, 3 blocked** (STATUS.tsv: 00931010 after 3 forms; 006a4b80 and 00be7d70 Leaf-less old rows
left alone per EXTRA_RULES). recheck_final.log is one vland --recheck-landed run over all 52 (flags patched from each row's `// Flags:` line).

## Order
REFS size order (few referencing rows first), interleaved with the first run's order; the widely referenced entries (q4449 55 rows,
q4524 73, q4452 120) are left open together for one later pass, as the brief asks.

## Claims read from the evidence column
q6236 (the tree operator=, not the map / stand-in spellings in correct_symbol) and q6324 (correct_symbol holds the stand-in; the PDB
spelling is in the evidence). q4376's claim did not hold (rejected).

## Self renames (RENAMES.tsv section 1) and their callers
Every landed row that binds a renamed address ends on the new spelling, except where a split already existed before (gate.py: not CREATED):
- 0x00502500 StartMovePlayerPos / 0x005063E0 SetPlayerPos (q4648): leaf-only respell (file-scope stand-in enum ELoadType -> the nested
  CWorldMap::ELoadType). Their other binders were respelled too: 0089b780 (0x00502500), 00487c20 and 00488ab0 (0x005063E0); 004a3740 /
  00489d40 already bound the PDB spelling. 005063e0's callees LoadLevel 0x00502620 / CacheMaps 0x00502850 now bind the protected PDB
  spellings their self rows define; 0x00502620 keeps a pre-existing second spelling in 005063d0 (?fl_helper@@YGXJH@Z, not mine).
  Gate follow-up: 00502850 (the CacheMaps self row) was the one other row on the old LoadLevel spelling; respelled in place too (own
  leaf -> ?CacheMaps@CWorldMap@@IAEXW4ELoadType@1@@Z, which 005063e0 / 005064b0 bind).

## Gate scan (gate.py scan + a probe of every other referencing row)
work/gate/: gate.py scan over RENAMES.tsv (every section-1 / section-2 address, 41 targets) found 96 landed rows outside rows/ that
reference a target; all 96 were compiled (work/gate/probe2.tsv, tools/probe2.py). After the 00502850 fix no row binds an old spelling
of a target. Rows binding a third spelling (pre-existing splits, none created here): 0x0043027A (004302cd, 0043151d, 00463334, 00463732,
00475026), 0x007122C0 (004c8c90, 006ad970, 00747da0, 008326a0, 008508b0, 00904380, 00904ee0), 0x0063A300 (006a4dd0, 006cd9c0, 00880200),
0x00502620 (005063d0), 0x004B85D0 (00569b30, NAME_FIXES), 0x00436C80 (00918640, linker fold, first run). 00be7d70 does not grade
(Leaf-less July row).
- 0x004BDB40 GetAllDummyObjectNames (q4448): parameter type only. Its callee 0x00A35B30 moves to the PDB C3DMeshObject spelling; that
  address's own self row is an old placeholder (fn_00a35b30) under a wrong catalog name -> NAME_FIXES.tsv.
- 0x004DA2AC (_M_erase) and 0x004DB91F (clear) of the STLport map<long,long> tree (q4185): both were "AutoTiny" stand-in rows with
  invented names (BuildTreeArray / OnKill); 0x004DB91F's only binder 004e52f0 already bound the clear() spelling.
- 0x004C6140 RemoveBulletTimeSlowdownMultiplier (q5099): catalog name changes (was the descriptive CTimeMultiplierController_004c6140::
  RemoveRequest). Its callees: 0x004C6B60 now binds its self row's tree erase(const key&) Leaf; 0x004C5F10 RecalculateSoundPitch as
  004c6170 binds it.
- 0x00432B38 tree operator= (q6236): was CRetailTree_00432bab. Callees 0x0043027A clear() (already 5 spellings before; now one fewer
  stand-in) and 0x00432BAB _M_copy (its self row's Leaf).
- First run: 0x00436C80 0x004AF270 0x004B8D70 0x004C11D0 0x004C3120 0x004C5E90 0x004C6060 0x004C70F0 0x004C7440 0x004C7E70 0x004FCE00
  0x00517A10 0x00761620 (see RENAMES.tsv).

## New // Flags: lines (the coordinator must carry them into CompilerFlags)
vland / mkpay take flags from build_candidates.ps1 only; my payloads were patched from each row's `// Flags:` line (tools/fixflags.py).
- 004da2ac, 004db91f: `// Flags: /Os` (retail is size-optimised: push-mem / pop ecx / `and dword ptr [..],0`; without it DIFFER 51v45 /
  51v41). The old stand-in rows got the same effect from `#pragma optimize("s", on)`.
- 00a92740, 00a91180, 00b2c8e0: `// Flags: /d2inlS100 /d2inlT200` (retail expands STLport vector<unsigned char>::resize inline; the
  stand-in forced it with FABLE_FORCEINLINE; with the shared header and no flag, or /d2inlS20, resize is called: DIFFER 258v295).
  Not swept down further.

## Descriptive names
- Descriptive-by-library (STLport 4.6, no PDB copy; spelled from the instantiation the callers prove): 0x004DA2AC _M_erase and 0x004DB91F
  clear() of _Rb_tree<long, pair<const long,long>, ...>; 0x00432B38 _Rb_tree<CCharString, pair<const CCharString,long>, ...>::operator=;
  NAME_FIXES 0x0051AC70 __destroy_aux<CStaticMapGenerationVisibiilityInfo::CMapDesc*>.
- Typedef names in rows (no mangling effect): TLongLongMap, TCharStringLongTree.
- No made-up function / class names were introduced.

## Forms used
- Explicit instantiation over the shared STLport headers (0x007AE0E0 / 0x00432A35 recipes): 004da2ac, 004db91f, 00432b38.
- Stand-in containers replaced by the shared STLport ones: 0072e290 (vector<CCharString>), 00a91180 00a92740 00b2c8e0 (vector<unsigned
  char>), 006cf1d0 (map<long,long>), 004e4450 004e5c89 00675c00 00a2cf50 (map<CCharString,long>), 0045c54e (multimap<CCharString,long>).
- Stand-ins replaced by shared engine headers: 00488ab0 (CWorldMap.h SETPLAYERPOS), 007c25a0 (NGraphicAppearance_CTCGraphicAppearance.h
  HELPERNAMES + TINT), 00890ab0 00890b00 (CBulletTimeManager.h + overlay SETSLOWDOWN), 004c6140 (CBulletTimeManager.h INTERPOLATE +
  STLP_TREE_ERASE / ERASE_KEY + overlay RECALCULATESOUNDPITCH).
- In-place respelling inside pre-existing local models: 00487c20 0089b780 00502500 005063e0 (nested ELoadType), 004feec0 (class-key
  CCharString), 004bdb40 (declared std::vector), 00774610 (_Select1st parameter, private _M_erase), 0079d020 0079d110 007aa4d0 007ab190
  008579d0 (CBulletTimeManager members).
- Same-TU stand-in kept with its name (first run, 005252b0): see STATUS.

## Tests
Changed tests (rows/*_test.cpp, all first run): 004af270, 004c6060, 004c70f0, 004c7440, 004fce00, 0071ab20 (rewritten to link the
renamed row). Every other test of the 51 rows is self-contained (does not link the row) and is unchanged; vland PASS for all 51.

## Key sentinels (not cast addresses)
00890ab0 / 00890b00 pass `(void*)0xBEEFBEEF` / `(void*)0xCABBA9E5` as the bullet-time slowdown key: the retail immediates the landed rows
already passed (as unsigned long), values outside the image that identify the requester, not data addresses (no relocation in retail).

## EH funclets
No landed *Unwind* funclet includes any of the 51 changed row files (grep by file name), so none needed regrading. The self rows
renamed here keep their catalog files until the coordinator renames them; no funclet includes them.

## Header overlay
hdr/README.md: 6 opt-ins in 5 headers (CQuestManager, CWorld, NGraphicAppearance_CTCGraphicAppearance, CTCScriptedControl from the
first run; CBulletTimeManager RECALCULATESOUNDPITCH + SETSLOWDOWN this run). No "needs header claim" renames.

## Analysed-only entries and blocked rows, for the next pass
- q4447 / q4645: the CThingSearchToolsMethods::GetAllThings family (K106: 0x00516200, 0x0048D400 and the same local-model shape at
  0x004ACD70 / 0x007D4CE0) defines template instances on a stand-in derived class over local CThing / list / vector models; the PDB
  leaf needs the retail engine/CThingSearchTools.h GetAllThings over engine/CThing.h retail. 004887c0 needs a same-TU strong copy.
- q4454: 0x0051CD80 / 0x0051D400 / 0x0051D530 all model CNavigationAreaUpdate's types under stand-in names; 0x0051D530's layout is the
  implicit ~CNavigationAreaUpdate (own self rename).
- q5002: two different ~list rows (004ba720, 004bae00) call one clear: their element types are unproven (fold candidate).
- q4526: 006a4b80 is Leaf-less; 007032f0 needs the 0x0082D6F0 retail chain.
- q4528 (first run): 00931010 after 3 forms (STATUS).
- 00be7d70 (q4453): its lock dir reads `released`; I did not take it (Leaf-less July row anyway).

## Rule slips
- In this run I first recorded q4447 / q4454 / q4645 / q5002 as blocked with 0 grades (the brief's blocked needs 3 failed forms); they
  are now `open` with their findings, and their untouched rows have no STATUS line.
- The first run left hdr/README.md unwritten; written now.
