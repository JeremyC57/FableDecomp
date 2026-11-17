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
// NF52 (round 7): moved onto engine/CScopedShadersEnable.h / engine/CShaderRenderManager.h; the singleton is
// CShaderRenderManager::SingletonInstance (?SingletonInstance@CShaderRenderManager@@0V1@A, 0x013BC470, through Get()) and the
// inlined setters are the header's SetVertexShadersEnable / SetPixelShadersEnable (FableWin.pdb). Was a local CShaderRenderManager
// model + the made-up extern g_ShaderRenderManager.
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_DTOR_OUTOFLINE
#include "engine/CScopedShadersEnable.h"

CScopedShadersEnable::~CScopedShadersEnable()
{
    CShaderRenderManager::Get().SetVertexShadersEnable(OldVertexEnable, 0);
    CShaderRenderManager::Get().SetPixelShadersEnable(OldPixelEnable, 0);
}
