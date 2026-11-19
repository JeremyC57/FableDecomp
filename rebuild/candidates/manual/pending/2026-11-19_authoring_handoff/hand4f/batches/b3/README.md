# hand4f batch 3 (2026-11-19): 0x0053848F, 0x005384DD -- both RELOCATION_MATCH + test PASS in the strict read-only vland
- 0x0053848F NUISystem::CList::AddActionOnInactive; 0x005384DD is NUISystem::CList::AddActionOnSelected (NAME_FIXES.tsv; vland WARN
  STANDIN_LEAF = the catalog label).
- Existing opt-ins only (FABLE_RETAIL_CLIST, FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE, FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE);
  TU pragmas `optimize("y", off)` + `("s", on)` as the landed same-TU rows; no flags; hdr/ empty.
- vland WARN RELOC_NAME: 0x00535140 is STLport _List_base(const allocator&) for list<CCountedPointer<NUISystem::CAction> >, not the landed
  label std::list<T_0x8_00535140>::list (NAME_FIXES.tsv; the shared body grades RELOCATION_MATCH there under /O2). Respell the landed row.
