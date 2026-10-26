// CScopedShadersEnable::~CScopedShadersEnable() -- end of a scoped shader switch: restore the vertex- and pixel-shader enables the constructor saved, switching the global render manager only where its current state differs
// Leaf: ??1CScopedShadersEnable@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8B40, length 66 bytes.
// PDB (FableWin): ??1CScopedShadersEnable@@QAE@XZ, ??0CScopedShadersEnable@@QAE@_N0@Z (the constructor is the landed
// 0x009D8AD0, filed under the descriptive name CShaderStatesScope); members OldVertexEnable (+0) / OldPixelEnable (+1).
// The catalog label "CShaderRenderManager_ApplyShaderStates" is descriptive.
// Callees: CShaderRenderManager::EnableVertexShaders 0x00987FE0 / DisableVertexShaders 0x009880E0 /
// EnablePixelShaders 0x00988110 / DisablePixelShaders 0x00988190, on the singleton at 0x013BC470
// (VertexShadersEnabled +0x390 = 0x013BC800, PixelShadersEnabled +0x391 = 0x013BC801; the Ego_r PDB offsets are 0x10 higher).
// Shape: the manager's SetVertexShadersEnable / SetPixelShadersEnable(bool, bool* old) are inlined with a null `old`
// (the save folds away), leaving compare-and-switch; the pixel-shader calls are tail jumps (`jmp`), the destructor
// returning nothing.
class CShaderRenderManager
{ public:
    unsigned char _pad000[0x390];
    bool          VertexShadersEnabled;          // +0x390
    bool          PixelShadersEnabled;           // +0x391
    void EnableVertexShaders();                  // 0x00987FE0
    void DisableVertexShaders();                 // 0x009880E0
    void EnablePixelShaders();                   // 0x00988110
    void DisablePixelShaders();                  // 0x00988190
    void SetVertexShadersEnable(bool enable, bool* old)        // out of line @ 0x009D8A80
    {
        if (old)
            *old = VertexShadersEnabled;
        if (enable != VertexShadersEnabled)
        {
            if (enable)
                EnableVertexShaders();
            else
                DisableVertexShaders();
        }
    }
    void SetPixelShadersEnable(bool enable, bool* old)         // out of line @ 0x009D8AA0
    {
        if (old)
            *old = PixelShadersEnabled;
        if (enable != PixelShadersEnabled)
        {
            if (enable)
                EnablePixelShaders();
            else
                DisablePixelShaders();
        }
    }
};

extern CShaderRenderManager g_ShaderRenderManager;   // 0x013BC470

class CScopedShadersEnable
{
public:
    CScopedShadersEnable(bool vertexEnable, bool pixelEnable);   // 0x009D8AD0
    ~CScopedShadersEnable();
    bool OldVertexEnable;                        // +0x00
    bool OldPixelEnable;                         // +0x01
};

CScopedShadersEnable::~CScopedShadersEnable()
{
    g_ShaderRenderManager.SetVertexShadersEnable(OldVertexEnable, 0);
    g_ShaderRenderManager.SetPixelShadersEnable(OldPixelEnable, 0);
}