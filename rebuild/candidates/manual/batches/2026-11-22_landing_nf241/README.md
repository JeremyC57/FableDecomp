# Landing nf241 (Account A, 2026-11-22): the list<NUISystem::CComponent*> family (35 rows) and CBankFile::UpdateEntry

Agent NF241. Hand-back: `pending/2026-11-22_nf241_handback/`. Entry 1 done as one unit; queue line L107 done; the rest open (NF243).
- **Recheck** (`logs/recheck.log`, landed tree): 36 rows, 1 MATCH + 35 RELOCATION_MATCH, every test PASS, 0 RELOC_CONFLICT.
- **The claim:** 0x0042ABAF / 0x0042ABCA are `_List_base<NUISystem::CComponent*>`'s ctor / dtor, not list's own: retail's list(const
  allocator&) 0x0042B6A6 only calls 0x0042ABAF and ~list 0x0042B6D2 is a 5-byte `jmp 0x0042ABCA` (STLport 4.6's layout, the shape nf239
  landed list<CActionParam*> on; the PDBs carry Dinkumware and cannot decide it). The two self rows are nf239's held ones; catalog-only
  renames (STL).
- **33 binders** moved with them through existing opt-ins (`_LIST_BASE_VISIBLE_NOINLINE`, `_LIST_BASE_DTOR_VISIBLE_NOINLINE`, `_DTOR_IN_LIST`
  dropped, two explicit `~list` specialisations rewritten, one in-place local model, 0x0042BED4 / 0x005C3947 / 0x005AE6AF / 0x005AE9DE /
  0x005AEAEE onto the shared headers). Census from the compiled objects (`logs/census_final.tsv`): all 18 binders of 0x0042ABAF and all 65 of
  0x0042ABCA bind `_List_base`: 3 -> 1 and 2 -> 1; no binder names an undefined function. Per row, only those two relocations change.
- **Header:** stlport/fable_containers.h comment-only (token-identical after stripping comments; equivalence over 2,258 includers: 0
  differ): the `DTOR_IN_LIST` and `DEFAULT_CTOR_NOINLINE` comments now cite STLport's shape, not the Dinkumware PDB.
- **0x009CBA80** CBankFile::UpdateEntry (both PDBs; the exe's one reference in 0x00B3BC90 already spells it) on engine/CBankFile.h, MATCH
  (`ret 4`); renamed from `Nop_ret4` (files, catalog, manifest): 2 -> 1.
- **reloc_bindings:** the two stand-in `list` lines deleted (the `_List_base` lines were already there). Pick-any: equal copies 201 -> 213,
  none goes from equal to unequal; link: the same 12 HEAD duplicate warnings.

## Coordinator rulings on NF241's questions
1. The extra `DEFAULT_CTOR_NOINLINE` comment fix: kept (accurate, comment-only, proven token-identical).
2. The per-instantiation census of the 19 other `DTOR_OUTOFLINE` rows (`logs/dtoroutofline_all.txt`): queued for NF243 with 0x004046B0.
3. The 62-B `~_List_base` copies in 0x00545B10 / 0x00607330: moved HEAD debt (the same bytes sat under `~list` at HEAD), accepted.
4. The unreferenced `list(const allocator&)` copies in 0x0042BED4 / 0x005C3947: no table owner, so not pick-any holds; adding 0x0042B6A6 /
   0x0042B6D2 as catalog rows is queued (they then join the 11 HEAD rows' copies as debt).
