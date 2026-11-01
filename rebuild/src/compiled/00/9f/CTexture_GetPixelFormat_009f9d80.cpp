// CTexture::GetPixelFormat()
// Leaf: ?GetPixelFormat@CTexture_Methods@@QAE?AVCPixelFormat@@XZ
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9D80, oracle length 44 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx] ; mov ecx,[eax] ; sub esp,20h ; push esi ; lea edx,[esp+4] ; push edx ; push 0 ; push eax ; call [ecx+44h] ; mov eax,[esp+4] ; mov esi,[esp+28h] ; push eax ; mov ecx,esi ; call CPixelFormat::CPixelFormat(D3DFORMAT) ; mov eax,esi ; pop esi ; add esp,20h ; ret 4
// engine/CTexture.h (PDB): +0x00 IDirect3DTexture9* PD3DTexture. Asks Direct3D for the level-0
// surface description (IDirect3DTexture9::GetLevelDesc, COM slot 17) and converts its D3DFORMAT
// (the first dword of D3DSURFACE_DESC) into a CPixelFormat through the constructor at
// 0x009E3830, built directly in the caller's hidden result slot.
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
#include "engine/CTexture.h"

struct D3DSURFACE_DESC_Local {
    _D3DFORMAT Format;              // +0x00 D3DFORMAT
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
class CPixelFormat { public:
    long FormatIndex;                                   // +0x00 (PDB CPixelFormat::FormatIndex)
    CPixelFormat(_D3DFORMAT d3dFormat) { Initialise(d3dFormat); }
    void Initialise(_D3DFORMAT d3dFormat);       // 0x009E3830
};

struct CTexture_Methods : CTexture {
    CPixelFormat GetPixelFormat();
};

CPixelFormat CTexture_Methods::GetPixelFormat()
{
    D3DSURFACE_DESC_Local desc;
    reinterpret_cast<IDirect3DTexture9*>(this->PD3DTexture)->GetLevelDesc(0, &desc);
    return CPixelFormat(desc.Format);
}