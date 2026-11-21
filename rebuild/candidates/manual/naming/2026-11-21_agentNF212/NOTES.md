# NF212 notes (round 10, family pass FP-VECTORINSERT leftovers; single agent)

Counts (from STATUS.tsv / APPLIED.tsv): 4 entries: q6332 / q6341 (one unit, 0x00433530) done; q6333 / q6342 (one unit, 0x00485DBC)
open (identity census only, see below). Rows: 4 ported (0x004333B3, 0x004B2FE0, 0x004B4A10, 0x00A2ED10; all RELOCATION_MATCH + PASS,
recheck_final.log), 2 not-a-binder (0x00433530 self, 0x004B2850), 7 not reached (0x00485DBC self + its 6 locked binders).

## q6332 / q6341 (0x00433530 vector<CCharString>::_M_insert_overflow, IAE)
- With NF208's 8 rows, all 39 landed referencing rows (REFS.tsv) bind the IAE spelling after this hand-back. The three stand-in
  spellings witnessed in reloc_bindings (one-parameter `vector@VCCharString@@@std@@QAE`, `CCharStringVector@@QAE` I_N / K_N,
  `CStringVector@@QAE`) had these four rows as their only binders at 0x00433530 (NF204 / NF208 probe census: the other 35 referencing
  rows bind IAE). Other landed rows still model CStringVector / CCharStringVector locally for other purposes (e.g. 0x004AF740,
  0x004B0160, 0x004B2510, 0x0098DB20); none of them binds 0x00433530 under a stand-in per that census.
- 0x004333B3 (vector<CCharString>::_M_fill_insert): port onto the shared vector (FABLE_RETAIL_STLP_VECTOR body). Leaf-only self rename
  (RENAMES s1): HEAD object defined the one-parameter spelling; the 3 landed referencers (0x0043332C, 0x0049B760, 0x0049B860, Fable.exe
  scan work/refs_extra.tsv) all bind the new allocator spelling at HEAD (work/probe_004333b3_refs.txt): 2 spellings -> 1.
  Its 0x00433504 call moves from a non-template stand-in `?uninitialized_fill_n@std@@YI...` to the template spelling the 0x00433504 row
  defines (this row was its only referencer): 2 spellings -> 1.
  New overlay opt-in FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY (hdr/README.md), because the visible noinline body gave a 41-byte
  copy against the owner's 38 (cmpbody False). Uses FABLE_RETAIL_STLP_FALSE_TAG_POD (existing) for the zeroed tag byte.
- 0x004B2FE0 / 0x004B4A10 (CQuestManager): the vector stand-ins move onto the shared STLport vector (0x004B4A10 also CArray.h and
  fable_string.h). 0x004B2FE0 keeps its local CCharString (inline operator== over the Compare_00411570 stand-in), its same-TU
  FindString (0x004B8E40) body and the local CQuestManager: HEAD debt, unchanged (no class token added; vland WARN
  SHARED_HEADER_AVAILABLE CQuestManager was already true at HEAD). 0x004B4A10's ActivateMultipleQuests stand-in body (0x004B4260) already
  differed from its owner at HEAD (q.py DIFFER(89v359) before and after: nf203 debt; the owner defines it strongly, so its pre-existing
  LNK2005 is unchanged).
- 0x00A2ED10 (C3DMeshStats::UnpackNames): the header-less CCharStringVector token keeps its name (its self leaf names it) and sits on
  std::vector<CCharString> (narrow rule: grep finds no class CCharStringVector under rebuild/include). Self spelling debt left as it
  was: 3 spellings at this address (QAE stand-in self + 0x00A2F0F0, QBE stand-in 0x00A2F150, PDB ABE vector form 0x00A2F170;
  0x00A2F120 unprobed). NAME_FIXES.tsv has the PDB spelling; it needs a four-row family pass (with a C3DMeshStats.h opt-in).

- Tests: 0x004333B3 / 0x004B4A10 rewritten on the shared headers; 0x004B2FE0's verbatim-copy test follows the new source; 0x00A2ED10
  keeps its HEAD test (a standalone behaviour model that never compiles the row; a row-source copy, work/00a2ed10_test_rowcopy.cpp, PASSes
  but would add the row's local C3DMeshStats / CCRC / CMatrix3x4 models to a test: selfcheck NEWLOCAL). selfcheck.log: 0 problems.

## Pick-any checks (every newly defined symbol with a table line or a landed Leaf)
- IAE _M_insert_overflow (0x00433530): 0x004B4A10 / 0x00A2ED10 copies vs a fresh owner object: 316 = 316, bytes / relocs equal True.
  0x004333B3 / 0x004B2FE0 keep it declaration-only (as at HEAD).
- push_back (0x0044BFF0, NF208's landed port): 0x004B2FE0 / 0x004B4A10 / 0x00A2ED10 copies 59 = 59, equal True.
- ~vector<CCharString> (0x00414EA0): 0x004B4A10 copy 49 = 49, equal True; q.py RELOCATION_MATCH at 0x00414EA0.
- vector<CCharString>(const allocator&) (table line -> 0x00414DE0, a folded CIndexBuffer ctor body, line made by another row): the
  0x004B4A10 copy grades MATCH at 0x00414DE0 (q.py).
- uninitialized_fill_n (0x00433504): declaration-only in 0x004333B3 (no copy).
- All other new symbols are shared-header inline COMDATs (_Construct, _Destroy, __destroy*, __uninitialized_*, __copy_backward*, fill,
  _M_clear, _M_set, size, allocator members, max<unsigned>, tag ctors, CArray<CCharString>::operator[] const): no reloc_bindings line,
  no landed Leaf (grep). The owner objects hold their symbols in LINK_ONCE (COMDAT) sections: no new LNK2005.

## Decision items for the coordinator
1. 0x00A2ED10 local CCharStringVector now derives from std::vector<CCharString> (the brief's narrow rule: header-less local class
   keeps its token on a shared base). Its self spelling family (NAME_FIXES) is left for a later pass.
2. New overlay opt-in on a widely used header (stlport/fable_containers.h, 2180 includers, none defines it): add-only, declaration-only.
3. No stand-in type was cast to a real one in any row.

## 0x00485DBC (q6333 / q6342): open
- Identity census only (context); no row changed; locks kept. work/probe_00485dbc.txt (one HEAD probe per referencing row).
- Self: the 0x00485DBC object defines ?_M_insert_overflow@?$vector@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@IAEXPAPAVCComponent@NUISystem@@ABQAV34@ABU__true_type@2@I_N@Z,
  the q6342 claim (the brief's fill_n reading did not hold: the body is the __true_type overflow, calling __copy_trivial 0x00405BA0 and
  fill_n 0x0048514C). Its Leaf line is cut at the template prefix; completing it is a same-symbol edit.
- Binders: 6 of 11 already bind it (0x00486024, 0x005418C0, 0x0055FEC0, 0x0056AD40, 0x005E5F30, 0x00622A00). 5 bind stand-ins, every one
  over NUISystem::CComponent* (so a stand-in debt, not a pointer-element fold, GOTCHAS nf104): 0x00541390 ScrollingArray::InsertOverflow,
  0x005414A0 ChildVector::Grow (no Leaf line), 0x00563130 one-parameter vector<CComponent*> over a global stand-in struct CComponent,
  0x006116E0 / 0x007E4B60 CRetailVector<NUISystem::CComponent*>. 0x006116E0's lock reads `released` (not claimed here).

## Descriptive names
- None new. 0x004333B3 / 0x00433504 spellings are STLport library spellings (descriptive-by-library: STL instances, no PDB holds them).

## Flags lines
- 0x004333B3: new `// Flags: /d2inlS34` (with an `// Inline-budget:` line). Its catalog entry has no CompilerFlags: the landing must add
  `/d2inlS34` there (vland reads flags from the catalog / payload, not the row line).

## Funclets
- None: grep of rebuild/src/compiled for the four row file names finds only the rows themselves. No row file moves (0x004333B3's
  self rename keeps its file name stdvectorCCharString_Mfillinsert_004333b3.cpp).

## Locks
- Used only rows in rows.txt (0x004333B3, 0x004B2FE0, 0x004B4A10, 0x00A2ED10 changed). No new claims.

## Rule slips
- 0x004333B3 forms 1-3 carried FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE before the /d2inlS sweep proved it unneeded; dropped in the
  hand-back. The first two vland runs of 0x004333B3 lacked the payload flag (one DIFFER(300v337) report); counted in grades_used.
