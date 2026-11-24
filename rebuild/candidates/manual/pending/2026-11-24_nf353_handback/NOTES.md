# NF353 hand-back: FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY family port on the map<CCharString,STB>::operator[] emitters (Account A, 2026-11-24)

Hand-back folder: `$S/w22/agentNF353/` (S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad).
Files: `NOTES.md`, `STATUS.tsv` (one line per candidate: 59 ACCEPT + 9 holes), `batches/b1 b2 b3` (land_scripted run dirs), `evidence/`
(cross-batch drop census, links, witness lines), `tools/` (every helper written). `work/` is scratch: it holds the vland oracle files
(retail bytes) and the objects; it is not part of the hand-back. No header changed, no repo write, no git. STB =
CScriptGameResourceObjectScriptedThingBase. Every lock taken under NF353 is released at the end (`work/locks.log`).

## Scope (brief) and the candidate set
- Start: NF350's census (`pending/2026-11-24_nf350_handback/evidence/census_emitters_00cd3d2e.tsv`): 70 rows define the exact 4-argument
  `??A`. Minus the two landed pilots (0x00EA59E0, 0x00E8EF80) = 68. (0x00D58490 was already UND; the 4 Account B claims and the 2
  non-emitters were never in the 70; 0x00626750 is not in this family.)
- Screen (`tools/screen.py`, `work/screen.tsv`): no Codex reservation (every `pending/*/RESERVED.tsv`), no Account B claim, no lock held by
  another agent, none already defining the opt-in, all define FABLE_RETAIL_STLP_MAP. No landed source folds any of them in
  (`#include "../src/compiled/..."` scan).
- **T3 column-2 holes, excluded (9):** 0x00CBFB7D (nf311, nf326), 0x00D13580, 0x00D19BD0, 0x00D1B200, 0x00D22F30 (pickany_d2inlS15),
  0x00D2D930 (pickany_d2inlS15, nf331), 0x00D52E90 (nf331), 0x00D6D670 (nf331), 0x00E55CE0 (nf294). All five /d2inlS15 rows are among
  them, so the brief's "S15 included only if they pass" had nothing to apply to (ruling request 1).
- **Ported: 59 rows**, 3 batches by address: b1 = 25, b2 = 25, b3 = 9. All 59 ACCEPT; nothing skipped for grade, leaf or pick-any.
  Flags among them: /d2inlT89690 (0x00CE7670), /d2inlT53 (0x00D45060), /Gr (0x00E0A820, 0x00E9A970, 0x00E9B240, 0x00E9BE60).

## Text (per row)
Inserted right before the row's first `#include` other than "fable_compiler.h" (so it precedes stlport/fable_map.h however it is reached):
```
// Fidelity: scaffolding, not retail (NF353 family port, T3 nf340 0x00CD3D2E): FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY keeps map::operator[]
//   declaration-only, so this TU no longer emits its map<CCharString,CScriptGameResourceObjectScriptedThingBase>::operator[] copy
//   (NEQ to retail 0x00CD3D2E: retail's is size-compiled, 118 bytes); the leaf is unchanged and calls the external symbol, which the
//   landed owner row 0x00CD3D2E defines under the same four-argument STLport spelling (NF350 respell).
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
```
The opt-in is TU-wide. 11 rows also emitted map<CCharString,CCharString>::operator[] (NEQ to retail 0x009AC700, 159 v 149, leaf-called);
those rows carry one more comment line naming it: 0x00D33730, 0x00D5AE70, 0x00D5E0C0, 0x00DBB310, 0x00DE6730, 0x00E6BC50, 0x00E6DA80,
0x00E78100, 0x00EB9980, 0x00ED0B10, 0x00F0FB70. No other `??A?$map` instance is emitted by any of the 59 (HEAD-object scan). Tests unchanged.

## Proof per row (all 59)
- **Grade:** vland --recheck-landed, HEAD payload (landed text) v new payload (rows/), run in chunks of 9 (`work/vland_vchunk_*`,
  concatenated per batch into `batches/bN/logs/recheck_{head,new}.log`): see each batch's `evidence/grade_head_v_new.txt`. Also my own
  one-path compiles: RELOCATION_MATCH HEAD and new for all 59 (`work/grades_{H,N}.log`).
- **Leaf:** `leafrel.py` (nf349, section-name keyed relocations) on the row's full mangled `// Leaf:`: data and relocations identical, 59/59.
  objdiff (`tools/objdiff.py`, sections paired by stable keys): 0 sections changed in every row; only COMDATs dropped (23 in 45 rows, 25 in
  0x00CE7670, 29 in 0x00D45060 / 0x00DC1770, 41 in 10 and 46 in 1 of the CCharString-map rows); 0 added; external DEFs only dropped.
- **Pick-any, both checkers HEAD v new** (repo pk_all.py `--tsv --leaf=<full Leaf> --row-length=<catalog>`; pickany_vs_retail.py
  `--leaf=<full Leaf>`), FABLE_EXE and FABLE_ROOT set: pk_all's `# leaf` line names the row's own Leaf on HEAD and new for all 59
  (`PKLEAF OK`, 0 mismatches). `pkcmp346.py`: 0 TURNED_NEQ, 0 NEWLY_NEQ, GATE PASS in every batch; the reversed control FAILs.
  Coordinator ruling (reach flips): `tools/pkreach.py` compares pk_all's leafref / reach columns HEAD v new: 0 flips n -> y in all 59
  (`batches/bN/pk_reach.txt`; the control with sides swapped shows the removed `??A` copies as flips). The pair dtor 0x00CD27B4 (NEQ)
  goes the other way (reach y -> n in 26 rows) because the `??A` copy that reached it is gone.
  Removed leaf-called NEQ copies: `??A` 0x00CD3D2E (59 rows), `??A` 0x009AC700 (11 rows), the pair ctor 0x00CD27C7 (1 row).
  Debt left, leaf-reached NEQ, all NEQ at HEAD too: `_M_erase` 0x00CD344A (59), ~CScriptGameResourceObjectMovieBase 0x006E7B80 (13),
  ~map 0x00CDBFB0 (1), STB operator= 0x008ABD10 (1), CCountedPointer<STB>::ReleaseData 0x007E70E0 (1).
- **Resolve test:** `tools/resolve.py`: every UND the new object adds (1 per row; 2 in the 11 CCharString-map rows) is DEFINED under the
  identical string by the landed owner object compiled from HEAD (0x00CD3D2E; 0x009AC700), whose own grade is RELOCATION_MATCH and whose
  `// Leaf:` is that string. Plus a /FORCE link per batch (`evidence/link_bN.txt`; the file's "HEAD set" label is the negative control =
  the batch's new objects alone, its "new set" = the same plus the two owner objects): control leaves the STB `??A` unresolved 25 / 25 / 9
  times and the CCharString `??A` 4 / 4 / 3 times; with the owners nothing map-operator[] is unresolved. Duplicates: the strong
  `??0CTimer@@QAE@XZ` / `??1CTimer@@QAE@XZ` pairs (0x00D51520 v 0x00D33280; 0x00E7C8E0 v 0x00E26E30) are in the HEAD objects too (no
  row adds a DEF).
  The two owners are T3 column-2 rows (nf311 / nf326 / nf340); they were locked as NF353, prechecked and compiled read-only.
- **Drop test, part 1:** `tools/witness.py`: 0 reloc_bindings lines witnessed by any of the 59 name a symbol the new object no longer
  names (`evidence/witness_lines.tsv`).

## Cross-batch drop census (`tools/dropcensus.py`, `evidence/drop_census.tsv`, `evidence/drop_census_definers.tsv`)
- Batches simulated landed: the 59 new objects (none still defines a dropped symbol). Definers counted only among 159 OUTSIDE objects
  compiled from current HEAD, read-only, under NF353 locks with prechecks: every landed source naming STB and a map outside the 59 (172), except the
  NF354-locked (15) / Account B-claimed (0x00D3A4A0) rows, plus 0x00CD3D2E, 0x00CD3CAE, 0x00CD28BC, 0x00CD3AB6, 0x00905330, 0x00CD27C7,
  0x00CD378D, 0x00626110, 0x0058A0C7, 0x005949C3, 0x00626750, 0x009AC700. Includes the 3 landed pilots and 0x00626750 (as landed:
  DECL_ONLY). EQ = the copy's body compared at the retail address (pk_all.check with an override), not just a DEF.
  Counted twice: all outside, and outside without the 9 holes. It is a lower bound (706 rows define FABLE_RETAIL_STLP_MAP; not all compiled).
- Union of dropped symbols: 50; 10 carry a retail address. Dropped copies that were EQ:
  | copy | address | dropped by | outside EQ definers (all / no holes) |
  |---|---|---|---|
  | `??8_Rb_tree_base_iterator@std@@QBE_NABU01@@Z` | 0x0041C820 | 59 | 15 / 6 (0x005949C3, 0x00626110, 0x009AC700, 0x00D347B0, 0x00D38F70, 0x00E64FB0) |
  | STB copy ctor `??0CScriptGameResourceObjectScriptedThingBase@@QAE@ABV0@@Z` | 0x00905330 | 59 | 9 / 1 (the owner row 0x00905330 itself: its object defines the symbol as a 45-byte COMDAT with selection 1 = no-duplicates, i.e. strong, and grades RELOCATION_MATCH with that symbol as its Leaf; holes 8, all EQ) |
  | `??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ` | 0x007E72A0 | 1 | 85 / 78 |
  | `??0CScriptGameResourceObjectBase@@QAE@XZ` | 0x006E7A60 | 1 | 80 / 72 |
  | `??_GCScriptGameResourceObjectBase@@UAEPAXI@Z` | 0x006E70A0 | 2 | 81 / 73 |
  The other five addressed drops were NEQ at HEAD (`??A` 0x00CD3D2E and 0x009AC700, pair ctor 0x00CD27C7, insert 0x00CD3CAE, less
  0x0042AAB3; less has no EQ definer anywhere in the census, all 16 NEQ: pre-existing debt, nothing EQ was lost). The other 40 dropped
  symbols got no address from pk_all: inline helpers retail expands, plus data COMDATs and destructors pk_all does not address (next item). **Nothing is left undefined:** every dropped symbol that any census or new object references still has an
  outside definer. No count reached zero, so no batch was stopped.
- **Vftable and other unaddressed drops (`evidence/drop_noaddr_copies.tsv`, `tools/datacopy.py`).** pk_all checks function copies only, so
  these were compared object to object: each dropping row's HEAD copy (bytes, relocation offsets / types / symbol names, COMDAT
  selection) against every outside census object's copy.
  - `??_7CScriptGameResourceObjectScriptedThingBase@@6B@` (120 B, selection 2) is dropped by 4 rows: 0x00CE7670, 0x00D45060, 0x00DC1770
    and 0x00E6DA80. 50 outside non-hole objects keep an identical copy (59 counting holes, of 91 outside definers).
  - `??_7CScriptGameResourceObjectBase@@6B@` (8 B) is dropped by 0x00D45060 and 0x00DC1770: 70 identical non-hole copies remain.
  - The same rows drop `??_G` STB (87 B in 0x00CE7670 / 0x00E6DA80: 61 identical non-hole copies; 30 B in 0x00D45060 / 0x00DC1770: 18), the
    base `??1` / `??_G` (73 / 73), `~CCountedPointer<STB>` (55), and the STB / base default ctors (78 / 72).
  - `??0CCountedPointer<STB>(const&)` (25 B, NOADDR, dropped by all 59) keeps 8 identical copies, all in holes, and 0 outside them. No
    census or new object references it UND, so nothing is left undefined.
  So every dropped vftable keeps byte-identical outside copies and the image's choice among them is unchanged. These four rows are ACCEPT
  with the note in STATUS / RESULTS, pending ruling 5.
- Census grades: 158 compiled outside sources: 154 RELOCATION_MATCH, 4 MATCH (`work/census_grades.log`); plus the owner 0x00CD3D2E
  (RELOCATION_MATCH, `work/grades_H.log`).
- **0x0058A0C7 does NOT define `??8` EQ** in my measurement: its copy is 19 bytes, NEQ at 0x0041C820 (pk_all with the address override;
  the size-compiled EQ copies are 16 bytes). The coordinator's note lists it among the EQ definers; 0x005949C3 and 0x00626110 are EQ.

## Rulings requested
1. **Holes and S15.** All five /d2inlS15 rows, plus four more, are T3 column-2 holes and were excluded (59 ported, not ~67). The S15 and
   nf331 lines are debt lines on the row's own copies, which this opt-in addresses. Release them to a follow-up batch (the drop census
   already counts them separately), or keep them out?
2. **self_symbols.tsv is stale for the owners:** its 0x00CD3D2E line still names the 3-argument stand-in and it has no 0x009AC700 line.
   The resolve test therefore uses the owner's `// Leaf:` plus its compiled object, not the table. Regenerate the table at landing?
3. **0x0058A0C7's `??8` copy** is NEQ (19 bytes), so the coordinator's EQ-definer list for 0x0041C820 should drop it (6 non-hole EQ
   definers remain).
4. **NF354 locks:** 15 STB-map rows were locked by NF354 during the census and were not compiled; they are outside the batches and only
   could add definers. One Account B-claimed row (0x00D3A4A0) was left out as well.
5. **Vftable drops.** 0x00CE7670, 0x00D45060, 0x00DC1770 and 0x00E6DA80 also drop data COMDATs (the STB vftable; in two rows the base
   vftable too), which the NF340 drop test, written for function copies, does not name. Each dropped copy has byte-identical copies in 50+
   outside non-hole objects. Accept under the drop test as read for data COMDATs, or hold the four?

## Rule notes
- Prechecks: landed tests before the locks (clang; the 14 that failed only on `_assert` / VC7.1 headers passed `cl /Zs`); sources after
  the locks (clang, or `cl /Zs` where clang failed only on the `~map` specialization error, which fires identically on HEAD and new:
  NF305 Q2); census sources the same way. Every compile ran in the background with its own `timeout 900`.
- 3 rows (0x00EB9980, 0x00ED0B10, 0x00F0FB70) had their comment extended (CCharString-map line) after the first precheck; they were
  prechecked again before their new-object compile (`work/pre.log` CN2_, `pre_zs.log` ZN2_), and the objects post-date the edit.
