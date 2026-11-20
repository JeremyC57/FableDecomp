// CIndexBuffer::Lock(unsigned long flags, unsigned long offset, unsigned long size) -- lock the Direct3D index buffer (PD3DIndexBuffer +0x04): a zero size means the whole buffer (its description's Size, GetDesc slot 13), the render manager waits while the buffer is busy (CRenderManagerCore::WaitWhileResourceIsBusy 0x00A0AA70, weight 1.0f), then IDirect3DIndexBuffer9::Lock (slot 11: offset, size, &data, flags) and the mapped pointer is returned
// Leaf: ?Lock@CIndexBuffer@@QAEPAEKKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC1FB0, 96 bytes, thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb + FableWin.pdb ?Lock@CIndexBuffer@@QAEPAEKKK@Z (engine/CIndexBuffer.h); the landed callers 0x00A8F9E0 / 0x00B51570 /
//   0x00BE3110 bind this address under that spelling. Callee 0x00A0AA70 (an empty `ret 8` in this build): Ego_r.pdb + FableWin.pdb
//   ?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z, the spelling the landed CTexture::Lock 0x009FA450 binds
//   (was the stand-in CRenderStatistics_Retail::NotifyLock here); reached as 0x009FB750 does, GFGetSystemManager 0x009A4EC0 ->
//   GetDisplayManager (+0x60) -> GetRenderManager (+0x08).
// Port (NF149, q4558): was a local CIndexBuffer model with the leaf ?Lock@CIndexBuffer@@QAEPAXKKK@Z; now on engine/CIndexBuffer.h.
// Shape: the call result in a local (the call first, the argument next, then the chain); the mapped pointer homed in a dead argument slot.
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_PASS22_CORE_COMPLETE_ORIGINAL_DECLARATION
#define FABLE_RETAIL_PASS22_CORE_ORIGINAL_BUFFER_ACCESSORS
#define FABLE_RETAIL_PASS22_RENDERSTATE_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CTEXTURE
#include "engine/Direct3D9Types.h"
#include "engine/Direct3DSurface9.h"
// IDirect3DIndexBuffer9 (DirectX 9 SDK d3d9.h, DECLARE_INTERFACE_(IDirect3DIndexBuffer9, IDirect3DResource9)): no shared header defines
// it (engine/CIndexBuffer.h and engine/IDirect3DDevice9.h forward-declare it); modelled as the landed sibling 0x00AC1EE0 does, on the
// shared IUnknown / IDirect3DResource9 (engine/Direct3DSurface9.h), with the SDK names: Lock slot 11 (+0x2C), Unlock slot 12 (+0x30),
// GetDesc slot 13 (+0x34). _D3DINDEXBUFFER_DESC is d3d9types.h's (Format, Type, Usage, Pool, Size; 0x14 bytes).
struct _D3DINDEXBUFFER_DESC { _D3DFORMAT Format; _D3DRESOURCETYPE Type; unsigned long Usage; _D3DPOOL Pool; unsigned int Size; };
struct IDirect3DIndexBuffer9 : public IDirect3DResource9 {
    virtual long FABLE_STDCALL Lock(unsigned int OffsetToLock, unsigned int SizeToLock, void** ppbData, unsigned long Flags) = 0;   // slot 11
    virtual long FABLE_STDCALL Unlock() = 0;                                                                                      // slot 12
    virtual long FABLE_STDCALL GetDesc(_D3DINDEXBUFFER_DESC* pDesc) = 0;                                                          // slot 13
};
#include "engine/CIndexBuffer.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"

unsigned char* CIndexBuffer::Lock(unsigned long flags, unsigned long offset, unsigned long size)
{
    if (size == 0) {
        _D3DINDEXBUFFER_DESC desc;
        PD3DIndexBuffer->GetDesc(&desc);
        size = desc.Size;
    }
    CSystemManager* manager = GFGetSystemManager();
    manager->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(PD3DIndexBuffer, 1.0f);
    void* data;
    PD3DIndexBuffer->Lock(offset, size, &data, flags);
    return (unsigned char*)data;
}
