// CGraphicInfo::CGraphicInfo(unsigned long index, CGraphicDataBank* bank, const CGraphicDataBank::CGraphicHeader& header) -- a graphic entry of a data bank: default header overwritten by the given header, its bank and entry index, no frames yet, and the texture type the bank chooses for it
// Grade: DIFFER(126v126) -- prologue scheduling only: retail emits "push ebp ; mov ebp,[esp+10h]" (bank) after the header's bit-field load and "push edi" after the bit-field store; ours hoists push ebp/bank load to just after "mov ebx,ecx" and push edi above the bit-field store
// Leaf: ??0CGraphicInfo@@QAE@KPAVCGraphicDataBank@@ABVCGraphicHeader@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD700, 126 bytes, ret 0Ch. Ego_r.pdb
//   ??0CGraphicInfo@@QAE@KPAVCGraphicDataBank@@ABVCGraphicHeader@1@@Z (donor 0x00B57C80): the header type is the NESTED
//   CGraphicDataBank::CGraphicHeader. Same TU: CGraphicHeader() 0x009FD6C0 (landed, the init-list form used here) and the
//   default CGraphicInfo() 0x009FD780 (Bank 0, Index -1, Frames 0).
// Callees: CBankFile::GetEntryDataType(unsigned long) const 0x009CC210 (Ego_r QBEKK), CGraphicDataBank::GetTextureType(const
//   CGraphicHeader&, bool) const 0x009FD2B0 (Ego_r private ABE -> friend CGraphicInfo here).
// Tried (this session): all-body header ctor (bank load placed right, header stores sink below PixelFormat), init-list shorts
//   + body rest, the landed 0x009FD6C0 init list (this file), member-based call arguments (127v126). Earlier: init-list header
//   ctor, chained / separate body assignments.
// Layout: Ego_r.pdb CGraphicInfo (Header +0, TextureType +0x1c, Bank +0x20, Index +0x24, Frames +0x28) and CGraphicHeader.
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
};
class CGraphicInfo;
class CGraphicInfoFrame;
class CBankFile {
public:
    unsigned long GetEntryDataType(unsigned long index) const;      // 0x009CC210
};
class CGraphicDataBank : public CBankFile {
public:
    enum ETextureType { };
    class CGraphicHeader {
    public:
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
            : Width(0), Height(0), Depth(0), FrameWidth(0), FrameHeight(0), FrameCount(1), PixelFormat(),
              TransparencyType(0), MipmapLevels(0), IsFlatAnimatedGraphic(false), FrameDataSize(0)
        {
            CompressedMipmapSizes[0] = 0;
        }
    };
private:
    friend class CGraphicInfo;
    ETextureType GetTextureType(const CGraphicHeader& header, bool flag) const;   // 0x009FD2B0
};
class CGraphicInfo {
public:
    CGraphicDataBank::CGraphicHeader Header;           // +0x00
    CGraphicDataBank::ETextureType   TextureType;      // +0x1c
    CGraphicDataBank*  Bank;                           // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
    CGraphicInfo(unsigned long index, CGraphicDataBank* bank, const CGraphicDataBank::CGraphicHeader& header);
};

CGraphicInfo::CGraphicInfo(unsigned long index, CGraphicDataBank* bank, const CGraphicDataBank::CGraphicHeader& header)
    : Bank(bank), Index(index), Frames(0)
{
    Header = header;
    TextureType = bank->GetTextureType(Header, bank->GetEntryDataType(index) == 2);
}
