# Landing hand2b overflow (2026-11-19): two STLport vector::_M_insert_overflow instantiations

vland --land, strict headers: RELOCATION_MATCH + PASS (105 cases each, 7/7 mutants each). No new header opt-ins.
- 0x006A3A70 `vector<pair<long,CIntelligentPointer<CThing> > >::_M_insert_overflow`, 0x00723110 `vector<CTCSmokeGenerator::CGenerationPos>::_M_insert_overflow`.
- Closed by explicit instantiation of the shared stlport/fable_containers.h body + FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N and the existing
  FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN (FableWin copy ctor `SetPointer(item.GetPointer())`), which closes the newFinish register swap.
- 0x00723110 defines `CTCSmokeGenerator::CGenerationPos { C3DVector Pos; CIntelligentPointer<CThing> PEmitter; }` locally (no engine header; Ego_r
  LF_CLASS size 20, PEmitter +12). Queued: a shared header.
- Landed slugs are `std_Minsertoverflow_<addr>` (the landing tool dropped the element types); respell queued with the catalog labels
  `Vector_ReserveAndFill` / `std::std::vector_ReserveAndCopy` (`NAME_FIXES.tsv`); vland RELOC_NAME on 0x00722B70 / 0x00722BD0 label spellings.
