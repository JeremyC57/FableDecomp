// CEngineMap::OnGameMapUnloaded() -- notify the scene object at +0 (when present) and clear the map pointer at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3EDC0, length 23 bytes.
// Callees: CEngineLandscapeMap::OnGameMapUnloaded() @ 0x00BDBFF0 (an empty `ret` in the retail build; called non-virtually on the object at +0). Shape: `mov ecx,[esi] ;
// test ecx,ecx ; je` around the call, then a direct `mov dword ptr [esi+8],0`.
// Callee respelled (NF177, PRIORITY q11978): 0x00BDBFF0 was the stand-in ?OnGameMapUnloaded@CEngineScene_00bdbff0@@QAEXXZ; now Ego_r.pdb
//   0x009B4730 / FableWin.pdb 0x02CC75D0 ?OnGameMapUnloaded@CEngineLandscapeMap@@QAEXXZ (the object at +0 is CEngineMap::LandscapeMap),
//   declared by engine/_quarantine/CEngineLandscapeMap.h's NF160 opt-in (the include the caller 0x00B40230 uses).
#define FABLE_RETAIL_CENGINELANDSCAPEMAP_ONGAMEMAPUNLOADED
#include "engine/_quarantine/CEngineLandscapeMap.h"

class CEngineMap { public:
    CEngineLandscapeMap*   m_pScene;               // +0x00 (Ego_r LandscapeMap)
    void*                  m_pData;                // +0x04
    void*                  m_pGameMap;             // +0x08
    void OnGameMapUnloaded();
};

void CEngineMap::OnGameMapUnloaded()
{
    if (m_pScene)
        m_pScene->OnGameMapUnloaded();
    m_pGameMap = 0;
}