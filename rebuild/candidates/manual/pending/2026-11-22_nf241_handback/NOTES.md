# NF241 notes (2026-11-22, round 13b): the list<CComponent*> family (NF239's L17 / L18) and the open CODEXP27 lines

## 1. Entry 1: the list<NUISystem::CComponent*> ctor / dtor family (done; 35 rows)
**Claim.** 0x0042ABAF is `std::_List_base<NUISystem::CComponent*>::_List_base(const allocator&)` and 0x0042ABCA its `~_List_base()`,
not list's own members. Sources: (a) retail bytes: 0x0042B6A6 `56 ff742408 8bf1 e8fdf4ffff 8bc6 5e c20400` is list's size-optimised
list(const allocator&) calling 0x0042ABAF; 0x0042B6D2 `e9f3f4ffff` is list's `~list() {}`, a 5-byte jmp to 0x0042ABCA (neither wrapper
has an E8/E9 caller in the exe: tools/xsites.py); (b) STLport 4.6 _list.h (stlport/fable_containers.h): the sentinel allocation and
clear + free live in _List_base, list's ctor / dtor are empty wrappers. Both PDBs carry a Dinkumware list (FableWin: 0 `_List_base`,
14,069 `_List_val` symbol lines; Ego_r 0 / 1,258), so they cannot name STLport layering: descriptive-by-library, as nf239 landed for
list<CActionParam*> (EXTRA_RULES 2026-11-20).
**Census** (tools/census.py, NF239's fixed regex, re-run now: work/census_1718.txt): 18 referencers of 0x0042ABAF, 65 of 0x0042ABCA,
the same sets NF239 compiled (work/hc/ = NF239's HEAD compile census; every binder source is older than that census, mtimes checked).
HEAD bindings: 0x0042ABAF 7 `??0?$_List_base` + 10 `??0?$list` + 1 `??0CComponentList` (0x005C3947); 0x0042ABCA 34 `??1?$_List_base`
+ 31 `??1?$list`. After this hand-back, measured from the compiled objects (work/census_final.tsv: the new objects for my 33 rows,
NF239's HEAD compile for the 35 untouched referencer rows): 18 / 18 rows bind 0x0042ABAF as `??0?$_List_base`, 65 / 65 bind 0x0042ABCA
as `??1?$_List_base`, no other spelling (3 -> 1 and 2 -> 1). The 33 changed rows are exactly the BAD ones; the OK bindings (34 + 7,
in 35 distinct rows) are untouched.
**Recipe** (no new header opt-in: existing macros only; NF239 worried BASE_OUTOFLINE is TU-wide):
- 9 rows with `_DTOR_OUTOFLINE` + `_DEFAULT_CTOR_NOINLINE` -> `_LIST_BASE_VISIBLE_NOINLINE` (0x0047D648, 0x0047D6F2, 0x005836D2,
  0x0058376C, 0x00583815, 0x005838DD, 0x005839A5, 0x00595356, 0x005C69AA);
- 14 rows with `_DTOR_OUTOFLINE` alone -> `_LIST_BASE_DTOR_VISIBLE_NOINLINE` (0x004830A1, 0x00531E90, 0x00555390, 0x00584F15,
  0x0058F016, 0x0059DB82, 0x0059DE04, 0x0059DE76, 0x0059E6BD, 0x005A887D, 0x005A9A87, 0x005C39F0, 0x005E5F30, 0x00627400);
  tools/xf1.sh. Both opt-ins are TU-wide like the ones they replace (`_DTOR_OUTOFLINE` / `_DEFAULT_CTOR_NOINLINE` were TU-wide too),
  and the relocation diff of every row against its HEAD compile shows ONLY the 0x0042ABAF / 0x0042ABCA relocations changing: no other
  list instantiation in those TUs moved (scall_r1.log; the other differences are anonymous-namespace hashes and $L labels from the
  scratch file path);
- 0x005AECC0, 0x00607330: `_DTOR_IN_LIST` dropped (default header path; 0x00607330's name note rewritten);
- 0x005B43A2: the explicit `list<CComponent*>::~list()` specialisation declaration dropped (its BASE_OUTOFLINE already declares the
  base dtor); 0x00626E40: its explicit `~list() {}` specialisation becomes `_List_base<CComponent*>::~_List_base()` (same body);
- 0x00545B10: in-place respell of its row-local `_List_base` / `list` model (same class tokens): the clear + free moved from ~list to
  ~_List_base, `~list() { }`;
- ports (a local model of a class with a shared header removed, none added): 0x0042BED4 onto fable_containers.h +
  engine/NUISystem_CDeletion.h (FABLE_RETAIL_CDELETION, BASE_OUTOFLINE); 0x005C3947 (CComponentList / SAllocator stand-ins -> the shared
  list); 0x005AE6AF / 0x005AE9DE / 0x005AEAEE (row-local std::allocator / list / pair, and vector in 0x005AE6AF, -> the shared ones; the
  map / tree iterator models stay, in place). NF239's five UND binders (0x0042BED4, 0x005AE6AF, 0x005AE9DE, 0x005AEAEE, 0x005B43A2)
  are among them, and 0x005C3947 UND'd its own stand-in: every row now UNDs or defines the _List_base spellings, which the self rows
  define, so no binder names a function nobody defines. No row gained a class / struct definition (HEAD v rows/ definition sets:
  only removals, in 0x0042BED4, 0x005C3947, 0x005AE6AF / 0x005AE9DE / 0x005AEAEE, and the self rows), so the vland
  SHARED_HEADER_AVAILABLE WARNs (22 rows) are HEAD's local models, and the non-family RELOC_NAME WARNs bind relocations the diffs show
  unchanged.
- Self rows: NF239's held 0x0042ABAF / 0x0042ABCA (pending/2026-11-22_nf239_handback/rows/), unchanged.
**Proof.** Every row RELOCATION_MATCH + test PASS (recheck_final.log). pk.py HEAD v NEW (work/pk_k1.tsv; owners: `??1?$list<CC>` ->
0x0042B6D2, `??0?$list<CC>(alloc)` -> 0x0042B6A6, the _List_base pair -> 0x0042ABAF / 0x0042ABCA): no (row, symbol) goes EQ -> NEQ;
EQ 201 -> 213. Now EQ: 33 `~list` copies (the 5-byte jmp = 0x0042B6D2; HEAD had 22 EQ + 5 NEQ), 20 `~_List_base` copies (HEAD 19).
Left NEQ, all of them NEQ at HEAD under one spelling or the other: 5 x 62-byte `~_List_base` with clear inlined (3 at HEAD; 0x00545B10 /
0x00607330 moved their 62-byte HEAD `~list` copy onto `~_List_base`), 2 x 25-byte, 6 x 32-byte `_List_base` ctor (frame TUs), and
unreferenced `list(const allocator&)` copies (21-byte frame-TU copies: 11 at HEAD, 12 now with 0x005C3947; 0x0042BED4's /O2 copy is 19
bytes; 1 EQ). Link (work/lnkH_k1.txt v lnkN_k1.txt, 35 objects): 12 LNK4006 before and after, the same lines (0x0042BED4's strong
??0CDeletion v other rows' copies, 0x005AECC0's tree / LowerBound copies: HEAD debt).
EH funclets / includes: no landed row or test #includes any of the 35 sources (grep of the file names: only comment mentions).
Source greps for `??[01]?$list@PAVCComponent` / `CComponent*>::~list` outside the 35: 0x0042BF06 (list(const list&), a different
member), 0x005F11A9 (a stale comment only: it binds ~_List_base), tests 0x0042BF06 / 0x00626E40 (self-contained host models
that do not link the row: PASS). Tests: every changed row keeps its HEAD test (all PASS); the self rows' HEAD tests are host models
(NF239's test follow-up stands).
**Table:** TABLE_CHANGES deletes the two `list` lines (witness 0x00583815); the _List_base lines are already there (0x0042BF06,
0x005F11A9 witnesses).
**Header:** comment-only fix in stlport/fable_containers.h (hdr/README.md; token-identical; equiv_check over its 2,258 direct includers
and their tests: work/eq/equiv.tsv).

## 2. Entry 1b: 0x004046B0 (list<CStringParserCommentBoundaries>, ~70 referencers): not started (context). Same shape: the recipe
above (existing macros, per-row swap of DTOR_OUTOFLINE / DEFAULT_CTOR_NOINLINE; explicit ~list specialisations -> ~_List_base;
row-local list models: in place when the model already has _List_base, else port) should carry over; census first (compiled).

## 3. Entry 2 (QUEUE_LINES): L107 done; the rest open
- **L107 0x009CBA80** CBankFile::UpdateEntry(unsigned long): self row onto engine/CBankFile.h (FABLE_RETAIL_CBANKFILE_ENTRIES +
  _EDITOR_ENTRY_UPDATE, the binder 0x00B3BC90's opt-ins), `void CBankFile::UpdateEntry(unsigned long index) { }` = `ret 4`, MATCH.
  Two sources: both PDBs' S_PUB32 ?UpdateEntry@CBankFile@@QAEXK@Z (llvm-pdbutil dump -publics: FableWin 0002:25188960, Ego_r
  0001:7455216, each the address of an S_GPROC32 `CBankFile::UpdateEntry` typed `void CBankFile::(unsigned long)`), plus the exe: the only reference
  (E8/E9 and absolute scan) is 0x00B3BDBA in SetupEditorAnimation, already bound under that spelling, between Begin- / EndUpdateEntries.
  The PDB bodies are 525 / 1104 bytes; TLC's is empty (the editor update compiled out). Stand-in ?M@C009cba80@@QAEXH@Z was bound by
  nobody: 2 -> 1. Not a descriptive name.
- **L108 / L109 / L110** (0x009CBD30 / 0x009CBD50 / 0x009CBD70, CBankFile::SetBankUpdateStatus / SetBankUpdateProgress / WriteBankLog):
  engine/CBankFile.h already declares all three INLINE (FABLE_RETAIL_CBANKFILE_UPDATE_PASS27, protected / private / protected as the
  claims say), which 0x009CEC60 (CBankFile::Update) uses. A self row cannot define an inline in-class member out of line, so the unit
  needs one add-only opt-in (e.g. FABLE_RETAIL_CBANKFILE_PROGRESS_WRAPPERS_DECL: the three declared only, the bodies in the rows) and
  the three rows together; 0x009CEC60 unchanged. Not started (context).
- Everything else: open in NF239's NOTES 3 order (APPLIED.tsv). Locks: rows.txt (36 rows); none of NF240's touched.

## 4. Decisions needed from the coordinator
1. The extra comment fix in stlport/fable_containers.h (DEFAULT_CTOR_NOINLINE: hdr/README.md item 2): only the DTOR_IN_LIST one was
   authorised. Keep it (comment-only, line count kept), or land only item 1? (A DTOR_OUTOFLINE rewrite was reverted to the base text.)
2. The 19 other landed rows that still set FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE (other list types; work/dtoroutofline_all.txt minus
   rows.txt, e.g. 0x004804DE, 0x00814260, 0x004C9120): the same shape may hold for them (a call bound under list's name at a _List_base destructor). Queue a census per
   instantiation (0x004046B0's family first)?
3. 0x00545B10 / 0x00607330 define a 62-byte `~_List_base<CComponent*>` pick-any copy (clear inlined) that is NEQ to 0x0042ABCA's
   23-byte body; at HEAD the same copy sat under `??1?$list` (also bound at 0x0042ABCA). Accept as moved HEAD debt (no EQ -> NEQ)?
4. Two new unreferenced `list(const allocator&)` copies NEQ to 0x0042B6A6 (0x0042BED4 /O2 19 bytes, 0x005C3947 frame TU 21 bytes, the
   11 HEAD rows' 21-byte copies likewise): nothing binds that symbol (NAME_FIXES: 0x0042B6A6 / 0x0042B6D2 are catalog additions).

## Flags lines / funclets / tests / descriptive names
No row needed new flags. No funclet row includes a changed source. Tests: unchanged (HEAD tests, all PASS). Descriptive names: none
new (PDB publics; the _List_base spellings are STLport 4.6's, descriptive-by-library as nf239 landed). Catalog renames: RENAMES
section 1 (0x0042ABAF / 0x0042ABCA from NF239, 0x009CBA80). Source-file renames for the coordinator: 00/42/stdlistNUISystemCComponent_
list_0042abaf.cpp / _0042abca.cpp (now _List_base), 00/9c/E2_E2_009cba80.cpp (CBankFile_UpdateEntry).
