// CVolumeTexture::LoadFromDataStream(CDataInputStream& stream) -- read the 21-byte CVolumeTextureFileInfo, create the volume texture it describes, then fill every level: a compressed level is read straight into the locked box; an uncompressed one is read into the box and copied row by row into a scratch buffer.
// Leaf: ?LoadFromDataStream@CVolumeTexture@@QAEXAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A68DE0, 919 bytes, thiscall `ret 4`; no EH frame.
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_INIT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_CVOLUMETEXTURE_INITIALISE
#define FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST
#define FABLE_RETAIL_CVOLUMETEXTURE_LOADFROMDATASTREAM
#include <string.h>
#pragma intrinsic(memcpy)
#include "stlport/fable_containers.h"
#include "engine/C3DExtentsI.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CPixelFormatInit.h"
#include "engine/CPixelFormat.h"
#include "engine/CDataInputStream.h"
#include "engine/CVolumeTexture.h"
#include "engine/CVolumeLockedBox.h"
#include "engine/CVolumeTextureFileInfo.h"

struct _D3DVOLUME_DESC { _D3DFORMAT Format; _D3DRESOURCETYPE Type; unsigned long Usage; _D3DPOOL Pool; unsigned int Width, Height, Depth; };
struct _D3DBOX { unsigned int Left, Top, Right, Bottom, Front, Back; };
struct _D3DLOCKED_BOX { int RowPitch, SlicePitch; void* pBits; };
struct IDirect3DVolume9;
struct IDirect3DVolumeTexture9 : public IDirect3DBaseTexture9 {
    virtual long __stdcall GetLevelDesc(unsigned int, _D3DVOLUME_DESC*) = 0;                              // slot 17 (+0x44)
    virtual long __stdcall GetVolumeLevel(unsigned int, IDirect3DVolume9**) = 0;
    virtual long __stdcall LockBox(unsigned int, _D3DLOCKED_BOX*, const _D3DBOX*, unsigned long) = 0;
    virtual long __stdcall UnlockBox(unsigned int) = 0;                                                   // slot 20 (+0x50)
    virtual long __stdcall AddDirtyBox(const _D3DBOX*) = 0;
};

inline long CVolumeTexture::GetNoLevels() const { return PD3DVolumeTexture->GetLevelCount(); }
inline void CVolumeTexture::Unlock(unsigned long level) { PD3DVolumeTexture->UnlockBox(level); }
inline CPixelFormat CVolumeTexture::GetPixelFormat() const
{
    _D3DVOLUME_DESC desc;
    PD3DVolumeTexture->GetLevelDesc(0, &desc);
    return CPixelFormat(desc.Format);
}

void CVolumeTexture::LoadFromDataStream(CDataInputStream& stream)
{
    CVolumeTextureFileInfo file_info;
    stream.Read(&file_info, sizeof(file_info));
    CPixelFormat pf(file_info.PixelFormatInit);
    Initialise(C3DExtentsI(file_info.Width, file_info.Height, file_info.Depth), file_info.NoLevels, pf, file_info.UseageFlags, file_info.Pool, false);
    for (long c1 = 0; c1 < GetNoLevels(); c1++)
    {
        CVolumeLockedBox locked_box = Lock(c1, 0x10);
        if (pf.IsCompressed())
        {
            _D3DVOLUME_DESC desc;
            long hr = PD3DVolumeTexture->GetLevelDesc(c1, &desc);
            stream.Read(locked_box.PMem, GetLevelByteLength(c1));
        }
        else
        {
            unsigned __int64 overflow_check = (unsigned __int64)locked_box.Dimensions.X * GetPixelFormat().GetByteColourDepth();
            if (overflow_check > 0xffffffff)
            {
                return;
            }
            unsigned long line_length = locked_box.Dimensions.X * GetPixelFormat().GetByteColourDepth();
            unsigned __int64 overflow_check2 = (unsigned __int64)((__int64)locked_box.Dimensions.Y * locked_box.Dimensions.Z) * line_length;
            if (overflow_check2 > 0xffffffff)
            {
                return;
            }
            unsigned long data_length = locked_box.Dimensions.Y * locked_box.Dimensions.Z * line_length;
            std::vector<unsigned char> src_buffer(data_length);
            unsigned char* dst = &src_buffer[0];
            unsigned char* src = locked_box.PMem;
            stream.Read(src, data_length);
            for (long cz = locked_box.Dimensions.Z; cz > 0; cz--)
            {
                for (long cy = locked_box.Dimensions.Y; cy > 0; cy--)
                {
                    memcpy(dst, src, line_length);
                    dst += locked_box.RowPitch;
                    src += line_length;
                }
                dst += locked_box.SlicePitch;
            }
        }
        Unlock(c1);
    }
}
