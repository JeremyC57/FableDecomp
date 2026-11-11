# 2026-11-11 landing br -- chunk H40b (Fable 5.2, second pass on the longest parked rows): 4 NScript quest-script Main bodies

Source: `pending/2026-11-11_linux_handH40b/` (STATUS / NOTES / LESSONS / LABELS / SPLITS / BOUNDS_FIX / differ). vland --land: 4 RELOCATION_MATCH,
tests PASS. All within the 10-grade budget.
- 0x00E3EB10 CV_BordelloScript::CBordelloLady::Main (8 grades): `for (;; NSF) { if (T) return; ... }` main loop + one in-body
  `#pragma inline_depth(0)` at the first in-loop thread-exit return (retail calls ~seh_me there, 0x00E3ED00).
- 0x00DFE0F0 CQ_TraderConflictGoodScript::CTraderToRescue::Main (4): begging loop `for (;; NSF) { if (T) return; if (talked) break; ... }`.
- 0x00D8BCD0 CQ_HerosOldHouseScript::CFishermansWife::Main (2): `while (!T)` main loop (one ~WifeTimer reference fewer re-sorts the frame).
- 0x00E84B80 CV_FisticuffsClubScript::CFisticuffsMember::Main (2): switch case LEAVING_FC written second (enum order).
CPredicateBase NOVTABLE is used by 0x00D8BCD0 / 0x00E3EB10 only; their retail bodies have no base-vtable store 0x012C2FF4 (checked on the
oracle bytes); 0x00E84B80 has that store and does not use it.
Headers (base ab4bb2a46, hdrcheck2 default=same): CQ_HerosOldHouseScript (CFISHERMANSWIFE), CQ_TraderConflictGoodScript (TRADERTORESCUE),
CV_BordelloScript (CBORDELLOLADY) -- opt-ins carried from older hand-backs. Parked 31 (8 worked, 23 not reached); true extents for
0x00D0F640 (2101) and 0x00B10B10 (4276) in BOUNDS_FIX.
