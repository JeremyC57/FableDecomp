// CEngineLandscapeMap::OnGameMapUnloaded() -- empty in the retail build (`ret`)
// Leaf: ?OnGameMapUnloaded@CEngineLandscapeMap@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDBFF0, 1 byte (`ret`).
// Identity (NF177, PRIORITY q11978): Ego_r.pdb 0x009B4730 + FableWin.pdb 0x02CC75D0 ?OnGameMapUnloaded@CEngineLandscapeMap@@QAEXXZ, both
//   PDB-adjacent to OnGameMapLoaded (Ego_r 0x009B4720 / FableWin 0x02CC75C0 = retail 0x00BDBFE0, the landed sibling: retail keeps the
//   same order, 0x10 apart). CEngineMap::OnGameMapUnloaded (0x00B3EDC0) calls it on LandscapeMap (+0x00); all three landed callers
//   (0x00B3EDC0, 0x00B40230, 0x00B42B10) bind this spelling.
//   Re-authored under it: the landed row was the address-named placeholder C00bdbff0::M.
// Class: engine/_quarantine/CEngineLandscapeMap.h with the NF160 opt-in FABLE_RETAIL_CENGINELANDSCAPEMAP_ONGAMEMAPUNLOADED (the include the
//   caller 0x00B40230 uses).
#define FABLE_RETAIL_CENGINELANDSCAPEMAP_ONGAMEMAPUNLOADED
#include "engine/_quarantine/CEngineLandscapeMap.h"

void CEngineLandscapeMap::OnGameMapUnloaded()
{
}
