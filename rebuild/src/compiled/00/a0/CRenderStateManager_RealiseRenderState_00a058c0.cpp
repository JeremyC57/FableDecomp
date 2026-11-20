// CRenderStateManager::RealiseRenderState() -- send every queued state whose desired value differs from the cached one to the
// device (render, texture-stage or sampler state by kind), clear each entry's dirty flag and empty the update list.
// Leaf: ?RealiseRenderState@CRenderStateManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A058C0.
// Port (NF133, q4660): the July CRenderStateManagerRealiseView stand-in (fable_render_state.h) replaced by the shared
//   engine/CRenderStateManager.h retail branch (PD3DDevice +0x04, StateUpdateList +0x2008, StateUpdateListSize +0x280C; CStateInfo
//   DesiredState / CurrentState / the D3D state id union +0x0C / DirtyListFlag +0x10 / StateType +0x11 / TextureStage +0x12) and
//   engine/IDirect3DDevice9.h (SetRenderState slot 57, SetTextureStageState slot 67, SetSamplerState slot 69). Own leaf now
//   Ego_r.pdb 0x00B70370 / FableWin.pdb 0x030F16C0 ?RealiseRenderState@CRenderStateManager@@QAEXXZ, which all 29 landed callers bind.
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#include "engine/CRenderStateManager.h"
#include "engine/IDirect3DDevice9.h"

void CRenderStateManager::RealiseRenderState()
{
    for (long index = 0; index < StateUpdateListSize; ++index)
    {
        CStateInfo* state = StateUpdateList[index];
        unsigned long desired = state->DesiredState;
        state->DirtyListFlag = false;
        if (desired != state->CurrentState)
        {
            switch (state->StateType)
            {
            case 1:
                PD3DDevice->SetRenderState(state->RenderStateParam, desired);
                break;
            case 2:
                PD3DDevice->SetTextureStageState(state->TextureStage, state->TextureStageStateParam, desired);
                break;
            case 3:
                PD3DDevice->SetSamplerState(state->TextureStage, state->SamplerStageStateParam, desired);
                break;
            }
            state->CurrentState = state->DesiredState;
        }
    }
    StateUpdateListSize = 0;
}
