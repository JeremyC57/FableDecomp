// Flags: 
// CEnginePrimitiveManagerWeaponTrails::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass) -- render one weapon trail inside a render-state capture block: pixel shaders off, the engine component at 0x01436E90 prepares its state, two tracked render states set (0 and the global default), the trail drawn, pixel shaders restored to their previous state and the capture block closed
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerWeaponTrails@@UAEXAAVCEngineSubPrimitiveBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCD490, length 368 bytes (with int3 padding; body 0x16a), `ret 8` (virtual).
// PDB: ?RenderSubPrimitive@CEnginePrimitiveManagerWeaponTrails@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z.
// Callees: CShaderRenderManager::DisablePixelShaders @ 0x00988190 / EnablePixelShaders @ 0x00988110 (global @ 0x01436E14,
//   PixelShadersEnabled byte +0x391), the virtual slot 0 of the object at +0x20 of the global @ 0x01436E90, RenderWeaponTrail
//   (const CEngineInternalPrimitiveWeaponTrail&) @ 0x00BCC800 (the sub-primitive's Parent +0x0c), CRenderStateManager::
//   RestoreCaptureBlock @ 0x00A05840 (global @ 0x013CB508).
// Globals: the render-state manager is reached both as 0x013CB508 (capture block open/close) and as the render core
//   0x01436E18 (tracked states +0x2854 and +0x2890, the model of the landed EnableFog 0x00B46890); the second state's value
//   is the global @ 0x01396FB8.
// Tracked state: `state.Set(v)` tests the value on the state and forwards to an inline manager ChangeState(state, v)
//   (core in eax, state in ecx, the manager mask in edx as retail); a pure state-side or manager-side setter swaps registers.
// Shape: opening the capture block is inline (push the block marker +0x3a24 on the capture stack, shift the capture mask);
//   each state change is the inline tracked-state Set; the restore arms tail-duplicate the RestoreCaptureBlock call.
#include "fable_compiler.h"
class CEngineSubPrimitiveBase;

// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderStateManager at offset 0): the shared
//   inline setters ZBufferWriteEnable(false) (+0x2854) and PolyCullMode (+0x2890) [POLY_CULL_NONE] through D3DPolyCullModeTable
//   0x01396FB0; the capture open / RestoreCaptureBlock pair on 0x013CB508 is a CRenderStateScopedCaptureBlock
//   (engine/CRenderStateCaptureBlock.h). Was a local CRenderStateManager / CTrackedRenderState model typed onto g_pRenderManagerCore
//   and g_pRenderStateManager, and g_renderStateDefault_01396fb8. 0x01436E90 is the shared GPEngineStateBlocks singleton. Its AddSmoothAlphaMode is at +0x20.
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18

class CShaderRenderManager { public:
    unsigned char _pad[0x391];
    bool          PixelShadersEnabled;             // +0x391
    void DisablePixelShaders();                    // 0x00988190
    void EnablePixelShaders();                     // 0x00988110
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }   // 0x01436E14

#define FABLE_RETAIL_CENGINESTATEBLOCKS
#include "engine/CEngineStateBlocks.h"
namespace NEngineInternal { extern CEngineStateBlocks* GPEngineStateBlocks; } // 0x01436E90

class CEngineInternalPrimitiveWeaponTrail;
class CEngineSubPrimitiveBase {
public:
    void*                                _vptr;    // +0x00
    unsigned long                        RenderFlags;   // +0x04
    unsigned char                        Alpha;    // +0x08
    CEngineInternalPrimitiveWeaponTrail* Parent;   // +0x0c
};

class CEnginePrimitiveManagerWeaponTrails {
public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long pass);
    private: void RenderWeaponTrail(const CEngineInternalPrimitiveWeaponTrail& trail); public:   // 0x00BCC800
};

void CEnginePrimitiveManagerWeaponTrails::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long pass)
{
    CRenderStateScopedCaptureBlock capture_block;
    bool pixelShaders = NEngineInternal::GPShaderRenderManager->PixelShadersEnabled;
    if (pixelShaders)
        NEngineInternal::GPShaderRenderManager->DisablePixelShaders();
    NEngineInternal::GPEngineStateBlocks->AddSmoothAlphaMode.Apply();
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    RenderWeaponTrail(*subPrimitive.Parent);
    if (pixelShaders != NEngineInternal::GPShaderRenderManager->PixelShadersEnabled) {
        if (pixelShaders)
            NEngineInternal::GPShaderRenderManager->EnablePixelShaders();
        else
            NEngineInternal::GPShaderRenderManager->DisablePixelShaders();
    }
}