# 2026-11-20 landing nf139: PRIORITY slice 11 third pass (17 rows)

Records: `naming/2026-11-20_agentNF139/` (selfcheck 0 problems). 9 entries: q6308 done; q4291, q4292, q4295, q4296, q4299, q4659, q6168,
q6305 deferred (their remaining binders were locked by NF136 when NF139 ran; NF142 has them). Their self rows land now as finished,
separable ports (the nf133 q4727 form): each address's spelling set only shrinks and no split is created.

## Rows (17): 4 MATCH + 13 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- Self ports onto shared headers (leaf renames, both PDBs): 0x009BEDC0, 0x009BEF20 (engine/CDisplayManager.h), 0x009E3790
  (engine/CPixelFormat.h), 0x009F9F70, 0x009FA1C0 (engine/CTexture.h), 0x00A03B30 (engine/CInputEvent.h ~CInputEvent), 0x009CC2A0
  (engine/CBankFile.h ReadEntryData).
- 0x009CC240 GetEntryDataSize: strong body -> the GOTCHAS nf86 / nf88 pick-any scaffolding (`// Fidelity: scaffolding, not retail`),
  because 0x009CC2A0 now expands the header's inline body as retail does. NF139's link proof: HEAD 0x009CC240 + new 0x009CC2A0 ->
  LNK2005; both new -> clean.
- CTexture binders onto engine/CTexture.h (their local CTexture model removed, nothing else changed): 0x0049B3B0, 0x0049B480,
  0x0049B5A0, 0x0049B7F0, 0x00495620, 0x009E1650. In place on the row's existing local model (declarations only, defined-symbol set
  unchanged): 0x00499880, 0x0049AE30, 0x009DE6F0.
- Files renamed with their rows: E2_E2_00a03b30 -> CInputEvent_CInputEvent_00a03b30, CTexture_Assignment_009fa1c0 ->
  CTexture_operator_009fa1c0 (sources and tests).

## Header (add-only opt-in; token-identical without it)
engine/CBankFile.h `FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE` (inline GetEntryDataSize / GetEntryDataOffset; Ego_r.pdb 0x00B19F40 /
0x00B19F70, FableWin.pdb 0x02F91CC0 / 0x02F91EA0). No landed includer defines it.

## Checks
- 247 rows rechecked, 246 good. The one bad row, 0x009E15E0, is DIFFER(99v99) with the HEAD headers too (pre-existing; a blocked q4296
  row with no Leaf line). Gate 0; bindings_multi_check 1 (the pre-existing NullCoord).
- Table: no set grew; 0x00A0B480 2 -> 1, 0x009BEF20 3 -> 2.
- One definition (`logs/symbol_diff.txt`): own leaves, shared-header inlines and COMDATs. The six CTexture binders now emit
  `??1CTexture@@QAE@XZ` as a header inline COMDAT. The table no longer binds that name anywhere (the q4296 leftovers 0x009C1AC0 /
  0x009C2520 bind `FableMemberDtor` stand-ins), and it is no row's leaf. Linking the 17 objects with every rechecked object that defines it
  (34 more, `logs/link_ctexture_context.log`) gives no ~CTexture clash: every copy is pick-any.
- Link test over the 17 objects: no LNK2005. The context link shows 6 LNK2005s, all reproduced with HEAD's 0x009DE6F0 object
  (`logs/link_control_head.log`) and queued as DEBT: CQuickDrawTriInfo::operator= (0x009DE6F0 / 0x009D9C80), ~CGraphicDataBank
  (0x00A013D0 / 0x009FEBC0), and CEngineLandscapeMemoryManager FreeVirtual / DeleteIndexBuffer / DeleteVertexBuffer / DeleteTexture
  (0x00BDA150 / 0x00BD9CE0).
