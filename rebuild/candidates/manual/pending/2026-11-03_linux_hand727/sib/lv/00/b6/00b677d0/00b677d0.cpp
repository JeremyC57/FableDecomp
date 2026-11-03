// CEngineLandscapeRenderer::SetupForegroundStates() -- the foreground landscape pass's texture-stage-1 sampler states: U and V address modes and the minification filter, each written through the render-state manager's cached SetRenderState (skip an unchanged value; otherwise bookmark the old value on the state stack once per capture block, store the new one and queue the state for the device update)
// Leaf: ?SetupForegroundStates@CEngineLandscapeRenderer@@AAEXXZ
// Grade: RELOCATION_MATCH (retry wave 2) -- the three SetTexture* inline helpers take the D3D value by const reference,
//   so the value stays a memory operand (as retail's table lookup would be) and the core-pointer copy precedes the value
//   copy in blocks 2 and 3. NOTE: Ego_r.pdb spells these helpers SetTextureUVModeU/V(long, TEXTURE_UV_MODE) and
//   SetTextureFilterModeMin(long, TEXTURE_FILTER_MODE) by value; the retail helpers most likely index a D3D-value table
//   by the enum (0x01396DC8 / 0x01396E60 are table entries), which this const& models without knowing the table bases
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B677D0, length 333 bytes, plain `ret`; `this` is never read. Single caller
//   0x00B6B17E (CEngineLandscapeRenderer::Render).
// Name: Ego_r.pdb `?SetupForegroundStates@CEngineLandscapeRenderer@@AAEXXZ` (private; the catalog binding spells QAE).
// Callees: none (everything inline).
// Globals: g_pRenderManagerCore 0x01436E18 (re-read after every state, as retail does); the two D3D values come from the
//   engine's enum-to-D3D lookup tables -- 0x01396DC8 (texture address table entry 0 = 1, D3DTADDRESS_WRAP; existing
//   descriptive binding g_renderStateValue_01396dc8) and 0x01396E60 (filter table entry = 3, D3DTEXF_ANISOTROPIC; descriptive).
// Layout: CRenderManagerCore derives from CRenderStateManager (Ego_r.pdb sizes: the manager is 0x3A3C bytes and the core's
//   own DisplayManager starts at +0x3A3C). CRenderStateManager (Ego_r.pdb names): StateStack +0x8, StateUpdateList +0x2008,
//   StateStackSize +0x2808, StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814, DeviceState +0x2818; CDeviceState
//   TextureUVModeU[8] +0x6CC, TextureUVModeV[8] +0x76C, TextureFilterModeMin[8] +0x8AC; CStateInfo (0x14): DesiredState +4,
//   BookmarkMask +8, DirtyListFlag +0x10. Stage 1 entries: +0x2EF8 / +0x2F98 / +0x30D8, as retail addresses them.
//   engine/CRenderStateManager.h keeps StateStack / DeviceState opaque, so the nested types are added on the donor layout.
// Shape: the PDB's public setters SetTextureUVModeU / SetTextureUVModeV / SetTextureFilterModeMin (stage, mode) map the mode
//   through a lookup table and call the private SetRenderState(value, state) (modelled here with the looked-up value); the
//   bookmark test reads the manager mask first, and the dirty flag is tested before the new value is stored.
#define CRenderStateManager CRenderStateManagerLayout
#include "engine/CRenderStateManager.h"
#undef CRenderStateManager

extern unsigned long g_renderStateValue_01396dc8;   // 0x01396DC8 texture address-mode table entry (D3DTADDRESS_WRAP)
extern unsigned long g_renderStateValue_01396e60;   // 0x01396E60 texture filter-mode table entry (D3DTEXF_ANISOTROPIC)

class CRenderStateManager : public CRenderStateManagerLayout {
public:
    struct CStateInfo {                                // 0x14 bytes
        unsigned long CurrentState;                    // +0x00
        unsigned long DesiredState;                    // +0x04
        unsigned long BookmarkMask;                    // +0x08
        unsigned long StateId;                         // +0x0C (union)
        bool          DirtyListFlag;                   // +0x10
        unsigned char StateType;                       // +0x11
        unsigned char TextureStage;                    // +0x12
    };
    struct CStateStackElement {
        CStateInfo*   State;                           // +0x00
        unsigned long Value;                           // +0x04
    };
    struct CDeviceState {
        unsigned char _pad000[0x6cc];
        CStateInfo    TextureUVModeU[8];               // +0x6CC
        CStateInfo    TextureUVModeV[8];               // +0x76C
        unsigned char _pad80c[0x8ac - 0x80c];
        CStateInfo    TextureFilterModeMin[8];         // +0x8AC
    };
    CStateStackElement* GetStateStack() { return reinterpret_cast<CStateStackElement*>(StateStack); }
    CDeviceState& GetDeviceState() { return *reinterpret_cast<CDeviceState*>(DeviceState); }

    __forceinline void SetTextureUVModeU(long stage, const unsigned long& d3dValue) { SetRenderState(d3dValue, GetDeviceState().TextureUVModeU[stage]); }
    __forceinline void SetTextureUVModeV(long stage, const unsigned long& d3dValue) { SetRenderState(d3dValue, GetDeviceState().TextureUVModeV[stage]); }
    __forceinline void SetTextureFilterModeMin(long stage, const unsigned long& d3dValue) { SetRenderState(d3dValue, GetDeviceState().TextureFilterModeMin[stage]); }
private:
    __forceinline void SetRenderState(unsigned long value, CStateInfo& state)
    {
        if (state.DesiredState != value) {
            if ((CurrentBookmarkMask & state.BookmarkMask) == 0) {
                state.BookmarkMask |= CurrentBookmarkMask;
                GetStateStack()[StateStackSize].State = &state;
                GetStateStack()[StateStackSize].Value = state.DesiredState;
                ++StateStackSize;
            }
            state.DesiredState = value;
            AddStateToUpdateList(state);
        }
    }
    __forceinline void AddStateToUpdateList(CStateInfo& state)
    {
        if (!state.DirtyListFlag) {
            state.DirtyListFlag = true;
            StateUpdateList[StateUpdateListSize] = reinterpret_cast< ::CStateInfo*>(&state);
            ++StateUpdateListSize;
        }
    }
};
class CRenderManagerCore : public CRenderStateManager {
};
extern CRenderManagerCore* g_pRenderManagerCore;   // 0x01436E18

#define CEngineLandscapeRenderer CEngineLandscapeRendererLayout
#include "engine/CEngineLandscapeRenderer.h"
#undef CEngineLandscapeRenderer
class CEngineLandscapeRenderer : public CEngineLandscapeRendererLayout {
private:
    void SetupForegroundStates();
};

void CEngineLandscapeRenderer::SetupForegroundStates()
{
    g_pRenderManagerCore->SetTextureUVModeU(1, g_renderStateValue_01396dc8);
    g_pRenderManagerCore->SetTextureUVModeV(1, g_renderStateValue_01396dc8);
    g_pRenderManagerCore->SetTextureFilterModeMin(1, g_renderStateValue_01396e60);
}