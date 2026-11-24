# NF386 hand-back (Account A naming agent, 2026-11-24): the T3 lines that blocked NF384's 82 split_fixable T1 targets

Input: BLOCKERS.tsv (83 lines over 74 binders), NF384_STATUS.tsv. Every line is classified in STATUS.tsv:
**A 15** (worked and closed), **B 22** (respell is independent of the line: set the line aside, land, and restore it unchanged; this
count includes 2 table-only stale-witness lines and 1 held row), **C 46** (blocked for real, with the reason).
Compiles: 128 row compiles of the ~150 budget (g1 26, gB1 36, gB2 6, gC 6, lk1 17, lk2 29, lk3 8). Every grade ran at one
LANDVERIFY_WORK path per run (work/<run>/{head,new}).

## Run dirs (land_scripted.py shape: rows/, tests/, orig/, head/payload.json, RESULTS.tsv, UNITS.tsv, TABLE_CHANGES.tsv, rows.txt, T3.txt; hdr/ + orig/hdr + HEADERS.tsv where headers change)
| dir | rows | what | T3 |
|---|---|---|---|
| batches/b1 | 13 ACCEPT | Copy moves (NF337 form): 12 nf324 + 0x007ADEF0. Self -> `?Copy@C@@UAEXPBVCDefClassBase@@@Z`, callee -> `??4C@@QAEAAV0@ABV0@@Z`; 11 headers get additive opt-ins (9 `COPY_DECLS` with Copy + operator=, CSpecialAbilitiesFireballSpellDef / CAIScratchpadDef operator= only); 0x00460A14 local model (no header exists); 0x004642F5 existing CThingObjectDef VALIDATE branch | `--release-t3 nf324,nf320,nf322,nF325` closes 15 lines |
| batches/b2 | 16 ACCEPT | binder respells (B). 0x00434B50 + 0x00B314E0 are one unit (both binders of the GFVBoxToABox pointer form). 0x00BDC020 / 0x00BF6D10 / 0x0089A2D0 use shared headers with new flat-branch opt-ins (CLandscapeBackgroundTreeNode.h, CTCDoor.h) | 18 lines set aside + restored unchanged (T3.txt) |
| batches/b3 | 1 ACCEPT | 0x00659EFC: six RemoveThingFromTask instances (needs ruling R1) | nf310 line set aside + restored |
| held/h1 | 1 | 0x00838BA0 (needs ruling R2) | nf315 line set aside + restored |
| held/h2 | 1 | 0x0080F240: NF307 D1 hold (R3); not for landing | line stays |
| HAND_TABLE_CHANGES.tsv | - | 2 deletes: the stale `_Retail` W lines of 0x005BDC08 / 0x005BDB94 (table only, independent of any row) | lines kept |

**Order inside each dir:** copy hdr/engine/*.h into rebuild/include/engine and tests/<a>_test.cpp over the TestSource, THEN run
land_scripted (it copies rows/ only: the Copy rows do not compile without the opt-ins and the old tests do not link with the new rows).
If a row is held, restore its test (orig/tests) and, when no landed row of the batch uses it, its header (orig/hdr).

## Gates (every ACCEPT row)
- vland HEAD and new MATCH / RELOCATION_MATCH + PASS at one path (work/g1, gB1, gB2, gC GRADE.tsv; RESULTS reasons quote each).
- objsec (tools/objpair386.py; evidence/objsec/<a>.txt): ALL_EQ under exactly the intended renames for every row; LIBCP (only
  `/DEFAULTLIB:"libcp"` added) on the four header-route Copy rows 0x0045FE26 0x00460251 0x004608C1 0x00460A5C (the NF318 Q1 / NF322 Q3 allowance);
  0x00659EFC .drectve EQUAL_UNDER_MAP (R1). Where HEAD already defines the same vftable / RTTI set (0x0040A740 0x009B02E0 0x009D6490), the
  check compares the DEF sets instead of refusing.
- pk_all + pickany_vs_retail HEAD v new (tools/pk386.py, pkdiff386.py): SAME for all 31 rows, 0 reach flips (evidence/pkdiff_all.txt, pkdiff_gC.txt).
- /FORCE /MAP link (tools/link377.py): b1 + 13 operator= owners + 4 context rows (evidence/link_g1.txt); b2's 16 rows + 29 callee / census
  objects (evidence/link_b2.txt); 0x00659EFC / 0x00838BA0 / 0x0080F240 in evidence/link_gB.txt. 0 duplicates both sides, no NEW_UNRESOLVED,
  nothing HEAD resolved left unresolved; every old callee spelling disappears and every new one resolves to its target object.
- Census by compile for every changed table spelling: evidence/census_compile.txt (the 6 other callers of 0x00753840 and 2 of 0x00B30480 bind
  only the PDB forms; lk3), census_gfv.txt (0x009DC6E0 0x009DC870 0x00A0B560 bind the reference form of GFVBoxToABox; lk2),
  census_select.txt; retail callers per target evidence/callers_B.txt (exe scan). Each made-up Copy callee is named only by its own row.
- apply_table.py --dry on b1 / b2 / b3 / held/h1 / HAND: 0 misses, no new multi-target symbol. Applied all five to a scratch copy:
  each of the 39 targets left with exactly one spelling, DEFINED by the target's own compiled HEAD object (0x005BDC08 / 0x005BDB94: no line
  left; the targets define the PDB names their callers UND) (evidence/t1_unblocked.txt).
- Headers: equiv_check(repo copy, work/tree) over the 152 includer rows of all 13 changed headers: 0 differing (evidence/equiv_all.tsv).
  Planted controls flag every header: COPY_DECLS headers (evidence/equiv_g1_ctl2.tsv, equiv_g1_ctlcover.txt); flat opt-ins
  (evidence/equiv_flat_ctl.tsv: 17 of 29 includers flagged, the other 12 read the retail branch). Probes: override probe compiles and the
  made-up-name control fails (Copy); with the macro the use compiles, without it fails (flat opt-ins); 0 asserts moved except
  CAIScratchpadDef (40 pinned). HEADERS.tsv per batch.
- Names: every new spelling is an exact line of the Ego_r and FableWin name sets (evidence/pdb_exact_all.txt), except the seven FableWin-only
  names in R7.
- Moved-test control (HEAD row + new test) was NOT run (NO_CTL=1 on every grade); the new tests were graded only with the new rows.
- Freshness: all landed sources / tests equal the HEAD payloads and all 13 orig headers equal the repo at hand-back; the T3 lines are
  unchanged in text (line numbers shifted by one; T3.txt quotes text, not numbers).

## T1 targets unblocked (when the dirs land and their tables apply)
- b1: 0045370a 00453911 00453d46 00453f63 00453f93 004541df 0045fe2b 00460256 004608c6 00460a19 00460a61 004642fa 007adf00 (13)
- b2: 00409fd0 009e1e70 009f0500 00647403 00cb7f60 006d6ea0 00692f30 006893d0 00753840 009ad950 009d5a30 00b32610 00b30480 00bf6720 00bf6b70 009e99c0 00730ae0 (17)
- b3: 00660ae2 00660b5d 00660bd8 00660c53 00660cce 00660d49 (6, after R1)
- HAND: 005bdc08 005bdb94 (2, now)
- held/h1: 00838aa0 (after R2)
Total 38 now or after one ruling, plus 1 held. Not unblocked: 0x004BA390 (QUEUE_PROPOSALS q5), 0x007FDEB0 (R3), and the C lines' targets.

## Rulings needed
- **R1 (NF303 Q2)** 0x00659EFC: HEAD's .drectve already carries /alternatename lines for the six member-template instances; the respell
  only renames them (old -> new under the objsec map; every other section ALL_EQ). Is a renamed (not added) /alternatename OK?
- **R2** 0x00838BA0: an EMPTY local `class CThingGameObject { protected: void UpdateStandingOnThing(); };` base of the row's local
  CThingCreatureBase (no layout, `this` unchanged) while engine/CThingGameObject.h exists. The header's flat layout cannot be the base.
- **R3 (NF307 D1)** 0x0080F240: the PDB spelling of the list adds the allocator argument, so the row's emitted pick-any list
  begin / end COMDATs rename from one non-PDB spelling to another. Held.
- **R4 (confirm)** LIBCP on the four header-route Copy rows (as NF337 b2).
- **R5 (confirm)** 0x004B3A20 keeps its existing local CDrawQuestInfo model, moved into `namespace NPlayerGui` (header
  engine/NPlayerGui_CDrawQuestInfo.h exists but pulls fable_string.h / CCountedPointer.h, which clash with the row's own local
  CCharString / CCountedPointer models; a row port).
- **R7 (confirm, two-source rule)** the five RemoveThingFromTask instances of b3 (CarryCrate, Horn, Patrol, SecuritySweep, Torch) and
  b1's CSpecialAbilitiesUnholyPowerDef Copy / operator= are exact lines of FableWin.pdb only (Ego_r.pdb lacks them; it has the PickCrops
  instance in the same ABEX form). Second source: the target rows' landed selves define exactly these names (link resolves them) and, for
  UnholyPower, the RTTI vftable placement of 0x004608C1. Accept as two sources?
- **R6 (confirm)** b2's 0x0089A2D0 test keeps a local mock class, now named CTCDoor (tests are standalone mocks).

## Discrepancies / disclosures
- I ran `git log -1` once, read-only, against the "never run git" rule. Nothing else touched git or the repo.
- The summary of NF384's skip listed GFACoordToVCoord's 0x0043A2A0 as a single binder; the pointer form has four other binders (q3).
- Header files in hdr/ are whole copies of the repo header at hand-back plus one add-only block. Older pending dirs in the repo carry
  their own copies of CHeroMarriageDef.h, CInventoryItemDef.h, CSpecialAbilitiesEnflameDef.h and CTattooDef.h (2026-11-03 .. 11-12, Account B
  and Codex staging). If the repo header changed before landing, merge the block (diff orig/hdr -> hdr) instead of copying the file.
- Agent NF388 (string-literal audit) was claimed during this pass; the 32 rows' landed sources were rechecked fresh after that claim.
- 0x00B314E0 (no T3 line) is a hidden second binder of the GFVBoxToABox pointer form; it moves as a unit with 0x00434B50.

## Locks
All 88 NF386 locks are released (evidence/locks_released.txt): the 31 rows, the 17 lk1 operator= / context owners, the 29 lk2 owners /
census rows, the 8 lk3 census rows, 0x005BDD8C / 0x005BDDB8 and 0x0080F240. land_scripted can lock as COORD.

## Files
STATUS.tsv (83 lines), QUEUE_PROPOSALS.tsv, NAME_FIXES.tsv (the stale witness pair), HAND_TABLE_CHANGES.tsv, LESSONS.md, evidence/, tools/
(drv386 grade / pre, objpair386, pk386 + pkdiff386, link377, hdredit386 + hdrflat386, plant386 + plantflat386, includers386, undcensus386,
assemble386, status386), work/ (graded objects and payloads).
