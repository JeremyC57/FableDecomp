// CTexture::GetDimensions(unsigned long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9DB0, oracle length 43 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx] ; mov ecx,[eax] ; sub esp,20h ; lea edx,[esp] ; push edx ; mov edx,[esp+2Ch] ; push edx ; push eax ; call [ecx+44h] ; mov eax,[esp+24h] ; mov ecx,[esp+18h] ; mov edx,[esp+1Ch] ; mov [eax],ecx ; mov [eax+4],edx ; add esp,20h ; ret 8
// engine/CTexture.h (PDB): +0x00 IDirect3DTexture9* PD3DTexture. COM slot 17 (+0x44) is
// IDirect3DTexture9::GetLevelDesc(level, desc); Width/Height of the requested mip level are
// returned as extents through the hidden result pointer (first stack slot; `level` is the second).
#include "engine/CTexture.h"

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
struct C2DExtentsI {
    long X;
    long Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};

struct IDirect3DTexture9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual void __stdcall v30(); virtual void __stdcall v34(); virtual void __stdcall v38(); virtual void __stdcall v3c();
    virtual void __stdcall v40();
    virtual long __stdcall GetLevelDesc(unsigned long level, D3DSURFACE_DESC_Local* desc);   // slot 17 (+0x44)
};

struct CTexture_Methods : CTexture {
    C2DExtentsI GetDimensions(unsigned long level);
};

C2DExtentsI CTexture_Methods::GetDimensions(unsigned long level)
{
    D3DSURFACE_DESC_Local desc;
    reinterpret_cast<IDirect3DTexture9*>(this->PD3DTexture)->GetLevelDesc(level, &desc);
    return C2DExtentsI(desc.Width, desc.Height);
}
