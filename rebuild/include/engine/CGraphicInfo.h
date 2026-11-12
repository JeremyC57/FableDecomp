#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const {return Frames+index;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
    // Retail opt-in (wf8, row 0x00BA4A40): FableWin.pdb ?GetTextureType@CGraphicInfo@@QBE?AW4ETextureType@CGraphicDataBank@@XZ
    // (inline in retail: +0x1C read in place, 0x00BA4B1C). No layout change.
    CGraphicDataBank::ETextureType GetTextureType() const {return TextureType;}
#endif
#ifdef FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
    CGraphicInfo(unsigned long index,CGraphicDataBank* bank,const CGraphicDataBank::CGraphicHeader& header);
    bool IsBump() const {return Bank->GetEntryDataType(Index)==2;}
private:
#endif
#if defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE) || defined(FABLE_RETAIL_CGRAPHICINFO_SPRITE)
private:
    friend class CGraphicInfoFrame;
#endif
    CGraphicDataBank* GetBank() const {return Bank;}
private:
#if defined(FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT) || defined(FABLE_RETAIL_CGRAPHICINFOFRAME_FREE) || defined(FABLE_RETAIL_CGRAPHICINFO_SPRITE)
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
#ifdef FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
// FableWin.pdb ?GetTextureType@CGraphicInfoFrame@@QBE?AW4ETextureType@CGraphicDataBank@@XZ (inline: GraphicInfo +0xC, then +0x1C).
inline CGraphicDataBank::ETextureType CGraphicInfoFrame::GetTextureType() const {return GraphicInfo->GetTextureType();}
#endif
#endif
#endif
