// CRenderStateManager::ForceResendRenderStatesToHardware() -- push every known state's desired value to the device again (render, texture-stage or sampler state by kind) and empty the update list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A044E0, length 151 bytes.
// engine/CRenderStateManager.h: +0x04 PD3DDevice, +0x2408 AllStatesList[256], +0x280C StateUpdateListSize,
// +0x2810 AllStatesListSize. CStateInfo (PDB, 20 bytes): CurrentState, DesiredState, BookmarkMask, the D3D state
// enum at +0x0C, DirtyListFlag +0x10, StateType +0x11 (1 render, 2 texture stage, 3 sampler), TextureStage +0x12.
// Callees: IDirect3DDevice9 SetRenderState (slot 57, +0xE4), SetTextureStageState (slot 67, +0x10C),
// SetSamplerState (slot 69, +0x114). Shape: the switch on StateType is a `dec edx` chain; the loop bound is
// re-read every pass (the virtual calls may alias it).
#include "engine/CRenderStateManager.h"

struct IDirect3DBaseTexture9;

// d3d9.h is not on the verifier include path, so the IDirect3DDevice9 vtable is declared locally
// (COM methods are __stdcall with `this` pushed; unnamed slots are placeholders).
struct IDirect3DDevice9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual void __stdcall v30(); virtual void __stdcall v34(); virtual void __stdcall v38(); virtual void __stdcall v3c();
    virtual void __stdcall v40(); virtual void __stdcall v44(); virtual void __stdcall v48(); virtual void __stdcall v4c();
    virtual void __stdcall v50(); virtual void __stdcall v54(); virtual void __stdcall v58(); virtual void __stdcall v5c();
    virtual void __stdcall v60(); virtual void __stdcall v64(); virtual void __stdcall v68(); virtual void __stdcall v6c();
    virtual void __stdcall v70(); virtual void __stdcall v74(); virtual void __stdcall v78(); virtual void __stdcall v7c();
    virtual void __stdcall v80(); virtual void __stdcall v84(); virtual void __stdcall v88(); virtual void __stdcall v8c();
    virtual void __stdcall v90(); virtual void __stdcall v94(); virtual void __stdcall v98(); virtual void __stdcall v9c();
    virtual void __stdcall va0(); virtual void __stdcall va4(); virtual void __stdcall va8(); virtual void __stdcall vac();
    virtual long __stdcall SetTransform(long state, const void* matrix);   // slot 44 (+0xB0)
    virtual void __stdcall vb4(); virtual void __stdcall vb8(); virtual void __stdcall vbc(); virtual void __stdcall vc0();
    virtual void __stdcall vc4(); virtual void __stdcall vc8(); virtual void __stdcall vcc(); virtual void __stdcall vd0();
    virtual void __stdcall vd4(); virtual void __stdcall vd8(); virtual void __stdcall vdc(); virtual void __stdcall ve0();
    virtual long __stdcall SetRenderState(long state, unsigned long value);   // slot 57 (+0xE4)
    virtual void __stdcall ve8(); virtual void __stdcall vec(); virtual void __stdcall vf0(); virtual void __stdcall vf4();
    virtual void __stdcall vf8(); virtual void __stdcall vfc(); virtual void __stdcall v100();
    virtual long __stdcall SetTexture(unsigned long stage, IDirect3DBaseTexture9* texture);   // slot 65 (+0x104)
    virtual void __stdcall v108();
    virtual long __stdcall SetTextureStageState(unsigned long stage, long type, unsigned long value);   // slot 67 (+0x10C)
    virtual void __stdcall v110();
    virtual long __stdcall SetSamplerState(unsigned long sampler, long type, unsigned long value);   // slot 69 (+0x114)
    virtual void __stdcall v118(); virtual void __stdcall v11c(); virtual void __stdcall v120(); virtual void __stdcall v124();
    virtual void __stdcall v128(); virtual void __stdcall v12c(); virtual void __stdcall v130(); virtual void __stdcall v134();
    virtual void __stdcall v138(); virtual void __stdcall v13c(); virtual void __stdcall v140();
    virtual long __stdcall DrawPrimitive(long primitiveType, unsigned long startVertex, unsigned long primitiveCount);   // slot 81 (+0x144)
    virtual long __stdcall DrawIndexedPrimitive(long primitiveType, long baseVertexIndex, unsigned long minVertexIndex, unsigned long numVertices, unsigned long startIndex, unsigned long primitiveCount);   // slot 82 (+0x148)
    virtual long __stdcall DrawPrimitiveUP(long primitiveType, unsigned long primitiveCount, const void* vertices, unsigned long stride);   // slot 83 (+0x14C)
    virtual long __stdcall DrawIndexedPrimitiveUP(long primitiveType, unsigned long minVertexIndex, unsigned long numVertices, unsigned long primitiveCount, const void* indices, long indexFormat, const void* vertices, unsigned long stride);   // slot 84 (+0x150)
    virtual void __stdcall v154(); virtual void __stdcall v158(); virtual void __stdcall v15c(); virtual void __stdcall v160();
    virtual void __stdcall v164(); virtual void __stdcall v168(); virtual void __stdcall v16c(); virtual void __stdcall v170();
    virtual void __stdcall v174(); virtual void __stdcall v178(); virtual void __stdcall v17c(); virtual void __stdcall v180();
    virtual void __stdcall v184(); virtual void __stdcall v188(); virtual void __stdcall v18c();
    virtual long __stdcall SetStreamSource(unsigned long stream, void* data, unsigned long offset, unsigned long stride);   // slot 100 (+0x190)
};

struct CStateInfo {
    unsigned long CurrentState;    // +0x00
    unsigned long DesiredState;    // +0x04
    unsigned long BookmarkMask;    // +0x08
    long          State;           // +0x0C D3DRENDERSTATETYPE / D3DTEXTURESTAGESTATETYPE / D3DSAMPLERSTATETYPE
    bool          DirtyListFlag;   // +0x10
    unsigned char StateType;       // +0x11
    unsigned char TextureStage;    // +0x12
};

struct CRenderStateManager_Methods : CRenderStateManager {
    void ForceResendRenderStatesToHardware();
};

void CRenderStateManager_Methods::ForceResendRenderStatesToHardware()
{
    for (long i = 0; i < AllStatesListSize; ++i)
    {
        CStateInfo* state = AllStatesList[i];
        unsigned long desired = state->DesiredState;
        state->CurrentState = desired;
        state->DirtyListFlag = false;
        switch (state->StateType)
        {
        case 1:
            PD3DDevice->SetRenderState(state->State, desired);
            break;
        case 2:
            PD3DDevice->SetTextureStageState(state->TextureStage, state->State, desired);
            break;
        case 3:
            PD3DDevice->SetSamplerState(state->TextureStage, state->State, desired);
            break;
        }
    }
    StateUpdateListSize = 0;
}
