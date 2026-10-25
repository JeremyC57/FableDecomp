// CScopedShadersEnable::CScopedShadersEnable(bool vertexShaders, bool pixelShaders) -- scoped shader-state switch: remember the global render manager's vertex/pixel shader enables, then switch each to the requested state when it differs
// Leaf: ??0CScopedShadersEnable@@QAE@_N0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8AD0, length 112 bytes (103 + int3 padding).
// Callees: CShaderRenderManager::EnableVertexShaders @ 0x00987FE0 / DisableVertexShaders @ 0x009880E0 /
// EnablePixelShaders @ 0x00988110 / DisablePixelShaders @ 0x00988190, all on the global manager.
// Globals: the CShaderRenderManager singleton @ 0x013BC470 (VertexShadersEnabled +0x390 = 0x013BC800,
// PixelShadersEnabled +0x391 = 0x013BC801; PDB +0x3a0/+0x3a1).
// Identity: FableWin.pdb ??0CScopedShadersEnable@@QAE@_N0@Z / ??1CScopedShadersEnable@@QAE@XZ and Ego_r.pdb's 2-byte
// CScopedShadersEnable {bool OldVertexEnable +0, bool OldPixelEnable +1}: the body returns `this` (`mov eax,esi`), stores
// two saved bytes at this+0/+1 and has no callers by direct call (the scope objects are inlined); its restoring
// counterpart is ~CScopedShadersEnable 0x009D8B40 (FINDINGS chunk 412), which reads this+0/+1 back and makes the same
// enable/disable calls. The catalog's "CShaderRenderManager::SetShaderStates" and the earlier descriptive
// "CShaderStatesScope" are replaced by the PDB name.
// Shape: the manager's two out-of-line setters SetVertexShadersEnabled(bool, bool* old) @ 0x009D8A80 and
// SetPixelShadersEnabled(bool, bool* old) @ 0x009D8AA0 (`if (old) *old = current;` then compare-and-switch)
// are inlined with old = &OldVertexEnable / &OldPixelEnable, which is where VC's null tests of `this`
// and `this+1` come from; the pixel-shader switch is tail-duplicated into both returns.

struct CShaderRenderManager {
    unsigned char _pad000[0x390];
    bool          VertexShadersEnabled;          // +0x390
    bool          PixelShadersEnabled;           // +0x391
    void EnableVertexShaders();                  // 0x00987FE0
    void DisableVertexShaders();                 // 0x009880E0
    void EnablePixelShaders();                   // 0x00988110
    void DisablePixelShaders();                  // 0x00988190
    void SetVertexShadersEnabled(bool enable, bool* old)       // out of line @ 0x009D8A80
    {
        if (old)
            *old = VertexShadersEnabled;
        if (enable != VertexShadersEnabled) {
            if (enable)
                EnableVertexShaders();
            else
                DisableVertexShaders();
        }
    }
    void SetPixelShadersEnabled(bool enable, bool* old)        // out of line @ 0x009D8AA0
    {
        if (old)
            *old = PixelShadersEnabled;
        if (enable != PixelShadersEnabled) {
            if (enable)
                EnablePixelShaders();
            else
                DisablePixelShaders();
        }
    }
};

extern CShaderRenderManager g_ShaderRenderManager;   // 0x013BC470

class CScopedShadersEnable {
public:
    bool OldVertexEnable;                        // +0x00
    bool OldPixelEnable;                         // +0x01
    CScopedShadersEnable(bool vertexShaders, bool pixelShaders);
};

CScopedShadersEnable::CScopedShadersEnable(bool vertexShaders, bool pixelShaders)
{
    g_ShaderRenderManager.SetVertexShadersEnabled(vertexShaders, &OldVertexEnable);
    g_ShaderRenderManager.SetPixelShadersEnabled(pixelShaders, &OldPixelEnable);
}