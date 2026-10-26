// CGraphicInfoFrame::CFrameData::ReleaseTexture() -- when no asynchronous read is pending, ask the owning graphic info to unload this frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FEA00, length 24 bytes (32 with int3 padding).
// Leaf: ReleaseTexture
// Callees: CGraphicInfo::UnloadFrame(unsigned long) @ 0x009FE8E0 (landed, thiscall, `ret 4`).
// Name: vtable 0x0129C89C slot 10. FableWin.pdb lists the CFrameData virtual overrides IsResourceLoaded,
// OnResourceLoaded, ReleaseTexture (UAEXXZ), GetUnloadableMemory, GetDebugMemoryInUse, ResetDebugMemoryInUse;
// slots 3/4/5/8 are landed or identified as GetUnloadableMemory/IsResourceLoaded/OnResourceLoaded/
// GetDebugMemoryInUse, and this void body that frees the frame's texture through its owner is ReleaseTexture.
// Layout: Ego_r.pdb CFrameData (0x44): FileData object word +0x30, GraphicInfoFrame +0x40; CGraphicInfoFrame
// (16 bytes): FrameNum (unsigned short) +4, GraphicInfo +0xc.
// Shape: the frame number is zero-extended (`movzx`) into the unsigned long argument and pushed before the
// GraphicInfo load; the call is the last statement, so the function falls into a single `ret`.
class CBankFileAsyncData;
class CGraphicInfo { public:
    void UnloadFrame(unsigned long frameNum);   // 0x009FE8E0
};

class CGraphicInfoFrame { public:
    void*          __vftable;           // +0x00
    unsigned short FrameNum;            // +0x04
    signed char    SmallMipmapLevel;    // +0x06
    void*          Data;                // +0x08 CIVCountedPointer<CFrameData>
    CGraphicInfo*  GraphicInfo;         // +0x0c
};

struct CCountedPointer_BankFileAsyncData {
    CBankFileAsyncData* m_pObject;      // +0
    void*               m_pCount;       // +4
};

struct CFrameData {
    unsigned char                     _pad[0x30];
    CCountedPointer_BankFileAsyncData FileData;            // +0x30
    signed char                       CurrentMipmapLevel;  // +0x38
    signed char                       LoadingMipmapLevel;  // +0x39
    signed char                       TouchedMipmapLevel;  // +0x3a
    unsigned long                     TouchedFrame;        // +0x3c
    CGraphicInfoFrame*                GraphicInfoFrame;    // +0x40
    void ReleaseTexture();
};

void CFrameData::ReleaseTexture()
{
    if (!FileData.m_pObject)
        GraphicInfoFrame->GraphicInfo->UnloadFrame(GraphicInfoFrame->FrameNum);
}