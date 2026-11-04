#ifndef FABLE_ENGINE_CGRAPHICINFOFRAME_H
#define FABLE_ENGINE_CGRAPHICINFOFRAME_H
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
#endif // FABLE_ENGINE_CGRAPHICINFOFRAME_H
