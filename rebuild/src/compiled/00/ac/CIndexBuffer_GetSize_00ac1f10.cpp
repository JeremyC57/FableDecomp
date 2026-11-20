// CIndexBuffer::GetSize() const -- the byte size of the Direct3D index buffer: its description's Size (IDirect3DIndexBuffer9::GetDesc, slot 13, on PD3DIndexBuffer +0x04)
// Leaf: ?GetSize@CIndexBuffer@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC1F10, 24 bytes, thiscall: a 0x14-byte _D3DINDEXBUFFER_DESC frame, GetDesc, return desc.Size (+0x10).
// Identity: Ego_r.pdb ?GetSize@CIndexBuffer@@QBEJXZ (engine/CIndexBuffer.h); the landed callers 0x00B51570 / 0x00B6EDF0 bind this
//   address under that spelling. (0x00AC1F30, the same body plus 0x18, is a separate function.)
// Port (NF149, q4556): was the free stand-in ?CIndexBuffer_GetSize@@YIJPAVCIndexBuffer@@@Z on a local CIndexBuffer / IBufImpl model; now
//   on engine/CIndexBuffer.h (FABLE_RETAIL_CINDEXBUFFER).
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

long CIndexBuffer::GetSize() const
{
    _D3DINDEXBUFFER_DESC desc;
    PD3DIndexBuffer->GetDesc(&desc);
    return desc.Size;
}
