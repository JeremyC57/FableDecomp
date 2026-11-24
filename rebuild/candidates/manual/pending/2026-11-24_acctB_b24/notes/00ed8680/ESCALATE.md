# 0x00ED8680 CTB_Bard::Main -- escalation (B24A10, 2026-11-24)
Row grades RELOCATION_MATCH with strict vland PASS. One pick-any question for Account A before landing:
- pickany_vs_retail.py: ??1CScriptGameResourceObjectMovieBase@@UAE@XZ is a LEAF-REFERENCED NEQ against 0x006E7B80. Retail Main both
  expands ~MovieBase (0x00ED8BAA: call 0x006E7AB0 + vtable 0x0126008C + call 0x0099A430) and calls 0x006E7B80 (0x00ED93BA, 0x00ED946C),
  so the dtor cannot be declaration-only (FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR: 3636 bytes, DIFFER). The
  copy this TU emits (call form, 25 bytes) is byte-identical to the copy landed 0x00D62480 already emits (a10/pk/cmpcopy.py), i.e.
  HEAD debt of the family (B24A4 SHARED_FORM: retail's 0x006E7B80 comes from another TU). EXTRA_RULES 2026-11-21 lists
  "leaf-bound, newly bound, different -> held"; this symbol is bound at HEAD, so the case is not listed: decide land-with-debt or hold.
- Devices used (FINAL, stock flags): inline_depth(0) round the row-local CIsThingAlive ctor (H40, + NOVTABLE) and round an explicit
  CSpawnedFunc1 ctor specialization (as landed 0x00ED5F50). The loop shape is FableWin-evidenced (no flag, no do-while(0)).
- Also not leaf-referenced, no table line: ??_GCIsThingAlive differs from 0x00CDF160, identical to landed 0x00D82640 / 0x00D8BCD0 copies.
