# 2026-11-20 landing nf129: PRIORITY slice 10 (37 rows)

Records: `naming/2026-11-20_agentNF129/` (selfcheck 0 problems). 31 entries: 15 done, 3 done-already, 4 blocked (q3967, q4249, q4626,
q4933), 4 deferred and 5 open (q4019, q4055, q4100, q4283, q4285, q4458, q6186, q6233, q6375 / q6379: NF132 took them).

## Rows (37): 1 MATCH, 36 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- CCharString / CWideString / CStringParser callers moved off Append / Sub / FrontEndWideString stand-ins onto fable_string.h and the
  shared engine/CStringParser.h; 0x009BAB50 CPersistContext::WriteString onto engine/CPersistContext.h.
- CCountedPointer: 0x009A9B60, 0x009A9BB0, 0x009A9BF0, 0x009A9C80, 0x009A9D10, 0x009D1EE0 self-renamed onto engine/CCountedPointer.h
  (explicit instantiations; local stand-in models dropped).
- CSystemManager::Update callers 0x004189C2, 0x00496CF0, 0x00499880 bind `?Update@CSystemManager@@QAE?AW4ESystemReturn@@XZ`.
- 0x009A0CF0 CRenderManagerCore::AttachTextureToStage onto engine/CRenderManagerCore.h (MATCH); 0x00594F36 onto
  engine/CNewFrontendGameComponent.h. Tests for both rewritten on the shared classes (the old tests linked the stand-in symbols).

## Headers (add-only opt-ins, default path token-identical; assert_names_check 0 mismatches)
CStringParser.h FABLE_RETAIL_CSTRINGPARSER_CALCINQUOTES_DECL; CSystemManager.h partial mode FABLE_RETAIL_CSYSTEMMANAGER_ESYSTEMRETURN_ONLY
(declares only the enum); CParsedItem.h FABLE_RETAIL_CPARSEDITEM_GETVALUE_OUTOFLINE.

## Checks
- bindings_multi_check 1; funclet_includes_check 1355 / 0.
- Split gate (GOTCHAS nf120 line): six universally referenced callees (0x0099EC30 1316 rows, 0x0099E4B0 737, 0x0099B7D0 248,
  0x0099F0A0 78, 0x0099F100 74, 0x0099B800 54) were dropped from the recompile. The landing only moves rows onto the spelling each
  address already binds in the table, and none of the six sets grew (0x0099EC30 63 -> 62, 0x0099E4B0 9 -> 8, 0x0099B7D0 5 -> 2,
  0x0099F0A0 1 -> 1, 0x0099F100 4 -> 1, 0x0099B800 3 -> 2). 204 rows rechecked, 204 good; gate 0 created splits.
- Table sets that grew, all witness only (p_before / p_land pairs SAME): 0x0099A300 29 -> 30 and 0x0099A2F0 1 -> 2 (CBaseClassNonCopyable,
  row 0x009E5C90), 0x00994930 6 -> 7 (row 0x00A62120), 0x00DC0830 0 -> 1. First bindings: 0x009A90B0, 0x009B9890, 0x009B98A0.
  0x009A9030 / 0x009A9040 stay at one spelling (Destroy_ stand-in -> CCountedPointer<..>::DeleteData).
- Link test (`LINK=` cleared, all 37 landing objects + a CCountedPointer.h includer): the explicit instantiations raise no LNK2005.
  One LNK2005, `CPersistTraits<unsigned long>::TransferBinaryIn` between 0x005079C0 and 0x004FDE80, is pre-existing: the HEAD objects of
  the same two rows give the same error (`logs/link_control_head.log`). Queued as debt.
NAME_FIXES queued: 3 from the agent (0x0099EF90 Reserve, 0x00D3AB00 / 0x00D3ABE0 nested CFH_Prisoner members) plus the TransferBinaryIn debt.
