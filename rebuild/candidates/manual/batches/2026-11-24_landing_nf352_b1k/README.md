# nf352 b1k: 0x00626750 CTCQuickAccessMenu::ContructItemLists (vector) on FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY (1 row)

NF350's approved b3 text (NF347 pilot, advisor-approved), handed back once from NF352's batch as ordered; NF352's size bodies do not apply
to this row (ctor / dtor in any form move the leaf). The row of agent b1 kept here is 0x00626750 only; the two Reset rows landed from r1.
- Grade: RELOCATION_MATCH PASS at HEAD and after landing (coordinator repo recheck).
- 20 COMDATs dropped, 0 added. `??A?$map@JPAVCComponent@NUISystem@@...@QAEAAPAVCComponent@NUISystem@@ABJ@Z` is now UND; it is exactly
  the `// Leaf:` of landed owner 0x005949C3 and its reloc_bindings line (witness 0x0058A0C7) (the declaration-only rule).
- Drop test: no reloc_bindings witness names a dropped symbol; `??8_Rb_tree_base_iterator` 0x0041C820 stays defined EQ by 0x00626110
  (coordinator pk_all after r1), 0x0058A0C7 and 0x005949C3.
- Both checkers, HEAD v new with `--leaf`: only drops (??A NEQ, ??8 EQ and 18 no-address helpers); nothing turned NEQ, no reach flip.
- T3 nf340 0x005949C3: annotated; the remaining emitter is 0x00626110 (NF353's family port).
