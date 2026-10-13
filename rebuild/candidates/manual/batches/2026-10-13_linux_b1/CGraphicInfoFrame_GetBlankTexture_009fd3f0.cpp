// CGraphicInfoFrame::GetBlankTexture()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD3F0, oracle length 17 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+0Ch] ; mov ecx,[eax+1Ch] ; mov edx,[eax+20h] ; lea eax,[edx+ecx*8+2B4h] ; ret
// PDB: CGraphicInfoFrame +0x0C = CGraphicInfo* GraphicInfo; CGraphicInfo +0x1C = ETextureType
// TextureType, +0x20 = CGraphicDataBank* Bank. The data bank keeps one 8-byte "blank texture"
// slot per texture type starting at +0x2B4; this returns the address of the slot for this
// frame's texture type. The slot type is opaque here (manifest return type: CTexture*).

struct CBlankTextureSlot { unsigned char _slot[8]; };   // 8-byte per-type slot (manifest return type: CTexture*)

struct CGraphicDataBank {
    unsigned char _pad_0x000[0x2B4];
    CBlankTextureSlot BlankTextures[1];           // +0x2B4 indexed by ETextureType
};

struct CGraphicInfo {
    unsigned char     Header[0x1C];               // +0x00 CGraphicHeader (PDB)
    long              TextureType;                // +0x1C ETextureType (PDB)
    CGraphicDataBank* Bank;                       // +0x20 (PDB)
};

struct CGraphicInfoFrame {
    void*          __vftable;                     // +0x00
    unsigned short FrameNum;                      // +0x04 (PDB)
    signed char    SmallMipmapLevel;              // +0x06 (PDB)
    unsigned char  _pad_0x07;
    void*          Data;                          // +0x08 CIVCountedPointer<CFrameData> (PDB)
    CGraphicInfo*  GraphicInfo;                   // +0x0C (PDB)
    CBlankTextureSlot* GetBlankTexture();
};

CBlankTextureSlot* CGraphicInfoFrame::GetBlankTexture()
{
    CGraphicInfo* info = this->GraphicInfo;
    return &info->Bank->BlankTextures[info->TextureType];
}
