// CIndexBuffer::Unlock() -- unlock the Direct3D index buffer (IDirect3DIndexBuffer9::Unlock, slot 12, on PD3DIndexBuffer +0x04)
// Leaf: ?Unlock@CIndexBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC1F00, 10 bytes, thiscall: `mov eax,[ecx+4] ; mov ecx,[eax] ; push eax ; call [ecx+30h] ; ret`.
// Identity: Ego_r.pdb + FableWin.pdb ?Unlock@CIndexBuffer@@QAEXXZ (engine/CIndexBuffer.h); the landed callers 0x00A8F9E0 / 0x00B51570 /
//   0x00BE3110 bind this address under that spelling.
// Port (NF149, q4555): was a generated `CallInterfaceSlot` body landed as CVertexBufferWin32::DoUnlock (leaf ?DoUnlock@CVertexBufferWin32@@QAEXXZ)
//   on local CVertexBufferWin32 / IInterface models; now on engine/CIndexBuffer.h (FABLE_RETAIL_CINDEXBUFFER).
#define FABLE_RETAIL_CINDEXBUFFER
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

void CIndexBuffer::Unlock()
{
    PD3DIndexBuffer->Unlock();
}
