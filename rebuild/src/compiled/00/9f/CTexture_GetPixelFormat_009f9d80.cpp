// CTexture::GetPixelFormat()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9D80, oracle length 44 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx] ; mov ecx,[eax] ; sub esp,20h ; push esi ; lea edx,[esp+4] ; push edx ; push 0 ; push eax ; call [ecx+44h] ; mov eax,[esp+4] ; mov esi,[esp+28h] ; push eax ; mov ecx,esi ; call CPixelFormat::CPixelFormat(D3DFORMAT) ; mov eax,esi ; pop esi ; add esp,20h ; ret 4
// engine/CTexture.h (PDB): +0x00 IDirect3DTexture9* PD3DTexture. Asks Direct3D for the level-0
// surface description (IDirect3DTexture9::GetLevelDesc, COM slot 17) and converts its D3DFORMAT
// (the first dword of D3DSURFACE_DESC) into a CPixelFormat through the constructor at
// 0x009E3830, built directly in the caller's hidden result slot.
#include "engine/CTexture.h"

struct D3DSURFACE_DESC_Local {
    unsigned long Format;              // +0x00 D3DFORMAT
    unsigned long Type, Usage, Pool, MultiSampleType, MultiSampleQuality, Width, Height;
};

struct IDirect3DTexture9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual void __stdcall v30(); virtual void __stdcall v34(); virtual void __stdcall v38(); virtual void __stdcall v3c();
    virtual void __stdcall v40();
    virtual long __stdcall GetLevelDesc(unsigned long level, D3DSURFACE_DESC_Local* desc);   // slot 17 (+0x44)
};

// CPixelFormat with its D3DFORMAT constructor (the engine header holds only the layout).
struct CPixelFormatFromD3D {
    long FormatIndex;                                   // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormatFromD3D(unsigned long d3dFormat);       // 0x009E3830
};

struct CTexture_Methods : CTexture {
    CPixelFormatFromD3D GetPixelFormat();
};

CPixelFormatFromD3D CTexture_Methods::GetPixelFormat()
{
    D3DSURFACE_DESC_Local desc;
    reinterpret_cast<IDirect3DTexture9*>(this->PD3DTexture)->GetLevelDesc(0, &desc);
    return CPixelFormatFromD3D(desc.Format);
}