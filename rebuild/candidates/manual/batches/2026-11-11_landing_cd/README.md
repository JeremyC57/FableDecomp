# 2026-11-11 landing cd -- family FAM02: 346 STLport algorithm stand-ins renamed to their STLport 4.6 instantiations

Source: `naming/2026-11-11_agentFAM02/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES / LEFT / hdr README). vland --recheck-landed in four
parts: all 346 MATCH / RELOCATION_MATCH (201 / 145); 330 tests PASS in vland, the other 16 print their catalog PassPattern -- checked by
running each exe, 16/16. Bindings re-derived; bindings_multi_check unchanged (NullCoord baseline).
- `_global::UninitCopy_* / UninitFillN_* / Fill_* / Copy_* / CopyBackward_* / DestroyRange_*_<addr>` stand-ins are now
  std::__uninitialized_copy<I,O> (109), std::__uninitialized_fill_n<I,unsigned int,T> (55, `ret 8`), std::uninitialized_fill_n (51, `ret 4`),
  std::fill<I,T> (54), std::__copy<I,O,int> (33), std::__copy_backward<R,B,int> (19), std::__destroy_aux<I> (25) -- each one explicit
  instantiation over stlport/fable_containers.h with the real element type, `// Flags: /Gr`, T proven from the landed binder + the caller's
  container / the element callees (`// Identity:` lines).
- Header: stlport/fable_containers.h opt-in FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE (11 size-optimised rows; default path unchanged).
- 316 rows left (LEFT.tsv): no STLport binder (117), stand-in element only (24), ICF-shared addresses (16), invented element helpers (16),
  rewrite does not grade yet (~143).
- Name fixes queued as land1011cd (41 landed binders of the old spellings).
