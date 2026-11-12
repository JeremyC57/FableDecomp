// CTexture::LoadFromDataStreamToPreallocatedSurface(CDataInputStream& in) -- read the texture file info, then for every mip level lock the preallocated surface and stream the level in: compressed formats as one block sized from the level dimensions, others row by row through a temporary buffer honouring the lock pitch (overflowing sizes abandon the load)
// Grade: DIFFER(789v789) -- Q20 port onto the shared CTexture / CPixelFormat / CDataInputStream / stlport vector headers fixed the ebx/ebp swap (14 diff lines left): the non-compressed branch's `width` load is hoisted above the first GetByteColourDepth call in ours, and the row-copy loop keeps src/dst in edx/eax where retail uses eax/edx. No-width-local, swapped src/dst declarations and a row<height loop were all worse.
// Leaf: ?LoadFromDataStreamToPreallocatedSurface@CTexture@@QAEXAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FB750, 789 bytes, thiscall, `ret 4`. Ego_r.pdb
//   ?LoadFromDataStreamToPreallocatedSurface@CTexture@@QAEXAAVCDataInputStream@@@Z.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the header's inline Read), CPixelFormat::Initialise(const
//   CPixelFormatInit&) 0x009E3640 / (D3DFORMAT) 0x009E3830, ::IsCompressed 0x009E3940, ::GetColourDepth 0x009E3820,
//   ::GetByteColourDepth 0x009E3800, CTexture::Lock(ulong, ulong) 0x009FA450 (CSurfaceLockedRect by value),
//   std::vector<unsigned char>(size_type) 0x005136B0 (out of line), _allmul 0x00BFEB50, free 0x00BFEA14 (the vector's release);
//   IDirect3DTexture9 GetLevelCount (+0x34), GetLevelDesc (+0x44), UnlockRect (+0x50).
// Layout: Ego_r.pdb CTexture {PD3DTexture +0, ByteLength:28 / AllocationSource:4 +4} (engine/CTexture.h), CSurfaceLockedRect
//   {Dimensions +0, Pitch +8, PMem +0xC}, CTextureFileInfo (19 bytes, packed: Width, Height, NoLevels, PixelFormatInit +5,
//   UseageFlags +0xB, Pool +0xF) and CPixelFormatInit (6 bytes) -- no shared header for the last two, modelled here from Ego_r.pdb.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_LOCK
#define FABLE_RETAIL_CTEXTURE_LOADFROMDATASTREAM
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_INIT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CDataInputStream.h"
#include "engine/ESurfacePool.h"

struct D3DSURFACE_DESC {
    _D3DFORMAT    Format;
    long          Type;
    unsigned long Usage;
    long          Pool;
    long          MultiSampleType;
    unsigned long MultiSampleQuality;
    unsigned int  Width;
    unsigned int  Height;
};
struct IDirect3DTexture9 {                         // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 + texture slots
    virtual long __stdcall QueryInterface(); virtual unsigned long __stdcall AddRef(); virtual unsigned long __stdcall Release();
    virtual long __stdcall GetDevice(); virtual long __stdcall SetPrivateData(); virtual long __stdcall GetPrivateData();
    virtual long __stdcall FreePrivateData(); virtual unsigned long __stdcall SetPriority(); virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad(); virtual long __stdcall GetType(); virtual unsigned long __stdcall SetLOD();
    virtual unsigned long __stdcall GetLOD();
    virtual unsigned long __stdcall GetLevelCount();                                  // +0x34
    virtual long __stdcall SetAutoGenFilterType(); virtual long __stdcall GetAutoGenFilterType();
    virtual void __stdcall GenerateMipSubLevels();
    virtual long __stdcall GetLevelDesc(unsigned int level, D3DSURFACE_DESC* desc);  // +0x44
    virtual long __stdcall GetSurfaceLevel(); virtual long __stdcall LockRect();
    virtual long __stdcall UnlockRect(unsigned int level);                            // +0x50
};

#pragma pack(push, 1)
class CPixelFormatInit {                          // Ego_r.pdb, 6 bytes
public:
    unsigned char Type : 4;
    unsigned char Linear : 1;
    unsigned char ColourDepth;
    unsigned char RBits;
    unsigned char GBits;
    unsigned char BBits;
    unsigned char ABits;
};
class CTextureFileInfo {                          // Ego_r.pdb, 19 bytes
public:
    unsigned short   Width;                       // +0x00
    unsigned short   Height;                      // +0x02
    unsigned char    NoLevels;                    // +0x04
    CPixelFormatInit PixelFormatInit;             // +0x05
    unsigned long    UseageFlags;                 // +0x0B
    ESurfacePool     Pool;                        // +0x0F
};
#pragma pack(pop)

void CTexture::LoadFromDataStreamToPreallocatedSurface(CDataInputStream& in)
{
    CTextureFileInfo info;
    in.Read(&info, sizeof(info));
    CPixelFormat format(info.PixelFormatInit);
    for (long level = 0; level < (long)PD3DTexture->GetLevelCount(); ++level)
    {
        CSurfaceLockedRect locked = Lock(level, 0);
        if (format.IsCompressed())
        {
            D3DSURFACE_DESC levelDesc;
            PD3DTexture->GetLevelDesc(level, &levelDesc);
            D3DSURFACE_DESC desc;
            PD3DTexture->GetLevelDesc(level, &desc);
            D3DSURFACE_DESC topDesc;
            PD3DTexture->GetLevelDesc(0, &topDesc);
            CPixelFormat levelFormat(topDesc.Format);
            in.Read(locked.PMem, (levelFormat.GetColourDepth() * desc.Width * desc.Height) >> 3);
        }
        else
        {
            long width = locked.Dimensions.X;
            if ((unsigned __int64)format.GetByteColourDepth() * (__int64)width > 0xffffffffu)
                return;
            unsigned long rowBytes = format.GetByteColourDepth() * width;
            long height = locked.Dimensions.Y;
            if ((__int64)height * (unsigned __int64)rowBytes > 0xffffffffu)
                return;
            std::vector<unsigned char> buffer(height * rowBytes);
            unsigned char* dst = locked.PMem;
            unsigned char* src = &buffer[0];
            in.Read(&buffer[0], height * rowBytes);
            for (long row = locked.Dimensions.Y; row > 0; --row)
            {
                memcpy(dst, src, rowBytes);
                dst += locked.Pitch;
                src += rowBytes;
            }
        }
        PD3DTexture->UnlockRect(level);
    }
}
