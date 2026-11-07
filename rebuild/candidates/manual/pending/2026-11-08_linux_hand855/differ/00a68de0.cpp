// CVolumeTexture::LoadFromDataStream(CDataInputStream& stream) -- read the 21-byte CVolumeTextureFileInfo header, create the volume texture it describes, then fill every mip level: compressed levels as one block, others through a packed buffer slice by slice and row by row
// Grade: DIFFER(923v919) -- 134 diff lines: the `this` spill slot ([esp+10h] where retail uses [esp+1Ch]) and the uncompressed branch's register roles (retail keeps the box width in ebx across the second GetPixelFormat call and Z in edi; ours recomputes through ebp/ebx); a named width local moves the box/pixel-format temporaries instead (md 161)
// Next idea: a same-TU noinline CPixelFormat::GetByteColourDepth / GetPixelFormat body (retail homes the by-value CPixelFormat temporaries in dead slots), or naming the 64-bit products as locals in retail's evaluation order
// Leaf: ?LoadFromDataStream@CVolumeTexture@@QAEXAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A68DE0, 919 bytes, thiscall `ret 4`; one caller (0x00A69530); no EH frame.
// Identity (proven): Ego_r.pdb 0x00BEA360 / FableWin.pdb 0x0332D160 ?LoadFromDataStream@CVolumeTexture@@QAEXAAVCDataInputStream@@@Z;
//   the body reads the 21-byte CVolumeTextureFileInfo, calls the proven CVolumeTexture::Initialise 0x00A68370 / Lock 0x00A685E0 /
//   GetLevelByteLength 0x00A681F0 on `this`, the mirror of CVolumeTexture::SaveToDataStream 0x00A68AE0.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inline Read), CPixelFormat::Initialise(const CPixelFormatInit&)
//   0x009E3640 / Initialise(_D3DFORMAT) 0x009E3830 (the by-value GetPixelFormat copies) / GetByteColourDepth 0x009E3800 /
//   IsCompressed 0x009E3940, __allmul 0x00BFEB50 (the two 32-bit overflow guards), vector<1-byte>(n) 0x005136B0 (byte-identical twin
//   of 0x00514E20: both landed as ??0?$vector@EV?$allocator@E@std@@@std@@QAE@I@Z, one of them is another 1-byte element type), free
//   0x00BFEA14; IDirect3DVolumeTexture9 slots 13 GetLevelCount (+0x34), 17 GetLevelDesc (+0x44), 20 UnlockBox (+0x50).
// Shape: the uncompressed level reads into the locked memory, then walks rows (RowPitch) and slices (SlicePitch) into the packed
//   buffer; both size guards return straight out of the level loop (the box stays locked), as retail.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_INIT
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#include <string.h>
#pragma intrinsic(memcpy)
#include "stlport/fable_containers.h"
#include "engine/CDataInputStream.h"
#include "engine/CVolumeTexture.h"
#include "engine/CVolumeTextureFileInfo.h"

void CVolumeTexture::LoadFromDataStream(CDataInputStream& stream)
{
    CVolumeTextureFileInfo info;
    stream.Read(&info, sizeof(info));
    CPixelFormat pixelFormat(info.PixelFormatInit);
    Initialise(C3DExtentsI(info.Width, info.Height, info.Depth), info.NoLevels, pixelFormat, info.UseageFlags, info.Pool, false);
    for (long level = 0; level < GetNoLevels(); ++level) {
        CVolumeLockedBox box = Lock(level, 0x10);
        if (pixelFormat.IsCompressed()) {
            C3DExtentsI dimensions = GetDimensions(level);
            stream.Read(box.PMem, GetLevelByteLength(level));
        } else {
            if ((unsigned __int64)GetPixelFormat().GetByteColourDepth() * box.Dimensions.X > 0xffffffff)
                return;
            unsigned long rowBytes = GetPixelFormat().GetByteColourDepth() * box.Dimensions.X;
            if ((unsigned __int64)((__int64)box.Dimensions.Y * box.Dimensions.Z) * rowBytes > 0xffffffff)
                return;
            unsigned long size = box.Dimensions.Y * box.Dimensions.Z * rowBytes;
            std::vector<unsigned char> buffer(size);
            unsigned char* dst = buffer.begin();
            unsigned char* src = box.PMem;
            stream.Read(box.PMem, size);
            for (long z = box.Dimensions.Z; z > 0; --z) {
                for (long y = box.Dimensions.Y; y > 0; --y) {
                    memcpy(dst, src, rowBytes);
                    src += rowBytes;
                    dst += box.RowPitch;
                }
                dst += box.SlicePitch;
            }
        }
        Unlock(level);
    }
}
