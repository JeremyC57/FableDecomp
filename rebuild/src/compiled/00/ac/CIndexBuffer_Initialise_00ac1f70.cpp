// CIndexBuffer::Initialise(unsigned long, unsigned long, EIndexBufferFormat, EIndexBufferPool, EPrimitiveType) -- create the Direct3D index buffer (IDirect3DDevice9::CreateIndexBuffer, slot 27, no shared handle) into PD3DIndexBuffer (+0x04) and keep the byte length (+0x08) and the primitive type (+0x0C)
// Leaf: ?Initialise@CIndexBuffer@@QAEXKKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC1F70, length 64 bytes, thiscall, `ret 14h`.
// Identity: Ego_r.pdb + FableWin.pdb ?Initialise@CIndexBuffer@@QAEXKKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z
//   (five arguments); the landed callers 0x00A8F9E0 / 0x00B51570 / 0x00BA1B00 bind this address under that spelling.
// Port (NF149, q4557): was a local CIndexBuffer model with the stand-in leaf ?Initialise@CIndexBuffer@@QAEXKKJJJ@Z; now on
//   engine/CIndexBuffer.h (FABLE_RETAIL_CINDEXBUFFER), the device reached through GFGetSystemManager 0x009A4EC0 ->
//   CSystemManager::GetDisplayManager (+0x60) -> CDisplayManager::GetD3DDevice (+0x58), engine/IDirect3DDevice9.h slot 27.
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/IDirect3DDevice9.h"
#include "engine/Direct3D9Types.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#include "engine/CIndexBuffer.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"

void CIndexBuffer::Initialise(unsigned long length, unsigned long usage, EIndexBufferFormat format, EIndexBufferPool pool, EPrimitiveType primitiveType)
{
    GFGetSystemManager()->GetDisplayManager()->GetD3DDevice()->CreateIndexBuffer(length, usage, format, pool, &PD3DIndexBuffer, 0);
    ByteLength = length;
    *(EPrimitiveType*)PrimitiveType = primitiveType;
}
