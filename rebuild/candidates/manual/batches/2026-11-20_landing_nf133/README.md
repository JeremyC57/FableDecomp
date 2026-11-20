# 2026-11-20 landing nf133: PRIORITY slice 11 (14 rows)

Records: `naming/2026-11-20_agentNF133/` (selfcheck 0 problems). 33 entries: 8 done, 4 done-already, 1 rejected (q5005), q4727 landed here
(below), 3 deferred, 16 open (hand-back on context). NF136 took the 16 open entries plus q4292 / q6168, which nf129 unblocked; q5050 waits
for nf132.

## Rows (14): 3 MATCH, 11 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q4727 trio (advisor decision, owner delegated): 0x00A0AA20 -> CRenderManagerCore::EnableXBoxShaderConstants, with its binders 0x00987FE0 /
  0x009880E0 taking the PDB leaves `?Enable/DisableVertexShaders@CShaderRenderManager@@QAEXXZ` (both PDBs; already bound by 0x009D8AD0). Their
  old self leaves were the `...View` stand-ins, not the `CShaderEnableState` spelling four outside rows bind (0x00B5E700, 0x00B61690,
  0x00B88BC0, 0x00BCB800). That split exists at HEAD and does not grow; the four rows are queued.
- 0x009B08C0 CDefinitionManager::Compile: GFGetTime.h in place of the local extern (its only change vs HEAD; NF131's port of the same row
  was held at nf131).
- Other self renames and binder respells: 0x009E50C0, 0x009E9080, 0x009FE620, 0x00A02220, 0x00A04090, 0x00A058C0, 0x00A62E20, 0x00B26360,
  0x00B2C8E0, 0x00B4F510 (RENAMES_applied.tsv). Tests for 0x00A058C0, 0x00987FE0, 0x009880E0, 0x00A04090 rewritten (same PassPattern).

## Checks
- 237 rows rechecked, 236 good; the bad one (0x00BF4570) is a pre-existing DIFFER. No-new-split gate 0; bindings_multi_check 1.
- Table: no set grew; 0x009E2590 12 -> 11, 0x009BE420 2 -> 1, 0x009E1BC0 2 -> 1.
- One-definition: full defined-symbol diff vs the HEAD objects (`logs/symbol_diff.txt`). Each new symbol is the row's own leaf, except
  0x00A62E20's `??_GCMovableResourceMemoryPool@@UAEPAXI@Z`: a pick-any COMDAT (LINK_ONCE_DISCARD) produced by the shared
  CMovableResourceMemoryPool.h class, defined strongly by no landed row (retail 0x00A62F90 is a free stand-in row). Link test over the 14
  objects (`LINK=` cleared): no LNK2005.

## Coordinator answers (advisor, owner delegated)
- q5005 rejection stands: retail is VC7.1 without /Zc:wchar_t, so wchar_t parameters spell `G`, not `_W`. Row 0x00540320 (built with
  /Zc:wchar_t, the only `_W` source) is queued as the outlier.
- q4941: done except 0x00BEBED0, an old ungraded multi-block row; with 0x00BEC000 / 0x00BEC470 / 0x00BEC950 (q4942 / q4943) left as found
  and queued.
