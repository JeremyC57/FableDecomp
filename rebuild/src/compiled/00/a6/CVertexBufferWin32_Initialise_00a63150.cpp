// CVertexBufferWin32::Initialise(EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool) -- create the Direct3D vertex buffer (IDirect3DDevice9::CreateVertexBuffer, slot 26: count * stride bytes, the vertex type as the FVF, no shared handle) into PVertexBuffer (+0x04), record Length (+0x08), Stride (+0x10) and Type (+0x0C), reset the compression params (scale 1, bias 0); true when the device call succeeded
// Leaf: ?Initialise@CVertexBufferWin32@@QAE_NW4EVertexType@@JJKW4EVertexBufferPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A63150, 165 bytes, thiscall, `ret 14h`.
// Identity: Ego_r.pdb 0x00BD6C50 + FableWin.pdb ?Initialise@CVertexBufferWin32@@QAE_NW4EVertexType@@JJKW4EVertexBufferPool@@@Z
//   (engine/CVertexBufferWin32.h); the landed callers 0x00B51570 / 0x00B6EDF0 / 0x00B8DCD0 / 0x00BB2540 bind this address under that spelling.
// Port (NF149, q4552): was a local CVertexBufferInitialiseView / D3D / CSystemManager model (leaf
//   ?Initialise@CVertexBufferInitialiseView@@QAE_NJJJKJ@Z); now on engine/CVertexBufferWin32.h (FABLE_RETAIL_CVERTEXBUFFERWIN32), the device
//   reached through GFGetSystemManager 0x009A4EC0 -> GetDisplayManager (+0x60) -> GetD3DDevice (+0x58) (engine/IDirect3DDevice9.h).
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/IDirect3DDevice9.h"
#include "engine/Direct3D9Types.h"
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"

bool CVertexBufferWin32::Initialise(EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool)
{
    IDirect3DDevice9* device = GFGetSystemManager()->GetDisplayManager()->GetD3DDevice();
    long length = count * stride;
    long result = device->CreateVertexBuffer(length, usage, type, pool, &PVertexBuffer, 0);
    Length = length;
    Stride = (unsigned char)stride;
    Type = type;
    CompressionParams = CVertexCompressionParams();
    return result >= 0;
}
