# NF457 (2026-11-26): creature-action ctor respells (r454b), r456b, NF456 held units

Scratch: `$S/w22/agentNF457` (tools copied from NF456, paths moved; new: `tools/xref.py` retail call/jmp/abs scan, `tools/ties.py`
ret N + own-RTTI-vftable store, `tools/predit_cb.py` `(CThingCreatureBase*)`->`&` self edit, `tools/comps.py` census components,
`tools/hobj.py` HEAD-object compile, `tools/mkfiles_b5.py` batch files). Sandbox: `sandbox/` (copies of the 3 oracle TSVs + descriptive_check.py).
Repo oracle md5s recorded in work/repo_oracles_md5.txt (checked unchanged after every sandbox step).

## Correction to the NF456 census wording
The "34 one-argument ctors with one landed caller and no table line" are in fact 34 with NO retail caller at all (t5_creature_census.tsv
column 5 = `-`): xref.py over every section finds no call, jmp or absolute reference to any of the 34 (Ego_r/FableWin callers inline them).
So these units have no TABLE_CHANGES lines. Of the 34:
- 26 land self-only (batch b5a). 0x008B9150 ScreamerDrainOutOf is held (r457e): the shared header's in-class `&` body is DEF'd as a COMDAT by
  0x008B9010's object, so the respell would add a NEW duplicate (edited row kept in work/held_008b9150/, lock released).
- 0x0085BBA0 is not a Decapitation ctor: it stores vftable 0x012767EC = RTTI CCreatureActionHeroRunningStop, sits among the landed
  HeroRunningStop members, and 0x0066A560 already owns ??0CCombatAction_GenericResponseToDecapitation@@QAE@AAVCThingCreatureBase@@@Z
  (stores 0x0125AD94, bound by 0x00667450). Identity fix -> ??0CCreatureActionHeroRunningStop@@QAE@AAVCThingCreatureBase@@@Z (both PDBs).
- 6 have in-class inline `(CThingCreatureBase*)` models in other landed rows (their retail callers inline them), which join the unit:
  008cf920 TentacleIdle (008cf610, 009250f0), 008e4b80 SummonerDead (008dfef0), 00925250 TentacleRise (00925170),
  009252c0 TentacleStrikeInto (009251e0), 008cfa10 TentacleStrikeOutOf (008cf7d0, which also calls 008cf990 J-form: family unit),
  008c9620 BanditKingUnstuck (008c8c30 / 008c8f70 / 008c91e0, with 008c94d0 / 008c95b0: family unit; 008c8c30 models a `(CThingCreatureBase*, bool)`
  form that no PDB spells -> expect a hold).

## Batches (NF456 layout: rows/, orig/ = exact HEAD copies incl. tests, head/payload.json, RESULTS, SELF_SYMBOLS, UNITS, CHECKS, TABLE_CHANGES,
## HEADERS, BEST_GUESS where needed, evidence/ = vland HEAD/new logs, vobjproof, link)
| batch | rows | units | verdict | what |
|---|---:|---:|---|---|
| b5a_creature_ctor_selfonly | 26 | 26 | ACCEPT | one-argument creature-action ctors `(CThingCreatureBase*)` -> `&`, no retail caller, no table line, no other landed model or COMDAT DEF. 2 BEST_GUESS (FableWin-only: 008e4500, 00907f50). vland/objproof evidence is the 27-row run (008b9150 dropped afterwards); link re-run on the 26 (evidence/link_b5a26.txt). |
| b5b_creature_ctor_inline_units | 9 | 4 | ACCEPT (r457d) | 4 FableWin-only ctors (BEST_GUESS: 008cf920 TentacleIdle, 008e4b80 SummonerDead, 00925250 TentacleRise, 009252c0 TentacleStrikeInto) + the 5 landed rows whose in-class inline model DEFs the same ctor (COMDAT): 008cf610, 009250f0, 008dfef0, 00925170, 009251e0 (self unchanged; `X(p)` -> `X(*p)`). |
| b6_buyhouse_unit | 3 | 1 | ACCEPT (r457d) | NF456 held unit: 0x0072E690 ctor -> `(CThing&)`, factory 0x004D321B onto ?Alloc@CTCActionUseBuyHouse@@SIPAVCTCBase@@AAVCThing@@@Z (b4 recipe), 0x0072F040's in-class ctor respelt (self unchanged). TABLE_CHANGES 1 replace. NAME_FIXES.tsv: queue line for 0x004DA939 (the in-class CTCActionUseBase(CThing*) spellings, not changed). |
| b1p_r456b_particlegroup_hdr | 7 | 1 | ACCEPT pending r457b + header equiv | r456b: header opt-in FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_DTOR (hdr/engine/, orig/hdr_*.h, HEADERS.tsv) + the 6 deleting dtors re-ported on the shared headers; 0x00BBA140 = NF456's edit unchanged. TABLE_CHANGES 1 add (NF456's). |
| b5x_heroRunningStop_identity | 1 | 1 | HOLD r457a | 0x0085BBA0 -> ??0CCreatureActionHeroRunningStop@@QAE@AAVCThingCreatureBase@@@Z; NAME_FIXES.tsv. Row comment corrected after the first grading and re-graded (tag b5y). |

Gates run per batch (all at one LANDVERIFY_WORK path per tag, tools/runpair.sh; r456b with tools/runpair_hdr.sh = FABLE_HDR_OVERLAY on the new side only):
vland --recheck-landed RELOCATION_MATCH PASS HEAD and new for every row; WARN sets identical (b5a/b5b/b6), strictly smaller (b1p), class-swapped (b5x);
objproof on vland's own objects ALL_EQ with only the intended renames (b1p: plus .drectve /DEFAULTLIB:"libcp", see r457b); link HEAD set v new set:
0 new duplicates, same unresolved (b5b: the same 5 pre-existing duplicates renamed; b6: the same 1 pre-existing duplicate renamed -- the caller
TUs emit the in-class ctor as a COMDAT beside the strong row definition: a HEAD-level link issue, not introduced here); sandbox: every
SELF_SYMBOLS applied, TABLE_CHANGES --dry 0 misses then applied, descriptive_check 0 FAIL with all BEST_GUESS as --extra, self/table clash 0
(tools/clash.py, 47 symbols). Repo oracle TSVs never written (md5 check).

## Finding (for GOTCHAS / FINDINGS)
- Rows that model a ctor in-class for an inline expansion (008cf610, 009250f0, 008dfef0, 00925170, 009251e0, 0072F040, and 0072E690's
  CTCActionUseBase) still emit that ctor as a COMDAT DEF in the lane object; next to the out-of-line row's strong definition the full link
  reports LNK2005 (pre-existing at HEAD). A census by source grep + HEAD-object DEF symbols finds them; the binding table and retail xrefs do not.

## Not done / next (resume point)
1. Rulings r457a..r457e (RULINGS_NEEDED.tsv).
2. Families still open from work/comps.txt (census components; "models" there is over-inclusive: any arity of a class's ctor):
   - 008cf990 TentacleStrikeLoop (J) + 008cfa10 TentacleStrikeOutOf with 008cf700, 008cf7d0 (in-class models).
   - BanditKing 008c94d0 / 008c95b0 / 008c9620 with 008c8c30 / 008c8f70 / 008c91e0 (008c8c30 models a `(CThingCreatureBase*, bool)` form no PDB spells: expect a hold).
   - the 11 one-argument ctors with a landed caller and a table line (census "landed TBL"), and the 10 whose call sites lie outside any row.
   - 006cd890 CGroundNavigatorExternalInterface is NF453-locked.
   - two-argument forms (census lines with more parameters).
3. QuestCard (r457c), TavernGame 0x005EC8A0 (NF453 lock on 0x007D09C0), and NF456's three creature-mode ctors.

## Locks (NF457)
All rows in batches/*/rows.txt (46): b5a 26, b5b 9, b6 3, b1p 7 (taken over from COORD_HOLD_NF456), b5x 1. 008b9150 taken and released. No other locks.
