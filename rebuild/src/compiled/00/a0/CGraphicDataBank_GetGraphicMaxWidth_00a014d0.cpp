// CGraphicDataBank::GetGraphicMaxWidth(unsigned long index) const -- the largest frame width (FrameWidth of each frame's owning graphic info) over the frames of graphic `index`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A014D0, length 100 bytes (padded to 112).
// Callees: none.
// Shape: the graphic is an inline bounds-checked getter (`index < size()` with the size from the signed /44 pointer
// difference, NULL when out of range -- the frame count is then read without a guard, as in retail); the loop runs a
// down-counter over 16-byte frames with the frame pointer pre-offset to +0x0c; `if (max <= width) max = width` gives
// `cmp eax,edx ; ja`.
// Layout: Ego_r.pdb CGraphicInfo / CGraphicHeader / CGraphicInfoFrame; the retail GraphicList sits at +0x1e0 (12-byte CArray).
// Leaf: ?GetGraphicMaxWidth@CGraphicDataBank@@
struct CGraphicInfo;
struct CGraphicHeader {                            // Ego_r.pdb, 28 bytes
    unsigned short Width;                          // +0x00
    unsigned short Height;                         // +0x02
    unsigned short Depth;                          // +0x04
    unsigned short FrameWidth;                     // +0x06
    unsigned short FrameHeight;                    // +0x08
    unsigned short FrameCount;                     // +0x0a
    unsigned char  _rest[0x10];
};
struct CGraphicInfoFrame {                         // Ego_r.pdb, 16 bytes
    long           _pad00;
    unsigned short FrameNum;                       // +0x04
    signed char    SmallMipmapLevel;               // +0x06
    void*          Data;                           // +0x08 CIVCountedPointer<CFrameData>
    CGraphicInfo*  GraphicInfo;                    // +0x0c
};
struct CGraphicInfo {                              // Ego_r.pdb, 0x2c bytes
    CGraphicHeader     Header;                     // +0x00
    long               TextureType;                // +0x1c
    void*              Bank;                       // +0x20
    long               Index;                      // +0x24
    CGraphicInfoFrame* Frames;                     // +0x28
};
struct CArray_CGraphicInfo {                       // retail CArray<CGraphicInfo>: 12 bytes
    CGraphicInfo* m_pFirst;
    CGraphicInfo* m_pLast;
    CGraphicInfo* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
};
struct CGraphicDataBank {
    unsigned char       _pad000[0x1e0];
    CArray_CGraphicInfo GraphicList;               // +0x1e0
    const CGraphicInfo* GetGraphicInfo(unsigned long index) const { return index < GraphicList.size() ? &GraphicList.m_pFirst[index] : 0; }
    unsigned long GetGraphicMaxWidth(unsigned long index) const;
    unsigned long GetGraphicMaxHeight(unsigned long index) const;
};

unsigned long CGraphicDataBank::GetGraphicMaxWidth(unsigned long index) const
{
    const CGraphicInfo* info = GetGraphicInfo(index);
    unsigned long maxWidth = 0;
    for (unsigned long i = 0; i < info->Header.FrameCount; ++i) {
        unsigned long width = info->Frames[i].GraphicInfo->Header.FrameWidth;
        if (maxWidth <= width)
            maxWidth = width;
    }
    return maxWidth;
}