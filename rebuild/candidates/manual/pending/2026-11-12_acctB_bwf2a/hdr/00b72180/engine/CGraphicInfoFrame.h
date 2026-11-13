#ifndef FABLE_ENGINE_CGRAPHICINFOFRAME_H
#define FABLE_ENGINE_CGRAPHICINFOFRAME_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
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
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
    // Opt-in (chunk 2048): Ego_r.pdb / FableWin.pdb ?BeginLoading@CGraphicInfoFrame@@QBEXKK@Z 0x009FE620 (const), and the inline
    // loaded test RenderSunFlare expands (`mov eax,[f+8] ; test ; mov edi,[eax+1Ch] ; test ; setne`): Data set and its frame initialised.
    void BeginLoading(unsigned long mipmap_level, unsigned long flags) const;   // 0x009FE620
    bool IsFrameDataLoaded() const;   // descriptive name (no PDB name found): Data set and its frame initialised
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
    // Retail opt-in (chunk 2031, rows 0x00B54E30 RenderSnow / 0x00B65A20 RenderOuterSky): FableWin.pdb
    // ?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z (no Ego_r.pdb copy, no retail out-of-line copy; expanded in both rows).
    // The body is in engine/CGraphicInfo.h (it needs the complete CGraphicInfo / CGraphicDataBank).
    bool GetFrame(CGraphicFrame& frame, unsigned long mipmap_level, unsigned long flags) const;
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
