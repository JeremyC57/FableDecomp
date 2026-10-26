// CSurface::GetDimensions()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2F90, oracle length 39 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+4] ; mov ecx,[eax] ; sub esp,20h ; lea edx,[esp] ; push edx ; push eax ; call [ecx+30h] ; mov eax,[esp+24h] ; mov ecx,[esp+18h] ; mov edx,[esp+1Ch] ; mov [eax],ecx ; mov [eax+4],edx ; add esp,20h ; ret 4
// engine/CSurface.h (PDB): +0x04 IDirect3DSurface9* PD3DSurface. COM slot 12 (+0x30) is
// IDirect3DSurface9::GetDesc; the 32-byte D3DSURFACE_DESC is filled on the stack and its
// Width/Height (+0x18/+0x1C) become the returned extents.
#include "engine/CSurface.h"

// Direct3D 9 surface description (d3d9.h is not on the verifier include path).
struct D3DSURFACE_DESC_Local {
    unsigned long Format;              // +0x00 D3DFORMAT
    unsigned long Type;                // +0x04 D3DRESOURCETYPE
    unsigned long Usage;               // +0x08
    unsigned long Pool;                // +0x0C D3DPOOL
    unsigned long MultiSampleType;     // +0x10
    unsigned long MultiSampleQuality;  // +0x14
    unsigned long Width;               // +0x18
    unsigned long Height;              // +0x1C
};

// C2DExtentsI is returned through a hidden pointer (it has a constructor).
class C2DExtentsI { public:
    long X;
    long Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};

struct IDirect3DSurface9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);   // slot 12 (+0x30)
};

struct CSurface_Methods : CSurface {
    C2DExtentsI GetDimensions();
};

C2DExtentsI CSurface_Methods::GetDimensions()
{
    D3DSURFACE_DESC_Local desc;
    this->PD3DSurface->GetDesc(&desc);
    return C2DExtentsI(desc.Width, desc.Height);
}