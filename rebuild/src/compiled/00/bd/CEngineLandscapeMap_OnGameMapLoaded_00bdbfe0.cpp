// CEngineLandscapeMap::OnGameMapLoaded() -- empty in the retail build (`ret`)
// Leaf: ?OnGameMapLoaded@CEngineLandscapeMap@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDBFE0, 1 byte (`ret`).
// Identity (NF59, round 7, queue q10537): Ego_r.pdb 0x009B4720 + FableWin.pdb 0x02CC75C0 ?OnGameMapLoaded@CEngineLandscapeMap@@QAEXXZ.
//   CEngineMap::OnGameMapLoaded(long) (Ego_r 0x009DD5D0, expanded in CEngineWorldMap::OnGameMapLoaded 0x00B401E0) ends with
//   LandscapeMap (+0x00)->OnGameMapLoaded(); both landed callers (0x00B401E0, CEngineWorldMap::SetWorld 0x00B42ED0) bind this spelling.
//   Re-authored under it: the landed row was the address-named placeholder C00bdbfe0::M (catalog _global::Nop_ret0_00bdbfe0).
// CEngineLandscapeMap has no shared header (engine/CEngineMap.h only forward-declares it): the one-member declaration below, as
//   0x00B401E0 declares it.
class CEngineLandscapeMap { public:
    void OnGameMapLoaded();
};

void CEngineLandscapeMap::OnGameMapLoaded()
{
}
