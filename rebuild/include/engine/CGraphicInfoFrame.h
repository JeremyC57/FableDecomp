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
#ifdef FABLE_RETAIL_CGRAPHICINFO_FRAMESECTIONS_PASS27
class CManagedTexture;
#endif
class CGraphicInfo; class CBankFileAsyncData;
class CGraphicInfoFrame : public CBaseClass {
public:
#ifdef FABLE_RETAIL_CGRAPHICINFO_FRAMESECTIONS_PASS27
private:
    friend class CGraphicInfo;
    void InitialiseFrame(CGraphicFrame&, CManagedTexture&, signed char);
public:
#endif
    class CFrameData;
    bool IsAlpha() const;
    unsigned long GetWidth() const;                          // FableWin.pdb ?GetWidth@CGraphicInfoFrame@@QBEKXZ (inline)
    unsigned long GetHeight() const;                         // FableWin.pdb ?GetHeight@CGraphicInfoFrame@@QBEKXZ (inline)
    bool IsLoaded() const;
    long GetCurrentLoadedMipmapLevel() const;
    bool GetFrame(CGraphicFrame& frame, unsigned long mipmap_level, unsigned long flags) const;
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_GETVOLUMETEXTURE
    // NF39 opt-in (row 0x009FFB20 CGraphicInfo::GetVolumeTexture, the Frames forwarder): Ego_r.pdb 0x00B5CAA0 / FableWin.pdb 0x03126C40
    // ?GetVolumeTexture@CGraphicInfoFrame@@QBE_NPAPAVCVolumeTexture@@K@Z, out of line in retail at 0x009FF710. Declaration only.
    bool GetVolumeTexture(class CVolumeTexture** texture, unsigned long level) const;   // 0x009FF710
#endif
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;      // 0x009FF6B0
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_GETDATA
    // NF79 opt-in (round 7, row 0x00A00D90): FableWin.pdb ?GetData@CGraphicInfoFrame@@ABEPAVCFrameData@1@XZ (private const), out of line in
    // retail at 0x00A00D90 (creates the frame data on first use through the mutable Data, as GetFrame below does, and returns it).
    // Declaration only, no layout change.
    CFrameData* GetData() const;                                         // 0x00A00D90
#endif
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
FABLE_STATIC_ASSERT_AT(67, sizeof(CGraphicInfoFrame)==0x10);
FABLE_STATIC_ASSERT_AT(68, sizeof(CGraphicInfoFrame::CFrameData)==0x44);
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
#ifdef FABLE_RETAIL_CGRAPHICINFO_SPRITE
    unsigned long GetWidth()const;
    unsigned long GetHeight()const;
#ifdef FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
    // Retail opt-in (Codex pass22): original FableWin0263de40/0263dc90 and native header TransparencyType tests.
    bool IsBooleanAlpha() const;
    bool IsAlpha() const;
#endif
#endif
    bool GetTexture(const CTexture**,unsigned long,unsigned long) const;
    const CTexture* GetBlankTexture() const; // native009fd3f0
    void TouchFrame(unsigned long) const;
#ifdef FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
    // Retail opt-in (wf8, row 0x00BA4A40): FableWin.pdb ?GetTextureType@CGraphicInfoFrame@@QBE?AW4ETextureType@CGraphicDataBank@@XZ;
    // the inline body is in engine/CGraphicInfo.h (needs the complete CGraphicInfo). No layout change.
    CGraphicDataBank::ETextureType GetTextureType() const;
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
    // Retail opt-in (chunk 2031, rows 0x00B54E30 RenderSnow / 0x00B65A20 RenderOuterSky): FableWin.pdb
    // ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z (no Ego_r.pdb copy, no retail out-of-line copy; expanded in both rows).
    // The body is in engine/CGraphicInfo.h (it needs the complete CGraphicInfo / CGraphicDataBank).
    bool GetFrame(CGraphicFrame& frame, unsigned long mipmap_level, unsigned long flags) const;
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_ISLOADING_PASS30
    bool IsLoading() const;
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
    // Opt-in (chunk 2048): Ego_r.pdb / FableWin.pdb ?BeginLoading@CGraphicInfoFrame@@QBEXKK@Z 0x009FE620 (const), and the inline
    // loaded test RenderSunFlare expands (`mov eax,[f+8] ; test ; mov edi,[eax+1Ch] ; test ; setne`): Data set and its frame initialised.
    void BeginLoading(unsigned long mipmap_level, unsigned long flags) const;   // 0x009FE620
    bool IsFrameDataLoaded() const;   // descriptive name (no PDB name found): Data set and its frame initialised
#ifdef FABLE_RETAIL_CENGINESKYRENDERER_MOON_PASS30
    bool IsLoaded() const { return IsFrameDataLoaded(); }     // FableWin 02D477A0, same original test.
#endif
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_GETVOLUMETEXTURE
    // NF39 opt-in (row 0x009FFB20 CGraphicInfo::GetVolumeTexture, the Frames forwarder): Ego_r.pdb 0x00B5CAA0 / FableWin.pdb 0x03126C40
    // ?GetVolumeTexture@CGraphicInfoFrame@@QBE_NPAPAVCVolumeTexture@@K@Z, out of line in retail at 0x009FF710. Declaration only.
    bool GetVolumeTexture(class CVolumeTexture** texture, unsigned long level) const;   // 0x009FF710
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
    bool IsFlatSequence() const;
#endif
private:
#if defined(FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME) && !defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE)
    CGraphicDataBank* GetBank() const;                                  // FableWin.pdb ?GetBank@CGraphicInfoFrame@@ABEPAVCGraphicDataBank@@XZ
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
    friend class CFrameData;
    CGraphicDataBank* GetBank() const;
    void ReduceMipmapLevel(CGraphicFrame&,CManagedTexture&,unsigned long) const;
    void ReleaseData() const;
#endif
#ifdef FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
    // Retail opt-in (wf12, row 0x009FF220): FableWin.pdb ?IsVolumeTexture@CGraphicInfoFrame@@ABE_NXZ,
    // ?UploadToVolumeTexture@CGraphicInfoFrame@@ABE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@K@Z and
    // ?UploadToTexture@CGraphicInfoFrame@@ABE_NAAVCGraphicFrame@@AAVCManagedTexture@@AAVCBankFileAsyncData@@K_N@Z (inline in the row).
    bool IsVolumeTexture() const;
    bool UploadToVolumeTexture(CManagedTexture&,CBankFileAsyncData&,unsigned long) const;
    bool UploadToTexture(CGraphicFrame&,CManagedTexture&,CBankFileAsyncData&,unsigned long,bool) const;
#endif
    bool InternalGetFrameForcedLoading(unsigned long) const; // native009ff6b0
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_GETINDEX
    // agentC27 (row 0x009FDE80): FableWin.pdb ?GetIndex@CGraphicInfoFrame@@ABEKXZ (private, inline: the graphic's bank index). No layout change.
    unsigned long GetIndex() const;
#endif
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
#if defined(FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING) && !defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE)
    // Ego_r.pdb / FableWin.pdb ?IsLoaded@CFrameData@CGraphicInfoFrame@@QAE_NXZ (the FREE branch's body).
    bool IsLoaded() {return Frame.IsInitialised();}
#endif
#ifdef FABLE_RETAIL_CFRAMEDATA_ISLOADING
    // FableWin.pdb ?IsLoading@CFrameData@CGraphicInfoFrame@@QBE_NXZ (14 debug instructions; listed in GetFrame's COMDAT run).
#ifdef FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
    // Nested opt-in (chunk 2048): FableWin.pdb ?IsLoading@CFrameData@CGraphicInfoFrame@@QBE_NXZ is 14 instructions / 32 bytes =
    // `!FileData.IsNull()` (FableWin.pdb debug size reproduced by plain VC7.1 /Od of this text: /opt/fable_lane/tools/fwsize.py); `FileData.Get() != 0` is 13 / 28.
    bool IsLoading() const { return !FileData.IsNull(); }
#else
    bool IsLoading() const { return FileData.Get() != 0; }
#endif
#endif
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
#ifdef FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
    bool UploadFrame(bool force);                            // 0x009FF220
#endif
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
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_ISLOADING_PASS30
// FableWin 0x02C93E80 tests Data.Get() before consulting CFrameData.
inline bool CGraphicInfoFrame::IsLoading() const
{
    return Data.Get() ? Data->IsLoading() : false;
}
#endif
FABLE_STATIC_ASSERT_AT(218, sizeof(CGraphicInfoFrame)==0x10);
FABLE_STATIC_ASSERT_AT(219, sizeof(CGraphicInfoFrame::CFrameData)==0x44);

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
FABLE_STATIC_ASSERT_AT(256, sizeof(CGraphicInfoFrame) == 0x10);
FABLE_STATIC_ASSERT_AT(257, sizeof(CGraphicInfoFrame::CFrameData) == 0x44);
#endif
#endif // FABLE_ENGINE_CGRAPHICINFOFRAME_H
