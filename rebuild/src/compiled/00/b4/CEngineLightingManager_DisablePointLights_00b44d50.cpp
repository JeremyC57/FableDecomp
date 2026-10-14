// CEngineLightingManager::DisablePointLights() -- reset shader lights 1..count on the shader render manager, then zero the enabled-point-light count at +0xa0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44D50, length 56 bytes.
// Callees: CShaderRenderManager::ResetLight @ 0x00988640 (on the global manager @ 0x01436E14, light index i+1). Shape: the count is re-read from the member after
// every call (the call may change it), the index runs in eax with `lea esi,[eax+1]` for the argument and a signed `jl` test; esi is saved only around the loop;
// the final zero store is a direct `mov dword ptr [edi+0A0h],0`. The count sits at +0xa0 in the TLC object (the Ego PDB has it at +0xa4), so it is modelled by address.
struct CShaderRenderManager {
    void ResetLight(long index);                   // 0x00988640
};
extern CShaderRenderManager* g_pShaderRenderManager_01436e14;   // 0x01436E14

struct CEngineLightingManager {
    unsigned char _pad[0xa0];
    long          m_shaderPointLightsEnabledCount; // +0xa0
    void DisablePointLights();
};

void CEngineLightingManager::DisablePointLights()
{
    for (long i = 0; i < m_shaderPointLightsEnabledCount; ++i)
        g_pShaderRenderManager_01436e14->ResetLight(i + 1);
    m_shaderPointLightsEnabledCount = 0;
}