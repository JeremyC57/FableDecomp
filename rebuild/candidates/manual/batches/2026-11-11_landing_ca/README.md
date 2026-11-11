# 2026-11-11 landing ca -- chunk Q1 (Fable 5.2 fast-authoring lane): 19 never-attempted rows

Source: `pending/2026-11-11_linux_handQ1/`. The user, 2026-11-11: "Add another agent authoring new rows as quickly and accurately as
possible". Chunk Q1 took the 50 smallest never-attempted unlanded rows: 19 matched (11 MATCH, 8 RELOCATION_MATCH; tests PASS), 15 are not
functions (interior fragments / tails / funclets -- STATUS.tsv, differ/ notes), 16 not reached. vland --land: 19 WINS.
- PDB-named: CTCGatherPointBase::OnInitialActivate, CTCTargetingAI::BulletTimeFrameUpdate, CUILocaleGraphicsDef::Copy, CRegionScriptDef::Copy,
  CDrawMoney::ClearPrimitivesIfNecessary, CSuspendableProcessMicrothread::ThreadEntry (158 bytes, not 46: 0x00A446F4 is its catch funclet).
- strmbase DECLARE_IUNKNOWN QueryInterface x3 (CBasePin, CBaseAllocator, CDeferredCommand).
- libpk (no PDB / RTTI): descriptive names per the owner made-up-name rule (NOTES.md), consistent with the landed libpk rows.
- 49 extent corrections / new starts appended to BOUNDS_FIX.tsv (SPLITS.tsv); 3 name fixes queued as land1011ca; 34 labels.
- Headers: new engine/CTCGatherPointBase.h, CRegionScriptDef.h, NUISystem_CUILocaleGraphicsDef.h; opt-ins FABLE_RETAIL_CPLAYERGUI_FRIEND_STAT_DRAWERS,
  FABLE_RETAIL_CASUSPENDABLEPROCESS_EXCEPTIONEXIT (hdrcheck2: default paths unchanged).
