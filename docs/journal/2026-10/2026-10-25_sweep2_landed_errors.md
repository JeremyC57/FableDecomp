# sweep2: landed-errors sweep 2, 2026-10-25

Hand-over for the coordinator. Every RENAMES and FIXES row was regraded fresh (MATCH or RELOCATION_MATCH) and its
test printed `<addr>_TEST PASS` under VC7.1 before it was appended (`tools/row.py`). Nothing under the repo was
written and no git was run.

## Counts (data rows, header excluded)
| output | rows | split |
|---|---|---|
| A. RENAMES.tsv | 184 | 1a 6, 1b 94, 1c 44, 1f 4, 1g 8, queue (2) 25, queue (3) 3. Grades: 105 MATCH, 79 RELOCATION_MATCH |
| B. FIXES.tsv | 139 | 1e 2, 1g 125 (120 of them the pGlobalScriptDef spelling), queue (2) 11, queue (3) 1. All RELOCATION_MATCH |
| C. LABELS.tsv | 92 | 1e 2, 1g 1, queue (2) 23, queue (3) 66 (RTTI/PDB-checked ctors, dtors and sole-slot vfuncs) |
| D. UNRESOLVED.tsv | 43 | 1g 2, queue (2) 8, queue (3) 33 (rejected, not provable, or already correct) |
| BINDINGS_DROP.tsv | 101 | stale `reloc_bindings.tsv` rows. Each names the row whose regrade made it stale ("no longer emitted") |

Source files: renames/ holds 368 files (184 sources + 184 tests). fixes/ holds 142 files: 139 sources plus the 3 tests
that had to change (0x5DBFD0, 0x629F50, 0x9C2900).

Final checks, run over every output row:
- `tools/l1chk.py`: line 1 and the name agree, 0 flagged.
- `work/leafchk.py`: exactly one `// Leaf:` that names exactly one object symbol by exact prefix, at most one
  `// Flags:` line, 0 bad.
- `tools/bdrop.py`: compares the reloc bindings of each regraded object with `reloc_bindings.tsv`. Every
  "no longer emitted" pair is in BINDINGS_DROP. No symbol is bound to two targets.

## Method
- **Tools.** Grading uses the sweep1 tools, re-pointed at sweep2:
  - `tools/g.py`: grade, `-v` for listings.
  - `tools/row.py R|F`: regrade, then replace or append the row.
  - `tools/l1chk.py`.
  - `tools/bdrop.py`.
  - `cat.py`, `xr.py`, `vt.py`, `rtti_vt.py`.

  New scripts in work/:
  - `1b/gen_iter.py`, `gen_find.py`, `gen_iu.py`: one STLport 4.6 template model per family. The `spec_*.py` files
    hold one row per address with the key and value types, constness and key signedness.
  - `1b/ident*.py`: finds which map/set instantiation each stand-in serves. It walks callers up to the owning map and
    reads the Ego_r member type.
  - `leafchk.py`, `leaffix.py`: leaf checking and repair.
  - `pairs.py`: lists the reloc bindings of one row.
  - `xref_all.py`.
  - `q3chk.py`: the queue (3) checker. It looks at the RTTI vtable store order, the RTTI base list, the PDB name set
    and whether the bytes end where expected.
- **Evidence.** Every rename is backed by at least two sources:
  - a PDB name set (Ego_r, Ego_d or FableWin; used as name sets only);
  - RTTI (rtti-vtables.tsv, plus type descriptors demangled with msvc_demangle.py);
  - xrefs and callers;
  - the bytes.

  libpk has no PDB or RTTI. Its names are descriptive and the evidence column says so.
- **Leaf normalisation.** 132 of this pass's rows first had a Leaf that was missing or matched only as a substring,
  and grading silently used the largest function instead. `leaffix.py` set exact mangled prefixes (`??F?$_Rb_tree_iterator@`,
  `?find@?$_Rb_tree@`, `??$insert@...`), and every one of those rows was regraded and re-appended.
- **Call targets.** Each regraded object's relocations were compared with `reloc_bindings.tsv` (bdrop.py, `objdump -r`).
  A binding that moved to a real name is either already right in the table or listed in BINDINGS_DROP for removal.

## Decisions (owner items 1a-1g)

**1a: nested `Ns::std` moved to top-level `std`.** This was done for the six rows whose `// Leaf:` mangles the nested
namespace: 0x462CB0 vector<CBrainUpdateZone>::operator= with its _M_clear 0x462E04, 0xA4A520, 0x7871D0, 0x8085C0 and
0x6E22F0.
- **What the rows are now.** Each is a real STLport 4.6 `vector::operator=` in top-level `namespace std`
  (`_Vector_base` + `_STLP_alloc_proxy`).
- **Helpers.** The out-of-line helpers are `__fastcall` and declaration-only. The exception is where the retail frame
  needs a same-TU noinline body (`NOINLINE` in work/vec/gen.py): 0x8085C0 and 0x6E22F0.
- **The other 19 sources.** These landed sources open a `namespace std` inside another namespace, but it does not
  reach their leaf. Their std model is either a wrapper (`X_00756250` and similar) or an insert_unique /
  _M_insert_overflow over a CRetailTree-style stand-in. Moving them means rewriting them as real STLport templates.
  That is 1b work; see 1b for the ones that were done.

**1b: CRetailTree stand-ins replaced with STLport 4.6 templates.** 94 rows (RENAMES) in three families. The largest
families were done first, and the families whose element type could be proved went first.
- **Iterator `operator++` / `operator--`: 59 rows** (23 `++`, 36 `--`). The old names were
  CRetailTreeIterator::Increment / Decrement.
  - **Model.** `_Rb_tree_iterator<V, _Nonconst_traits<V> >` with the static `_Rb_global<bool>::_M_increment` /
    `_M_decrement`.
  - **Picking the instantiation.** `work/1b/ident.py` walks callers up to the owning map or set, then reads the
    Ego_r member type. A row was taken only when it reached exactly one PDB member type.
  - **Rows skipped.** 170 Inc/Dec stand-ins were identified. The rest were skipped because they had no PDB member,
    several candidate containers, or no reachable owner.
- **`_Rb_tree::find`: 18 rows**, including const/non-const twins. The old names were CRetailTree_longkey /
  _ulongkey::find.
  - **Constness.** It comes from the callers' PDB names (QAE or QBE).
  - **Key signedness.** It comes from the compare (setl/jl or jb).
  - **Type spelling.** Pointer keys must be spelled `pair<K const, T>`. The generators substitute K as text, so
    `pair<const CThing*, T>` would mangle `PBVCThing@@` (pointer to const) instead of retail's `QAVCThing@@`
    (const pointer).
  - **Rows skipped.** 81 find stand-ins were found. The rest had no unique owner or a non-PDB key.
- **`_Rb_tree::insert_unique(const value_type&)`: 17 rows.**
  - **Model.** The verbatim STLport 4.6 body, with the private `_M_insert` (AAE).
  - **Matching trick.** Retail calls `operator--` out of line. A same-TU `auto_inline(off)` + `__declspec(noinline)`
    real `operator--` body makes VC7.1 home `__j` in the dead `__v` slot, and that is what makes it match.
  - **Pairing.** Each row's decrement callee is that map's renamed `operator--` row, so the two share one symbol.
- **Access.** New models use the 1d access from the start.
- **Remaining.** 1,032 landed sources still mention CRetailTree (1,126 before this pass). By line-1 family:
  - `~CRetailTree_erase` 227;
  - Decrement 73, find 64, operator= 54;
  - insert 48, `_Insert` 48, InsertUnique 42;
  - Increment 38, Copy 35, Erase 24;
  - `distance<CRetailTreeIterator>` 22, Insert 16, EraseRange 11;
  - smaller families after those (`_M_copy` 8, BuyNode 5, Contains 4, ...).

  The `_M_insert` rows that the 17 insert_unique rows call are still stand-ins. Those rows now bind the real private
  `_M_insert` symbol to them, so landing them as STLport `_M_insert` is the natural next family.

**1c: Xiph module spelling unified on `libogg` / `libvorbis`.** 44 rows were renamed (RENAMES, module `libogg` or
`libvorbis`).
- **vorbisfile rows** go to `libvorbis`, because vorbisfile ships in the libvorbis tree.
- **What changes.** Each source keeps its body; only line 1 and a Module note change.
- **Not done:**
  - 0xCA3B10 FtoL = vorbis_ftoi. It is `retail_asm` (a bare `fistp`) and is already in sweep1 UNRESOLVED.
  - 0xC8C730 and 0xC8DB10 are asm_bake rows, which this pass cannot land.
- **Not Xiph.** 0xC89BE0 FtoL is a libpk reverb helper, so it keeps its module.

**1d: STLport `_Rb_tree` member access.**
- **No PDB evidence.** No PDB name set contains an `_Rb_tree` symbol. The Ego_r, Ego_d and FableWin sets are
  Dinkumware (`_Tree`).
- **Source used instead.** The access comes from the STLport 4.6 `_tree.h` declaration (SGI heritage):
  - private (mangled `AAE` / `AAI`): `_M_insert`, `_M_copy`, `_M_erase`;
  - protected (`IAE` / `IAI`): `_M_create_node`, `_M_clone_node`;
  - public (`QAE`): `insert_unique`, `insert_equal`, `erase`, `find`, `lower_bound` and the rest.
- **Deferred.** Applying this to the landed trees means 159 sources. Several of their tests call `_M_insert` /
  `_M_erase` directly and would need a `friend` or a restructure, so the application pass was deferred and is not in
  FIXES. All 94 new 1b models already use this access.

**1e: swap vs iter_swap at 0x4B7800 / 0xCB6230.**
- **Both addresses are `std::swap<pair<..> >`, so the landed names are correct.** The bytes show two `__fastcall`
  reference arguments in ecx/edx and a plain `ret`. A 3-argument `__iter_swap(a, b, _Tp*)` would end in `ret 4`.
- **The out-of-line `iter_swap` copies are 0xCB63F0 and 0x4B86A0.** Each is a bare `jmp` to the swap (LABELS).
- **The partitions 0xCB67B0 and 0x4BA080 are FIXED.** They had declared an out-of-line `iter_swap` bound to the swap's
  address. They now use STLport's inline `iter_swap(i1, i2) { swap(*i1, *i2); }` over a declaration-only
  `__fastcall swap`. Results:
  - both still grade RELOCATION_MATCH;
  - their bound symbol is now exactly the swap row's own leaf (`objdump -t` / `-r` compared);
  - the TU emits the same bare-`jmp` iter_swap COMDAT that retail has at 0xCB63F0 / 0x4B86A0.
- **Tests and bindings.** The tests are unchanged (standalone models). The old `??$iter_swap@..._STL@@` bindings go
  to BINDINGS_DROP.

**1f: 0x81E0F0 vs 0x470CF1, one `std::sort<pair<unsigned long,CSoundPair>*,..>` symbol.**
- **0x470CF1 is right.** Its chain is CSoundMap::TransferBinaryIn 0x47042A, then CVectorMap<unsigned long,
  CSoundPair,..>::Sort 0x470CD8, then 0x470CF1.
- **0x81E0F0 is a different instantiation.** A single symbol cannot have two retail bodies, because COMDAT folds by
  name.
  - **Where it sits.** 0x81E0F0 is in the morph TU, next to CSkeletalMorphs::TransferBinaryIn/Out 0x81AED0 /
    0x81ADC0, CParticleMorphs and CExpressionSet.
  - **What the bytes show.** The median of its introsort loop compares keys unsigned (`jae`/`jb` at 0x81DDF8), and
    the pairs are 12 bytes.
  - **Which map fits.** Ego_r gives CSkeletalMorphs::Morphs = CVectorMap<unsigned long, CSkeletalMorphs::CEntry>, a
    12-byte pair. The neighbours don't fit: CExpressionSet's key is an enum, which would compare signed, and the
    CParticleMorphs pair is 40 bytes.
  - **PDB names.** The FableWin.pdb name set holds both `sort<pair<unsigned long,CSkeletalMorphs::CEntry>*,..>` and
    that map's `Insert` / `Sort` / `Update`.
- **The wrong row was 0x81B220** (landed as CVectorMap<unsigned long,CSoundPair,..>::Insert). It is renamed to the
  CSkeletalMorphs::CEntry map. CEntry follows Ego_r: CDefString MorphName +0, short Priority +4, bool
  SupressesLowerMorphs +6.
- **The rest of the family was renamed with it,** so the shared callee is one symbol:
  - 0x81E0F0 becomes std::sort (was the stand-in Sort_SElement12_0081e0f0);
  - 0x81E190 becomes ::Sort (was "CGuiVarTransferBool::TransferToGui");
  - 0x81E1F0 becomes ::Update (was "CTCVillage::OnInitialActivate").

**1g: the rest of sweep1's owner list.**
- **0x494106 and 0x493CF5 (stale bindings) and the non-nested ctor 0x48DDF6.** The subtitle text-info queue is a
  real STLport `queue<CSubtitleRenderer::CSetTextInfo>` over `deque`, and CSetTextInfo is nested in the PDB
  (`??0CSetTextInfo@CSubtitleRenderer@@`).
  - RENAMES:
    - 0x4935A7 and 0x48DDF6 become the nested CSetTextInfo ctors;
    - 0x494106 becomes `deque::_M_push_back_aux_v`;
    - 0x4941C0 becomes `deque::push_back`;
    - 0x493CF5 becomes `deque::pop_front` (stride 0x1C, `_Destroy` through ~CSetTextInfo).
  - FIXES: 0x49319C SetAsInCutscene now binds the renamed pop_front.
  - LABELS: 0x4942BE `queue::push` (a bare jmp).
  - UNRESOLVED: 0x494168, the queue default ctor, which is landed as CGameEvent::AddUBYTE. It was left out because
    it needs the `_Deque_base(alloc, n)` callee identified first.
- **0x5BAF94.** The comment is fixed: the 15 insert callers are in two functions (5 + 10).
- **0x99A2F0.** The Leaf was missing and the model class was wrong. It is now CBaseClassNonCopyable's protected ctor
  with a polymorphic protected dtor, Leaf `??0CBaseClassNonCopyable@@IAE@XZ`. 0x99A300 is its dtor, `MAE`.
- **0xF37390.** The Leaf was added; the body is unchanged.
- **Catalog names lagging the sources** (0xC0BCC0, 0xCAF580, 0x431F10). The landed sources already carry the right
  names, so only the catalog changes. No row is needed: this is a catalog regeneration item (owner list).
- **0xC31B90 and its family.**
  - RENAMES: 0xC31B90 is `PKSerialiseCParser::TransferString`, with its family 0xC31B30 Transfer and 0xC31B50
    TransferSize.
  - Naming: descriptive, because libpk has no PDB or RTTI. The class comes from the body's error strings and the
    callers' vtable.
  - UNRESOLVED: 0xC31B60 / 0xC31B70 / 0xC31B80, whose value types are disputed by their callers.
- **0xC73A00.** The CString_00c73a00 placeholder now binds the landed libpk CString members: static `SafeStrlen`
  0xC0BCC0, `CompareNoCase` 0xC0BD60 and `~CString` 0xC0BA40.
- **0x9C7060 `_Retail` layout classes** are header debt (owner list). They are not fixable per row.
- **`$E2` at 0x121AEC0 = `_$E1`** is a catalog item (owner list).
- **Ego_r `pGlobalScriptDef` vs the landed `g_pScriptDef`.** All 120 landed sources that spell the global 0x143E90C
  `g_pScriptDef` are FIXED to the PDB name `pGlobalScriptDef`. Their bodies are unchanged, and each regrades
  RELOCATION_MATCH.

## Queue (2): FINDINGS_LOG chunks 471-476
The log was re-read at the end. It is still 621 lines, and its last entry is chunk 476: chunks 477 and 478 had not
been logged yet when this pass finished. Taken from the log:
- **Chunk 471.**
  - RENAMES:
    - 0x7BEDD0 `NThingShot::CTCShotDefinitionBase::GetShootAtPos`;
    - 0x692620 `CCreatureActionBase::GetActionID` (it replaces the propagated 'DrawGetEnvironment').
  - LABELS: 0xB84470 CEngineSubPrimitiveRenderer ctor, 0xC74D30 `CDspGain::Blend` (descriptive), 0x713630
    `CTCScriptedControl::CleanupAllActions`, and 13 NParticleEngine CPSC* `InterpolateTo` / `GetComponentCost`
    callees. Each is the sole entry of its class's vtable slot 14 or 15 and is in the PDB name set.
  - FIXES: 0x714900 and 0x713EE0 bind the PDB callee names CleanupAllActions and SetNavigationTarget.
  - UNRESOLVED: the claim that 0x6CB960 is SetNavigationInput is rejected. The PDB holds only SetNavigationTarget,
    which is the landed name.
- **Chunk 472.**
  - RENAMES:
    - 0x99CFE0 `CSystemRegistry::GetBOOL`, with FIX 0x4017B0 (it calls GetBOOL);
    - 0x4083E0 `CUserProfileManager::GetConfigOptionsDefaultsDef`;
    - 0x9C7A40 `NGameText::CTextEntry::GetText`;
    - 0x9A4EC0 `GFGetSystemManager` (returns &GSystemManager);
    - 0x40DDC0 `std::__uninitialized_copy<CDisplayModeDesc*>`, with FIX 0x9C2900 (vector<CDisplayModeDesc>::reserve);
    - the CEngineLocalDetailObjectDef vector family: 0x4774B9 `_Destroy`, 0x477BFC `_M_clear`, 0x47747C
      `~CEngineLocalDetailLayerDef`.
  - FIXES: 0x477AB7, 0x47800E and 0x4774D6, the real STLport vector over class CEngineLocalDetailObjectDef.
  - LABELS: 0x4124C0 `_M_insert_overflow`, 0x9C2030 / 0x477C18 `_M_allocate_and_copy`.
  - UNRESOLVED:
    - 0x9F2080 ($E2 / DumpLog);
    - 0x9CC410 (LookupValue vs GetTagFromCRC; no PDB name for either);
    - 0x992B80 (the STLport `__adjust_heap` spelling is right, but its element record is named three ways across
      the family; owner list).
- **Chunk 473.**
  - RENAMES: 0x9C9070 `NGameText::CDataBank::GetPTextEntry`.
  - FIXES: 0x488B20, whose find_if layer is now in `std` and binds the landed 0x48CC60 `__find_if` symbol.
  - LABELS: 0x4EC08A `std::_Construct<CCardDef,CCardDef>`.
- **Chunk 474.**
  - RENAMES:
    - the CDataOutputStream writers 0x993E80..0x993EE0 (WriteUWORD, WriteSWORD, WriteULONG, WriteSLONG,
      WriteUHUGE, WriteFloat);
    - 0x520330 `CEntityRuntimePersistence::AddPersistedDeadEntity` (real STLport map), with LABEL 0x52A130 and the
      iterator row 0x527B60;
    - the CDeletion / list chain: 0x42CD84 and 0x42BED4 `NUISystem::CDeletion::CDeletion`, 0x42B945 `list::insert`,
      0x42B4C3 `list::_M_insert_dispatch`.
  - LABELS: 0x42BF06 list copy ctor, 0x42BEEA ~CDeletion.
  - FIXES: 0x5DBFD0, whose callee 0xA242C0 is `CMeshDataBank::PeekAnimation`.
  - UNRESOLVED: 0x47DB73 SavedConfirmed placeholder types (a retype, not a rename).
- **Chunk 475.**
  - RENAMES: 0x4107C0 `CPersistContext::Transfer<bool>` (the 2-argument overload), 0x686A80 `CTCBase::PeekWorld`.
  - FIXES: 0x629F50, whose +0x5EC member is TextBoxButtonBSoundCriteria (CPlayerGuiDef retail = Ego_r - 0x3C).
  - UNRESOLVED:
    - 0x49D870 GFGetCurrentFrame (no PDB name);
    - 0x7F1840 and 0x5DC020, which are already correct.
- **Chunk 476.**
  - RENAMES: 0x66AE10 `CTCSoundPlayer::IsAttachedSoundPlaying`.
  - FIXES: 0x6E8E10, which now lives in `namespace { }` (PDB `?A0x` name).

## Queue (3): $S/names/callees
Only rows with PDB or RTTI evidence were taken, and none that sweep1 or rename1 had already handled. `work/q3chk.py`
re-proved every row from the bytes:
- **Store order.** A dtor's own vptr store comes first, before any call. A ctor's own store comes last.
- **RTTI.** The RTTI base list must agree.
- **Unique definer.** A vfunc is accepted only when exactly one class in the hierarchy can define that slot.
- **Bounds.** The function's end is found from ret/jmp padding.

Results:
- **RENAMES (3):**
  - 0x401630 `CDiskFileWin32::~CDiskFileWin32` (was CAIStateGroup_CreatureUseCoverBase);
  - 0x994430 `CFileDataOutputStream::~CFileDataOutputStream` (was CFileDataInputStream);
  - 0x956760 `CCombatAction_WillShieldInto` ctor (was WillEnflameInto; the vtable binding is corrected).
- **FIXES (1):** 0x925780 KrakenHighAttackLoop ctor, moved into `namespace { }`.
- **LABELS (66):** checked ctors, dtors and sole-slot vfuncs, among them `CDiskFileWin32::Open`, the
  CFileDataOutputStream Set/GetPosition and the `CBankFile::Update/OpenReadOnly` rows.
- **UNRESOLVED (33).** Most are "inlined base dtor inside a derived dtor": the base store follows the derived
  teardown. The others failed on a scan overrun, a non-unique definer in the hierarchy (FinishAction,
  OnActivate, Close, PerformAction, Initialise, ...), or a missing PDB name. Two need no change: 0xE3E320 and
  0x98DD10 are already right; 0xE3E320's NScript namespace convention is on the owner list.

## Dependency chains (apply together)
Each item lists rows that bind one another's symbols. Each one's BINDINGS_DROP rows go with it.
- 0x462CB0 + 0x462E04: vector<CBrainUpdateZone>. operator= calls the renamed `_M_clear` symbol.
- 1f family: 0x81B220 + 0x81E0F0 + 0x81E190 + 0x81E1F0. All of them bind or define
  `??$sort@PAU?$pair@KVCEntry@CSkeletalMorphs@@@std@@...`.
- 1e: 0xCB67B0, and separately 0x4BA080, each with its BINDINGS_DROP row.
- Subtitle deque: 0x4935A7 + 0x48DDF6 + 0x494106 + 0x4941C0 + 0x493CF5 (RENAMES) + FIX 0x49319C.
- PKSerialiseCParser: 0xC31B30 + 0xC31B50 + 0xC31B90.
- 0x99CFE0 + FIX 0x4017B0.
- The 120 pGlobalScriptDef FIXES rows (one global name; apply all or none).
- 0x40DDC0 + FIX 0x9C2900 (CDisplayModeDesc).
- CEngineLocalDetailObjectDef: 0x4774B9 + 0x477BFC + 0x47747C (RENAMES) + FIXES 0x477AB7 / 0x47800E / 0x4774D6.
- FIX 0x488B20 with the landed 0x48CC60 `__find_if`.
- FIXES 0x714900 / 0x713EE0 (labels 0x6CB960 / 0x713630 are names already landed or in LABELS).
- 0x66AE10 and 0x692620: their caller bindings change with them (BINDINGS_DROP).
- CDeletion / list: 0x42CD84 + 0x42BED4 + 0x42B945 + 0x42B4C3 (LABELS 0x42BF06 / 0x42BEEA).
- 0x520330 + LABEL 0x52A130 + iterator row 0x527B60.
- FIX 0x5DBFD0 with the landed 0xA242C0 PeekAnimation.
- Each insert_unique row and the `operator--` iterator row of the same map, which it binds by symbol:
  - 0x47C720 / 0x47B760;
  - 0x512DA0 / 0x50E190;
  - 0x52E230 / 0x52DD10;
  - 0x567BB0 / 0x566210;
  - 0x67BA30 / 0x67ACD0;
  - 0x67BAF0 / 0x67AD40;
  - 0x67BBB0 / 0x67ADB0;
  - 0x6DB450 / 0x6D9CD0;
  - 0x6DCDD0 / 0x6D9D30;
  - 0x76A1E0 / 0x7690D0;
  - 0x777860 / 0x776C60;
  - 0x8017F0 / 0x8012F0;
  - 0x884560 / 0x882E00;
  - 0x990AD0 / 0x98FE20;
  - 0x9EF1A0 / 0x9EE910;
  - 0xAD3EF0 / 0xAD1850;
  - 0xBD8C00 / 0xBD7E30.

  The insert_unique TU also emits that `operator--` as a COMDAT.
- find const/non-const twins (0x777120/0x777170, 0x8AEED0/0x8AEF20, 0xBE08E0/0xBE0930) and iterator `++`/`--` pairs of
  one map, which share the `_Rb_global` callee names.
- 0x401630 with the landed 0x99A920 `CDiskFileWin32::Close` (now bound by its PDB name).
- 0x994430 (binds `~CBaseClassNonCopyable` 0x99A300 and `~CCharString` 0x99EAE0).
- 0x956760, with the drop of the `??_7CCombatAction_WillEnflameInto` binding of vtable 0x129461C.

## Owner items (not fixable per row; nothing was edited in headers)
- **Case-only path renames:** 0x993EA0 and 0x9A4EC0 (git mv on a case-insensitive Windows tree needs two steps).
- **Leaf debt:**
  - 0x42ABCA's leaf mangles `U?$allocator`, which should be `V`;
  - 0x42ABAF and 0x42AB8A are still list stand-ins.
- **NScript namespace convention** for the CV_ / CQ_ quest script rows (0xE3E320). Line 1 is right, but the
  namespace and class letter need one decision for the whole family.
- **1d access application** (159 sources) and the protected `_M_clear` / `_M_insert_overflow` on vectors were
  deferred. Several tests call the private members directly.
- **Header debt:**
  - CParentDefClassBase.h is 0x40 bytes against retail 0x3C.
  - CSubtitleRenderer.h's TextInfoQueue is 0x18 bytes against retail 0x28.
  - the `_Retail` layout classes at 0x9C7060.
  - mixed `__false_type` spellings.
  - pack(1) rows 0x6A3C70, 0x5D96A0 and 0x95F7D0, and C3DVector (1-aligned in retail).
- **Caller-binding header debt:**
  - 62 landed callers bind 0x686A80 under assorted names; after the PeekWorld rename they should use
    `?PeekWorld@CTCBase@@IBE`;
  - the CInterfaceMap LowerBound spellings;
  - inconsistent caller bindings for 0x9C9070, 0x9C7A40, 0x9CC410 and 0x9A4EC0.
- **CSoundBankRecord heap family 0x992B80:** it is landed with the Dinkumware `_Adjust_heap` spelling, while its
  callers bind the STLport `__adjust_heap`. It needs the whole family moved together.
- **Descriptive names that need a decision:**
  - 0x4774AB = `_Vector_base<CEngineLocalDetailObjectDef>::~_Vector_base` (landed as FreeMember0);
  - 0x66ADD0: the catalog says CTCSoundPlayer::StopSample and the file says CGameScriptThing (unverified).
- **Catalog regeneration:**
  - 0xC0BCC0, 0xCAF580 and 0x431F10, whose sources are already right;
  - 0x121AEC0 `$E2`, which is `_$E1`.
- **0x6DA140** binds a const-traits `map<long,CDefString>` symbol.
- **0x713EE0** models CActionFollowCreature at top level, although its name is NTCScriptedControl::.
- **Anonymous-namespace bindings.** The `?A0x` hash in bindings is per-TU (0x925780, 0x6E8E10). The table should
  match these by prefix.

## Descriptive names (no PDB/RTTI name exists)
- `PKSerialiseCParser::Transfer / TransferSize / TransferString` (libpk).
- `CDspGain::Blend` (libpk).
- `g_pThingManager` for 0x13B8A1C. It is the thing manager, from the GetThingManager pair 0x686AE0 / 0x686AF0.
- The +0x28 member name in 0x9C7A40.

## GOTCHAS one-liners
- Spell a textually substituted pointer map key `pair<X* const, T>` (retail `QAVX@@`). `pair<const X*, T>` is
  pointer-to-const (`PBVX@@`) and matches no retail symbol.
- STLport insert_unique matches only with a same-TU `auto_inline(off)` + noinline real `operator--` body, so `__j`
  goes in the dead `__v` slot.
- A substring `// Leaf:` (e.g. `?find@` also matching `?_M_find@`) silently grades the largest function. Always use the
  exact mangled prefix and run leafchk.
- Inline STLport `iter_swap` emits its own bare-`jmp` COMDAT. That is retail's 0xCB63F0 / 0x4B86A0.
- `static_cast<bool>` reproduces retail's BOOL `movzx`.
- Interleaved C3DVector copies need the C3DVector copy ctor.
- The header-policy regex in the verifier only sees `struct T { char pad[..] }`; class-based local models pass it
  silently.
- A dtor's own vptr store comes first, before any call. A base store after the derived teardown is the inlined base
  dtor, not the base class's own function.
