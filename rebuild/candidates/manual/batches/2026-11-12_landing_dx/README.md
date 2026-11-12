# 2026-11-12 landing dx -- chunk Q28 (first pass): 3 of 8 small parked rows matched

Source: `pending/2026-11-12_linux_handQ28/`. Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1):
0x009B00D0 CParentDefClassBase::AddSubDef, 0x00997880 NFile::GetItemFromPath, 0x004BE3E0
NGraphicAppearance::CTCGraphicAppearance::IsResourcesLoaded.

Headers: only the two the matched rows use are landed: CDefClassBase.h SUBDEFINFO_CTOR and C3DMeshInfo.h ISLOADED / PHYSICSMESH.
Both are additions only (hdrcheck2 default=same), merged against 49900ed94. The other 11 overlay headers belong to rows still being
worked.
Coordinator fixes: payload addresses carried a `0x` prefix and the name column repeated the class; both normalised.
Checks: reserved_check 0 reserved rows (0x00997880 cites its landed twin 0x009E4CB0 in a comment); bindings_multi_check at baseline;
funclet_includes_check 1317 checked, 0 broken.
Queued (land1011dx): 0x00AE9450 is the CCountedPointer<NParticleEngine::CParticleEmitter>(CParticleEmitter*) ctor (landed
placeholder `CCountedPointer_00ae91d0`).
Header debt noted by the agent (NOTES): two headers for CParticleEmitterDatabase; CMap.h declares
GetThemePaletteIndexFromThemeDefIndex const but both PDBs say non-const; CHeightMap::GetIndexIntoCells contradicts 0x008F6FC0.
