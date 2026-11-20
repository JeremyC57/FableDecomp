// CVertexBufferWin32::DoLock(unsigned long flags, unsigned long offset, unsigned long size) -- lock `size` bytes (the whole buffer when 0) of the D3D vertex buffer after notifying the render manager; returns the mapped pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A63240, length 82 bytes.
// Callees: GFGetSystemManager @ 0x009A4EC0, the render manager's buffer notification @ 0x00A0AA70
// ((buffer, 1.0f), a `ret 8` stub), IDirect3DVertexBuffer::Lock (slot 11, +0x2c: (offset, size, &data, flags)).
// Shape: `size` in edi replaced by m_size (+8) when zero; the chain fetch and the 1.0f push as in
// Uninitialise; the Lock pushes right to left (flags = arg 1, size, the out pointer, offset = arg 2, `this`),
// with the out pointer `data` homed in the dead `size` argument slot (`lea edx,[esp+18h]`, the
// `mov [esp+24h],0` initialiser scheduled after the pushes) and returned from that slot; `ret 0Ch`.
// Layout: engine/CVertexBufferWin32.h is an opaque 0x34-byte record; +4 buffer and +8 size are modelled.
// Naming (NF172, 2026-11-20, q11898): the render-core callee 0x00A0AA70 respelled in place in the row's local CRenderManagerCore model to
//   Ego_r.pdb 0x00B74020 / FableWin.pdb 0x030EB290 ?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z (the spelling
//   0x009FA450 / 0x00AC1FB0 / 0x00A685E0 bind; was ?VertexBufferLocked@CRenderManagerCore@@QAEXPAUIDirect3DVertexBuffer@@M@Z). The header-less local IDirect3DVertexBuffer stand-in now sits on the
//   shared IDirect3DResource9 (engine/Direct3DSurface9.h; d3d9.h derives IDirect3DVertexBuffer9 from it), so the buffer converts implicitly.
#include "engine/Direct3DSurface9.h"
struct IDirect3DVertexBuffer : IDirect3DResource9 {   // IUnknown slots 0-2 + IDirect3DResource9 slots 3-10 from the shared header
    virtual long __stdcall Lock(unsigned long offset, unsigned long size, void** data, unsigned long flags);   // slot 11 (+0x2c)
};

struct CRenderManagerCore {
    void WaitWhileResourceIsBusy(IDirect3DResource9* resource, float weight);   // 0x00A0AA70
};

class CDisplayManager { public:                       // PDB: +4 SystemManager, +8 PRenderManager
    void*               __vftable;             // +0x00
    void*               SystemManager;         // +0x04
    CRenderManagerCore* PRenderManager;        // +0x08
};

class CSystemManager { public:                        // PDB: +0x60 CCountedPointer<CDisplayManager> PDisplayManager
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;          // +0x60
    void*            PDisplayManagerCount;     // +0x64
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

class CVertexBufferWin32 { public:
    virtual void v00();
    IDirect3DVertexBuffer* m_pBuffer;          // +0x04
    unsigned long          m_size;             // +0x08
    void* DoLock(unsigned long flags, unsigned long offset, unsigned long size);
};

void* CVertexBufferWin32::DoLock(unsigned long flags, unsigned long offset, unsigned long size)
{
    if (size == 0)
        size = m_size;
    CSystemManager* system = GFGetSystemManager();   // the call result in a local: evaluated before the pushes
    system->PDisplayManager->PRenderManager->WaitWhileResourceIsBusy(m_pBuffer, 1.0f);
    void* data = 0;
    m_pBuffer->Lock(offset, size, &data, flags);
    return data;
}