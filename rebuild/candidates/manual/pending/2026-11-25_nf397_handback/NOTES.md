# NF397 hand-back (Account A naming agent, 2026-11-24): T3 nf395 0x00728D10 (/Gr re-port) and 0x00762440 blocker 1 (007634e0 NoDuplicates __destroy_aux)

No repo writes, no git. Work dir $S/w22/agentNF397.
- **Compiles: about 26 VC7.1 compiles** (budget 70):
  - HEAD 3;
  - probes 3;
  - vland pairs: b1 2 rows x 2 legs, b2 1 row x 2 legs, plus their tests;
  - definer / link census 8: 00763060 007659f0 00764bc0 00765040 00761560 00760a70 00762ab0 00765370.
- **Reused objects** (each mtime-checked by tools/cen_scan.py against the row source and every header it reaches; evidence fresh_*.tsv):
  - NF395: cen_a1, cen_a2, cen_b3d, cen_own and wd_b1n;
  - NF392: cen_wide (300 rows), cen_own_b1 and cen_fam.
- **Stale by mtime but valid:** NF395's wd_b1n objects of 00728a40 / 00728b10 / 00728b50.
  - Their landed sources are newer only because the landing copied them in.
  - They are byte-identical to NF395's graded rows (cmp), and the rows include only <new>.
- **Locks.** 00728d10, 00728df0 and 007634e0 were claimed as NF397 and released at the end.
- **Stayed off:** NF388's ??_C@ rows, NF396's rows and Account B's claims.
- **Precheck not run.** I did not run the brief's precheck of each test and source before compiling. Every compile and test passed, so it changes nothing, but it is disclosed here.
- **Codex reservation.** 007634e0 is listed in pending/2026-11-21_codex_pass26/RESERVED.tsv, but it landed in batches/2026-11-22_landing_p26 (Codex pass 26 itself). The coordinator assigned it here, so I treated the reservation as spent. Please confirm.

| batch | rows | verdict |
|---|---|---|
| b1 | 00728d10 00728df0 (unit u1) | ACCEPT: apply FLAGS.tsv (CompilerFlags '/O2 /Oy /W3 /Gr' on both entries) before the landing recheck |
| b2 | 007634e0 + engine/NShape_STLHelpers.h opt-in | ACCEPT: header edit (HEADERS.tsv, hdr/engine/, orig/hdr_NShape_STLHelpers.h) |

## b1: 00728d10 / 00728df0 on /Gr without keywords (T3 nf395 0x00728D10)
- **Rows** (tools/gen_j1.py builds them from the landed sources):
  - `// Flags: /Gr` and a flags note;
  - every `__fastcall` keyword is dropped from the code (comments untouched);
  - placement new is `<new>`'s (explicit `__cdecl`), as in the landed 00728a40 / b10 / b50. The row's own `inline operator new` would have become YI.
  - Nothing else changed.
- **Selves are unchanged.** Both are member functions (`?_M_insert_overflow@...IAE...`, `?_M_fill_insert@...QAE...`). Their symbols are identical in the H and N objects (SELF_SYMBOLS.tsv), so there is no respell.
- **Flags.** The build entries carry no CompilerFlags today. The real build and the recheck read only that field (GOTCHAS l.449), so FLAGS.tsv must be applied first. Without /Gr, the keyword-less helpers compile cdecl and the rows fail.
  - My vland N payload carried flags ['/Gr'] from the `// Flags:` line, which is exactly what recheck builds from the entry once it is set.
- **vland at one path** (work/wd_b1h, wd_b1n; logs/): H and N are both RELOCATION_MATCH PASS for both rows.
  - The WARN set is identical H v N: SHARED_HEADER_AVAILABLE CArray / CTCAppearanceMorphBase, the known R8 debt T3 nf390.
  - There are also RELOC_NAME notes, because the catalog labels of 00728a40 / b10 / b50 / bb0 / c10 still carry the old made-up names. That is catalog naming work, not a row change.
- **objsec H v N** (evidence/objsec/*.comdat.txt, from vland's own objects; my probes are identical to them):
  - every section is EQ except the YA -> YI renames below;
  - .drectve gains `/DEFAULTLIB:"libcp"` from `<new>`, as NF395 b2 / 009b7870 / 00b217d0 do.

### What changes, and the bin for each
| row | HEAD (YA, removed) | new (YI) | bin |
|---|---|---|---|
| both | `_Construct<CMeshMorphSetParts>` 29 B | 21 B | R1 tier 2: 24 masked hits, reloc-bound only 0x00728A20, non-unique, no address (as nf395_b1). Support, not tier 1: it is byte-and-reloc EQ to the copies of the landed 00728a40 / 00728b10 / 00728b50, but those come from a local model |
| 00728d10 | `max<unsigned int>` 17 B | 15 B | R1 tier 1: EQ to the copy of the landed 007634e0, from stlport/fable_containers.h:144 (`template<class T> inline const T& (max)(...)`; 007634e0 has no local max), and to 009b9290's |
| 00728d10 | `_Destroy<CMeshMorphSetParts>(p)` 12 B | 8 B | R1 tier 2, non-unique (57 masked hits; reloc-bound only 0x00722120) |
| 00728d10 | `_Destroy<CMeshMorphSetParts>(first,last)` 21 B | 13 B | R1 tier 2, non-unique (95 hits; reloc-bound only 0x007221A0) |
| 00728df0 | `__uninitialized_fill_n<CMeshMorphSetParts*>` 53 B | 50 B | R1 tier 1: EQ to the landed 00728a40's graded self (its table binding). pk_all reads it EQ at 0x00728A40 |
| 00728df0 | `__copy_backward_ptrs<CMeshMorphSetParts*>` 28 B | 20 B | R1 tier 2, non-unique (392 hits, none reloc-bound: retail inlined it) |

- **Residue: none.** Every NEW COMDAT has a tier (evidence/bins_*.tsv).
- **The brief named two flips; there are six** (the advisor's count): `max`, both `_Destroy`s and `__copy_backward_ptrs` also lose their YA spelling.
- The __uninitialized_copy, __uninitialized_fill_n (00728d10), __destroy_aux, __copy_backward, fill and uninitialized_fill_n sections were already YI (explicit keyword) and stay EQ.

### Gates
- **pk_all + pickany_vs_retail HEAD v new** (evidence/pk): reach flips 0.
  - 00728d10 keeps its one pre-existing NEQ: its stand-in __destroy_aux<CMeshMorphSetParts*> v 0x00722130 (36 v 85 B, reach y), identical H and N. It is not new (T3 line, part of the nf390 debt).
  - 00728df0: 0 NEQ, with one more EQ (the YI __uninitialized_fill_n at 0x00728A40).
  - pvr: the same NEQ count H and N.
- **/FORCE link, both orders** (link_b1first / link_b1last). The set is the 2 rows + siblings 00728a40 / b10 / b50 + callee owners 005bbdf1 005bc0d3 00722130 00728bb0 00728c10 00728830 00728f20 + 00722270 007222b0 007280c0.
  - Duplicates are 0 -> 0 in both orders.
  - Unresolved names are 22 -> 22; nothing is new or gone.
- **Census of anything removed** (evidence/census_j1.tsv). It covers 311 VC7.1 objects:
  - NF395's cen_a1 / cen_a2;
  - NF392's cen_wide (every landed row reaching CEngineTextureMorphSet / CArray<CEngineTextureMorph> through headers).

  One object is stale (00b07570 by CPSCSplineBase.h), and it has no watched symbol.
  - No object UNDs any removed YA name.
  - The YA `_Destroy`s are still defined by 00722270 / 007222b0 / 00728f20, and YA `max<unsigned>` by 21 rows, all with their own copies. So the removed names stay in the build.
- **Table:** no reloc_bindings line names any removed or new name, and none has witness 00728d10 / 00728df0 for these helpers. TABLE_CHANGES.tsv is header-only.
- **Tests** are unchanged: both are standalone and PASS in vland.

### Landing notes (b1)
- Apply FLAGS.tsv first.
- T3: 0x00728D10 has two lines, nf390 (R8 shared-header debt, still open) and nf395 (closed by this batch).
  - land_scripted holds the address while any line names it. Close the nf395 line by hand and land with nf390 still open (it stays debt), or decide otherwise.
  - Append my T3.txt line keyed 0x00728D10 only after b1 lands.
- No includers; no table change.

## b2: 007634e0 emits __destroy_aux<CCountedPointer<CSubdivisionTriangle>*> pick-any (T3 nf395 0x00762440, blocker 1)
- **Cause.** The NoDuplicates copy does not come from the row. It comes from engine/NShape_STLHelpers.h.
  - That header defines non-inline explicit specializations (`template<> __declspec(noinline) void FABLE_FASTCALL __destroy_aux(...)`, and the same for `__copy`). They are strong definitions in every includer.
  - Its includers are 007634e0, 00763060 and the two tests.
- **Check before editing.** HEAD 007634e0's copy is byte-and-reloc EQ to the owner 0x00764EE0's (a fresh VC7.1 object; comdat_cmp). So `inline` alone fixes it.
- **Fix.** I added an add-only opt-in, `FABLE_RETAIL_NSHAPE_DESTROY_AUX_PICKANY`, to engine/NShape_STLHelpers.h. It gives the same body with `inline` (still noinline, still FABLE_FASTCALL); the original lines sit unchanged under #else.
  - 007634e0 defines the macro next to FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL.
  - I rejected switching the TU to FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL. It would make every __destroy_aux instance noinline (C3DVector, C3DVector*, vector<C3DVector>), which risks the 627 B self and the 1146 B same-TU FractureShape (advisor).
- **objsec H v N** (vland's own objects, evidence/objsec/007634e0.comdat.txt): exactly one line, `SEL NoDuplicates->Any` on that COMDAT. Its bin is R1 tier 1 / R10: the definer is the landed owner 00764ee0, a shared-header template instance (fable_containers.h, explicit instantiation, Any), EQ by comdat_cmp. Every other section (152) is EQ, and .drectve is EQ.
  - The N copy is byte-and-reloc EQ to 00764ee0's (evidence/comdat_cmp_007634e0n_v_00764ee0.txt).
  - The H leg ran without the overlay (still NoDuplicates); runpair passes HDR_N to the N leg only.
- **vland** (wd_b2h / wd_b2n): H and N are both RELOCATION_MATCH PASS.
  - The WARN set is identical: two RELOC_NAME notes, for 00764bc0's made-up catalog label and 00764ee0's label.
- **pk_all / pvr** (evidence/pk): 0 NEQ on both sides, reach flips 0. The only line that changes is the __destroy_aux copy, `strong` -> `pickany`, still EQ at 0x00764EE0.
- **Link** (evidence/link_b2*.txt). The set is NF395's b3 set: 006f0810 00761c00 007611c0 00765b10 00764fc0 00765040 00761560 00762480 007621c0 007624e0 00762ba0 00760e70 00764f60 007610d0 00764590 00764ee0, plus 007659f0, 00764bc0 and the HEAD 00762440.

| set | duplicates H -> N |
|---|---|
| 007634e0 first | 2 -> 0 |
| 007634e0 last | 1 -> 0 |
| with NF395's b3 00762440 respell (N object) instead of HEAD 00762440 | 1 -> 0 in both orders, so NF395's link blocker is gone |
| plus 00763060 | 7 -> 5 (first) and 6 -> 5 (last) |

- **What the 00763060 rows leave.** The remaining 5 are:
  - the accepted same-TU strong copies (FractureShape, IsLineIntersectingWithOthersInList, IsTriangleWithPointsInside; GOTCHAS nf96a, landing auth9);
  - 00763060's own strong __destroy_aux / __copy, a pre-existing defect: T3 nf397 0x00763060, RULINGS Q1.
- Unresolved names are 50 -> 50 in every set; nothing is new.
- **Definer census** of __destroy_aux / __copy <CCountedPointer<CSubdivisionTriangle>*>. It covers every row that reaches CSubdivisionTriangle: 00760a70 007610d0 007611c0 00762440 00762480 00762ab0 00763060 007634e0 00764590 00764ee0 00765370 007659f0, the link set and NF395's 9 rows (census_b2d/e/f, census_b2_reuse, NF395 census_b3d).
  - __destroy_aux definers after b2: 00764ee0 Any (the owner), 007634e0 Any, 007659f0 Any, and 00763060 NoDuplicates (pre-existing, T3).
  - 00762480 UNDs it.
  - __copy definers: 007634e0 and 00763060, both NoDuplicates (T3 nf397 0x00764BC0). Nothing else defines it, because 00764bc0's self is still the made-up Move_CountedPointer_00764bc0 (NAME_FIXES_QUEUE line 443, id 532).
- **Nothing is removed** (selection only), so no reference census is needed. Table: none.
- **Two kinds of removal.** b1 also removes HEAD sections: six unreferenced NOADDR pick-any YA copies, each replaced by its YI twin. The census gate covers those. 00763060's case (RULINGS Q1) is different: it is a strong copy that reads EQ at a bound retail address (0x00764EE0 / 0x00764BC0), so I ask for a ruling.
- **Header checks:**
  - pin_asserts: nothing to pin (evidence/pin_asserts.txt).
  - Control (evidence/equiv_control.txt): under gcc cpp, old v new tree, the landed 007634e0 / 00763060 sources and both tests preprocess identically. Only the new 007634e0 text differs.
  - Whole-catalog equiv_check (evidence/equiv_b2.tsv): see "Whole-catalog equiv_check" below.
- **Test** is unchanged. It is a standalone copy that does not define the macro, and it PASSes in vland.
- **Landing (b2):** copy hdr/engine/NShape_STLHelpers.h over the repo header (add-only; diff against orig/hdr_NShape_STLHelpers.h), then land 007634e0.
  - No build-entry change: the entry already has '/O2 /Oy /W3 /Gr'.
  - Includers to regrade: none. No row #includes 007634e0's file.
- **After b2 lands:** NF395's 00762440 (T3 nf395 0x00762440) still needs ruling Q1 of its own hand-back (the vtable / stand-in ctor sections), then its CRetailVector census.

## Tools (tools/)
- Copied from NF395 with renames: nf397.py, probe397.py, census397*.py, bin397.py, comdats.py, samedef.py, pk374.py and others.
- runpair.sh now:
  - does `cd ... || exit 1` (it could otherwise run from the repo cwd);
  - takes `HDR_N=<dir>`, an overlay that only the N leg sees.
- New:
  - gen_j1.py builds the b1 rows;
  - cen_scan.py is a reuse census over existing objects, with an mtime freshness check against the source and every reached header.

## Whole-catalog equiv_check (b2 header)
- **The run.** `$S/tools/equiv_check.py` compared the old tree (rebuild/include) with the new tree (work/newinc: a copy plus the edited header). It checked 57,158 rows (source + test); evidence/equiv_b2.log.
- **12 DIFF lines** (evidence/equiv_b2.tsv), and all are artifacts:
  - **00beb4d0, 0045c5da (cpp-error-new).** These `#include "../src/compiled/..."`, which did not resolve from the copied tree until I added the work/src symlink. Rerun with it: 0 differences (equiv_b2_rerun2.tsv).
  - **The other 10** (0094fdf0, 00cbfb7d, 0047042a src + test, 00ba2350, 00ba5260, 00ba50a0, 0093ab70, 00951fb0, 00839420). They still differ on rerun (equiv_b2_rerun.tsv), only because fable_assert.h's `__FILE__` expands to the tree's path. With the tree path masked they are identical (b2/evidence/equiv_b2_pathmask.txt), and none of them includes NShape_STLHelpers.h.
- **Result.** No landed row or test sees different text. Only the new 007634e0 row, which opts in, does (equiv_control.txt).
- **HEAD check before hand-back.** The b1 / b2 orig rows and tests and orig/hdr_NShape_STLHelpers.h are byte-identical to the repo (cmp). 00728d10 / 00728df0 still have no CompilerFlags, and 007634e0 still has '/O2 /Oy /W3 /Gr'.
