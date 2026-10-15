// CRenderStateManager::Init(IDirect3DDevice9* device) -- bind the device, forget every registered state, rebuild the state table, apply the defaults and resend them all to the hardware
// Grade: DIFFER(117v115) -- retail calls ForceResendRenderStatesToHardware right after SetDefaultRenderStates without reloading ecx (VC knew the 8 KB call-free SetDefaultRenderStates 0x00A059E0 body in the same TU keeps ecx = this); ours adds `mov ecx,esi`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A07900, length 115 bytes (+13 int3 padding).
// engine/CRenderStateManager.h: +0x04 PD3DDevice, +0x2408 AllStatesList, +0x2808 StateStackSize, +0x280C
// StateUpdateListSize, +0x2810 AllStatesListSize, +0x2814 CurrentBookmarkMask, +0x3A24 BookmarkDummyState
// (its StateType byte +0x3A35). 0x013CB508 is the render-state-manager singleton pointer.
// Callees: SetupRenderStates @ 0x00A04630, SetDefaultRenderStates @ 0x00A059E0, ForceResendRenderStatesToHardware
// @ 0x00A044E0 (all members, called out of line). Shape: one zero register (edx) for the sizes, the per-state
// StateType reset (`mov [edi+11h],dl`) and the AllStatesListSize clear; the loop bound is re-read every pass.
#include "engine/CRenderStateManager.h"

struct CStateInfo {
    unsigned long CurrentState;    // +0x00
    unsigned long DesiredState;    // +0x04
    unsigned long BookmarkMask;    // +0x08
    long          State;           // +0x0C
    bool          DirtyListFlag;   // +0x10
    unsigned char StateType;       // +0x11
    unsigned char TextureStage;    // +0x12
};

struct CRenderStateManager_Methods;
extern CRenderStateManager_Methods* g_pRenderStateManager;   // 0x013CB508

struct CRenderStateManager_Methods : CRenderStateManager {
    void Init(IDirect3DDevice9* device);
    void SetupRenderStates();                    // 0x00A04630
    void SetDefaultRenderStates();               // 0x00A059E0
    void ForceResendRenderStatesToHardware();    // 0x00A044E0
};

void CRenderStateManager_Methods::Init(IDirect3DDevice9* device)
{
    StateStackSize = 0;
    StateUpdateListSize = 0;
    g_pRenderStateManager = this;
    PD3DDevice = device;
    for (long i = 0; i < AllStatesListSize; ++i)
        AllStatesList[i]->StateType = 0;
    AllStatesListSize = 0;
    SetupRenderStates();
    reinterpret_cast<CStateInfo*>(BookmarkDummyState)->StateType = 4;
    CurrentBookmarkMask = 1;
    SetDefaultRenderStates();
    ForceResendRenderStatesToHardware();
}
