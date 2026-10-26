# keypass: retail class-keys (`class` V / `struct` U), 2026-10-26

Worktree: `$S/w22/keypass/repo` (HEAD 36337a493). No git was run. Every changed path is in `CHANGED_FILES.txt`
(34,074 paths). No symlinks were made inside the worktree. The baseline root `$S/w22/keypass/base` (outside the repo)
symlinks the worktree but points src/tests/include at the pre-pass copies in `work/orig`, `work/engine.orig` and
`work/inc.orig`. It exists only to tell regressions apart from pre-existing failures.

## Counts
| Item | Count |
|---|---|
| KEYS.tsv rows | 3,675 (3,109 V, 199 U, 366 W4 enum, 1 U/V); 3,633 types + 42 class-template base names |
| Types keyed both ways in retail | 1: `` `anonymous namespace'::RandFunc `` (two different TU-local types; see D2) |
| Engine headers changed | 951: 715 in `engine/` (667 definitions now `class X {` + `public:`, the rest only forward declarations) and 236 in `_quarantine/`; 897 definitions and 668 forward declarations rewritten in total |
| Engine headers left `struct` | 290 in `engine/` and 28 in `_quarantine/`: no retail type reference names them (only their own members), so their key is unknown (mostly CAIStateGroup_* / CCombatSequence*) |
| Hand headers `rebuild/include/*.h` | 8 files, 10 declarations |
| Sources / tests changed | 18,966 sources (`rebuild/src/compiled`) and 14,144 tests. keyconv rewrote 106,951 declarations in 33,108 files: 87,046 definitions, 19,470 forward declarations and 360 elaborated uses struct->class, plus 67 definitions and 8 forward declarations class->struct |
| Leaf lines fixed | 99 by `leaffix.py` (key letters), 21 by `leafobj.py` (against the graded object), and 2 malformed ones by hand (0x4498C0 / 0x449910 `...VCPlayer@J@Z` -> `...VCPlayer@@J@Z`, plus the same typo in their `Retail:` comments) |
| Rows regraded | 21,120 (every catalog row whose source or test changed, or that includes a changed header, transitively) |
| Result | 21,085 good (2,125 MATCH + 18,960 RELOCATION_MATCH, test PASS); 35 not good, all pre-existing (identical status in the baseline, see below) |
| Rows reverted | 0 |
| RELOC_IDENTITY | fired 0 times in any run |
| Bindings dropped | 7,043 (BINDINGS_DROP.tsv), all class-key respellings. 6,436 were re-recorded under the new spelling (same target, same address); the other 607 already had their new spelling in the table. Table: 33,502 -> 32,895 rows, no symbol bound to two targets (none before either) |
| Bindings stale but not this pass | 484 rows: the recorder no longer emits the symbol and there is no class-key respelling. They predate this pass (e.g. 0x4069E0's `?Fmt@FmtObj@@...`). Listed in `work/BINDINGS_STALE_PREEXISTING.tsv`, NOT applied |
| Purity | no `__asm` / `naked` / `_emit` / `volatile` / `_STL` introduced. 3 added lines match the grep, but each only changes the key on a line that already had `volatile` or `g_emitters` |

## The 38 rows the coordinator called regressions
Only 3 of them regressed, and all 3 are fixed. The other 35 have byte-identical grades in the baseline (pre-pass sources,
headers and tests through the same vland): `rg/base/chunk.*.d/out.log` against `rg/full/chunk.*.d/out.log`
(`work/full_out.log`, `work/base_out.log`). No struct reverts were made.

Regressed and fixed (`rg/fix1`):
- 0x76BBC0 std::__partial_sort (SRC_FAIL). keyconv bug: in `template<> template<> struct
  CParticleUpdateParameters<unsigned long,unsigned long>::CParticleUpdateCCompareParamDesc<unsigned long> {`, the key
  belongs to the nested member template, but keyconv read it as an elaborated use of `CParticleUpdateParameters` (V). It
  changed the keyword without adding `public:`, so `operator()` became private. Fix: keyconv now skips `A<..>::B`
  (SKIPPED_NESTED_TID). Re-running it over all originals changes only this file. The source is back to its original text.
  Now RELOCATION_MATCH PASS.
- 0xA326D0 std::pair<long,C3DMeshVolume>::~pair and 0xB6BB10 std::uninitialized_fill_n<CVertexShader*> (DIFFER). Their
  mangled Leaf no longer named a symbol, so the largest function was graded. 0xA326D0's leaf is truncated (`...JUC3DMeshVolume`
  without `@`), and 0xB6BB10's carries backreference keys (`IU1@`). `leafobj.py` rewrote both from the object. Now
  RELOCATION_MATCH PASS.

Pre-existing (status identical in the baseline; not touched):
- RELOCATION_MATCH FAIL: 0x404720, 0x40D2A0, 0x40D400. The test prints a different pass line, or exits non-zero.
- MATCH FAIL: 0xAB6AD0, 0xAB6AE0. MATCH TCC_FAIL: 0x9E1440.
- NOT_GENUINE: 0x5BD993, 0x5DACF0, 0x7022C0, 0x890750, 0x899EF0, 0x9BEF80, 0xC8DB10.
- DIFFER (most have a descriptive display leaf that names no symbol, so the largest function is graded): 0x4AF220, 0x566530,
  0x56C840, 0x59C747, 0x8AA480, 0x8D04B0, 0x8FECEC, 0x900533, 0x918150, 0x93A890, 0x9445C0, 0x944CE0, 0x95EE40,
  0x9CDF50, 0x9DF060 (+TCC_FAIL), 0xA01C50, 0xA8E160, 0xA9C960, 0xAA9190, 0xBC9C30, 0xCAC300, 0xF35B30.
  These are owner items for a catalog-recheck sweep, not class-key work.

Also fixed: 21 rows that still graded good but whose mangled Leaf no longer named a symbol (vland's "WARN leaf ... graded
the largest function"). They are only those whose warning was absent in the baseline, fixed by `leafobj.py` and
regraded in `rg/fix1`: 0x46C3B9, 0x472BDD, 0x493FF8, 0x49DDD0, 0x49DEC0, 0x4B6380, 0x6445BA, 0x6A7390, 0x6B6010,
0x75F4E0, 0x953DF0, 0x99B440, 0x99B8D0, 0x99B9A0, 0x9CB5C0, 0xA39B70, 0xAE1260, 0xAEB850, 0xB6B700 (plus 0xA326D0 /
0xB6BB10 above). There are 3,097 other leaf warnings, but they are pre-existing (descriptive leaves) and identical in the baseline.

## Method
1. `keys.py` (in the repo as `tools/decomp_pipeline/pdb_class_keys.py` -> `ghidra_out/pdb_class_keys.tsv`, identical to
   KEYS.tsv). It runs llvm-undname over the Ego_r (41,882 mangled names) and FableWin (160,969) name sets and collects
   every `class|struct|union|enum <qualified name>` in the demangled text, counted per PDB. The Ego_d name set holds 1
   mangled name, so the cross-check is Ego_r against FableWin: no type is keyed differently between them. 1,048 types
   appear in only one of the two, mostly FableWin-only (TLC) classes. std:: entries are Dinkumware (the PDB STL), while
   the reconstruction uses STLport, so std:: is out of scope and never converted.
2. VC7.1 mangling rule, tested in `work/kt` (cl 13.10.3077 RTM): the DEFINITION's key wins (`struct A; class A{}` ->
   `V`; `class B; struct B{}` -> `U`). With no definition, the FIRST declaration's key wins. So every declaration has to
   agree, not just the definition (C4099 is only a warning).
3. Generator (`gen_class_headers.py`): `class_key()` reads `ghidra_out/pdb_class_keys.tsv`; it emits `class X {` plus a
   `public:` line for V-keyed X, `struct X {` otherwise, and keyed forward declarations. Headers were patched with
   `patch_engine.py` to exactly that output rather than regenerated: on Linux `class_struct.py` hard-codes
   `D:\Documents\FableTLC`, and a regeneration would also pull in unrelated pending reconciliation changes. diff
   engine.orig against engine: only `struct X` -> `class X` lines (1,565) and `public:` lines (897).
4. `keyconv.py files...` (idempotent; it prints a count; `-v` gives a per-file log; `--dry`). It works on a copy with
   comments, strings and preprocessor directives blanked, tracks namespace/class scope by braces, and resolves a name the
   way C++ does (a definition or forward declaration declares <scope>::X exactly; a use is looked up innermost-first).
   Function-local types, anonymous namespaces, invented namespaces (absent from KEYS), std::, unions/enums, template
   parameters and `A<..>::B` are left alone. A definition converted to `class` gets ` public:` after its `{` and
   `public` on any base without an access specifier. A definition converted to `struct` gets ` private:` / `private`
   likewise, unless the body already opens with an access specifier. A definition-shaped match after an unexpected
   lead-in is reported as SUSPECT_DEF and left alone; there was 1, the 0x72B420 test after a macro with no `;`, and it
   was fixed by hand.
   An earlier run (aborted, `rg/full_aborted1`) treated `#define CHECK(c) do {..} while (0)` as code, so definitions after
   it lost their `public:`. Fixed by blanking directives, and everything was re-applied from the originals.
5. `leaffix.py`: resolves each `U`/`V` type reference in a mangled Leaf to its full qualified name (`Name@Q1@Q2@@`), and
   rewrites only exact KEYS entries. `leafobj.py <addr>`: after a grade, picks the object symbol equal to the leaf once
   key letters are ignored.
6. Regrade: `rg_run.sh` / `rg_run2.sh` = `$S/tools/recheck.py` + `vland.py --recheck-landed` in 400-row chunks, 3 in
   parallel, FABLE_ROOT = worktree (`rg/full`, 53 chunks; `rg/fix1`; baseline `rg/base`, 3,139 rows).
7. `bind.py rg/full rg/fix1 --apply`: for every row graded good, `reloc_identity.check` on its lvw object. A table row
   at that address whose symbol is no longer emitted is dropped when the object emits its class-key respelling. The
   respelling is re-recorded (as `record_bindings` would: only when it is new to the table). A row with no respelling is
   listed, not dropped. 4 multi-block rows (0x557C10, 0x5E3EA0, 0x6818A0, 0x6818F0) are RELOC_UNCHECKED, as vland
   reports them, and keep their bindings.

## Decisions
- D1. Unknown key = unchanged. Classes that appear only as the scope of their own members (no `V`/`U` reference, no RTTI
  name in the sets) keep `struct`. A retail symbol can only observe the key through a reference, so these cannot
  mismatch a retail name.
- D2. RandFunc: FableWin has `struct RandFunc` (global, used with Dinkumware random_shuffle) and ``class `anonymous
  namespace'::RandFunc`` in another TU. These are two types, not an inconsistency. Anonymous namespaces are skipped, and
  no landed source declares RandFunc with a mismatching key. No other type is keyed both ways in either PDB.
- D3. Invented namespaces (e.g. `FableIVVector_009cb5c0::CIVCountedPointer`, `FableHeap_00448950`) are not converted:
  the qualified name does not exist in retail. Their leaves keep their own spelling (0x9CB5C0's leaf now says
  `U?$CIVCountedPointer@VCIVCountedPointeeBase@@` = what it emits).
- D4. Retail-`U` types that the reconstruction declared `class` were converted to `struct` with `private:` kept: 67
  definitions and 8 forward declarations (e.g. `CIDrawEnvironmentParams::CThemeChannel` U x54/x272 -> 0x6B6010's leaf
  is now `ABU01@`).
- D5. `verify_and_land.py` header policy: its SHARED_HEADER_AVAILABLE check matched only `struct X {`, so after this pass
  it would miss every local re-model now spelled `class X {`. The regex is now `(?:struct|class)`. This is a warning only
  (strict mode rejects).
- D6. 0x4498C0 / 0x449910 Leafs: Ego_r and FableWin both spell `?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z` /
  `...QBEABVCPlayer@@J@Z`; the landed ones lacked the `@` that closes the CPlayer name.

## Pending improvements (not included)
- A regeneration of `engine/` on Linux is not reproducible (class_struct.py ROOT is a Windows path), so these headers
  were patched. Running the generator on the Windows lane should give the same key lines plus whatever reconciliation
  drift is pending.
- The 290 + 28 unknown-key headers: RTTI type-descriptor names (`.?AV`/`.?AU`) from the retail .data would settle most
  of them (all CAIStateGroup_* / CCombatSequence* are polymorphic). The PDB name sets carry no `??_R0` entries for them.
- The 484 pre-existing stale bindings (`work/BINDINGS_STALE_PREEXISTING.tsv`) are sweep material.
- The 35 pre-existing non-good rows above.

## Files here
KEYS.tsv, keys.py, keyconv.py, leaffix.py, leafobj.py, patch_engine.py, affected.py, bind.py, rg_run.sh, rg_run2.sh,
diag.sh, BINDINGS_DROP.tsv, CHANGED_FILES.txt, STATE.md, rg/ (regrade logs: full, fix1, base, base1, base2,
full_aborted1), work/ (apply_src.log = keyconv per-file log, re.log, leaffix.log, affected.tsv, bind_add.tsv,
BINDINGS_STALE_PREEXISTING.tsv, reloc_bindings.orig.tsv, originals).

## GOTCHAS one-liner (proposal)
- VC7.1 mangles a type with its definition's class-key, else its first declaration's (`V` class, `U` struct), so declare
  every type with retail's key everywhere (`ghidra_out/pdb_class_keys.tsv`; `class X { public:` keeps the members public);
  a `#define ... do {...} while (0)` line is not a statement boundary for text tools.

## FINDINGS_LOG paragraph (proposal)
2026-10-26 class-key pass. Retail's class-keys come from the Ego_r and FableWin name sets
(tools/decomp_pipeline/pdb_class_keys.py -> ghidra_out/pdb_class_keys.tsv: 3,109 class, 199 struct, 366 enum). The two
PDBs never disagree, and only `RandFunc` is keyed both ways (two distinct TU types). VC7.1 mangles with the definition's
key, else the first declaration's. gen_class_headers.py now emits `class X { public:` for V-keyed classes. 951 engine
headers were patched to that, and 106,951 declarations in 33,108 landed sources/tests were converted (keyconv.py:
struct->class + `public:`, class->struct + `private:`, bases keep their access). 120 mangled Leafs were respelled, and the
malformed 0x4498C0/0x449910 Leafs were fixed. All 21,120 affected rows were regraded: 2,125 MATCH + 18,960
RELOCATION_MATCH with tests PASS, 0 RELOC_IDENTITY. The 35 rows that do not grade fail identically on the pre-pass tree.
7,043 reloc_bindings rows were respelled (dropped plus 6,436 re-recorded). 290 engine headers keep `struct`: no retail
reference names them.
