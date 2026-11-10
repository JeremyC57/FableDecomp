# 2026-11-10 landing bb -- Codex pass 19 return: 33 of 40 parked near misses matched

Source: `pending/2026-11-10_codex_pass19/return_2026-11-10/` (STATUS, LABELS, NAME_FIXES, LESSONS, FINDINGS, PACKING_APPROVAL,
hdr/README.md, differ/ for the 7 parked rows with their PROBES.md). The zip came back by Drive; SHA256SUMS checked.
vland --land on our lane: 33 wins (RELOCATION_MATCH), tests PASS. Manifest names/modules respelled for 12 rows.
- 0x009E4CB0 CDriveManager::GetItemFromPath came back with `/Zc:wchar_t` (`PB_W`); landed WITHOUT the flag per the owner
  2026-10-29 wide-text rule, so it binds `?GetItemFromPath@CDriveManager@@AAE_NPBGKPAG@Z` (same bytes).
- 0x00488620 / 0x004886F0 are the non-const / const overloads of CPlayer::GetPNearestTCVillage (distinct leaves).
- 0x00C1D1C0 InvertMatrix3x4 and 0x00C462E0 CFloatQuad::Interpolate are descriptive names (no original found after a search).
Headers: 119 from Codex's hdr/ (39 new, 80 existing) 3-way merged onto HEAD with the pass 19 package include/ as base, no conflicts;
hdrcheck2 default=same except CAIBrain.h (only `__LINE__`-numbered static-assert names move) and CVectorMap.h (whitespace).
New: engine/CFileInstaller.h carries `volatile` members that both PDBs qualify (Ego_r/FableWin cv modifier records, `// Retail-volatile:`
line in 0x009C5890); engine/C3DVector.h opt-in FABLE_RETAIL_C3DVECTOR_PACKED (pack 1; Ego_r puts
CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::OurPos at +1), used by 0x00752E00 and 0x00961640.
Parked (7): 0047dd9d, 0047ed76, 00680050, 00774c60, 0086be40, 00933db0, 00986ed0 -- residues in STATUS.tsv. Reservation released.
Queued land1011bb: Codex's 140 NAME_FIXES (5 `_W` spellings marked reject-for-G).
