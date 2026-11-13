// CGraphicDataBank::LoadGraphicFrame(CGraphicFrame& frame, CManagedTexture& managed_texture, CBankFileAsyncData& data, CGraphicInfo* info, bool, unsigned long mipmap_level, ETextureType texture_type, bool) -- upload one loaded graphic into its managed texture: when the texture's top surface already has the frame's (mip-reduced) byte size and the bank's pixel format for the texture type, stream every mip level straight into the locked surfaces (the top level decompressed); otherwise decompress into a buffer, build an image surface, copy it into the texture and regenerate the mipmaps; then initialise the frame (or the flat animation's sections) on the texture
// Leaf: ?LoadGraphicFrame@CGraphicDataBank@@AAE_NAAVCGraphicFrame@@AAVCManagedTexture@@AAVCBankFileAsyncData@@PAVCGraphicInfo@@_NKW4ETextureType@1@4@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x009FED30, 1206 bytes, thiscall `ret 20h`, then int3 padding 0x009FF1E6..0x009FF1EF; the bytes
//   0x009FF1F0..0x009FF21F are a separate 48-byte function that calls this one (`call 0x009FED30`, `ret 14h`), so 1206 is the true
//   length (the reservation's 1264 ran to the next catalog start 0x009FF220). Ego_r.pdb / FableWin.pdb
//   ?LoadGraphicFrame@CGraphicDataBank@@AAE_NAAVCGraphicFrame@@AAVCManagedTexture@@AAVCBankFileAsyncData@@PAVCGraphicInfo@@_NKW4ETextureType@1@4@Z
//   (private); the two bool parameters are not read by the retail body.
// Callees: CPixelFormat::GetColourDepth 0x009E3820 / IsEquivalent 0x009E37F0, CTexture::GetSurfaceLevel 0x009F9E00 / GetNoLevels
//   0x009F9DF0 / GenerateMipmaps 0x009F9E60, CSurface::GetByteLength 0x009F2FC0 / Lock 0x009F33E0 / Unlock 0x009F2DC0 / Uninitialise
//   0x009F2E20 (the inline destructor) / InitialiseAsImage 0x009F3230 / CopyFromMemory 0x009F4390 / CopyFromSurface 0x009F37E0,
//   CBankFileAsyncData::PeekData 0x009D5580, CMemoryDataInputStream(const void*, unsigned long) 0x00994930 / ReadCompressed 0x00995B10
//   (called directly), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inside the inline Read), ~CDataStream 0x00993B10,
//   vector<unsigned char>::_M_fill_insert 0x00485F3A (inside resize), free 0x00BFEA14, CGraphicInfo::InitialiseFramesAsSurfaceSections
//   0x009FE780, CGraphicFrame::InitialiseAsSurface 0x009FCB20, operator delete 0x00BFE9BC (the texture counted pointer's release).
//   Vtable: CSurface 0x0122F84C.
// Layout: shared headers (static-blocks branch) with declaration opt-ins in the overlay (hdr/README.md); CGraphicDataBank::PixelFormats
//   typed CPixelFormat[11] at +0x288 (same 44 bytes); the info's header is read in place (Width +0x00 .. FrameDataSize +0x14,
//   CompressedMipmapSizes +0x18, TransparencyType +0x10, IsFlatAnimatedGraphic +0x12 bit 0).
// Shape: the top surface's byte length is a named local, so the GetSurfaceLevel temporary is destroyed before the size / format test
//   (retail 0x009FEDF1..0x009FEE12; tested inside the condition VC7.1 keeps a bool across the temporary's destructor, +27 bytes); the
//   mip reduction counts the level down (`dec eax ; jne`, no aligned loop head -- an up-counting `for` adds a `jmp` and a `lea` pad).
//   The rest is the natural spelling: the stream's inline bounds-checked Read (memcpy as `rep movsd / movsb`), CArray::resize and
//   GetAsCArray (the zero-size array is the CArray itself), the C2DBoxI temporaries rebuilt per call.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CPIXELFORMAT_ISEQUIVALENT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_LEVELS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_LOADFRAME
#define FABLE_RETAIL_CGRAPHICDATABANK_LOADGRAPHICFRAME
#define FABLE_RETAIL_CMANAGEDTEXTURE_GETTEXTURE
#define FABLE_RETAIL_CBANKFILEASYNCDATA_PEEKDATA
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_LOADGRAPHICFRAME
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include <string.h>
#include "engine/CGraphicInfo.h"
#include "engine/CManagedTexture.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CTexture.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CSurface.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CGraphicFrame.h"
#include "engine/CArray.h"

bool CGraphicDataBank::LoadGraphicFrame(CGraphicFrame& frame, CManagedTexture& managed_texture, CBankFileAsyncData& data,
                                        CGraphicInfo* info, bool, unsigned long mipmap_level, ETextureType texture_type, bool)
{
    CPixelFormat pixel_format = info->Header.PixelFormat;
    pixel_format.GetColourDepth();                                       // result unused in retail too (0x009FED4B)
    CPixelFormat texture_format = PixelFormats[texture_type];
    bool same_format = false;
    if (texture_format.IsEquivalent(pixel_format))
        same_format = true;
    unsigned long width = info->Header.Width;
    unsigned long height = info->Header.Height;
    unsigned long frame_width = info->Header.FrameWidth;
    unsigned long frame_height = info->Header.FrameHeight;
    unsigned long data_size = info->Header.FrameDataSize;
    for (unsigned long level = mipmap_level; level > 0; --level) {
        width >>= 1;
        height >>= 1;
        frame_width >>= 1;
        frame_height >>= 1;
        data_size >>= 2;
    }
    CCountedPointer<CTexture> texture = managed_texture.GetTexture();
    unsigned long byte_length = texture->GetSurfaceLevel(0).GetByteLength();
    if (byte_length == data_size && same_format) {
        CMemoryDataInputStream stream(data.PeekData(), data.GetLength());
        unsigned long levels = texture->GetNoLevels();
        for (unsigned long level = 0; level < levels; ++level) {
            CSurface surface = texture->GetSurfaceLevel(level);
            long bytes = surface.GetByteLength();
            CSurfaceLockedRect rect = surface.Lock(0);
            unsigned long mipmap = mipmap_level + level;
            if (mipmap < 1 && info->Header.CompressedMipmapSizes[mipmap] > 0)
                stream.ReadCompressed(rect.PMem, bytes);
            else
                stream.Read(rect.PMem, bytes);
            surface.Unlock();
        }
    } else {
        CArray<unsigned char> buffer;
        const void* source = data.PeekData();
        if (mipmap_level < 1 && info->Header.CompressedMipmapSizes[mipmap_level] > 0) {
            CMemoryDataInputStream stream(source, data.GetLength());
            unsigned long bytes = pixel_format.GetColourDepth() * height * width / 8;
            buffer.resize(bytes);
            stream.ReadCompressed(buffer.GetAsCArray(), bytes);
            source = buffer.GetAsCArray();
        }
        long pitch = pixel_format.GetColourDepth() * width / 8;
        CSurface image;
        image.InitialiseAsImage(C2DExtentsI(width, height), pixel_format, false);
        image.CopyFromMemory(&C2DBoxI(0, 0, width, height), source, &C2DBoxI(0, 0, width, height), pixel_format, pitch, 0xffffffff);
        texture->GetSurfaceLevel(0).CopyFromSurface(&C2DBoxI(0, 0, width, height), image, &C2DBoxI(0, 0, width, height), 0xffffffff);
        texture->GenerateMipmaps(0xffffffff, 0);
    }
    bool alpha = info->Header.TransparencyType != 0;
    bool boolean_alpha = info->Header.TransparencyType == 2 || info->Header.TransparencyType == 4;
    if (info->Header.IsFlatAnimatedGraphic)
        info->InitialiseFramesAsSurfaceSections(managed_texture, (signed char)mipmap_level, texture, alpha, boolean_alpha);
    else
        frame.InitialiseAsSurface(texture, frame_width, frame_height, alpha, boolean_alpha);
    return true;
}