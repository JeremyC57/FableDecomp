// CTexture::LoadFromDataStreamToPreallocatedSurface(CDataInputStream& stream) -- read the texture file info, then for every mip level lock the preallocated surface and stream the level in: compressed formats as one block of the level's byte length, others row by row through a temporary buffer honouring the lock pitch (overflowing sizes abandon the load)
// Leaf: ?LoadFromDataStreamToPreallocatedSurface@CTexture@@QAEXAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FB750, 789 bytes, thiscall, `ret 4`. Ego_r.pdb ?LoadFromDataStreamToPreallocatedSurface@CTexture@@QAEXAAVCDataInputStream@@@Z;
//   Ego_d.pdb (BBBLibrary lib_texture.obj) gives stream, pf, file_info, c1, locked_rect, hr / desc (compressed block), src, src_buffer,
//   overflow_check, overflow_check2, data_length, dst, line_length and cy, and the TU order GetPixelFormat, GetDimensions, Lock, Unlock,
//   GetNoLevels, ..., GetLevelByteLength, ..., this function.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the header's inline Read), CPixelFormat::Initialise(const
//   CPixelFormatInit&) 0x009E3640 / (D3DFORMAT) 0x009E3830, IsCompressed 0x009E3940, GetColourDepth 0x009E3820, GetByteColourDepth
//   0x009E3800, CTexture::Lock 0x009FA450, std::vector<unsigned char>(size_type) 0x005136B0, _allmul 0x00BFEB50, free 0x00BFEA14;
//   IDirect3DTexture9 GetLevelCount (+0x34, GetNoLevels), GetLevelDesc (+0x44), UnlockRect (+0x50, Unlock).
// Same-TU bodies (what closed the row): defined here in Ego_d order and each graded at its own retail address with this file:
//   GetPixelFormat 0x009F9D80 RELOCATION_MATCH, GetDimensions 0x009F9DB0 MATCH, Lock 0x009FA450 RELOCATION_MATCH, Unlock 0x009F9DE0 MATCH,
//   GetNoLevels 0x009F9DF0 MATCH, GetLevelByteLength 0x009F9E80 RELOCATION_MATCH. GetPixelFormat / GetDimensions / GetNoLevels / Unlock /
//   GetLevelByteLength expand in the row as in retail; Lock stays a call (retail 0x009FB80A) with no inline control. With Lock's body visible VC7.1 knows the
//   result slot does not escape and keeps locked_rect.Dimensions.X in edi across the GetByteColourDepth calls (0x009FB8FB..0x009FB930),
//   which before needed a width local Ego_d does not list; Ego_d's data_length then gets retail's ebx.
// Inline-budget: none (audit e13: VC7.1 /O2 keeps the same-TU Lock body out of line by itself; the row and all six bodies match without
//   `__declspec(noinline)`).
// Fidelity: the hr locals (Ego_d T_HRESULT in GetPixelFormat, GetDimensions, Unlock, GetLevelByteLength and the row's compressed block)
//   hold results the release build never reads (debug-only checks); the row's own `hr = GetLevelDesc(c1, &desc)` is the retail call at
//   0x009FB82E whose result is unused.
// Headers: NEW engine/CTextureFileInfo.h, engine/CPixelFormatInit.h, CTexture.h opt-in FABLE_RETAIL_CTEXTURE_LEVELBYTELENGTH (hdr/README.md).
//   HRESULT (Ego_d T_HRESULT for hr) comes from engine/CRenderManagerCore.h (`typedef long HRESULT;` under
//   FABLE_RETAIL_PASS22_CORE_COMPLETE_ORIGINAL_DECLARATION, defined above).
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_LOCK
#define FABLE_RETAIL_CTEXTURE_LOADFROMDATASTREAM
#define FABLE_RETAIL_CTEXTURE_LEVELBYTELENGTH
#define FABLE_RETAIL_CTEXTURE_GETNOLEVELS
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_INIT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_PASS22_CORE_COMPLETE_ORIGINAL_DECLARATION
#define FABLE_RETAIL_PASS22_CORE_ORIGINAL_BUFFER_ACCESSORS
#define FABLE_RETAIL_PASS22_RENDERSTATE_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CRENDERMANAGER2D
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CDataInputStream.h"
#include "engine/CTextureFileInfo.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"

CPixelFormat CTexture::GetPixelFormat() const
{
    _D3DSURFACE_DESC desc;
    HRESULT hr = PD3DTexture->GetLevelDesc(0, &desc);
    return CPixelFormat(desc.Format);
}

C2DExtentsI CTexture::GetDimensions(unsigned long mipmap_level) const
{
    _D3DSURFACE_DESC desc;
    HRESULT hr = PD3DTexture->GetLevelDesc(mipmap_level, &desc);
    return C2DExtentsI(desc.Width, desc.Height);
}

CSurfaceLockedRect CTexture::Lock(unsigned long level, unsigned long lock_flags)
{
    C2DExtentsI dimensions = GetDimensions(level);
    tagRECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = dimensions.Y;
    rect.right = dimensions.X;
    GFGetSystemManager()->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(PD3DTexture, 1.0f);
    _D3DLOCKED_RECT locked_rect;
    HRESULT hr = PD3DTexture->LockRect(level, &locked_rect, &rect, lock_flags);
    if (hr >= 0)
    {
        CSurfaceLockedRect lock_info;
        lock_info.Dimensions = dimensions;
        lock_info.Pitch = locked_rect.Pitch;
        lock_info.PMem = (unsigned char*)locked_rect.pBits;
        return lock_info;
    }
    return CSurfaceLockedRect();
}

void CTexture::Unlock(unsigned long level)
{
    HRESULT hr = PD3DTexture->UnlockRect(level);
}

long CTexture::GetNoLevels() const
{
    return PD3DTexture->GetLevelCount();
}

unsigned long CTexture::GetLevelByteLength(unsigned long level) const
{
    _D3DSURFACE_DESC desc;
    HRESULT hr = PD3DTexture->GetLevelDesc(level, &desc);
    return GetPixelFormat().GetColourDepth() * desc.Width * desc.Height / 8;
}

void CTexture::LoadFromDataStreamToPreallocatedSurface(CDataInputStream& stream)
{
    CTextureFileInfo file_info;
    stream.Read(&file_info, sizeof(file_info));
    CPixelFormat pf(file_info.PixelFormatInit);
    for (long c1 = 0; c1 < GetNoLevels(); ++c1)
    {
        CSurfaceLockedRect locked_rect = Lock(c1, 0);
        if (pf.IsCompressed())
        {
            _D3DSURFACE_DESC desc;
            HRESULT hr = PD3DTexture->GetLevelDesc(c1, &desc);
            stream.Read(locked_rect.PMem, GetLevelByteLength(c1));
        }
        else
        {
            unsigned __int64 overflow_check = (unsigned __int64)pf.GetByteColourDepth() * (__int64)locked_rect.Dimensions.X;
            if (overflow_check > 0xffffffffu)
                return;
            unsigned long line_length = pf.GetByteColourDepth() * locked_rect.Dimensions.X;
            unsigned __int64 overflow_check2 = (__int64)locked_rect.Dimensions.Y * (unsigned __int64)line_length;
            if (overflow_check2 > 0xffffffffu)
                return;
            unsigned long data_length = locked_rect.Dimensions.Y * line_length;
            std::vector<unsigned char> src_buffer(data_length);
            unsigned char* src = &src_buffer[0];
            unsigned char* dst = locked_rect.PMem;
            stream.Read(src, data_length);
            for (long cy = 0; cy < locked_rect.Dimensions.Y; ++cy)
            {
                memcpy(dst, src, line_length);
                dst += locked_rect.Pitch;
                src += line_length;
            }
        }
        Unlock(c1);
    }
}