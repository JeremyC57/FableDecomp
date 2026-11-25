# NF437 hand-back (2026-11-25): the CVectorMap<ETCInterfaceType> LowerBound split

**1 batch ACCEPT: `batches/nf437_lb1` (39 rows).** It deletes 16 stand-in table lines at 0x0040F020 / 0x004365B0, rewitnesses 2, and adds none.
Tasks 2 and 3 were not reached because of the context budget. The analysis done so far is in RESIDUE.tsv, T3.txt and NAME_FIXES.tsv.
The coordinator's best-guess naming rule was noted. No row here needed it: no new names, no BEST_GUESS.tsv.

## Census (closes the "who emits the stand-ins" question)
- A retail-byte scan of every landed row found 2,998 rows that call 0x0040F020 or 0x004365B0. Of these, 1,365 use the GetTC expansion.
- Objects were compiled at HEAD for 349 rows: every local-model caller that has no `typedef CVectorMap<ETCInterfaceType` line, plus every row that defines a stand-in map class. All 349 grade the same as the catalog.
  - The objects are in `work/hobj/`. The list is in `evidence/census_rows.txt`.
  - Emitters per spelling: `evidence/lowerbound_emitters_head.tsv`.
- 30 stand-in spellings, 57 emitter rows.
  - One table line has no HEAD emitter: `?find@MapT@@QAEPAUNode@@PAH@Z`, witness 0x0089EC60. Its delete stays with NF435's hold_0089ec60.
  - Other rows also emit LowerBound-like names that are not in the table, so they do not affect the split: `?LowerBound_004365b0@CTCMap_Retail` (0x008D06C0 and three others), `?LowerBound@CTCMap_Retail@@QAEPAUSTCEntry_Retail@@PBJ@Z` (0x006CDC20), `?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@ABJ@Z` (0x00702810), and `?LowerBound@CInterfaceMap_Retail@CThing` (0x0089AE30).

## Recipe (every nf437_lb1 row)
1. Remove the row-local map stand-in class and any same-TU noinline body of it.
2. Add `#include "engine/ETCInterfaceType.h"` (0x00922600, the first pilot, instead keeps the family's local `enum ETCInterfaceType { ETCInterfaceType_FIRST };` as 0x00929F10 does; same mangling) and `#include "engine/CVectorMap.h"`, then `typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > <old name>;`. This is the typedef 1,725 landed rows already use (0x00929F10).
   - `CTCBase` must be the global `class` (PAV). For the namespaced models (0x008E79A0 NCreatureMode, 0x00935160, 0x008385B0, 0x00868B40), the typedef sits outside the namespace.
3. Rewrite the key and entry access:
   - the key becomes an `ETCInterfaceType` local or temporary;
   - `->type` becomes `->first`, `->component` becomes `->second`, and `->last` / `Finish` becomes `end()`;
   - rows whose retail call is to 0x004365B0 call through a `const` map pointer.
4. Grow the pad after the map by 8 bytes (CVectorMap is 0x10).
5. Where the stand-in carried inline helpers (Find / FindPick / Lookup<Key>), use CVectorMap::Find (0x006AB5D0, 0x00783380, 0x0089DF70) or row-local FABLE_FORCEINLINE free helpers of the same shape (0x0089B780, 0x0089BB30 `FindTC` / `FindTCPick`; the maze rows `MazeLookup<Key>` / `MazeLookupConst<Key>`).
   - These helpers are fully inlined. Their unreferenced COMDATs replace the stand-in's own (`?Find@CTCMap_Retail`, `??$Lookup@MazeTable`, ...). They are not retail names: they mark inline expansions and stand in for no out-of-line function.
6. Tests are unchanged. They are self-contained HEAD models, and every leaf is byte- and relocation-identical under the rename (NF419 r1). objgate ignores relocations to `$`-local labels, so this rests on objgate together with vland: RELOCATION_MATCH on both sides, 0 LITERAL warns.

## Gates (nf437_lb1, run tag lb1, one LANDVERIFY_WORK path; evidence/ in the batch)
- **vland HEAD v new:** 39/39 RELOCATION_MATCH with tests PASS on both sides, 0 LITERAL.
  - WARN kinds: HEAD {GENERIC_STRUCT 1, RELOC_ALIAS 1, RELOC_NAME 30, SHARED_HEADER_AVAILABLE 166, STANDIN_LEAF 3}; new {RELOC_ALIAS 1, RELOC_NAME 25, SHARED_HEADER_AVAILABLE 166, STANDIN_LEAF 3}. No new kind.
- **objgate** (`tools/objgate437.py`): 39/39 EQ_UNDER_RENAME.
  - Every paired section, the leaf included, has equal bytes, characteristics and relocations once each stand-in is renamed to the established spelling of its target.
  - Added sections are only shared-header pick-any COMDATs: the CVectorMap LowerBound and Find, and STLport `vector<pair<ETCInterfaceType,CTCBase*> >::begin` / `end`. Each is byte-and-reloc identical to the same-named COMDAT of a landed row (R10 tier 1). The rest are the row helpers above.
  - Dropped sections are only stand-in map members.
  - In four rows (0x00783380, 0x0089B780, 0x0089BB30, 0x0089DF70) the same-TU stand-in body, under the established name, becomes the template body, which equals landed 0x0047A9F0's copy.
  - `.drectve` gains `/DEFAULTLIB:"libcp"`, as every landed CVectorMap.h includer has.
- **Slot check:** 4 rows emit a `??_7` (CThingNoise, MazeCreaturePointer, MazePointer x2). The set is the same as HEAD and the sections pair EQ, so the slots are unchanged.
- **pk_all / pickany_vs_retail:**
  - NEQ is never up: 0x00783380 goes 2->1, and 0x0089B780, 0x0089BB30 and 0x0089DF70 go 1->0. Reach flips: 0 (`evidence/pk_flips.txt`).
  - New copies: 40 LowerBound EQ and reached, plus NOADDR unreferenced begin / end / Find / helper copies.
  - pkgate picked the GetInterface template as 0x008E79A0's leaf (the row has no `// Leaf:` line). With the real leaf `?FindHitEvent@CModeBriarRoseConcentrate@NCreatureMode@@QAE_NXZ`, LowerBound is EQ and reached (`evidence/pk/*_008e79a0.realleaf.pkall.tsv`).
- **/FORCE link**, batch objects plus the definers 0x0040F020 and 0x004365B0 (landed MATCH objects):
  - duplicates fall from 7 to 0 (the strong same-TU stand-in bodies);
  - watched unresolved fall from 25 to 0;
  - no row's UND set gains a symbol (`evidence/und_delta.txt`);
  - a control linking one object and its copy fires 3 duplicates.
- **apply_table --dry:** delete 16, rewitness 2, misses 0, multi-target 1 -> 1, none new.
- **reserved_check** (RESERVED_REFS=""): 0. No row's newest COORDINATION_CLAIMS line is Account B's.
- No row is named in T3_PORT_QUEUE col 2/3: the T3-blocked emitters were left out.
- **name_gate --test**, HEAD v new (`evidence/name_gate_summary.tsv`):
  - In 37 rows the only new FAIL line is the established LowerBound spelling (split_unknown, R-NF435-1). It replaces the HEAD stand-in FAIL line at the same target, and FAIL counts drop (LOCAL_CLASS lines for the stand-in maps go away).
  - 0x0089B780 and 0x00927C80 needed `--flags /GS` and `--flags /Oa` respectively.
  - 1 row has no worse line.
  - 0x0085B160 cannot be graded by name_gate on either side: its anonymous-namespace leaf hash depends on the TU path. vland grades it RELOCATION_MATCH.
- **equiv_check / hdrcheck:** not applicable. The batch changes no header, so it has no RENAMES, CATALOG_CHANGES or HEADERS files.
- **Includers:** `recheck.py --with-includers` on the 39 rows authors exactly the 39. No funclet, Option S thin row or fold row includes them.
- **Units:** `UNITS.tsv` maps rows to units and `UNIT_LINES.tsv` maps each table line to its unit, so a row held at landing drops only its own unit's line.
- **Helper COMDATs:** seven rows emit new FABLE_FORCEINLINE helper COMDATs. Five are row-written (`FindTC` / `FindTCPick` in 0x0089B780 and 0x0089BB30, `MazeLookup*` in the three maze rows) and two are retyped accessors (0x008385B0, 0x0093B8B0). They are unreferenced and NOADDR, with no retail home, so they are not R10 copies; the RESULTS reasons flag them for an R7 bin. Units lb11 and lb18 hold those deletes.
- **head/payload.json** is the HEAD recheck payload (p_lb1h), as in NF423.
- **orig/** was re-diffed against the repo just before hand-back: 0 drift.

## Not single-named yet
After lb1, 14 of the 30 stand-in spellings remain at the two addresses (30 minus 16 deletes; 2 of the 14 are rewitnessed lines). Each is held, blocked by a T3 line, or in task 2/3 (RESIDUE.tsv, T3.txt).
- Two lines are rewitnessed, not deleted, because one emitter is held:
  - `?LowerBound@CInterfaceMap@@QBEPAUSEntry@1@PBJ@Z` moves to 0x006CE730;
  - `?LowerBound@CTCMap@@QAEPAUSTCEntry@@ABK@Z` moves to 0x007E5230.
- `?LowerBound@CTCInterfaceMap@...` keeps its witness 0x0085B5B0, which is held. Its other two emitters are ported.

## Held (locks released)
- 0x006CE730 and 0x0085B5B0: a local `namespace std` model (vector, list) collides with STLport.
- 0x007E5230: DIFFER 817v817. Retail keeps the map pointer in edi across the call; four shapes were tried.

## Task 2 findings (not landed)
- **0x0074AC80** is CTCCreatureGenerator::AddCreatureTriggerer. The evidence:
  - the spelling is in both PDB name sets;
  - the real CTCShop::AddShopper is already catalogued at 0x00773DC0;
  - the address sits inside the CTCCreatureGenerator block;
  - the vector at +0x1C is CreatureTriggerers.

  The self row has a local `std::vector` / CIntelligentPointer model, so the respell needs a re-author on engine/CTCCreatureGenerator.h. Use the UNDEAD, TRIGGERERS and SCRIPTTRIGGERERS opt-ins, the 0x0074ACE0 preamble, and the table bindings `??$__find@...` 0x0074CEE0 and `?push_back@?$vector@V?$CIntelligentPointer@VCThingCreatureBase@@@@@std@@...` 0x0074DA40.
- **0x00A36FE0** is `?Validate@CClassFactory@@QAEXXZ` (both PDB name sets). The body walks ClassList (+0x18, STLport list) and calls CClass slot 1. Re-author it on engine/CClassFactory.h with _CLASSLIST plus NF435's draft _VALIDATE opt-in.
  - The table line `?InterruptAllNavigators@CNavigatorManager@@QAEXXZ` at 00a36fe0 (witness 0x006E72A0) goes when NF435's 0x006E72A0 draft lands.

## Tools (tools/)
- `hobj.py`: parallel HEAD/new compile and grade.
- `lbcensus.py`, `xf.py`, `grpA.py`, `grpB.py`: census and transforms.
- `objgate437.py`, `pkflip.py`, `ngpair.py`, `bdiff.py`: gates and diagnostics.
- From NF423: `runpair.sh`, `pkgate.py`, `link377.py`.
