# NF424 hand-back (2026-11-25): the compiler-generated deleting-destructor family

## Result
Only generator G1 was written: implicit ~T plus one bound base ~, on a class whose shared header has a VIRTUAL_MODEL opt-in. It covers 165
of the 1625 addresses. Every other shape is in T3.txt.

| batch | rows | verdict | what |
|---|---:|---|---|
| batches/g1_b1 | 7 | ACCEPT | G1 rows whose scaffold copy-ctor COMDAT has a byte-and-reloc identical landed copy from the same shared header (R1 tier 1 / R10): 0x007EBA90 0x007ED040 0x0084AAA0 0x0084AC00 0x0084AC20 0x008550E0 0x00933D30 |
| batches/g1_r1 | 56 | HOLD (R-NF424-1) | every gate passes; only the copy-ctor COMDAT's tier is open: identical to a landed copy from a row-local model (47), or differing / new (9) |
| batches/g1_h1 | 34 | HOLD | failed a family gate (reasons per row in RESULTS.tsv) |

- Locks: only the 7 ACCEPT rows are held as NF424. Every other lock I took is released (the 56 + 34 held rows, 62 CCombatSequence rows,
  3 precheck failures, the 5 pilot rows of the first probe).
- No header edits. No TABLE_CHANGES: none of the old stand-in selves has a table line. No Option S units. NAME_FIXES.tsv is a header only.
- Coordinator step outside the lander layout: CATALOG_CHANGES.tsv in g1_b1 and g1_r1 relabels the catalog name from
  `` `scalar_deleting_destructor' `` to `` `scalar deleting destructor' ``. Without it, vland prints WARN STANDIN_LEAF on the real ??_G leaf.
- One ruling: R-NF424-1, which covers the 56 g1_r1 rows. T3.txt has one line per address that is not ACCEPTed (1618), keyed by shape
  and next step.
- Each batch has rows/, tests/, orig/rows + orig/tests (HEAD copies by address, re-diffed equal to the repo at 08:45), head/payload.json,
  RESULTS, CHECKS, UNITS, SELF_SYMBOLS (current_line re-diffed equal), TABLE_CHANGES (header only), CATALOG_CHANGES, rows.txt,
  logs/vland_lines.txt and evidence/.

## Family and shape histogram (evidence/histogram.txt; tools/family.py, shape.py, sub1.py, hist.py)
- 1625 addresses:
  - 636 `rtti_identified` rows whose RTTI slot names `??_G<C>`;
  - 618 whose slot names `??_E<C>`;
  - 233 rows of the shared self `?CLandscapeBackgroundPatch_vector_deleting_destructor@@...`. The other 4 of its 237 binders are in the
    rtti groups or split.
  - 138 rows of `?CThingSwitch_CompositeScalarDeletingDestructor@@...`.
- **Kind.**
  - 0 of 1625 retail bodies are the vector form: none has `test flags,2` or a call to the eh vector destructor iterator 0x00BFEED8.
  - Every row's Leaf is therefore the scalar `??_G<C>@@UAEPAXI@Z`, including the 618 `??_E` slots. Those come from the debug build's
    vtable alias (FableWin.pdb).
  - Frame and EH frame: 0 rows have either.
  - Size mode (`pop ecx` after the delete): 462 rows. Speed mode (`add esp,4`): 1124. No delete call: 39.
- **Shapes** (count | shape | model):

| n | shape | note |
|---:|---|---|
| 611 | S1 implicit ~T, one call to a bound base ~ | 165 have a VIRTUAL_MODEL header (G1), 321 another header, 125 no header |
| 371 | S0 the two shared stand-in groups | no retail vftable slot names them: non-virtual ??_G or unknown class |
| 146 | S1u one call, callee not bound in the table | |
| 158 | S3 implicit ~T, two calls (member ~ + base ~) | |
| 109 | S5 user-declared ~T inlined (own vtable store first) | |
| 108 | S2 calls the class's own out-of-line ~T | |
| 73 | S1x one call to another class's ~ / stand-in callee | |
| 20 | S4 three or more calls | |
| 13 | S6 vtable store after a call | |
| 12 | S7 class operator delete | rtti rows; 27 more are in S0 |
| 4 | S8 indirect call | |

## Generator G1 (tools/gen_g1.py; worklist tools/wl_g1.py)
- **Shape.** The retail body is `push esi; mov esi,ecx; call ~Base; test byte [esp+8],1; je; push esi; call ??3; add esp,4 (pop ecx);
  mov eax,esi; pop esi; ret 4`.
  - The callee is bound in the table to a real `??1<Base>@@UAE@XZ`.
  - The class's shared header carries a `FABLE_RETAIL_<C>_VIRTUAL_MODEL` opt-in from NF322 / NF325 / NF329: declarations only, slot
    owners probed against retail.
- **Row text.**
  - Line 1, a `// Leaf: ??_G<C>@@UAEPAXI@Z` line, Retail / Name / Fidelity / Model / Callees lines.
  - The class's prelude: the `#define` / `#include` block of the first landed row that defines the opt-in (160 rows), else the
    header's `Needs` line (2 rows).
  - `#include <new>`, and an nf86 / nf88 anonymous-namespace helper that expands the implicit copy constructor (`new (where) C(other)`).
    That copy constructor's vtable store makes VC7.1 emit `??_7<C>@@6B@` and the `??_G` pick-any. This is the 0x009568E0 / 0x009539C0
    recipe.
  - Size-mode rows keep HEAD's `#pragma optimize("s", on)`.
- **Tests.** Each test is a self-contained behaviour model: a base with a counting virtual destructor and a derived class with an implicit
  destructor.
  - `delete` through a base pointer runs ~Base once and frees.
  - An explicit virtual destructor call (flags 0) does not free.
  - The row object cannot link into a test: its ??_7 names 70-80 engine virtuals.
  - The leaf is byte- and relocation-identical to HEAD's (NF419 r1). 2 rows differ only by the callee relocation name (see Gates).
- **Run.**

| step | rows |
|---|---:|
| G1 worklist | 165 |
| skipped: lock (NF423) | 1 |
| held: callee unbound | 2 |
| held: CCombatSequenceBase model (no vtable emitter that compiles cleanly; see T3) | 62 |
| failed clang precheck (abstract class, deleted copy ctor, header assert) | 3 |
| compiled with VC7.1, all RELOCATION_MATCH | 97 |
| ACCEPT (g1_b1) | 7 |
| HOLD on R-NF424-1, every other gate passed (g1_r1) | 56 |
| HOLD, a family gate failed (g1_h1) | 34 |

## Names (two sources per row; RESULTS / row Name lines)
- (1) Retail's own vftable placement and RTTI type descriptor (rebuild/manifest/rtti-vtables.tsv, func_va = the row) name the class.
  - This comes from retail, independent of the donor port (labels_rtti_port_dtors.tsv takes its names from FableWin).
  - The retail body is the scalar form, which VC7.1 emits as ??_G.
  - The raw reference scan (tools/refs.py) finds exactly one data reference for 1544 of the 1625 rows: the vftable slot.
- (2) A PDB name-set entry for the class:
  - FableWin.pdb `??_E<C>` / `??_G<C>` (the vtable alias) and/or `??1<C>@@UAE@XZ`;
  - Ego_r.pdb where present;
  - order_match where it names the same class.
- order_match disagrees with the retail RTTI on some rows (e.g. 0x0062D7D0, where it predicts PutDownTankard). The row text says so,
  and retail decides. No row rests on the donor port alone.

## Gates (per batch: CHECKS.tsv columns; evidence/)
- **vland --recheck-landed, HEAD and new at one LANDVERIFY_WORK path** (tools/runpair.sh; work/vland_g1{a,b}{h,n}.log).
  - All 97 compiled rows grade RELOCATION_MATCH PASS on both sides. 0 LITERAL_DIFF.
  - Rerun of the new side for the 63 g1_b1 + g1_r1 rows under LANDVERIFY_STRICT_LEAF=1, with the oracle `name` column relabelled to
    `` `scalar deleting destructor' ``: 63 RELOCATION_MATCH PASS, 0 WARN (evidence/vland_new_strictleaf_relabelled.log). This proves the
    STANDIN_LEAF warning below clears through vland itself.
  - WARNs removed: SHARED_HEADER_AVAILABLE (182 lines over the 97), RELOC_NAME (4), STANDIN_LEAF (4).
  - WARN added: STANDIN_LEAF on every row. This is a catalog-label artifact. The catalog names the rows `` `scalar_deleting_destructor' ``
    (underscores), which standin_leaf.py does not decode as ??_G.
  - CATALOG_CHANGES.tsv relabels them `` `scalar deleting destructor' ``. standin_leaf.standin() is clear for every row under the new label
    (evidence/standin_leaf_catalog.txt). The landed real ??_G rows (0x009539C0, 0x008CA9A0, 0x00BB1740) carry the same underscore label.
  - No new RELOC_NAME or GENERIC_STRUCT.
- **Leaf v HEAD** (tools/objsec_g1.py, evidence/objsec.tsv, by COMDAT symbol).
  - 61 of the 63 g1_b1 + g1_r1 rows are LEAF_EQ: bytes and relocations equal.
  - 2 rows (0x007EBA90, 0x007ED040) have equal bytes, and their one call relocation moves from an unbound row-local stand-in
    `?Dtor@<C>@@QAEXXZ` to `??1CCreatureActionBase@@UAE@XZ`, table-bound at retail's target 0x00693EF0. NF419 r1 does not cover
    these two. Their grade-level mutation control (VIRTUAL_MODEL define dropped) gives DIFFER (evidence/mutation_controls.txt).
- **.drectve** (work/drectve_g1.tsv, CHECKS column): every new object adds `/DEFAULTLIB:"libcp"` (from `#include <new>`), as in the
  NF412 / NF417 hand-backs. Nothing is removed.
  - Every HEAD section pairs equal (HEAD_PAIRED). The new-only sections are exactly ??_7<C>, the copy ctor ??0<C>@@QAE@ABV0@@Z,
    placement `??2@YAPAXIPAX@Z` and the helper.
- **Vtable** (tools/vtcheck.py, evidence/vtcheck.tsv): VT_EQ for every row.
  - The emitted ??_7 has retail's slot count (70-83).
  - Every slot's symbol is the name the table, self_symbols or tracker binds to retail's slot target. Slot 0 is the row's own ??_E / ??_G.
  - 4 rows have one slot whose retail target has no name anywhere (unverifiable, not a mismatch).
- **Mutation control (grade level).** Dropping the VIRTUAL_MODEL define (0x008550E0, 0x008FEC80) emits no ??_G: DIFFER, "leaf matches
  no symbol" (evidence/mutation_controls.txt). The test is a model (NF419 r1).
- **pk_all + pickany_vs_retail**, HEAD (old self leaf) v new (??_G leaf) (tools/pkrun.sh, pkflip.py).
  - All 63 g1_b1 + g1_r1 rows: `NEW_EQ=1 NEW_NOADDR=2`. Placement new is EQ at 0x00405C10; the copy ctor and the helper are NOADDR.
  - 0 REGRESS, 0 NEW_NEQ, 0 REACHFLIP. pvr: EQ only.
- **Census of new names** (tools/defscan.py, vtcen.py; evidence census_definers / vtable_census columns).
  - 579 landed rows whose source names one of the 97 classes were compiled with VC7.1 at HEAD (work/cen_g1).
  - Watched: ??_G / ??_E / ??_7 / copy ctor of each class, and the old self.
  - g1_b1 / g1_r1 rows: every other ??_G definer is byte-and-reloc identical (G_SAME).
  - Copy ctor (work/csame_src.tsv): for the 7 ACCEPT rows, a landed row that defines the same VIRTUAL_MODEL macro emits an identical
    copy, so R10 tier 1 applies.
  - For 47 rows, the identical landed copies all come from row-local `v00` models (the 0x0066A780 Clone form), and R10 needs the shared
    header: R-NF424-1 (a).
  - For 9 rows the copy differs or is new: R-NF424-1 (b).
  - Every helper form measured emits the copy ctor (placement new, a local copy, a derived probe class), so it cannot be avoided without a
    ctor in the header.
  - Other ??_7 definers: ctor / Clone rows on row-local stand-in models emit 1-slot `??_7<C>` (a `v00` model). That is existing debt
    (T3, below). Ours is the retail-shaped table.
- **Census /FORCE link, address order** (tools/link377.py with a response file). 676 objects: census + the 97 rows; HEAD objects for HEAD
  and the held rows.
  - evidence/link_g1_7.txt (the 7 ACCEPT rows): duplicates 11 -> 11, NEW_UNRESOLVED 0.
  - evidence/link_g1_54.txt (the 54 rows of g1_b1 + g1_r1(a)): the details below.
  - Duplicates: 11 -> 11, the same set.
  - NEW_UNRESOLVED: 0.
  - RESOLVED_TO_GONE: only the 52 old stand-in selves that match the watch pattern. The 2 that do not match are 0x007EBA90's and
    0x007ED040's `?ScalarDeletingDestructor@<C>@@QAEPAXI@Z`. Nothing references any of them.
  - The two stand-in callees `?Dtor@CCreatureAction_LookThroughWindow@@QAEXXZ` / `?Dtor@CCreatureAction_DropWeapon@@QAEXXZ` go from
    unresolved to gone.
  - Each ??_G is defined by the row itself (46) or by an identical lower-address copy (8).
- **apply_table --dry**: 0 misses; multi-target 1 -> 1.
- **reserved_check** (RESERVED_REFS="", 233 reserved addresses): 0 in each batch.
- **s_unit_check / includer recheck**: not applicable (no units, no header change).

## Holds (batches/g1_h1/RESULTS.tsv)
- 14: the scaffold instantiates the base's implicit copy constructor, vtable and ??_G. The model's base (CAIStateGroupBase / CBaseClass /
  CCreatureActionBase) declares no copy ctor in that prelude.
- 14: another landed object emits the same ??_G pick-any with a different body (its model calls ~C or a stand-in). An address-ordered
  link could keep that body.
- 5: retail has an out-of-line copy constructor for the class (a landed strong row). The scaffold's implicit copy is a NoDuplicates v Any
  clash (NF382 b3).
- 1: 0x008F56E0: the implicit copy ctor is NEQ at the bound retail copy ctor 0x008F57F0.
- The 56 R-NF424-1 rows are in g1_r1, not g1_h1.
- Cheap next yield: the 14 "base special members" holds whose base is CCreatureActionBase may clear with the existing
  FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR opt-in (the landed 0x007EBB30 prelude uses it). Check whether CAIStateGroupBase has an
  equivalent, then regrade and regate. Not done in this pass.

## Debt seen (in T3.txt or queue it)
- Landed ctor / Clone rows of these classes (e.g. 0x007EBA20, 0x00855C40, 0x0085BB10) emit a 1-slot `??_7<C>` from a `virtual void
  v00()` model. In an address-ordered link that copy can beat the retail-shaped one. Port them onto the VIRTUAL_MODEL headers.
- The 14 G_DIFF definers (e.g. 0x00855C40 emits ??_GCCreatureAction_BlockCounter calling ??1CCreatureAction_BlockCounter, where retail
  calls ~CCombatActionBase): their models declare a user ~T that retail's class does not have.

## Locks
- Kept NF424: the 7 ACCEPT rows (batches/g1_b1/rows.txt).
- Released: everything else I claimed (160).

## Deviations / notes
- Nothing written to the repo, no git, no wineserver -k. No cl hung.
- One early probe command blocked on stdin and was stopped by its own task id.
- link377.py (NF416's) put every object on the command line. With 676 objects Wine's command line was too long and link.exe printed
  nothing (a false "0 duplicates, 0 unresolved"). The copy here uses a response file and prints LINK_PRODUCED_NO_OUTPUT if that happens
  again. GOTCHAS candidate.
- Tools (tools/): family.py, shape.py, sub1.py, hist.py, filter.py, refs.py, wl_g1.py, gen_g1.py, prep.py, cc.py, vtcheck.py, vtall.py,
  objsec_g1.py, vtcen.py, pk.sh, pkrun.sh, mkbatch.py, t3.py. Also NF416's nf.py / link377.py / defscan.py / vccensus.py / pkflip.py,
  adapted.

## GOTCHAS candidates
- A /FORCE census link of hundreds of objects must use a response file under Wine: a too-long command line makes link.exe print nothing,
  which reads as a clean link.
- 0 of 1625 deleting-destructor bodies are the vector form. A tracker `??_E` RTTI name is the debug build's alias, and the row's Leaf is
  `??_G`.
- The catalog's `` `scalar_deleting_destructor' `` label (underscores) makes vland WARN STANDIN_LEAF on every real ??_G leaf. Relabel it
  to `` `scalar deleting destructor' ``.
