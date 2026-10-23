// Grade: DIFFER(881v955) -- frame now matches (size as a C2DCoordI); residue is the inlined CManagedTexture release/assign shapes: retail duplicates the failure-path epilogue, keeps the double Data=0 store of Assign(0), and uses dec-mem/jne vs mov/dec/mov forms per inline; edi/ebp swapped (newLevels vs &texture)
// CTextureManager::ReduceMipmapLevel(CGraphicFrame&, CManagedTexture&, ulong) -- drop the top `levels` mipmaps of a
// managed texture: allocate a new texture of the halved size with the remaining levels (same pixel format and owner),
// copy each remaining surface across, re-initialise the frame as a surface over the new texture (keeping its alpha
// flags), detach and unlock the old data and swap the managed texture over to the new one; false when allocation fails
// Leaf: ?ReduceMipmapLevel@CTextureManager@@QAE_NAAVCGraphicFrame@@AAVCManagedTexture@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A69F70, `ret 0xc`.
// Callees: CTexture::GetNoLevels 0x009F9DF0 / GetPixelFormat 0x009F9D80 / GetSurface 0x009F9E00, CSurface::CopyFromSurface
//   0x009F37E0 and its inline destructor (vtable 0x0122F84C, Uninitialise 0x009F2E20), CBase ctor 0x0099A2D0 / dtor
//   0x0099A2E0, CManagedTexture vtable 0x0129C880, CManagedTextureData::ReleaseTexture 0x00A69A80, CTextureManager::
//   AllocateTexture 0x00A69B70, CGraphicFrame::InitialiseAsSurface 0x009FCB20, CGraphicFrameData::Uninitialise 0x009FCBA0,
//   operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CGraphicFrameData (ReferenceCount +0, PTexture +8, Width/Height +0x14/+0x16, flag bits +0x1a:
//   Alpha 0, BooleanAlpha 1, IsInitialised 4); CManagedTextureData (intrusive vptr + count +4, Texture +0x20, Owner +0x30,
//   Locked +0x49); CManagedTexture {vptr, CIVCountedPointer Data +4}; CSurface 16 bytes with a vptr. The shared headers
//   are data-only, so the classes are modelled locally with these offsets (as the landed 0x00B2B120 caller does).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct CCPPointerInfo {
    long  RefCount;                                         // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void* Data;                                             // +0x08
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct CSurface {                                           // vtable 0x0122F84C, 16 bytes
    void*         PD3DSurface;                              // +0x04
    unsigned long AllocationSource;                         // +0x08
    void*         PAllocatedMemory;                         // +0x0c
    virtual void s000();
    void Uninitialise();                                    // 0x009F2E20
    __forceinline ~CSurface() { Uninitialise(); }
    void CopyFromSurface(const C2DBoxI* dstBox, const CSurface& src, const C2DBoxI* srcBox, unsigned long filter);   // 0x009F37E0
};
struct CPixelFormat { unsigned long Format; };
struct CTexture {
    long GetNoLevels() const;                               // 0x009F9DF0
    CPixelFormat GetPixelFormat() const;                    // 0x009F9D80
    CSurface GetSurface(unsigned long level) const;         // 0x009F9E00
};
template<class T> struct CCountedPointer {
    T*              Ptr;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer(const CCountedPointer& other) : Ptr(other.Ptr), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Ptr = 0;
        Info = 0;
    }
    T* operator->() const { return Ptr; }
};
struct CGraphicFrameData {
    unsigned long             ReferenceCount;               // +0x00
    void*                     Data;                         // +0x04
    CCountedPointer<CTexture> PTexture;                     // +0x08
    unsigned short            TLX, TLY;                     // +0x10
    unsigned short            Width;                        // +0x14
    unsigned short            Height;                       // +0x16
    unsigned short            Pitch;                        // +0x18
    unsigned char             Alpha : 1;                    // +0x1a bit 0
    unsigned char             BooleanAlpha : 1;             // bit 1
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;            // bit 4
    void Uninitialise();                                    // 0x009FCBA0
    __forceinline ~CGraphicFrameData()
    {
        if (IsInitialised)
            Uninitialise();
    }
};
struct CGraphicFrame {
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame(const CGraphicFrame& other) : FrameData(other.FrameData)
    {
        if (FrameData)
            ++FrameData->ReferenceCount;
    }
    __forceinline ~CGraphicFrame() { Free(FrameData); }
    static __forceinline void Free(CGraphicFrameData* data)
    {
        if (--data->ReferenceCount == 0) {
            data->~CGraphicFrameData();
            operator delete(data);
        }
    }
    __forceinline void Release()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            Free(data);
            FrameData = 0;
        }
    }
    CGraphicFrameData* operator->() const { return FrameData; }
    bool InitialiseAsSurface(const CCountedPointer<CTexture>& texture, long width, long height, bool alpha, bool booleanAlpha);   // 0x009FCB20
};
struct CManagedTextureOwner;
struct CManagedTextureData {                                // intrusive counted, vptr +0
    virtual void d000();
    virtual void Destroy();                                 // slot 1 (+0x4)
    long                      RefCount;                     // +0x04
    unsigned char             _pad08[0x20 - 0x08];
    CCountedPointer<CTexture> Texture;                      // +0x20
    unsigned char             _pad28[0x30 - 0x28];
    CManagedTextureOwner*     Owner;                        // +0x30
    unsigned char             _pad34[0x49 - 0x34];
    bool                      Locked;                       // +0x49
    void ReleaseTexture();                                  // 0x00A69A80
};
struct CIVCountedPointer {
    CManagedTextureData* Data;
    __forceinline void Clear()
    {
        if (Data) {
            if (--Data->RefCount == 0)
                Data->Destroy();
            Data = 0;
        }
    }
    __forceinline void Assign(CManagedTextureData* data)
    {
        if (Data != data) {
            Clear();
            Data = data;
            if (data)
                ++data->RefCount;
        }
    }
};
struct CBase {
    CBase();                                                // 0x0099A2D0
    virtual ~CBase();                                       // 0x0099A2E0
};
struct CManagedTexture : CBase {                            // vtable 0x0129C880
    CIVCountedPointer Data;                                 // +0x04
    __forceinline CManagedTexture() { Data.Data = 0; }
    __forceinline void Release()
    {
        if (Data.Data) {
            if (Data.Data && Data.Data->RefCount == 2)
                Data.Data->ReleaseTexture();
            Data.Assign(0);
        }
    }
    __forceinline ~CManagedTexture()
    {
        if (Data.Data) {
            if (Data.Data->RefCount == 2)
                Data.Data->ReleaseTexture();
            Data.Clear();
        }
    }
    __forceinline CManagedTexture& operator=(const CManagedTexture& other)
    {
        if (Data.Data != other.Data.Data) {
            Release();
            Data.Assign(other.Data.Data);
        }
        return *this;
    }
};
struct CTextureManager {
    bool AllocateTexture(CManagedTexture& texture, unsigned long width, unsigned long height, long levels,
                         const CPixelFormat& format, CManagedTextureOwner* owner);   // 0x00A69B70
    bool ReduceMipmapLevel(CGraphicFrame& frame, CManagedTexture& texture, unsigned long levels);
};

bool CTextureManager::ReduceMipmapLevel(CGraphicFrame& frame, CManagedTexture& texture, unsigned long levels)
{
    CTexture* oldTexture = frame->PTexture.Ptr;
    long newLevels = oldTexture->GetNoLevels() - levels;
    C2DCoordI size(frame->Width, frame->Height);
    for (unsigned long i = 0; i < levels; ++i) {
        size.X /= 2;
        size.Y /= 2;
    }
    texture.Data.Data->Locked = true;
    CManagedTexture newTexture;
    bool allocated = AllocateTexture(newTexture, size.X, size.Y, newLevels, oldTexture->GetPixelFormat(), texture.Data.Data->Owner);
    if (!allocated) {
        texture.Data.Data->Locked = allocated;
        return false;
    }
    CCountedPointer<CTexture> newPTexture(newTexture.Data.Data->Texture);
    C2DBoxI box(0, 0, size.X, size.Y);
    for (unsigned long level = 0; level < (unsigned long)newLevels; ++level) {
        CSurface dst = newPTexture->GetSurface(level);
        CSurface src = oldTexture->GetSurface(level + levels);
        dst.CopyFromSurface(&box, src, &box, 1);
        box.BRX /= 2;
        box.BRY /= 2;
    }
    CGraphicFrame oldFrame(frame);
    frame.Release();
    frame.InitialiseAsSurface(newPTexture, size.X, size.Y, oldFrame->Alpha, oldFrame->BooleanAlpha);
    texture.Data.Data->Owner = 0;
    texture.Data.Data->Locked = false;
    texture = newTexture;
    return true;
}
