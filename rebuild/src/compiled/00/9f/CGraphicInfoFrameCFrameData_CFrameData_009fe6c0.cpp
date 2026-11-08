// CGraphicInfoFrame::CFrameData::~CFrameData() -- graphic frame data destructor: reset the texture (release the managed texture, drop the shared graphic frame, reset the mipmap levels), then the members (file data counted pointer, both managed textures, the graphic frame) and ~CResource
// Leaf: ??1CFrameData@CGraphicInfoFrame@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FE6C0, 182 bytes, thiscall, `jmp` tail to ~CResource.
// Identity (proven): the first store is vtable 0x0129C89C, RTTI .?AVCFrameData@CGraphicInfoFrame@@ (CFrameData :
//   CManagedTextureOwner : CResource : CIVCountedPointeeBase, ghidra_out/rtti_bases.tsv); its slot 0 is the scalar deleting
//   destructor 0x00A010A0, this body's only caller; Ego_r.pdb ??1CFrameData@CGraphicInfoFrame@@UAE@XZ. Members are Ego_r's
//   CGraphicInfoFrame::CFrameData at their retail offsets (no STL members): Frame +0x1c, ManagedTexture +0x20,
//   LoadingManagedTexture +0x28, FileData +0x30, CurrentMipmapLevel / LoadingMipmapLevel / TouchedMipmapLevel +0x38..+0x3a,
//   TouchedFrame +0x3c, GraphicInfoFrame +0x40 (the landed ctor 0x009FE380 initialises the same offsets).
// Callees: CManagedTexture::Release 0x00A69AD0, CGraphicFrame::UnInitialise 0x00A00B90 (Ego_r ?UnInitialise@CGraphicFrame@@QAEXXZ),
//   CManagedTexture::~CManagedTexture 0x00A69E20 (x2), CGraphicFrameData::Uninitialise 0x009FCBA0, operator delete 0x00BFE9BC,
//   ~CResource 0x009FC550; the count blocks' DeleteFunc (indirect).
// Shape: user dtor `ResetTexture();` -- the same-TU member 0x009FE440 (Ego_r ?ResetTexture@CFrameData@CGraphicInfoFrame@@QAEXXZ:
//   ManagedTexture.Release(); Frame.UnInitialise(); CurrentMipmapLevel = -1; TouchedMipmapLevel = 127;), auto-inlined; then
//   the member dtors in reverse: CCountedPointer<CBankFileAsyncData> (engine/CCountedPointer.h), the two ~CManagedTexture calls,
//   the forced-inline ~CGraphicFrame (see 0x00B2B910) sharing the zero register.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"                            // : CIVCountedPointeeBase; ~CResource 0x009FC550
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"                      // CCountedPointer<CTexture> (count block delete 0x00BFE9BC)
void __cdecl operator delete(void* block);               // 0x00BFE9BC
class CTexture;
class CManagedTextureData;
template <class T> class CIVCountedPointer { public:
    ~CIVCountedPointer();
    T* Data;
};
class CManagedTexture : public CBaseClass {              // RTTI CManagedTexture : CBaseClass; vtable 0x0129C880
public:
    virtual ~CManagedTexture();                          // 0x00A69E20
    bool Release();                                      // 0x00A69AD0
    CIVCountedPointer<CManagedTextureData> Data;         // +0x04
};
class CGraphicFrameData {                                // Ego_r.pdb, 27 bytes
public:
    ~CGraphicFrameData()
    {
        if (IsInitialised)
            Uninitialise();
    }
    void Uninitialise();                                 // 0x009FCBA0
    unsigned long             ReferenceCount;            // +0x00
    void*                     Data;                      // +0x04
    CCountedPointer<CTexture> PTexture;                  // +0x08
    unsigned short            TLX;                       // +0x10
    unsigned short            TLY;                       // +0x12
    unsigned short            Width;                     // +0x14
    unsigned short            Height;                    // +0x16
    unsigned short            Pitch;                     // +0x18
    unsigned char             Alpha : 1;                 // +0x1a
    unsigned char             BooleanAlpha : 1;
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;
    unsigned char             ScreenGraphic : 1;
};
class CGraphicFrame {                                    // Ego_r.pdb, 4 bytes (engine/CGraphicFrame.h layout, no methods there)
    // ~CGraphicFrame: no out-of-line copy in Ego_r / FableWin (no ??1CGraphicFrame) and every retail use is expanded, incl.
    // ??_GCGraphicFrame 0x00B2ED60; plain `inline` (with or without the UnInitialise call, `delete data` or the explicit
    // dtor + operator delete) leaves a call in ~CCompositeSurfaceCache 0x00B2B910 (that row's probes 3-5, 7, 9-11), so it is __forceinline like the other Lionhead
    // header release helpers (EXTRA_RULES owner decision 2026-11-01).
public:
    FABLE_FORCEINLINE ~CGraphicFrame()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            if (--data->ReferenceCount == 0) {
                data->~CGraphicFrameData();
                operator delete(data);
            }
            FrameData = 0;
        }
    }
    void UnInitialise();                                 // 0x00A00B90 (Ego_r ?UnInitialise@CGraphicFrame@@QAEXXZ)
    CGraphicFrameData* FrameData;                        // +0x00
};
class CBankFileAsyncData;
class CGraphicInfo;
class CManagedTextureOwner : public CResource {          // vtable 0x0129C848; implicit dtor
public:
    virtual void ReleaseTexture() = 0;                   // slot 10
};
class CGraphicInfoFrame {                                // Ego_r.pdb (members not touched here)
public:
    class CFrameData;
    long           Reserved;
    unsigned short FrameNum;
    signed char    SmallMipmapLevel;
    CFrameData*    Data;
    CGraphicInfo*  GraphicInfo;
};
class CGraphicInfoFrame::CFrameData : public CManagedTextureOwner {   // vtable 0x0129C89C
public:
    CFrameData(const CGraphicInfoFrame* frame);          // 0x009FE380
    virtual ~CFrameData();
    virtual void ReleaseTexture();                       // slot 10 0x009FEA00
    void ResetTexture();                                 // 0x009FE440
    CGraphicFrame                       Frame;                   // +0x1c
    CManagedTexture                     ManagedTexture;          // +0x20
    CManagedTexture                     LoadingManagedTexture;   // +0x28
    CCountedPointer<CBankFileAsyncData> FileData;                // +0x30
    signed char                         CurrentMipmapLevel;      // +0x38
    signed char                         LoadingMipmapLevel;      // +0x39
    signed char                         TouchedMipmapLevel;      // +0x3a
    unsigned long                       TouchedFrame;            // +0x3c
    const CGraphicInfoFrame*            GraphicInfoFrame;        // +0x40
};
void CGraphicInfoFrame::CFrameData::ResetTexture()
{
    ManagedTexture.Release();
    Frame.UnInitialise();
    CurrentMipmapLevel = -1;
    TouchedMipmapLevel = 127;
}
CGraphicInfoFrame::CFrameData::~CFrameData()
{
    ResetTexture();
}