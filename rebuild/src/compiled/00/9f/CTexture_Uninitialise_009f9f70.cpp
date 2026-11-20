// CTexture::Uninitialise() -- release the Direct3D texture, clear the pointer and the allocation-source bits
// Leaf: ?Uninitialise@CTexture@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F9F70, 34 bytes, thiscall, plain `ret`. Ego_r.pdb 0x00B69E70 / FableWin.pdb 0x030DE280
//   ?Uninitialise@CTexture@@QAEXXZ (public). When PD3DTexture (+0) is set: IUnknown::Release (slot 2, +8), PD3DTexture = 0 and the
//   4-bit AllocationSource (+4 bits 28..31, `and eax,0FFFFFFFh`) = 0; ByteLength (bits 0..27) is kept. The inline ~CTexture is this call.
// NF139 (2026-11-20, q4296): moved off the CTextureUninitialiseView stand-in (fable_texture_lifecycle.h) onto engine/CTexture.h and
//   engine/Direct3DTexture9.h; 40 landed callers already bind the spelling.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CTexture.h"

void CTexture::Uninitialise()
{
    if (PD3DTexture != 0)
    {
        PD3DTexture->Release();
        PD3DTexture = 0;
        AllocationSource = 0;
    }
}
