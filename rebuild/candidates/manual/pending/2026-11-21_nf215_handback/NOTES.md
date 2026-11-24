# NF215 notes (2026-11-21)

## Summary (counts from STATUS.tsv / APPLIED.tsv / recheck_final.log)
- Entries (6): done 3 (q12300, q12339, q12262); blocked 3 (q11760 + q11873 one unit, q12265). q12340 / 0x006E9360 is not mine (coordinator).
- Rows handed back (rows/): 6, all RELOCATION_MATCH or MATCH + PASS in one vland run (recheck_final.log: 5 RELOCATION_MATCH, 1 MATCH):
  0x0096FA90, 0x006ED9E0, 0x004115A0 (self rename), 0x006238A0, 0x004AFE20, 0x00859040. Blocked / not-a-binder: 0x006D41F0, 0x006D4180,
  0x00EE2A80 (blocked); probe 0x00E1FCF0 (unchanged).
- Header opt-in: FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO (fable_string.h, hdr/README.md).
- Paused once for the NF214 landing (no compile running); resumed on the coordinator message.
- Defined-symbol diffs (objdump -t, HEAD object vs port): 0x0096FA90 none; 0x006ED9E0 none; 0x006238A0 / 0x004AFE20 / 0x00859040 listed in
  STATUS (only the respelled callee copies, each equal to its owner, and header-inline COMDATs with no binding line).
- No new split: every address respelled ends with fewer or the same spellings (0x006E9310 2->1, 0x006EA050 2->1, 0x00662430 2->1,
  0x004115A0 4->3, 0x00411570 4 unchanged (one more row on the PDB spelling), 0x0099E9B0 / 0x013BD800 unchanged (one row moved to an
  existing spelling)). REFS = callers.py (E8/E9) + absrefs.py (absolute; checked on 0x0129AAF4 = 611 refs, 0 for every function address
  here). callers.py reproduces NF213's 11 referencers of 0x00485DBC.
- No EH funclet in rebuild/src/compiled/01/21 includes any changed row's file. No self rename moves a file except 0x004115A0's leaf
  (its file name is unchanged; the coordinator may rename it CBasicString...).
- Locks: 93 family rows claimed for q12262 at census time; 90 released (owner file `released`) once the unit shrank to four rows.
  Kept: the 8 brief rows + 0x00859040, 0x004AFE20, 0x006238A0 (rows.txt).
- Census: callers.py shows 0x0096FA90 is the only landed referencer of 0x006E9310 and 0x006EA050 (one call each). absrefs: none.
  Both self rows already carry their PDB leaves at HEAD (?IsAttackerOn@CCombatManager@@QBE_NABVCThingCreatureBase@@0@Z,
  ?IsDefenderDueForAttack@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z), so moving 0x0096FA90 removes an existing split and creates none.
  callers.py reproduces NF213's 11 referencers of 0x00485DBC (tool check passed).
- No EH funclet in rebuild/src/compiled/01/21 includes the 0x0096FA90 file.

## Queue-line discrepancy
- Coordinator (resume): the 0x006ED9E0 entry is q12339; q12340 / 0x006E9360 is not mine and is not touched.
  BRIEF.md wins; 0x006E9360 is not locked to me and will not be touched.

## q12300 (done): 0x0096FA90 form 1, RELOCATION_MATCH + PASS (work/b1_0096fa90.log)
- In-place respell on the row's existing local models (no new class token; CTCEnemySource keeps its name, slot 29 respelled
  GetPBestEnemyCreatureForAttack returning CThingCreatureBase*, GOTCHAS wf7 + engine/CTCTargetingBase.h). xref: 0x017B -> 0x006E9310
  ?IsAttackerOn@CCombatManager@@QBE_NABVCThingCreatureBase@@0@Z, 0x0187 -> 0x006EA050 ?IsDefenderDueForAttack@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z.
  Defined-symbol diff vs HEAD object: none (23 = 23). Test unchanged (standalone mock, no linked names).
- Split closed: both addresses had the self PDB leaf plus this row's stand-in; now one spelling each.
- Full header port not attempted: engine/CAIStateGroup_SummonerCharge.h is a generated pad layout (flattened vptr, no base, no retail
  opt-in), so moving `this` onto headers means a new retail-class opt-in for CAIStateGroup_SummonerCharge over CAIStateGroupBase: a header
  item, not a naming respell. The row's pre-existing local models (CThingCreatureBase, CCombatManager, CAIStateGroupBase, CAIBrain, CWorld,
  CIntelligentPointer ...) are HEAD debt (vland SHARED_HEADER_AVAILABLE warnings, unchanged from HEAD).

## q11760 / q11873 (blocked): FINDINGS for the coordinator (I cannot write FINDINGS_LOG)
- 2026-11-21 NF215: the advisor decision "give CCharString::operator!=(const char*) a plain inline body and spell the "(" test as
  `ReadAsSymbol() != "("`" does not match. Form (work/006d41f0_A.cpp): explicit specialisation of the primary
  CPersistTraits<std::pair<CCharString,long> >::TransferIn on fable_string.h (probe opt-in FABLE_RETAIL_CCHARSTRING_TEXT_NE_INLINE, body
  `!(*this == text)`, overlay kept in work/probe_hdr/, not handed back) + FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO + CStringParser /
  CPersistTraits (PASS22 CCharString traits) / CDataOutputStream: DIFFER(307v295), the same object as NF209 form 1 (Compare expanded at
  all three tests). The != folds into the same expansion as the == spelling.
- Why the hypothesis is weak: retail has an out-of-line ??9CCharString@@QBE_NPBD@Z at 0x0099E960 (landed row, 65 bytes, calls 0x004115A0),
  FableWin.pdb 0x02F61900 places it with the CCharString .cpp bodies (Truncate 0x02F61D70), not the header-inline group (GetAsCharArray
  0x0186C770, ??8CCharString@@QBE_NPBD@Z 0x01899D30), and retail 0x006D41F0 never calls 0x0099E960. A TU that used != would call it.
- What would still need trying: a "(" test that reaches CBasicString<char>::Compare directly (the shape HEAD's matching stand-in models
  as CompareEqual) with the other two calling 0x004115A0. CCharString cannot call the protected static Compare, so any such body changes
  fable_string.h access: needs header claim. Keep 0x006D41F0 / 0x006D4180 out of slices until then.
- Side note: HEAD's 0x006D41F0 binds 0x009BA790 as the stand-in ?ReadAsString@Parser@N006d41f0@@QAE?AUCCharString@2@XZ while 0x009BBA40 binds
  the PDB ?ReadAsSymbol@CStringParser@@QAE?AVCCharString@@XZ (an existing split; it closes with the port).

## q12339 (done): 0x006ED9E0
- Census of 0x00662430: 8 landed callers; 7 already bind ?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ (0x006E9960, 0x006EAC50,
  0x006ED510, 0x00733180, 0x007F6BF0, 0x007F6CB0, 0x00979910); 0x006ED9E0 bound ?IsCombatant@CThingCreatureBase@@QAE_NXZ. Respelled in
  place in its existing local CThingCreatureBase (as NF211 did 0x006ED510). Two sources: Ego_r.pdb 0x0053D2C0 + FableWin.pdb 0x01DA3370,
  and the self row's Leaf / engine/CThingCreatureBase.h opt-in.

## q12262 (done): the CBasicString<char>::operator==(const char*) unit (0x004115A0 + 3 binders)
- Census (work/census_full.tsv, tools/census.py: every row that calls 0x004115A0 (123) or carries a TEXT_EQ / local == body (204 rows
  compiled, objdump -t): binders of the old self leaf ??8CBasicStringChar@@QBE_NPBD@Z = 3 (0x00859040, 0x004AFE20, 0x006238A0). nf202's
  "about 20" counted rows that DEFINE a ??8CBasicStringChar copy: 13 non-callers define a dead one (NAME_FIXES), the self row defines it.
  91 objects reference or define the PDB spelling. 0x004FE630 (X_004fe630 template) and 0x006D41F0 (N006d41f0) keep their own spellings.
- Copies of the PDB spelling with a body other than the owner's 88 bytes (HEAD debt, unchanged; coordinator: not a hold):
  3 x 52 bytes (0x00463334, 0x00475026, 0x0047550F, found by the second census pass over all 134 CBasicStringChar rows: 105 define or
  reference no ??8 symbol, none references ??8CBasicStringChar), 61 x 20 bytes (0x00D13580 included, re-measured with its /d2inlS15), 5 x 19, 3 x 22, 3 x 72, 3 x 76, 1 x 79, 1 x 45; 14 objects define an 88-byte copy (not compared one by one).
- Proofs: 0x004115A0 NF202 draft MATCH (pick-any Compare copy = 0x00411570). Binders that CALL 0x004115A0 take the new opt-in
  TEXT_EQ_NOINLINE_COMPARE_ZERO + COMPARE_BODY: their == copy is then the owner's 88 bytes (0x006238A0, 0x004AFE20). The 13 other caller
  rows with a differing copy (caller=1 in work/definers.tsv) are the natural next batch for the same two macros.
- Expanded-site definers (TEXT_EQ_INLINE, 40 rows; leaf expands == with `call Compare`): one probe only, on 0x00E1FCF0, changing two
  macros at once (TEXT_EQ_INLINE -> TEXT_EQ_COMPARE_ZERO + COMPARE_BODY): copy 88 equal, leaf DIFFER(419v435). Not a general result;
  next slice: a q.py sweep of those 40 rows with each macro alone.
- 0x00859040 forms (work/00859040_f1..f3.cpp): see STATUS; the guarded `__declspec(noinline)` (#ifdef _MSC_VER) on the UnassignString
  definition replaces HEAD's auto_inline(off) region for that one function (an inline control on a same-TU copy, EXTRA_RULES 2026-11-08
  preference 3; the copies' callers in retail call 0x0099E9B0). Flag for the coordinator: it is a source-level inline control.
- 0x0129AAF4 in 0x006238A0: HEAD bound the in-object "" literal COMDAT there; the header port binds ?NullChar@CCharString@@1DB (fix).
- Tests: 0x006238A0 / 0x004AFE20 / 0x00859040 tests unchanged (standalone mocks; HEAD debt: 0x004AFE20 and 0x006238A0 tests model
  CBasicStringChar locally). 0x004115A0 takes NF202's header-based test.

## Descriptive names
- None new. g_CCharStringInstanceCount_013BD800 is the header's existing stand-in for 0x013BD800 (PDB candidate ?NoStrings@CCharString@@1JA,
  declared protected static in the RETAIL branch, bound by no landed row; not used so the spelling count does not grow).

## Rule slips
- I started q11760 before reading that 0x0099E960 exists; the operator!= probe overlay was moved out of hdr/ (work/probe_hdr/).
- The first census pass misparsed 0x00D13580's `// Flags:` comment (CLFAIL); fixed (anchored regex) and re-measured: DEF 20 bytes.
- Three Bash calls failed: one exited 1 (a sed address built from an empty grep), one hit a missing S variable (absrefs.py, rerun), and one
  foreground `sleep` was blocked by the harness (replaced by a background until-loop).
- Three rows with no ??8 symbol in the first pass (0x00DCD1C0, 0x00E84B80, 0x00F0CA30) are non-callers (caller=0): not binders.

## Flags for the coordinator
- 0x00859040 is the first landed user of FABLE_RETAIL_SHOVE_STRING_CLEANUP_PASS23 (fable_string.h; Codex pass 23, no landed user before). It also
  uses a guarded `#ifdef _MSC_VER __declspec(noinline) #endif` on its same-TU UnassignString copy (inline control, see q12262).
- Pick-any ?Compare@?$CBasicString@D@@KIJPBD0@Z copies: 0x004115A0, 0x006238A0 and 0x004AFE20 objects each carry one, equal to 0x00411570
  (40 = 40, relocs equal); against the strong owner that is the accepted LNK2005 debt the q12262 queue line names (nf181 / nf184).
- Same-TU strong copies in 0x00859040 (~CCharString, copy ctor) duplicate their owners' strong symbols as at HEAD (existing debt); the
  UnassignString copy's IAE symbol is not what owner 0x0099E9B0 defines (its local model spells it QAI), so no new LNK2005 today.
- 0x0096FA90: the full header port was not attempted (CAIStateGroup_SummonerCharge.h is a generated pad layout).
