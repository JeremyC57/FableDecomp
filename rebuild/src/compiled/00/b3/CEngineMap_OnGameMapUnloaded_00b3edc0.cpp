// CEngineMap::OnGameMapUnloaded() -- notify the scene object at +0 (when present) and clear the map pointer at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3EDC0, length 23 bytes.
// Callees: the scene's unloaded handler @ 0x00BDBFF0 (an empty `ret` in the retail build; called non-virtually on the object at +0). Shape: `mov ecx,[esi] ;
// test ecx,ecx ; je` around the call, then a direct `mov dword ptr [esi+8],0`.
struct CEngineScene_00bdbff0 {
    void OnGameMapUnloaded();                      // 0x00BDBFF0
};

class CEngineMap { public:
    CEngineScene_00bdbff0* m_pScene;               // +0x00
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