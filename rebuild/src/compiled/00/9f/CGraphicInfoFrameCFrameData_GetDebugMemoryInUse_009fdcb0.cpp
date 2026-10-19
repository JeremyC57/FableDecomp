// CGraphicInfoFrame::CFrameData::GetDebugMemoryInUse() const -- debug estimate of a graphic frame's memory: the
// object plus its frame data and loading texture, minus the mip levels above the touched/small level that are not
// resident (each level a quarter of the one above)
// Leaf: GetDebugMemoryInUse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FDCB0, length 226 bytes, `ret`.
// Callees: CGraphicFrameData::Sizeof @ 0x009FCBF0, CManagedTexture::Sizeof @ 0x00A69E60, CTexture::GetNoLevels
// @ 0x009F9DF0, the CTexture mip-surface getter (CSurface by hidden pointer) @ 0x009F9E00 (labelled GetNoLevels too),
// CSurface::GetByteLength @ 0x009F2FC0, CSurface::Uninitialise @ 0x009F2E20 (from the temporary's inline dtor).
// CSurface vtable 0x0122F84C. Graphics layer.
// Layout (Ego_r.pdb CGraphicInfoFrame::CFrameData, 0x44 bytes, retail agrees): Frame +0x1C (CGraphicFrame:
// FrameData), ManagedTexture +0x20, LoadingManagedTexture +0x28 (its Data +0x2C), FileData +0x30,
// CurrentMipmapLevel +0x38, LoadingMipmapLevel +0x39, TouchedMipmapLevel +0x3A (0x7F = never), TouchedFrame +0x3C,
// GraphicInfoFrame +0x40 (SmallMipmapLevel +6); CGraphicFrameData texture +8.
// Shape: the frame data's own size comes from an inline CFrameData::Sizeof() (object + CGraphicFrame::Sizeof, which is
// `data ? data->Sizeof() + 4 : 4`, + the loading texture), copied into the result local (retail's `mov ebx,edi`); the
// level is two Dinkumware min statements over long temporaries (the first result re-used in its slot); the mip-surface temporary is destroyed through CSurface's inline dtor; the loop
// subtracts then quarters.

class CSurface {
public:
    virtual void v00();
    void* PD3DSurface;
    unsigned long AllocationSource;
    void* PAllocatedMemory;
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                            // 0x009F2E20
    unsigned long GetByteLength() const;            // 0x009F2FC0
};

class CTexture {
public:
    long GetNoLevels() const;                       // 0x009F9DF0
    CSurface GetMipmapSurface(long level) const;    // 0x009F9E00
};

class CGraphicFrameData {
public:
    unsigned char _pad[8];
    CTexture* Texture;                              // +0x08
    unsigned long Sizeof() const;                   // 0x009FCBF0
};
class CGraphicFrame {
public:
    CGraphicFrameData* FrameData;
    CTexture* GetTexture() const { return FrameData->Texture; }
    unsigned long Sizeof() const { return FrameData ? FrameData->Sizeof() + sizeof(*this) : sizeof(*this); }
};
class CManagedTexture {
public:
    void* __vftable;
    void* Data;
    unsigned long Sizeof() const;                   // 0x00A69E60
};

template <class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }

class CGraphicInfoFrame {
public:
    void* __vftable;
    unsigned short FrameNum;                        // +0x04
    signed char SmallMipmapLevel;                   // +0x06

    class CFrameData {
    public:
        unsigned char _base[0x1c];
        CGraphicFrame Frame;                        // +0x1C
        CManagedTexture ManagedTexture;             // +0x20
        CManagedTexture LoadingManagedTexture;      // +0x28
        void* FileData[2];                          // +0x30
        signed char CurrentMipmapLevel;             // +0x38
        signed char LoadingMipmapLevel;             // +0x39
        signed char TouchedMipmapLevel;             // +0x3A
        unsigned long TouchedFrame;                 // +0x3C
        CGraphicInfoFrame* GraphicInfoFrame;        // +0x40
        unsigned long Sizeof() const
        {
            unsigned long size = Frame.Sizeof() + sizeof(*this);
            if (LoadingManagedTexture.Data)
                size += LoadingManagedTexture.Sizeof();
            return size;
        }
        long GetDebugMemoryInUse() const;
    };
};

long CGraphicInfoFrame::CFrameData::GetDebugMemoryInUse() const
{
    if (TouchedMipmapLevel == 0x7f || Frame.FrameData == 0)
        return 0;
    long memory = Sizeof();
    long level = Min<long>(TouchedMipmapLevel, GraphicInfoFrame->SmallMipmapLevel);
    level = Min<long>(level, Frame.GetTexture()->GetNoLevels());
    long levelSize = Frame.GetTexture()->GetMipmapSurface(0).GetByteLength();
    for (; level > 0; --level) {
        memory -= levelSize;
        levelSize /= 4;
    }
    return memory;
}