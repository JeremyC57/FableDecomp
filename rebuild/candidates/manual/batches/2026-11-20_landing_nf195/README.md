# Landing nf195 (Account A, 2026-11-20): remaining queue, next 50: STL / counted-pointer families (17 rows)

Agent NF195 (round 10) finished its slice: 50 entries, 12 done, 13 done-already, 1 rejected, 18 blocked, 6 deferred and 0 open. Its
files are in `naming/2026-11-20_agentNF195/`, with the probe of 45 referencing rows (`work/probe_q6320.tsv`), the 0.0f stand-in list
(`work/q6387_standins.tsv`) and the unfiltered pick-any logs. The families with rows left to port (q4389, q4394, q5089, q6315 / q6319,
q6332 / q6341, q6333 / q6342, q6416), the two very large census-only families (q4451, q4456) and the six entries deferred for rows
since landed by NF192 / NF194 go back to the queue unclaimed (18 entries).

- 17 rows land: 2 MATCH (0x006588CF, 0x00F0CA30) + 15 RELOCATION_MATCH, every test PASS.
- q4389: 0x006E9270, 0x006E92C0, 0x006E9E40 and 0x006EDC50 move onto stlport/fable_map.h with MFIND_NOINLINE; the 0x006E9270 /
  0x006E92C0 tests are rewritten. 0x006E93C0 / 0x006E9700 are left after three forms (frame +8).
- q4567: the six CEngineWaterRenderer rows (0x00B75CA0 .. 0x00B7F600) move onto engine/CArray.h, with the new
  FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY opt-in: the existing noinline form emits a 94-byte resize COMDAT against owner
  0x00B7EDF0's 102 bytes.
- q4774: 0x00579FAA binds `~_Rb_tree<EHeroTrainableStatType, ... SExpSpendUndo>`, its last stand-in (nf189 left it).
- q6206 / q6207: 0x00AAD5D0 CWaveFile::ConvertToMono moves onto CWaveFile.h / CArray.h and binds `vector<short>` ctor / operator=, the
  nf189 self leaves; test rewritten. Its HEAD `??4?$CArray@F@@` COMDAT (89 bytes, DIFF at HEAD) is now 19 bytes and binds nothing.
- q6240: 0x00442870 is `~vector<CSubtitleRenderer::CEmbeddedHighlight>` (FableWin.pdb); test rewritten. It uses the new
  FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDEDHIGHLIGHT_NATURAL_PACK opt-in. engine/CSubtitleRenderer.h is `#pragma pack(1)`, which makes the
  record 0x11 bytes where Ego_r.pdb has 0x14 (retail strides 0x14 in 0x00442870, 0x004940AC and ProcessEmbeddedHighlightTags). The
  opt-in lays out the nested record under the default packing; the outer class and its asserts are unchanged. Making it the default is
  a header claim, queued.
- Self renames (6, RENAMES section 1): 0x00798D30 CTCShopKeeper::GetHint, 0x006588CF CTCVillage::GetCostToBribe, 0x00485D25
  `vector<NUISystem::CComponent*>::clear`, 0x00F0CA30 `vector<CQ_AmbushTradersScript::TeleportData>::swap` (both PDBs), plus the
  q4567 / q6240 self rows.
- Headers (add-only, `orig` equals HEAD, default paths unchanged, no landed includer defines the macros, pin_asserts pinned 0):
  stlport/fable_containers.h VECTOR_RESIZE_VALUE_DECL_ONLY, engine/CSubtitleRenderer.h EMBEDDEDHIGHLIGHT_NATURAL_PACK.
- Pick-any (`logs/pickany_check.txt`, plus the agent's unfiltered `work/pickany_final.log` / `pickany_full.log`): 135 new symbols were
  matched against every landed Leaf and table target; the 9 with owners (0x0041C830, 0x00526120, 0x006F0250, 0x00AADD80, 0x00AADE80)
  are byte-identical. The MISSING lines are the folded iterator comparator 0x0041C830 (MATCH; nf177 / nf180 / nf185). 0x00442870's 12
  CEmbeddedHighlight COMDATs have no owner (retail inlines the destroy loop).
- Blocked with decision or header items: q5215 (engine/CConsoleCommandBase.h claim), q6245 (no destructor or EH frame for a zeroed
  temporary in retail), q6310 (anonymous-namespace functor scaffolding), q6373 / q6384 (0x00E81A60 defines the same strong CheckForFish
  as 0x00E81AD0; nested-class header item), q6387 (the merged 0.0f literal 0x0122DEDC, 1,092 referencing rows). q6377 is rejected:
  the folded STLport body carries both ~list and ~_List_base legitimately (nf104 advisor decision).
- Queued debt: 0x00485AA7 bound under six spellings (binder ports), 0x00440BA0 under two, the CSubtitleRenderer pack(1) header claim.
- Link (`logs/link_landing.log`): 1 LNK2005, the same symbol as HEAD (`logs/link_control_head.log`).
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 61 good, 0 bad;
  owner recheck good.
- tablecheck (`logs/tablecheck.txt`): 0x00485AA7 6 -> 5, 0x006F0250 10 -> 9, 0x00AADD80 / 0x00AADE80 / 0x00B7EDF0 2 -> 1; none grows. No
  out-of-image oracle line added.
