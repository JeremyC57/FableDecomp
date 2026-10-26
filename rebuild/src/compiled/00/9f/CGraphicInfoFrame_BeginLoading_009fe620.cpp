// CGraphicInfoFrame::BeginLoading(unsigned long a, unsigned long b) -- starts loading this frame's data; frames of a flat-animated graphic share frame 0's data, so the walk first moves to that frame, then creates its frame data on demand and forwards the request
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FE620, length 118 bytes (padded to 120).
// Callees: operator new @ 0x00BFEA1A (0x44 bytes), CGraphicInfoFrame::CFrameData::CFrameData(CGraphicInfoFrame*) @ 0x009FE380,
//          the frame data's virtual slot 1 (+4, the last-reference destroy), CFrameData::BeginLoading @ 0x009FDE80.
// Shape: `while (frame->FrameNum > 0 && frame->GraphicInfo->Header.IsFlatAnimatedGraphic) frame = frame->GraphicInfo->Frames;`
// rotated (the bit mask hoisted into cl, `lea esp,[esp]` pad); the CIVCountedPointer assignment from `new` is an inline
// Reset through a local pointer copy (count at +4 of the pointee, virtual destroy, pointer zeroed -- the dead zero store
// kept) then the store.
// Layout: Ego_r.pdb CGraphicInfoFrame (FrameNum +4, Data +8, GraphicInfo +0x0c) and CGraphicInfo (Frames +0x28);
// the header's flat-animated bit is +0x12 bit 0.
// Leaf: ?BeginLoading@CGraphicInfoFrame@@
void* operator new(unsigned int size);             // 0x00BFEA1A
class CGraphicInfoFrame;
struct CFrameData {                                // CGraphicInfoFrame::CFrameData (0x44 bytes): an intrusively counted pointee
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+4)
    long          m_references;                    // +0x04
    unsigned char _pad08[0x3c];
    CFrameData(CGraphicInfoFrame* frame);          // 0x009FE380
    void BeginLoading(unsigned long a, unsigned long b);   // 0x009FDE80
};
template <class T> class CIVCountedPointer { public:
    T* Data;
    void Reset()
    {
        T* p = Data;                               // a local copy: the count is decremented and tested through ecx
        if (p) {
            if (--p->m_references == 0)
                p->Destroy();
            Data = 0;
        }
    }
    CIVCountedPointer& operator=(T* p)
    {
        Reset();
        Data = p;
        return *this;
    }
};
struct CGraphicHeader {
    unsigned char _pad00[0x12];
    unsigned char IsFlatAnimatedGraphic : 1;       // +0x12 bit 0
    unsigned char _pad13[0x1c - 0x13];
};
class CGraphicInfo { public:
    CGraphicHeader     Header;                     // +0x00
    unsigned char      _pad1c[0x0c];
    CGraphicInfoFrame* Frames;                     // +0x28
};
class CGraphicInfoFrame { public:
    long                          _pad00;
    unsigned short                FrameNum;        // +0x04
    signed char                   SmallMipmapLevel;   // +0x06
    CIVCountedPointer<CFrameData> Data;            // +0x08
    CGraphicInfo*                 GraphicInfo;     // +0x0c
    void BeginLoading(unsigned long a, unsigned long b);
};

void CGraphicInfoFrame::BeginLoading(unsigned long a, unsigned long b)
{
    CGraphicInfoFrame* frame = this;
    while (frame->FrameNum > 0 && frame->GraphicInfo->Header.IsFlatAnimatedGraphic)
        frame = frame->GraphicInfo->Frames;
    if (!frame->Data.Data)
        frame->Data = new CFrameData(frame);
    frame->Data.Data->BeginLoading(a, b);
}