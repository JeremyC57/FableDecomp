#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#if defined(FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION) && !defined(FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS)
// Retail opt-in (chunk 2018, row 0x00B2D330; with engine/CGraphicDataBank.h's SURFACECOMPOSITION branch): Ego_r.pdb CGraphicInfo (44 bytes:
// Header +0x00, TextureType +0x1C, Bank +0x20, Index +0x24, Frames +0x28) with the inline FableWin.pdb accessors
// ?PeekInfoFrame@CGraphicInfo@@QBEPBVCGraphicInfoFrame@@K@Z (0x00B2D384 `mov edi,[esi+28h]`: Frames + index) and
// ?PeekGraphicHeader@CGraphicInfo@@QBEABVCGraphicHeader@CGraphicDataBank@@XZ, and the inline CGraphicDataBank::PeekGraphic body.
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const { return Frames + index; }
    const CGraphicDataBank::CGraphicHeader& PeekGraphicHeader() const { return Header; }
private:
    CGraphicDataBank::CGraphicHeader Header;                 // +0x00
    CGraphicDataBank::ETextureType   TextureType;            // +0x1C
    CGraphicDataBank*                Bank;                   // +0x20
    long                             Index;                  // +0x24
    CGraphicInfoFrame*               Frames;                 // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfo)==44);
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
// FableWin.pdb ?IsAlpha@CGraphicInfoFrame@@QBE_NXZ (inline in retail, 0x00B2D524..0x00B2D52A: the owner graphic's header TransparencyType
// byte tested for zero).
// FableWin.pdb ?GetWidth@ / ?GetHeight@CGraphicInfoFrame@@QBEKXZ (inline in retail: GetDimensionsAndAdjustMipmapLevel 0x00B2AEF0 reads the owner
// graphic's header FrameWidth +6 / FrameHeight +8 as `movzx`).
inline unsigned long CGraphicInfoFrame::GetWidth() const { return GraphicInfo->PeekGraphicHeader().FrameWidth; }
inline unsigned long CGraphicInfoFrame::GetHeight() const { return GraphicInfo->PeekGraphicHeader().FrameHeight; }
inline bool CGraphicInfoFrame::IsAlpha() const { return GraphicInfo->PeekGraphicHeader().TransparencyType != 0; }
#elif defined(FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS)
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const {return Frames+index;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
    CGraphicInfo(unsigned long index,CGraphicDataBank* bank,const CGraphicDataBank::CGraphicHeader& header);
    bool IsBump() const {return Bank->GetEntryDataType(Index)==2;}
private:
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
private:
    friend class CGraphicInfoFrame;
#endif
    CGraphicDataBank* GetBank() const {return Bank;}
private:
#if defined(FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT) || defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE)
    CGraphicDataBank::CGraphicHeader Header;
#else
    unsigned char Header[28]; // complete CGraphicDataBank::CGraphicHeader storage
#endif
    CGraphicDataBank::ETextureType TextureType;
    CGraphicDataBank* Bank;
    long Index;
    CGraphicInfoFrame* Frames;
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfo)==44);
#endif
#endif
