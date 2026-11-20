// CVertexBufferWin32::Uninitialise() -- when a D3D vertex buffer is held, tell the render manager it is going (weight 1.0), Release it and clear the pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A63200, length 64 bytes.
// Callees: GFGetSystemManager @ 0x009A4EC0 (`__fastcall`, no arguments; PDB CSystemManager +0x60 =
// PDisplayManager, CDisplayManager +8 = PRenderManager as the landed 0x00F39A50 model), the render manager's
// buffer notification @ 0x00A0AA70 (a `ret 8` stub: (buffer, 1.0f)), IUnknown::Release (slot 2, `push eax ;
// call [ecx+8]`).
// Shape: the buffer tested first, the render manager chain taken into a local BEFORE the argument pushes
// (a chained call would push first), the buffer re-read into ecx for the push, `push 3F800000h` for the float literal, then the COM release and the null store.
// Layout: engine/CVertexBufferWin32.h is an opaque 0x34-byte record (vptr + flattened base); the D3D buffer
// at +4 is modelled here.
// Naming (NF172, 2026-11-20, q11898): the render-core callee 0x00A0AA70 respelled in place in the row's local CRenderManagerCore model to
//   Ego_r.pdb 0x00B74020 / FableWin.pdb 0x030EB290 ?WaitWhileResourceIsBusy@CRenderManagerCore@@QAEXPAUIDirect3DResource9@@M@Z (the spelling
//   0x009FA450 / 0x00AC1FB0 / 0x00A685E0 bind; was ?VertexBufferReleased@CRenderManagerCore@@QAEXPAUIDirect3DVertexBuffer@@M@Z). The header-less local IDirect3DVertexBuffer stand-in now sits on the
//   shared IDirect3DResource9 (engine/Direct3DSurface9.h; d3d9.h derives IDirect3DVertexBuffer9 from it), so the buffer converts implicitly.
#include "engine/Direct3DSurface9.h"
struct IDirect3DVertexBuffer : IDirect3DResource9 {   // IUnknown slots 0-2 + IDirect3DResource9 slots 3-10 from the shared header
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
    void Uninitialise();
};

void CVertexBufferWin32::Uninitialise()
{
    if (m_pBuffer) {
        CSystemManager* system = GFGetSystemManager();   // the call result in a local: evaluated before the pushes
        system->PDisplayManager->PRenderManager->WaitWhileResourceIsBusy(m_pBuffer, 1.0f);
        m_pBuffer->Release();
        m_pBuffer = 0;
    }
}