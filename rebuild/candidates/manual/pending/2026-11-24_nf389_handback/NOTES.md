# NF389 naming hand-back: the CRetailTree_erase pairs (ruling R1), 2026-11-24

Scope: the 154 landed rows (77 pairs) whose own name is a `CRetailTree_erase<callee>` stand-in.
Each pair is two byte-identical 66-byte destructors that call one `_M_erase` (NF385 `rtree_pattern.tsv`: 12 TRIPLE, 65 OTHER).

Result:
- **0 ACCEPT.** No row was respelt, no lock was claimed, and nothing was written into the repo.
- **152 rows UNKNOWN** under R1 item 1.
- **2 rows PROPOSED_HELD:** 0x0051B720 (~map) and 0x0051AF20 (~_Rb_tree). Each has two sources and a byte-matching body, but R1 item 4 (the pilot's emission order) is not met.

11 compiles used of the ~150 budget. No batches/ run dir: nothing passed the gates for a respell.

## R1 item 1: reference sites (retail exe scan)
- NF385's index (`work/xref.pkl`) scans every section for E8/E9 rel32 and abs32 equal to a catalog start. Per pair: `evidence/xref_pairs.txt`.
- 75 pairs: no referrer on either address.
- 0x00569260: one "abs" hit at 0x0043256C. It is false: those bytes are the rel32 of `call 0x0099B7D0` at 0x0043256B (0x0099B7D0 - 0x00432570 = 0x00569260).
- **0x0051B720: two real calls**, at 0x005048C6 and 0x005048D2. They come from CWorldMap::PopulateRegionWithVillagers 0x00502E90 (13.6 KB):
  - each is `lea ecx,[esp+0B4h]` / `[esp+0CCh] ; call 0x0051B720`, two map locals dying at the function's end (`evidence/retail_0x5048c6_sites.txt`);
  - the same function's other exits expand the destructor (it calls `_M_erase` 0x00519970 directly at 0x005062FB and 0x00506354).
- What a compiled TU binds (`evidence/m2.cpp`, /O2, repo STLport, FABLE_RETAIL_STLP_MAP):
  - With 41 map locals, VC7.1 runs out of inline budget, and every out-of-line local death calls `??1?$map` (121 sites). None calls `??1?$_Rb_tree`.
  - With few locals (`m1.cpp`) everything is expanded.
  - So a site where a map local dies out of line binds ~map (`evidence/m2_populate_binds.txt`).
  - Precedent: NF132 named 0x009AC310 ~map on its 34 referrers and left the unreferenced twin 0x009AB770 as the stand-in.
- Elimination for 0x0051AF20:
  - Neither PDB name set holds a multimap or set over `_Rb_tree<CThingBuilding*, pair<CThingBuilding* const, list<CThingAICreature*> >>`.
  - FableWin world_map.obj holds only the map.
  - The table binds neither destructor spelling at a third address.
  - Every compiled object that destroys this map emits both destructors as ANY COMDATs.
  - The retail caller scan of 0x00519970 finds exactly two standalone 66-byte bodies: 0x0051AF20 and 0x0051B720. The other callers are _M_erase itself, clear 0x0051A560, erase 0x0051AF80 and PopulateRegionWithVillagers (`evidence/callers_00519970.txt`).
  - So the other 66-byte twin can only be ~_Rb_tree (QUEUE_PROPOSALS r2 asks whether elimination counts as a source).
- Caveat on m2: the same-region witness 0x0051C920 (vector<set<unsigned long> >::_M_fill_insert) binds ~_Rb_tree 0x00517850 from a set death. It does so through a row-local model that declares ~_Rb_tree only, so it neither confirms nor refutes m2. m2's 121 v 0 is measured for map locals under inline-budget exhaustion only.

## R1 item 2: the table
- No `rebuild/oracles/reloc_bindings.tsv` line binds `??1?$map@`, `??1?$_Rb_tree@` or `??1?$multimap@` to any of the 154 addresses.
- The only lines that touch them are the 77 `?Erase@CRetailTree_erase<callee>` callee bindings, with these rows as witness. None decides a pair.

## R1 item 3: full instantiation (live pair)
- Type: `std::map<CThingBuilding*, std::list<CThingAICreature*> >` (less, allocator default). Two sources:
  - FableWin world_map.obj (`std::map<CThingBuilding *,std::list<CThingAICreature *,std::allocator<...> >,...>`, next to PopulateRegionWithVillagers);
  - the Ego_r name on the landed operator[] row 0x0051BF90.
- Spelling: from the repo STLport (`stlport/fable_map.h` + `fable_containers.h` list). The table already spells this tree the same way, with the two-argument list, in 4 `_Rb_tree` member lines (0x0051AA00, 0x0051AAC0, 0x00514730, 0x0051AF80) and the map::operator[] line (0x0051BF90).
- Compiled bodies, relocations masked, each relocation on retail's target (`evidence/cmp_live_51af20.txt`):
  - ~_Rb_tree, ~map and _M_erase (108 B) are all EQ_MASKED to retail 0x0051AF20 / 0x0051B720 / 0x00519970.
- Exact symbols: `evidence/exact_symbols.txt`.

## R1 item 4: pilot, TRIPLE 0x00B44290 / 0x00B442E0 / 0x00B44320
**The pilot failed on emission order after two honest forms.** The byte comparison passed only as a scratch check: COMDATs from a holder TU, compared with relocations masked. Item 4's actual test was not run (vland MATCH / RELOCATION_MATCH at the leaf from a pick-any row, plus both pick-any checkers); the stop rule applied first.

- Holder: CEngineStaticMapFileBlockManager::Blocks, `std::map<long, CFileBlock>` (shared header engine/CEngineStaticMapFileBlockManager.h). CEngineMap::FileBlockManager owns it.
- `_M_erase` 0x00B441F0 is already landed under the STLport spelling, and the table binds that spelling there (witness: the row itself). The table also gives 0x00B441F0 two other spellings, `?Erase@CRetailTree_erase00b441f0@@` (witness 0x00B44290) and `?_Erase@CResourceTree@@` (witness 0x00B443B0); that is naming debt for the coordinator.
- Retail inline run: `_M_erase 92 | clear 51 | 66 | 54 | 66 | holder ctor 55 | holder dtor 106` (`evidence/retail_b44290_triple.txt`).

**Bytes (scratch, relocations masked): reproduced.** `p4.cpp` + the overlay `evidence/ovl` (the holder destructor implicit, as retail emits it) is the "p5" object. Its compiled COMDATs are all EQ_MASKED to retail (`evidence/cmp_pilot_b44290.txt`):
- ~_Rb_tree and ~map (66 each) against both 0x00B44290 and 0x00B44320;
- ??0?$map (54) against 0x00B442E0;
- _M_erase against 0x00B441F0, clear (51) against 0x00B44250, and the 2-argument _Rb_tree ctor (56) against 0x00B43D30;
- the holder ctor against 0x00B44370 and the holder dtor against 0x00B443B0.

**The 54 / 56 question is settled.** Retail's middle 54-byte function ends in a plain `ret`.
- For 0x00B442E0 it is `??0?$map@...@QAE@XZ`: the no-argument map ctor with the tree ctor expanded. Only this one was measured. In general a TRIPLE middle row is a no-argument wrapper ctor; 0x00869EC0 is landed as `??0?$multimap`, so that pair is ~_Rb_tree / ~multimap.
- It is not the 2-argument `_Rb_tree(const less&, const allocator&)`, which is 56 bytes and ends in `ret 8`. Retail has that one too, earlier in the TU (0x00B43D30, 56 B, EQ_MASKED to the compiled one).
- NF385's exp1 compared the wrong function.

**Order: not reproduced.**

| Form | Source shape | Emitted order |
|---|---|---|
| p1 | holder dtor only | ~RB, ~map, ~Holder |
| p2 | factory, then holder dtor | ~RB, ~map, ~Holder, ??0map, Holder() |
| p3 | holder dtor, then factory | ~RB, ~map, ??0map, ~Holder, Holder() |
| p4 | CEngineMap ctor, then dtor; holder dtor declared | ~RB, ~map, ??0map, Holder(); ~Holder not emitted |
| p5 | p4 + implicit holder dtor (FableWin order: ctor first) | ~RB, ~map, ??0map, Holder(), ~Holder |
| p6 | p5 with CEngineMap dtor first | ~RB, ~map, ??0map, ~Holder, Holder() |

- p5 matches retail except for one position. Retail has `66, ??0map, 66`; every form here gives `~_Rb_tree, ~map, ??0map`.
- Observed, cause unknown (nothing was linked): our objects put the ordinary (NODUP) functions after the inline (ANY) run, while retail lays the TU's ordinary members first.
- Emission order is therefore not a usable source for which twin is which (QUEUE_PROPOSALS r3).
- By the brief and the advisor, the failed pilot blocks the respell of the live pair: it is held for ruling r1.

## Candidate texts (not landed, not vland-graded)
- `proposals/rows/0051b720.cpp` and `0051af20.cpp`: repo STLport, with the 0x009AC310 recipe for the pick-any form (an anonymous-namespace helper calls the destructor).
- Scratch-compiled once each:
  - the leaf is present as an ANY COMDAT;
  - its bytes are EQ_MASKED to retail;
  - the object also carries ANY copies of _M_erase (EQ_MASKED to 0x00519970), clear, the list / pair helpers, and a `??_G` of map / _Rb_tree that the explicit destructor call makes (the 0x009AC310 recipe does the same).
  - Details: `evidence/order_c0051*.txt`.
- If r1 is granted, they still need every gate:
  - vland HEAD and new, plus tests (the current tests are generic tree models and can stay);
  - objsec: not ALL_EQ, because the self, the _M_erase callee and the COMDAT set all change. This is a T3-style port, not a T0 rename.
  - pk_all and pickany checkers;
  - /FORCE link with the landed row 0x00519970. That row defines `?_M_erase@CBuildingCreatureMapTree@@` strongly, which is a different symbol, so no clash is expected; but the new pick-any _M_erase copy has no table line until NAME_FIXES 0x00519970 lands.
  - census and apply_table --dry.
  - Land together with the 0x00519970 respell (NAME_FIXES).

## Locks, stay-off
- Locks claimed: none (no row is changed). NAME_FIXES 0x00519970: not in T3_PORT_QUEUE, COORDINATION_CLAIMS, acctB pending or nflocks.
- None of the 154 is NF387- or NF388-locked or in T3_PORT_QUEUE (`evidence/stayoff_154.tsv`). Three carry `released` locks from earlier agents. 009AB700 appears only in old done Account A claim lines.

## Header debt
- engine/CEngineStaticMapFileBlockManager.h declares `~CEngineStaticMapFileBlockManager()` out of line. Retail's destructor is the implicit inline one, emitted in the TU's inline run (QUEUE_PROPOSALS r4).

## Tools (tools/)
- `exp.py`: compile one scratch TU with the lane cl and BASE_FLAGS (+ extra flags) and list its COMDATs.
- `listsec.py`: list the COMDATs with their selection, no compile.
- `cmpsec.py`: compare one COMDAT with retail, relocations masked, and print each relocation's retail target.
- `rulings.py`: writes RULINGS.tsv and STATUS.tsv.
- `nf.py`: NF385's COFF reader.
