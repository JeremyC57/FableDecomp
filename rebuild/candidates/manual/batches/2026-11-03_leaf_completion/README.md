# 2026-11-03 Leaf completion + labels missing from the manifest (PLAN.md section 4, items 1 and 4)

No compiler authoring, no agents. The only compiles are read-only `vland.py --recheck-landed` regrades.
Nothing was committed. The catalog, the manifest and the labels are untouched: `sync_catalog_names.py --no-dry-run`,
`apply_labels.py` and `sync_manifest_from_catalog.py` are left to the coordinator.

## Task A: truncated `// Leaf:` lines

`tools/complete_leaf_prefixes.py` (new, pure Python) takes every landed source whose `?` Leaf llvm-undname rejects.
When exactly one symbol in `ghidra_out/{fablewin,egor}_pdb_names.tsv` starts with that prefix, and that symbol
demangles to the catalog's qualified name (the `qualified()` / `norm()` of `sync_catalog_names.py`), it completes the
Leaf to that symbol. Every other case is left alone and goes into a report.

### Classification of the 6,396 rows (4,544 distinct Leafs) undname rejected

| verdict | rows | distinct | action |
|---|---:|---:|---|
| complete: unique PDB symbol, same name as the catalog | 1,492 | 1,484 | regraded; **999 completed**, 493 kept (PDB symbol not in the object) |
| refine: the PDB name adds template args / outer scopes to the catalog name | 217 | 211 | regraded, 74 hold (`PROVEN_REFINE.txt`); **not applied**; listed in NAME_CONFLICTS |
| conflict: the PDB name is a different name | 115 | 63 | not applied; NAME_CONFLICTS (regraded: 23 objects compile the PDB symbol) |
| ambiguous: several PDB symbols start with the prefix | 1,137 | 305 | kept |
| no-match: STLport / template instantiations, invented spellings | 2,842 | 1,912 | kept |
| C-decorated (`@ov_pcm_tell@4`), not a prefix | 497 | 488 | kept |
| anon-namespace: the only symbol has `?A0x<hash>` (the hash is per TU) | 71 | 71 | kept (the prefix is the right leaf) |
| skip-respell: catalog `CVtable_` / `ScalarDeletingDtor_` / `C(IV)CountedPointer_Release_` | 25 | 16 | not touched (another pass is respelling them) |

The brief measured 1,839 unique prefixes. That is 1,492 + 217 + 115 + 15 of the 16 skip-respell leaves; the anon-namespace
71 were split out of it.

### Safety: every candidate regraded, not just a sample

`verify_and_land.py` grades the object function whose symbol contains the leaf. It falls back to the largest function,
with a WARN, when the leaf names nothing. All 1,709 complete + refine rows were regraded with the completed symbol as
the leaf (payload rewritten by `mkpayload.py`, 4 parallel splits, `RECHECK_GRADES_full.tsv`). A row holds only when
the grade is MATCH or RELOCATION_MATCH with PASS, there is no leaf-miss WARN, and the completed symbol is in the object's
symbol table (`parse_grades.py`).

- complete: 999 hold (951 RELOCATION_MATCH + 48 MATCH). 493 fail.
- refine: 74 hold, 143 fail.
- **Every one of the 636 failures has the same shape: the PDB symbol is not in the object.** Either the grade fell back
  to the largest function (still MATCH / RELOCATION_MATCH in 607 rows, which is why a grade-only check is not enough), or it
  picked a smaller one (DIFFER, 26 rows), or it never finished (3 NOT_GRADED, the `??1CShape@NShape` rows). No row whose
  completed symbol was in the object failed. The shape cannot be predicted from the text, so only the regrade decides.
  The 636 are kept as prefixes (`RECHECK_EXCLUDE.txt`).
- After applying, the files on disk were regraded: an 84-row sample spread over 0x004..0x00F (4 splits through
  `recheck.py`, which reads the Leaf from the file). 84/84 hold (81 RELOCATION_MATCH + 3 MATCH, all PASS, no leaf-miss
  WARN) (`RECHECK_GRADES_ondisk_sample.tsv`, `verify_ondisk_sample.log`).
- Only Leaf tokens changed: `git diff` covers 999 files, +999/-999 lines, all `// Leaf:`, no CRLF churn (`APPLIED.tsv`).
- The tool now refuses to apply without `--only <proven addresses>`. Its docstring says why: a rerun without the exclude
  list would offer the 493 unsafe rows again.

### SIGNATURE_DIFFS.tsv (636 rows): the reconstruction compiles a different symbol than retail's PDB

These are naming findings ("exact spelling: class key, access, const, calling convention, parameter types"):

| kind | rows |
|---|---:|
| parameter types / const | 194 |
| virtual / static (plus access) | 156 |
| access (public vs protected / private) | 115 |
| qualified name: stand-in classes `CPlayer_Methods`, `CQuestManagerRetail`, a free function for a static member | 77 |
| PDB symbol is Dinkumware STL; retail is STLport, so these are not reconstruction errors | 64 |
| class key (class / struct) | 10 |
| stand-in container (`CRetailVector`, `CUnproven*`) | 9 |
| return type | 7 |
| no object symbol carries the prefix | 4 |

Examples:
- 0x00403C20 `CStringParser::Init`: the PDB has a `__fastcall` callback taking `CCharString const&`; the source has a
  `__cdecl` callback taking `CCharString*`.
- 0x0041E3F6 `NUISystem::CManager::CManager`: the PDB has it protected; the source has it public.
- 0x0040FCA0 `GFSerialiseVectorBinaryOut`: the PDB has `std::vector`; the source uses `CRetailVector`.

### sync_catalog_names.py

`tools/sync_catalog_names.py` was fixed. llvm-undname echoes a symbol it rejects on stdout with no demangled line (the
`error: Invalid mangled name` goes to stderr). The parser now counts those failures and checks them against the stderr
count. It splits the undname rejects into prefix leaves (`?` symbols not ending in `Z`), C-decorated leaves and true
demangle failures, and lists the failures. Output:

| run | output |
|---|---|
| before, original tool (`sync_catalog_names_before.txt`) | `demangle failed 6396, unparsed 26, already consistent 11722, to rename 0` |
| before, fixed tool (`..._fixed_before_apply.txt`) | `rejects 6396: prefix leaf 5898 (4055 distinct), C-decorated 497 (488), demangle failed 1 (1)`; consistent 11722 |
| after (`sync_catalog_names_after.txt`) | `rejects 5397: prefix leaf 4899 (3062 distinct), C-decorated 497 (488), demangle failed 1 (1)`; **consistent 12721** (+999), to rename 0 |

The one true demangle failure is a typo. 0x0057FE4A (`rebuild/src/compiled/00/57/stdRbtree_Mlowerbound_0057fe4a.cpp`)
spells its first pair `U?$pair@$$CBVCCharString@@EMirroredCriminalStatus@...` (`E` where `U` belongs). That Leaf names
no object symbol, so the row is graded against its largest function.

### NAME_CONFLICTS.tsv (332 rows: 217 refinement + 115 different)

The evidence column says whether the landed object compiles the PDB symbol.

- **Placeholder catalog names the PDB fills, compiled by the object:** 0x0041D1C8 `NUISystem::CManager::vfunc_28` →
  `RemoveUIActionParam`; 0x0054FFF0 `NUISystem::CText::vfunc_150` → `GetAlignment`; 0x009F98D0 `_gapscan::sub_009f98d0`
  → `std::list<CProfile*>::remove`.
- **Placeholder names the PDB fills, not compiled by the object (needs a respell):** 29 rows. Among them,
  0x00533B30 `CComponent::vfunc_50` → `GetCurrentState`, 0x00814BC0 `CPhysicalPrimitiveMesh::vfunc_10` →
  `GetClosestPointToPos2D`, and `_global::Vector_Erase` ×5 → `std::vector<T>::erase`.
- **Garbled catalog names whose real spelling is in the object:** `_Sort_heap<std::_Sort_heap<...>` ×7 → the
  `std::sort_heap<...>` instantiations; `list<CTCBase*::remove` ×3 → `std::list<T>::remove`;
  `CGraphicFrame::ScalarDeletingDestructor` → `` `scalar deleting destructor' ``.
- **Stand-in leaves (46 rows):** byte-identical bodies graded under another class's symbol, where the Leaf is not the
  row's identity:
  - 16 rows, `ReturnFalse_*` / `vfunc_21` / `IsBreakBlockValid`, under `CThingFilter_IsUnconscious::operator()`;
  - 15 `*Def::~*Def` under `CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip`;
  - 8 under `CInputProcessManager::~`, 8 under `CChunkedFileChunk::~`, 4 under `C3DAnimationBlendState::~`.

  Each row needs its own class or a shared-header rebase.
- **Different, compiled (check which side is right):** for example 0x004432B0 / 0x0048D520 `CBankFileAsyncData::FreeBuffer`
  whose object is `std::deque<...>::~deque`; 0x00AEEF40 `CParticleEmitterManager::AddParticleSystem` whose object is
  `CGuiVarTransferStruct::Add`; 0x0044F81A `CDefPointeeBase::~CDefPointeeBase` whose object is `CHeroMarriageDef::~`.
- **Refinements (217):** for example `std::vector::erase` → `std::vector<CRGBColour, std::allocator<CRGBColour>>::erase`,
  or `CMazeJournal::Main` → `NScript::CQ_EndGameScript::CMazeJournal::Main`. 74 are proven. To apply them:
  `python3 tools/complete_leaf_prefixes.py --allow-refine --only PROVEN_REFINE.txt`. Then `sync_catalog_names.py`
  respells those catalog names to the PDB form.

## Task B: the 94 `.text` labels the manifest does not list

`MISSING_FROM_MANIFEST.tsv` holds 94 rows, made by `missing_from_manifest.py`. The labels files were parsed raw and ordered
by the date in their names. That raw parse picks up the header-less `labels_forge_autoname_t5.tsv` and the comment-headed
`labels_namepass_2026-10-26.tsv`, which `latest_labels()` skips. The scratch copy (`$S/leafs/`) also carries 24 retail bytes
per row; the committed copy drops them because they are unlanded retail bytes. `.text` ends at 0x0122C8A8, so 0x0122D06C
(`CDiskFileWin32::`vftable'`) is `.rdata` and is the 95th label.

- **2 inside-landed:** 0x00A0F6EA `Deque_InsertRange` and 0x00A0F98B `Deque_InsertRangeReverse` (autoname_t5) are jcc
  targets inside the landed 0x00A0F4B0 `std::deque<C2DVector>::_M_insert_aux` (+0x23A, +0x4DB). They label inlined
  blocks; drop them.
- **2 inside-unlanded:** 0x00499AAD `CProgressDisplay::Initialize` sits inside 0x00499AA0
  `CProgressDisplay::StartProgress`, after `cmp al,bl` (its `je` uses those flags). 0x009CF053
  `CBankFile_ProcessHeroQuestions` sits inside 0x009CEC60 `CBankFile::Update` (a jmp target from 0x009CF352). Both
  manifest rows have no catalog extent. Both labels come from autoname_t5.
- **90 unlisted starts.** Each one is an instruction boundary right after ret, jmp or int3 padding, found by linear
  disassembly from the previous known start. 7 of them start exactly where a landed row ends (contiguous tail stubs).
  The manifest is missing all 90.
  - **21 are referenced** (`PROPOSED_ROWS.tsv`, agent_source `gapscan`; length is the extent to the next start less
    int3 padding, an upper bound):
    - 18 by vtable / `.rdata` slots: 9 `CThing::vfunc_*` stubs (`xor eax,eax; ret`, `ret`, `ret 4`), 3
      `CSoundEmitterLH::vfunc_18..20`, 2 `CSoundManagerLH::vfunc_9/11`, `CTCStockItem::DrawPreparePrimitivesForRendering`,
      0x005BC68D `NInventory::CTCInventoryBase::ProcessButtonXPressed`, 0x00B122C0 `CPSCEmitterGeneric::InterpolateTo`,
      and 0x00CDF130 `NScript::CIsThingAlive::Clone`.
    - 3 by an absolute dword in `.text`: 0x00CA6F80 `EndOfStreamTimer`, 0x00DFCC10 `FUN_00dfcc10`, 0x00ED75F0
      `CV_SwordInTheStoneScript::Alloc`.
    - No label has a direct E8 call.
  - **69 are unreferenced** (`PROPOSED_ROWS_UNREFERENCED.tsv`): no rel32 call or jmp and no absolute dword in
    `.text`, `.rdata` or `.data`. Retail keeps such functions: 6,736 of the landed rows in `.text` have no reference
    either. So these are real functions too, but the brief's call/jump test does not cover them. Examples: 0x00C59600
    `Math_Log`, 0x00CB63F0 `std::iter_swap<...>`, 0x0042BEEA `NUISystem::CDeletion::~CDeletion`, and 0x00987710..0x00987A30
    (7 `CMatrix44` methods).
  - Several labels are placeholders or prose that need real names before landing: `FUN_*` ×14, `vfunc_*`,
    "CMatrix44 multiply this=a*b".

## Files

| file | contents |
|---|---|
| `LEAF_REPORT.tsv` | every rejected Leaf: verdict, completion, recheck, action |
| `APPLIED.tsv` | the 999 edits |
| `PROVEN_COMPLETE.txt` / `PROVEN_REFINE.txt` / `RECHECK_EXCLUDE.txt` | proven and excluded address lists |
| `RECHECK_GRADES_*.tsv` | regrade results |
| `NAME_CONFLICTS.tsv`, `SIGNATURE_DIFFS.tsv` | naming findings |
| `MISSING_FROM_MANIFEST.tsv`, `PROPOSED_ROWS*.tsv` | Task B |
| `sync_catalog_names_*.txt` | sync dry-run outputs |
| helper scripts | `mkpayload.py`, `parse_grades.py`, `sigdiff.py`, `missing_from_manifest.py`; they read and write `$S/leafs/` |
