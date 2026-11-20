# Landing hand3c2 b15: 0x00A48BA0 CLipSyncSampleData::WriteToFile (Account B, 2026-11-20)
RELOCATION_MATCH, test PASS, strict vland `--land`.
- Audited by the rolling auditor ($S/stage/audit_roll; report extract kept in Account B's scratch). Its fixes were to the overlay only: one
  assert pinned, and the fable_map.h change made additive.
- Written from Ego_d's line table and block-scoped locals; no fitted line.
- Headers (hdrcheck2 default=same):
  - stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_MERASE_PASS22_INLINE opt-out. The row defines it and retail keeps one erase level in place
    here; every other row's text is unchanged.
  - engine/CLipSyncPhoneme.h: GETSYMBOL_INLINE.
- Mutants 70/70.
- RELOC_NAME note: WriteUBYTE 0x00993E50 is labelled WriteCBYTE. The bodies are folded and no fix is proposed.
