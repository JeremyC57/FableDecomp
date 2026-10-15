// CGraphicInfo::CGraphicInfo(unsigned long index, CGraphicDataBank* bank, const CGraphicHeader& header) -- a graphic entry of a data bank: default header overwritten by the bank's header copy, its bank and entry index, no frames yet, and the texture type the bank chooses for it
// Grade: DIFFER(ours v retail) -- 126v126, scheduling only: retail emits `push ebp ; mov ebp,[esp+10h]` after the PixelFormat/bit-field loads and `push edi` after the bit-field store, ours hoists `push ebp` above the header zero stores and `push edi` before the bit-field store | tried: init-list header ctor, chained body assignments (reverses the stores), separate body assignments | next: lane gap: prologue push scheduling
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD700, length 126 bytes.
// Callees: CBankFile::GetEntryDataType @ 0x009CC210 (on the bank), CGraphicDataBank::GetTextureType(const CGraphicHeader*,
//          bool) @ 0x009FD2B0.
// Shape: CGraphicHeader has an inline default constructor (five zero words from the shared `xor eax,eax`, FrameCount 1,
// the CPixelFormat -1, the flat-animated bit cleared with `and cl,0FEh`, the remaining zeroes) run by the init list,
// then Bank / Index / Frames; the body copies the header (`rep movsd` x7, trivial assignment) and stores the chosen
// texture type (the `== 2` data-type test evaluated before `this` is pushed).
// Layout: Ego_r.pdb CGraphicInfo (Header +0, TextureType +0x1c, Bank +0x20, Index +0x24, Frames +0x28) and CGraphicHeader.
// Leaf: ??0CGraphicInfo
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
};
struct CGraphicHeader {                            // Ego_r.pdb, 28 bytes
    unsigned short Width;                          // +0x00
    unsigned short Height;                         // +0x02
    unsigned short Depth;                          // +0x04
    unsigned short FrameWidth;                     // +0x06
    unsigned short FrameHeight;                    // +0x08
    unsigned short FrameCount;                     // +0x0a
    CPixelFormat   PixelFormat;                    // +0x0c
    unsigned char  TransparencyType;               // +0x10
    unsigned char  MipmapLevels;                   // +0x11
    unsigned char  IsFlatAnimatedGraphic : 1;      // +0x12 bit 0
    unsigned long  FrameDataSize;                  // +0x14
    unsigned long  CompressedMipmapSizes[1];       // +0x18
    CGraphicHeader()
        : Width(0), Height(0), Depth(0), FrameWidth(0), FrameHeight(0), FrameCount(1), TransparencyType(0), MipmapLevels(0),
          FrameDataSize(0)
    {
        IsFlatAnimatedGraphic = 0;
        CompressedMipmapSizes[0] = 0;
    }
};
struct CGraphicInfo;
struct CGraphicInfoFrame;
struct CGraphicDataBank {
    unsigned long GetEntryDataType(unsigned long index);   // 0x009CC210 (CBankFile)
    long GetTextureType(const CGraphicHeader* header, bool flag);   // 0x009FD2B0
};
struct CGraphicInfo {
    CGraphicHeader     Header;                     // +0x00
    long               TextureType;                // +0x1c (ETextureType)
    CGraphicDataBank*  Bank;                       // +0x20
    unsigned long      Index;                      // +0x24
    CGraphicInfoFrame* Frames;                     // +0x28
    CGraphicInfo(unsigned long index, CGraphicDataBank* bank, const CGraphicHeader& header);
};

CGraphicInfo::CGraphicInfo(unsigned long index, CGraphicDataBank* bank, const CGraphicHeader& header)
    : Bank(bank), Index(index), Frames(0)
{
    Header = header;
    TextureType = bank->GetTextureType(&Header, bank->GetEntryDataType(index) == 2);
}
