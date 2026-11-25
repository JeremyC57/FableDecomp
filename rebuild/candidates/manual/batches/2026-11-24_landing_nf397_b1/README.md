# Landing nf397 b1: 0x00728D10 / 0x00728DF0 compile /Gr without __fastcall keywords (T3 nf395 0x00728D10)

nf395_b1 showed this retail translation unit was built /Gr: retail's `_Construct<CMeshMorphSetParts>` at 0x00728A20 passes its arguments in registers, with no keyword in STLport. The two sibling rows (`vector<CMeshMorphSetParts>::_M_insert_overflow` and `_M_fill_insert`) now drop their `__fastcall` keywords, take placement new from `<new>`, and carry `CompilerFlags = '/O2 /Oy /W3 /Gr'` in build_candidates.ps1. Both selves are member functions: same symbols, same bytes (coordinator check on the vland objects).
- **Extras:** six cdecl (YA) helper copies become their __fastcall (YI) twins.
  - `__uninitialized_fill_n` is EQ by COMDAT to the landed 00728a40 self (coordinator `comdat_cmp`, with `_Construct` and every CArray member) and reads EQ at 0x00728A40.
  - `max<unsigned>` is tier 1 against 007634e0's shared-header copy.
  - `_Construct`, `_Destroy` x2 and `__copy_backward_ptrs` are tier 2, non-unique; `_Construct`'s only reloc-bound match is 0x00728A20.
  - `.drectve` gains `libcp` (LIBCP allowance).
- **NF397 gates:** vland HEAD and new RELOCATION_MATCH; pk_all / pickany_vs_retail with no new NEQ and 0 reach flips; /FORCE link duplicates 0 -> 0 in both orders; a census of 311 objects found no reference to a removed YA name. No table change.
- **Coordinator checks:**
  - landed 2 of 2;
  - sources equal the graded sources;
  - selves unchanged (no `self_symbols.tsv` change);
  - baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- **T3:** nf395 0x00728D10 is closed. nf390 0x00728D10 (port the family onto the shared headers) stays open, joined by nf397 0x00728D10 (00728d10's stand-in `__destroy_aux` reads NEQ at 0x00722130, unchanged from HEAD).
