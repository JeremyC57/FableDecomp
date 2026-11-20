// CTexture::operator=(const CTexture&) -- drop the held Direct3D texture, copy the other's pointer and bitfields, add a reference
// Leaf: ??4CTexture@@QAEABV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FA1C0, 98 bytes, thiscall, `ret 4`. Ego_r.pdb 0x00B6A110 / FableWin.pdb 0x030D9F10
//   ??4CTexture@@QAEABV0@ABV0@@Z (public, returns const CTexture&). It opens with CTexture::Uninitialise's body (0x009F9F70: Release
//   slot 2 and clear PD3DTexture / AllocationSource) expanded in place, copies PD3DTexture and the ByteLength / AllocationSource
//   bitfields of the +4 word, then IUnknown::AddRef (slot 1, +4) on the new texture.
// NF139 (2026-11-20, q4659): moved off the CTextureAssignmentView stand-in (fable_texture_lifecycle.h, non-const AAU0@AAU0 spelling)
//   onto engine/CTexture.h + engine/Direct3DTexture9.h; 11 landed callers already bind this spelling.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CTexture.h"

const CTexture& CTexture::operator=(const CTexture& other)
{
    if (PD3DTexture != 0)
    {
        PD3DTexture->Release();
        PD3DTexture = 0;
        AllocationSource = 0;
    }

    PD3DTexture = other.PD3DTexture;
    ByteLength = other.ByteLength;
    AllocationSource = other.AllocationSource;
    if (PD3DTexture != 0)
    {
        PD3DTexture->AddRef();
    }
    return *this;
}
