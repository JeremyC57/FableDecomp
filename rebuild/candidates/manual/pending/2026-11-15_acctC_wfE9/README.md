# acctC workflow hand-back wfE9 (unworked rows round 3, 2026-11-15)


`open/` holds the best drafts of the batch's unmatched rows (full residues in NOTES.md):
- 0x00648820: parked, DIFFER(3419v3464). Not reworked in this run: I put the effort into the twins and 0x00931390. It still needs a full port from the local C2DVector / C2DBoxF / CThing / CPlayerGuiDef models to the engine/ headers. There is no NPlayerGui-namespaced CDrawMiniMap header: engine/CDrawM
- 0x005AC10E: near_miss, DIFFER(3538v3532). 4 hunks, unchanged. Measured with c2prio --trace: the begin() local #6 (2 refs) is checked for copy forwarding (0x107820b7) and refused. It then has priority 20 (cost 4 x K5) against 13 for `it`, so it is coloured eax first. `it` sees eax -4 from the copy rela
- 0x005A4CDD: near_miss, DIFFER(3664v3668). 1 hunk at 0x005A4E49 (loop 2's entry test is not threaded past loop 3). New probes: moving `long index = 0` before loop 2 (3 placements) gives 3667 but index loses edi and the entry test stays. A guard around both loops gives retail's control flow; with it I t
- 0x009A25D0: byte-matches but broke a lane rule; in the fix-up workflow, not staged yet.
- 0x00879900: blocked, BLOCKED. Cannot be authored on shared headers: rebuild/include has no engine/CCameraModeDef.h and no NCameraModes_CProjectileWeapon.h. A proposed PDB-based header is in probe/hdr_proposed (not in hdr/). CUserProfileManager::GetCameraSensitivity and CTCTargetingPlayer::
- 0x00931390: parked, DIFFER(3836v3863). Measured lead, still on local models. The 'inline binary search in the cower branch' is the non-const CVectorMap::LowerBound expanded at one GetTC(0x0F) site only (retail 0x00932070). Adding FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND (probe/00931390_inlinelb.cp

