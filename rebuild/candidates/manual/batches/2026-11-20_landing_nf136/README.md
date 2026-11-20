# 2026-11-20 landing nf136: PRIORITY slice 11 remainder (52 rows)

Records: `naming/2026-11-20_agentNF136/` (selfcheck 0 problems). 18 entries: q4290 done; q3887 blocked (0x009E4830 / 0x009E48F0: the
out-of-line `~_List_base` / `~list` of list<CMaintainedEvent>, unreferenced, cannot be told apart); q5199 / q5220 blocked on one row
(0x006D41F0, port not attempted; 44 of 45 binders moved); 14 open: NF139 took 9 (q4291, q4292, q4295, q4296, q4299, q4659, q6168, q6305,
q6308), NF140 took 5 (q4056, q4197, q4198, q4942, q4943).

## Rows (52): 52 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q5199 / q5220: 21 `CPersistContext_TransferableComponent<T>::TransferIn` rows re-authored as explicit instantiations on the shared headers
  (each T from the FableWin.pdb instance plus the owning `Transfer<T>` whose spelling the landed rows bind); 0x005D8167 / 0x00679910 become
  `CPersistTraits<long>` / `<EOpinion>::TransferIn` (the spellings their only callers bind); 21 rows that model CCharString / CStringParser
  locally drop their free 0x009E2590 stand-in for the partial header mode below. 0x009E2590's table set 11 -> 2.
- q4290: self row 0x009BE420 onto engine/CDisplayManager.h (`?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z`); test rewritten
  on the shared headers.
- q3887: self row 0x009E4710 (list<CMaintainedEvent> clear) and 6 binders. Its set goes 5 -> 2 in the table; the two blocked rows keep their
  stand-ins (an existing split that shrinks; advisor rule).

## Headers (add-only; default path token-identical)
engine/CPixelFormat.h FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX (declares the public static FindFormatInfoIndexFromD3DFormat,
0x009E3620); engine/CPersistTraits.h partial mode FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY (declares only CPersistTraitsSLONG, for TUs that
model the string classes locally). No landed includer defines either.

## Checks
- 390 rows rechecked, 390 good; gate 0 created splits; bindings_multi_check 1.
- Table: grew only by witness (row 0x0047550F, p_before / p_land SAME): 0x00453630 3 -> 4, 0x004758E3 1 -> 2, 0x00475861 1 -> 2,
  0x00475BF6 0 -> 1 (its CStringLongMap stand-ins, already in the row at HEAD; queued). Shrank: 0x009E2590 11 -> 2, 0x009E4710 5 -> 2,
  0x0129BA40 5 -> 4.
- One-definition (`logs/symbol_diff.txt`): new symbols are own leaves (explicit instantiations), shared-header inlines and COMDATs; the
  inline long-returning `CPersistTraitsSLONG::TransferIn` overload the partial mode exposes is a COMDAT. Link test over the 52 objects:
  4 LNK2005, all present at HEAD with the same objects (`logs/link_control_head.log`): `CPersistContext::TransferMap(const char*,
  CStringLongMap&, const long&)` in 0x00463334 / 0x00475026 / 0x0047550F and `CMemoryDataOutputStream::Clear` in 0x009E42C0 / 0x00A66860 /
  0x00A66C00. Queued as debt.

## Coordinator answers (advisor rules, owner delegated)
- 0x009E4710 self rename lands (its set shrinks, none new; the two blocked rows are an existing split).
- Vtable 0x01239378 slot 0 (0x004B8680 `<ENumAcceptedBoasts>`, in no PDB): queued from NAME_FIXES.
- q4056 moves as a family with its callers (NF140 has it).
