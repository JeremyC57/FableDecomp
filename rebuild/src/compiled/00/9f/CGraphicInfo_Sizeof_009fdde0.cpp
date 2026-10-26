// CGraphicInfo::Sizeof() const -- the memory held by a graphic entry: its own 0x2c bytes plus, per frame, the 16-byte frame record and its loaded frame data (the 0x44-byte CFrameData, its graphic frame and its managed texture)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FDDE0, length 137 bytes.
// Callees: CGraphicFrameData::Sizeof @ 0x009FCBF0, the frame data's texture Sizeof @ 0x00A69E60 (called on the embedded
//          counted pointer at +0x28 when its count block +0x2c is set; manifest "CManagedTexture::Sizeof").
// Shape: the per-frame sizes are inline Sizeof members: the graphic frame's ternary `FrameData ? FrameData->Sizeof() + 4 : 4`
// (`(.. : 0) + 4` folds the 4 into the 0x44), the frame data's `0x44 + frame + texture`, the frame record's 0x10 as a
// default-initialised local in a `__forceinline` helper (hoisted `mov ecx,10h`, `lea ecx,[edi+10h]` on the loaded arm); the running total and the counter live in the frame
// (register pressure), the frame offset steps by 16 in ebp.
// Layout: Ego_r.pdb CGraphicInfo (FrameCount +0x0a in the header, Frames +0x28) / CGraphicInfoFrame (Data +8).
// Leaf: ?Sizeof@CGraphicInfo@@
class CGraphicFrameData { public:
    unsigned long Sizeof() const;                  // 0x009FCBF0
};
class CGraphicFrame { public:                             // engine/CGraphicFrame.h: one pointer
    CGraphicFrameData* FrameData;
    unsigned long Sizeof() const { return FrameData ? FrameData->Sizeof() + sizeof(CGraphicFrame) : sizeof(CGraphicFrame); }
};
struct CManagedTexturePointer {                    // the frame data's counted texture pointer (+0x28)
    void* Data;
    void* Info;                                    // +0x04
    unsigned long Sizeof() const;                  // 0x00A69E60
};
struct CFrameData {                                // CGraphicInfoFrame::CFrameData, 0x44 bytes in the size sum
    unsigned char          _pad00[0x1c];
    CGraphicFrame          Frame;                  // +0x1c
    unsigned char          _pad20[8];
    CManagedTexturePointer Texture;                // +0x28
    unsigned char          _pad30[0x14];
    unsigned long Sizeof() const
    {
        unsigned long size = 0x44 + Frame.Sizeof();
        if (Texture.Info)
            size += Texture.Sizeof();
        return size;
    }
};
class CGraphicInfo;
class CGraphicInfoFrame { public:                         // Ego_r.pdb, 16 bytes
    long           _pad00;
    unsigned short FrameNum;                       // +0x04
    signed char    SmallMipmapLevel;               // +0x06
    CFrameData*    Data;                           // +0x08 CIVCountedPointer<CFrameData>
    CGraphicInfo*  GraphicInfo;                    // +0x0c
    __forceinline unsigned long Sizeof() const
    {
        unsigned long size = sizeof(CGraphicInfoFrame);
        if (Data)
            size += Data->Sizeof();
        return size;
    }
};
struct CGraphicHeader {
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight;
    unsigned short FrameCount;                     // +0x0a
    unsigned char  _rest[0x10];
};
class CGraphicInfo { public:                              // Ego_r.pdb, 0x2c bytes
    CGraphicHeader     Header;                     // +0x00
    long               TextureType;                // +0x1c
    void*              Bank;                       // +0x20
    long               Index;                      // +0x24
    CGraphicInfoFrame* Frames;                     // +0x28
    unsigned long Sizeof() const;
};

unsigned long CGraphicInfo::Sizeof() const
{
    unsigned long size = sizeof(CGraphicInfo);
    for (unsigned long i = 0; i < Header.FrameCount; ++i)
        size += Frames[i].Sizeof();
    return size;
}