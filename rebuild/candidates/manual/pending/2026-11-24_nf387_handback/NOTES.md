# NF387 hand-back (Account A naming agent, 2026-11-24)

Three jobs: T3 nf381 0x0044C0C0, T3 nf382 0x0067C490, T3 nf383 0x009B7870 (+ 0x0043E1C0 unknowns, nf374 question).
No repo writes, no git. Compiles: 117 VC7.1 row grades (HEAD + new each, tests included) + a few evidence compiles (~125 of 150).

## Run dirs (land_scripted)

| dir | rows | verdict | table | what |
|---|---|---|---|---|
| batches/b1 | 13 | ACCEPT | 5 delete + 1 rewitness | job 1: the 13 named nf381 rows move onto ??0CSubDefClassBase@@QAE@XZ |
| batches/b5 | 29 | ACCEPT | none | job 1 family: the 29 other landed binders of the made-up ??0CBase_0044c0c0 (004e284b left for nf372) |
| batches/b2 | 4 | HOLD_RULING (R1) | 2 delete (NF382's) | job 2: nf382 selves in pick-any form; units u1 00625350+006314c0, u2 0067c490, u3 00733020 |
| batches/b3 | 1 | ACCEPT | 1 rewitness | job 3: 006f3000 binds ??0?$vector<C3DVector> copy ctor (0x006F0810) via stlport/fable_containers.h |
| batches/b4 | 2 | HOLD_RULING (R2) | 1 delete + 1 rewitness | job 3: pilot 009b7870 + 00adc0d0 on engine/CArray.h (one unit: same ruling) |

Each dir: rows/, tests/ (only changed tests), orig/rows + orig/tests, head/payload.json (HEAD payload of exactly those rows),
RESULTS.tsv, TABLE_CHANGES.tsv, UNITS.tsv, rows.txt, T3.txt, logs/ (vland grade lines H and N), evidence/ (objsec, pk H/N[/T],
link + dupdiff, census, apply_table dry). No header edits (no hdr/). b2 also has SELF_SYMBOLS.tsv (NF382's, unchanged leafs).
apply_table.py --dry against the current repo table: b1 {delete 5, rewitness 1}, b2 {delete 2}, b3 {rewitness 1},
b4 {delete 1, rewitness 1}, all four together {delete 8, rewitness 3}: 0 misses, multi-target 1 -> 1, none new
(evidence/apply_table_dry_*.txt). The batches are independent; any order works.

## Gates, every changed row (evidence/)
- vland at one LANDVERIFY_WORK path per run (work/wd_<tag>{h,n}): every row RELOCATION_MATCH PASS on HEAD and new, test PASS.
- objsec (objsec374.py): b1 / b5: ALL_EQ renames=1 drectve BYTE_EQUAL for all 42. b3: ALL_EQ renames=1, .drectve gains
  /DEFAULTLIB:"libcp" from stlport/fable_containers.h (documented exception NF318 Q1). b2 / b4: NOT_EQ unpaired (one extra
  section each); per-COMDAT compare (tools/comdats.py, evidence/objsec/*.comdat.txt): leaf bytes + relocs EQ under the renames,
  the extra section is the reason for each HOLD (R1 / R2). 006314c0: ALL_EQ renames=2, drectve libcp (NF318 Q1).
- pk_all + pickany_vs_retail HEAD v new (evidence/pk_*, pkdiff_all.txt): 44 rows SAME after renames; b2 rows differ only by the
  leaf rename line and one NOADDR strong scaffolding helper; b4 rows show one new NEQ ??4?$CArray@K@@ against 0x0045BC09 that
  reads the OLD table line which b4 deletes: with the b4 table applied to a scratch root (work/troot) it is NOADDR
  (evidence/pk_j3/T_*). Reach flips n->y: 0 over 51 pairs (evidence/reachflip_all.txt).
- /FORCE link of changed rows + callee owners (link377.py; evidence/link_b*.txt, dupdiff_b*.txt): b1 0 -> 0 duplicates, HEAD
  13 unresolved references to the 0x0044C0C0 stand-ins resolve; b5 (link_b1x) 0 -> 0, HEAD 30 unresolved CBase_0044c0c0 -> 1
  (004e284b's: it is in the link set as an unchanged HEAD object, not in the batch);
  b2 3 -> 0 (GONE_DUP CCardDef 004fa1a8 v 0067c490, CConfigOptionsDefaultsDef 004345a0 v 00625350, HasBuildingAsHome 00982db0
  v 00733020); b3 0 -> 0, CRetailVector unresolved 2 -> 1 (009533e0's); b4 0 -> 0. Nothing HEAD resolved is left unresolved.
- Census by compile for each changed table spelling: clang prefilter (evidence/clcensus_j1.txt 689 rows; clcensus_j3.txt 5,127
  rows) + VC7.1 compiles of every clang failure / text hit (evidence/census/: c1 0044fec7 00663f50 007fbe50; c2 007abb30 007d7a00
  008db620; c3 009533e0 00adc120; c4 006f0810; c5 004e284b; c6 009b76f0) + xr of the witnesses (xr_census_witnesses.txt).
- Run r7 (last): comment-only edit of 006f3000 (the layout sentence) and the three respelled test mocks (0044c29c 004d4fcf
  004e373b) regraded HEAD and new: RELOCATION_MATCH PASS; their r7 objects are section-equal to the objects pk / link ran on
  (objsec ALL_EQ renames=0, evidence/objsec/<a>.r7_vs_graded.txt).
- Purity: no __asm / naked / _emit / volatile in any row or test handed back.

## Job 1: T3 nf381 0x0044C0C0 (closable with b1)
- Edit (generator tools/gen387j1.py, family tools/gen387j1x.py): stand-in token (CDefBase_0044c0c0 / CDefBase /
  CShoveHaPennyRegionDef_Base / CDefinitionBase / CBase_0044c0c0) -> CSubDefClassBase, `struct X {` -> `class CSubDefClassBase {
  public:` (same local model landing nf381_b1 used; 155 landed rows model it locally; vland warns SHARED_HEADER_AVAILABLE as for
  those), line-2 `// Name fix (NF387, T3 nf381 0x0044C0C0)` comment; tests respelled where they named the stand-in.
- 00472b63: a derived `WaypointTemp : CSubDefClassBase` emitted an implicit ??0WaypointTemp Any COMDAT (objsec unpaired); final
  form constructs a `CSubDefClassBase def;` and views it through `reinterpret_cast<WaypointTemp&>` (ALL_EQ renames=1). Test
  respelled the same way (native g++ PASS, vland PASS).
- Table (b1): delete ??0CDefBase_0044c0c0, ?Construct@WaypointTemp, ??0CDefBase, ??0CShoveHaPennyRegionDef_Base,
  ??0CDefinitionBase (all at 0x0044C0C0; census: no other landed object binds them; 007abb30 / 007d7a00 DEFINE an inline pick-any
  ??0CDefinitionBase of their own CResource-based model, which then reads NOADDR); rewitness ??0CBase_0044c0c0 0044f969 ->
  004e284b (VC7.1 census c5 + xr: its leaf relocation resolves to 0x0044C0C0). The line stays valid whether or not b5 lands.
- T3: the line closes with b1 (all 13 named rows land). Residual for the coordinator: 004e284b (T3 nf372 0x004E0A05) is the
  last binder of ??0CBase_0044c0c0 once b5 lands (batches/b1/T3.txt).
- Tests: b1 respells every test that named a stand-in (8 that declared it, plus 0044c29c / 004d4fcf / 004e373b whose test-local
  mock base was called CDefBase: regraded in run r7). b5's 29 tests name no stand-in (grep).
- Stayed off 008d9e30 (claims / other T3 line): clang shows it only has `virtual ~CDefBase`, no ??0CDefBase.

## Job 2: T3 nf382 0x0067C490 (pick-any form)
- Form (probed on 0067c490 first): the self moved into the class body (implicit-inline, COMDAT selection Any like the other
  landed TUs' copies) plus an anonymous-namespace `// Fidelity: scaffolding, not retail` function returning the member's
  address so VC7.1 emits the Any COMDAT (nf86 / nf88 pattern). 006314c0 is NF382's row unchanged (its callee is now Any).
  00733020 uses a const member pointer. All graded at their own leaf: RELOCATION_MATCH PASS, tests (NF382's respelled) PASS.
- Result: leaf bytes + relocs EQ, NoDuplicates -> Any, plus one 6-byte NoDuplicates `?Reference...@?A0x<hash>@@` helper section
  per row; /FORCE 3 -> 0 duplicates; pk no NEQ, 0 flips.
- R1 (ruling): objsec374 does not check COMDAT selection and reports the helper as an unpaired section; the scaffolding helper is
  not a documented objsec exception. Accept the helper (as nf86 / nf88 rows carry it) or name another spelling that makes VC7.1
  emit the Any COMDAT. With R1 yes, b2 is ACCEPT, T3 nf382 0x0067C490 closes, and T3 nf345 0x004345A0 closes with it
  (GONE_DUP 004345a0 v 00625350; 0x004345A0 untouched).
- Two forms per unit were not needed: the first form met the line's bar (exact leaf, MATCH, /FORCE 0 new duplicates).

## Job 3: T3 nf383 0x009B7870 (CArray pilot)
- Pilot 009b7870: local body-less `template<class T> struct CArray` replaced by `#define FABLE_RETAIL_STLP_VECTOR_ASSIGN` +
  `#include "engine/CArray.h"`. MATCH; the object binds ??4?$vector@KV?$allocator@K@std@@@std@@QAEAAV01@ABV01@@Z at 0x0045BC09
  (xr). So the binders' calls are the vector operator= (already bound; witness 009b7650), not a CArray symbol.
- Not ALL_EQ: VC7.1 also emits the generated CArray<unsigned long>::operator= as a 19-byte pick-any COMDAT (push arg; push esi;
  mov esi,ecx; call vector op=; mov eax,esi; pop esi; ret 4). Retail has exactly this function, unreferenced, at 0x009B5A60
  (int3-preceded, 16-aligned; tools/findwrap.py), and the landed 009b76f0 already emits the identical Any copy (link: 0 dups).
  R2 (ruling): allow a pick-any COMDAT that byte-equals a retail function (here unreferenced, kept once by the retail linker) as
  an objsec exception, or keep these binders on a declaration-only CArray. FABLE_RETAIL_STLP_VECTOR_C (out-of-line vector body)
  was tried as evidence only (work/ev/009b7870_evC.*): still 243 B NEQ vs 184 retail; not a route.
- 00adc0d0 (b4): Ego_r.pdb CVertexGroupBlock = {CArray<unsigned long> Groups; std::map GroupToIndexMap}; re-modelled through
  engine/CArray.h (leaf EQ, binds vector op= at 0x0045BC09, same extra CArray forwarder) -> held with the pilot. A plain
  std::vector member form (work/00adc0d0_form1_vector.cpp) is ALL_EQ but contradicts the PDB, so not handed back.
- 006f3000 (b3, ACCEPT): record = Ego_r CCombatDebugInfo layout; two std::vector<C3DVector> members with
  FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY + FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE (retail calls ~vector<C3DVector> 0x004DCFBB
  out of line). First form without DTOR_OUTOFLINE emitted 11 dtor-chain COMDATs (work/006f3000_form1.cpp). ALL_EQ renames=1.
  The vector spelling is also the PDB's: Ego_r CCombatDebugInfo = Zone, Attacker, AttackerNumber, Defender, MovingByAction,
  CurrentSequenceName, PlacementVectors / AttackerPos as vector<C3DVector> (ghidra_out/struct_layouts_egor.tsv). The self
  (CRecord_006f3000) and its member names stay as landed: the self respell to ??0CCombatDebugInfo@@QAE@ABV0@@Z is queued
  (NAME_FIXES.tsv, QUEUE_PROPOSALS q3), since it changes the self symbol and needs its own census.
- Per-binder blockers (not ported; the script waits for R2):
  - 009b77b0: CArray port emits ~10 extra pick-any COMDATs (dtor chain + forwarder): HOLD, not in any rows/ (lock released).
  - 00728d10: local namespace std + out-of-line ~CArray (0x005BC0D3) model; needs ~CArray decl-only opt-in in engine/CArray.h.
  - 00bd8b30: local std tree model (map) besides CArray; needs fable_map.h port first.
  - 00b217d0: inline ~CArray chain; retail has a 25-byte forwarder at 0x00B217B0 (dtor), so the port needs a dtor decision too.
  - 00709c90 / 00709ce0: local std + a global __false_type in the leaf mangling -> the self respells (unit with its catalog name).
  - 00728a40 / 00709a50 / 00762440: leaf parameters carry stand-in types -> the self respells; one unit each.
  - 00762d80: unit with T3 nf374 0x00762580 (below).
- T3 nf374 0x00762580: 00762d80 binds 0x006F0810 through CRetailVector and its own CreateShape / AddNewShape manglings carry the
  CShapePointer / CPointVector stand-ins; PDB ?AddNewShape@CTCShapeManager@@QAEXV?$CCountedPointer@VCShape@NShape@@@@@Z. It needs
  one unit: 0x00762580's spelling + 00762d80 on engine/CCountedPointer.h + stlport vector, with a census of the 0x00762580 stand-in.

### J3-U: the 14 lean-vector unknowns of T3 nf383 0x0043E1C0
Method: search retail for the 19-byte forwarder shape the pilot proves a generated CArray<T> member compiles to (tools/findwrap.py,
wrapscan.py, wrapinfo.py; evidence/wrapscan_*.txt, wrappers_19B*.txt). Controls: 0x0045BC09 (fwd 0x009B5A60), 0x005BBDF1
(0x00709A30, 0x00709BB0), 0x006F0810 (0x0075F6A0): all unreferenced, int3-preceded, 16-aligned.
| self | forwarder(s) | decision proposed (R3) |
|---|---|---|
| 005295b0 | 0052aab0 | vector body (CArray forwarder in front) |
| 00a46790 | 00a469c0 | vector |
| 00a5e2e0 | 00a5e5a0 | vector |
| 00f3b640 | 00f3b730 | vector |
| 00a3cdd0 | 00a3d000 | vector |
| 00a5e0a0 | 00a5e5c0 | vector |
| 00a87090 | 00a870f0 | vector |
| 00ae5d80 | 00ae6660 | vector |
| 006b6840 | 006b6c80, 006b6d30 | vector (PDB: CEnvironmentThemePriorityStack / CLipSyncFrameData sole CArray members: two CArray wrappers) |
| 00a49c60 | 00a4a4f0, 00a4a870 | vector |
| 00515ad0 | 0051a1b0 (inside a 129-byte catalog function: boundary conflict; the self is a size ctor, not a generated member) | stays unknown |
| 0043e1c0, 006f15e0, 009f6a70 | none found | stay unknown |
All 14 selves are already vector-spelled: no respell either way. The twin-body search (evidence/twinbody.txt) does not
discriminate (controls also have same-size twins). R3: rule the 10 as decided vector (forwarder + pilot = two sources).

## Rulings needed
- R1 (b2): accept the nf86/nf88 scaffolding helper section as an objsec exception for pick-any re-rows (or name the spelling).
- R2 (b4, and the rest of nf383 0x009B7870): accept a pick-any COMDAT that byte-equals a retail function (0x009B5A60) as an
  objsec exception for CArray ports. Census caveat: the b4 delete of ??4?$CArray@K@@ -> 0x0045BC09 rests on the clang census
  plus an argument for the 86 clang-failing CDefinitionManager rows (they model CArray<T> : vector<T> with the generated
  assignment, so they cannot reference it), not on a VC7.1 compile of each; a flip of b4 to ACCEPT should first compile-census
  those rows (or the subset naming CArray<unsigned long>).
- R3 (nf383 0x0043E1C0): decide the 10 forwarder-backed selves as vector.

## Process note
- One `git log` call was attempted by mistake (to read landing history); the permission layer blocked it, so no git ran. No repo
  writes.

## Locks
Held by NF387 for the 49 rows in batches (b1 13, b2 4, b3 1, b4 2, b5 29); 009b77b0 released (not handed back).
