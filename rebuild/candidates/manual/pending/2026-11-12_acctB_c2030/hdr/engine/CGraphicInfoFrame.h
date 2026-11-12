#ifndef FABLE_ENGINE_CGRAPHICINFOFRAME_H
#define FABLE_ENGINE_CGRAPHICINFOFRAME_H
#if defined(FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION) && !defined(FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS)
// Retail opt-in (chunk 2018, row 0x00B2D330 CEngineSurfaceCompositionManager::UpdateDiskCacheTexture): the retail classes (Ego_r.pdb layouts;
// CGraphicInfoFrame : CBaseClass 0x10 {FrameNum +4, SmallMipmapLevel +6, mutable Data +8, GraphicInfo +0xC}; CFrameData : CManagedTextureOwner
// 0x44 {Frame +0x1C, ManagedTexture +0x20, LoadingManagedTexture +0x28, FileData +0x30, CurrentMipmapLevel +0x38 .. GraphicInfoFrame +0x40})
// with the inline frame-fetch helpers UpdateDiskCacheTexture expands per layer (0x00B2D6D9..0x00B2D8BB), FableWin.pdb spellings with no
// out-of-line retail call: ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z, ?IsLoaded@CGraphicInfoFrame@@QBE_NXZ,
// ?GetCurrentLoadedMipmapLevel@CGraphicInfoFrame@@QBEJXZ and ?IsAlpha@CGraphicInfoFrame@@QBE_NXZ (defined in engine/CGraphicInfo.h); GetFrame
// spells the create-and-load in place, as the landed GetTexture 0x00B66C10 does.
// Out of line: InternalGetFrameForcedLoading 0x009FF6B0 (private const). The bodies of the inline helpers are inferred from the retail
// expansion (FableWin names only).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#endif
#ifndef FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#endif
#ifndef FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME
#endif
#ifndef FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#endif
#include "fable_compiler.h"
#include "engine/CBaseClass.h"
#include "engine/CManagedTextureOwner.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CGraphicFrame.h"
class CGraphicInfo; class CBankFileAsyncData;
class CGraphicInfoFrame : public CBaseClass {
public:
    class CFrameData;
    bool IsAlpha() const;
    unsigned long GetWidth() const;                          // FableWin.pdb ?GetWidth@CGraphicInfoFrame@@QBEKXZ (inline)
    unsigned long GetHeight() const;                         // FableWin.pdb ?GetHeight@CGraphicInfoFrame@@QBEKXZ (inline)
    bool IsLoaded() const;
    long GetCurrentLoadedMipmapLevel() const;
    bool GetFrame(CGraphicFrame& frame, unsigned long mipmap_level, unsigned long flags) const;
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;      // 0x009FF6B0
    unsigned short FrameNum;                                 // +0x04
    signed char SmallMipmapLevel;                            // +0x06
    mutable CIVCountedPointer<CFrameData> Data;              // +0x08
    CGraphicInfo* GraphicInfo;                               // +0x0C
};
class CGraphicInfoFrame::CFrameData : public CManagedTextureOwner {
public:
    CFrameData(const CGraphicInfoFrame* info_frame);         // 0x009FE380
    virtual ~CFrameData();
    void BeginLoading(unsigned long mipmap_level, unsigned long flags);   // 0x009FDE80
    // Ego_r.pdb CFrameData fields are public (the landed CGraphicInfoFrame::GetTexture 0x00B66C10 model).
    CGraphicFrame Frame;                                     // +0x1C
    unsigned char ManagedTexture[0x8];                       // +0x20 CManagedTexture (not modelled)
    unsigned char LoadingManagedTexture[0x8];                // +0x28 CManagedTexture (not modelled)
    CCountedPointer<CBankFileAsyncData> FileData;            // +0x30
    signed char CurrentMipmapLevel;                          // +0x38
    signed char LoadingMipmapLevel;                          // +0x39
    signed char TouchedMipmapLevel;                          // +0x3A
    unsigned long TouchedFrame;                              // +0x3C
    const CGraphicInfoFrame* GraphicInfoFrame;               // +0x40
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame)==0x10);
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame::CFrameData)==0x44);
inline bool CGraphicInfoFrame::IsLoaded() const { return !Data.IsNull() && Data->Frame.IsInitialised(); }
inline long CGraphicInfoFrame::GetCurrentLoadedMipmapLevel() const { return !Data.IsNull() ? Data->CurrentMipmapLevel : -1; }
// FableWin.pdb ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z: no Ego_r.pdb copy and no retail out-of-line copy, expanded in its
// callers (UpdateDiskCacheTexture 0x00B2D6DB..0x00B2D8A9 with flags 0x19) -- FABLE_FORCEINLINE per the owner's 2026-11-01 rule for
// Lionhead header helpers (plain `inline` leaves it called: c1 188, score 142 at f 3.75). The body is the landed out-of-line sibling
// CGraphicInfoFrame::GetTexture 0x00B66C10 with the frame copied instead of its texture: a reload (BeginLoading) of a finer level, the
// wait-for-load (flags & 0x10) and no-create (flags & 4) bits, and the forced load under flags & 1. GetTexture's resource-touch block
// under !(flags & 8) is not modelled (dead for the only caller here, flags 0x19; it needs the CManagedTexture / CResourceList models).
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetFrame(CGraphicFrame& frame, unsigned long mipmap_level, unsigned long flags) const
{
    if (!Data.IsNull()) {
        if (Data->Frame.IsInitialised()) {
            if (mipmap_level < (unsigned long)Data->CurrentMipmapLevel) {
                Data->BeginLoading(mipmap_level, flags);
                if ((flags & 16) && Data->FileData.Get())
                    goto load;
                frame = Data->Frame;
                return true;
            }
            frame = Data->Frame;
            return true;
        }
load:
        Data->BeginLoading(mipmap_level, flags);
    } else {
        if (flags & 4)
            return false;
        Data.Reset(new CFrameData(this));
        Data->BeginLoading(mipmap_level, flags);
    }
    if ((flags & 1) && InternalGetFrameForcedLoading(flags)) {
        frame = Data->Frame;
        return true;
    }
    return false;
}
#elif defined(FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS)
#include "engine/CBaseClass.h"
#include "engine/CManagedTextureOwner.h"
#include "engine/CManagedTexture.h"
#include "engine/CGraphicFrame.h"
#include "engine/CCountedPointer.h"
class CGraphicInfo;class CGraphicDataBank;class CBankFileAsyncData;
class CGraphicInfoFrame : public CBaseClass {
public:
    class CFrameData;
    bool GetTexture(const CTexture**,unsigned long,unsigned long) const;
    const CTexture* GetBlankTexture() const; // native009fd3f0
    void TouchFrame(unsigned long) const;
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
    bool IsFlatSequence() const;
#endif
private:
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
    friend class CFrameData;
    CGraphicDataBank* GetBank() const;
    void ReduceMipmapLevel(CGraphicFrame&,CManagedTexture&,unsigned long) const;
    void ReleaseData() const;
#endif
    bool InternalGetFrameForcedLoading(unsigned long) const; // native009ff6b0
    unsigned short FrameNum;
    signed char SmallMipmapLevel;
    mutable CIVCountedPointer<CFrameData> Data;
    CGraphicInfo* GraphicInfo;
};
class CGraphicInfoFrame::CFrameData : public CManagedTextureOwner {
public:
    virtual ~CFrameData();
    virtual bool IsResourceLoaded() const;
    virtual void OnResourceLoaded();
    virtual void ReleaseTexture();
    virtual unsigned long GetUnloadableMemory() const;
    virtual unsigned long GetDebugMemoryInUse() const;
    virtual void ResetDebugMemoryInUse();
public:
    CFrameData(const CGraphicInfoFrame*); // native009fe380
    void BeginLoading(unsigned long,unsigned long); // native009fde80
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
    bool FreeTextureMemory(bool allow_unload);
    bool IsLoaded() {return Frame.IsInitialised();}
    CGraphicDataBank* GetBank() const {return GraphicInfoFrame->GetBank();}
    unsigned long Sizeof() const {
        unsigned long size=sizeof(*this);
        size+=Frame.Sizeof();
        if(LoadingManagedTexture.IsValid())size+=LoadingManagedTexture.Sizeof();
        return size;
    }
    void ResetTexture();
    // Ego_r c5c8 fields are public; no layout change.
#else
private:
#endif
    friend class CGraphicInfoFrame;
    CGraphicFrame Frame;
    CManagedTexture ManagedTexture,LoadingManagedTexture;
    CCountedPointer<CBankFileAsyncData> FileData;
    signed char CurrentMipmapLevel,LoadingMipmapLevel,TouchedMipmapLevel;
    unsigned long TouchedFrame;
    const CGraphicInfoFrame* GraphicInfoFrame;
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame)==0x10);
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame::CFrameData)==0x44);

#else
// New header (chunk 767, row 0x009FD4D0 CGraphicInfoFrame::CFrameData::ResetDebugMemoryInUse). Retail RTTI .?AVCGraphicInfoFrame@@ :
// CBaseClass and .?AVCFrameData@CGraphicInfoFrame@@ : CManagedTextureOwner : CResource : CIVCountedPointeeBase (ghidra_out/rtti_bases.tsv;
// CFrameData vtable 0x0129C89C). Members: Ego_r.pdb (struct_layouts_egor.tsv), CGraphicInfoFrame 0x10 {vptr, FrameNum +4,
// SmallMipmapLevel +6, Data +8, GraphicInfo +0xC}; CFrameData 0x44 {Frame +0x1C, ManagedTexture +0x20, LoadingManagedTexture +0x28,
// FileData +0x30, CurrentMipmapLevel +0x38, LoadingMipmapLevel +0x39, TouchedMipmapLevel +0x3A, TouchedFrame +0x3C,
// GraphicInfoFrame +0x40}; composite members opaque. Needs FABLE_RETAIL_DEF_CLASS_BASE. Only what rows use is declared: the
// CFrameData override of CResource slot 9, ?ResetDebugMemoryInUse@CFrameData@CGraphicInfoFrame@@UAEXXZ (0x009FD4D0).
#include "rebuild_abi.h"
#include "engine/CManagedTextureOwner.h"
class CGraphicInfo;
class CGraphicInfoFrame {
public:
    class CFrameData;
private:
    void*          __vftable;                                // +0x00 CBaseClass
    unsigned short FrameNum;                                 // +0x04
    signed char    SmallMipmapLevel;                         // +0x06
    unsigned char  Data[0x4];                                // +0x08 CIVCountedPointer<CGraphicInfoFrame::CFrameData>
    CGraphicInfo*  GraphicInfo;                              // +0x0c
};
class CGraphicInfoFrame::CFrameData : public CManagedTextureOwner {
public:
    virtual void ResetDebugMemoryInUse();                    // slot 9 0x009FD4D0
private:
    unsigned char      Frame[0x4];                           // +0x1c CGraphicFrame
    unsigned char      ManagedTexture[0x8];                  // +0x20 CManagedTexture
    unsigned char      LoadingManagedTexture[0x8];           // +0x28 CManagedTexture
    unsigned char      FileData[0x8];                        // +0x30 CCountedPointer<CBankFileAsyncData>
    signed char        CurrentMipmapLevel;                   // +0x38
    signed char        LoadingMipmapLevel;                   // +0x39
    signed char        TouchedMipmapLevel;                   // +0x3a
    unsigned long      TouchedFrame;                         // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;                     // +0x40
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame::CFrameData) == 0x44);
#endif
#endif // FABLE_ENGINE_CGRAPHICINFOFRAME_H
