// CEngine::UpdateTextureMorphCache() -- change the graphic data bank's scene, then the surface-composition manager's texture-morph cache update (sibling call)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B239C0, length 27 bytes.
// Callees: CGraphicDataBank::ChangeScene @ 0x009FD0C0 (on the bank at +0x954 of the engine data record @ 0x01436E98), the surface-composition manager's
// UpdateTextureMorphCache @ 0x00B2D9F0 (on the manager @ 0x01436EAC; a `jmp` because it is the last call and takes no arguments). A static member: no
// `this`, the two objects come from the engine's static singletons.
struct CGraphicDataBank {
    void ChangeScene();                            // 0x009FD0C0
};

struct CEngineData_01436e98 {
    unsigned char      _pad[0x954];
    CGraphicDataBank*  m_pGraphicBank;             // +0x954
};

struct CEngineSurfaceCompositionManager {
    void UpdateTextureMorphCache();                // 0x00B2D9F0
};

extern CEngineData_01436e98*               g_pEngineData_01436e98;           // 0x01436E98
extern CEngineSurfaceCompositionManager*   g_pSurfaceComposition_01436eac;   // 0x01436EAC

struct CEngine {
    static void UpdateTextureMorphCache();
};

void CEngine::UpdateTextureMorphCache()
{
    g_pEngineData_01436e98->m_pGraphicBank->ChangeScene();
    g_pSurfaceComposition_01436eac->UpdateTextureMorphCache();
}
