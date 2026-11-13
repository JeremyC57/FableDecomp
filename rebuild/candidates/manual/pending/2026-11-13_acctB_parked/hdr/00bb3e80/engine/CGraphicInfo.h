#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const {return Frames+index;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
    // Opt-in (chunk 2060, row 0x00BA5260): FableWin.pdb ?GetWidth@CGraphicInfo@@QBEKXZ (18 bytes / 9 debug instructions = one
    // movzx of a header word); RenderMeshPrimitive's expanded GetAppropriateMipmap reads the header word +6 (FrameWidth,
    // 0x00BA54EB `movzx edx,word ptr [esi+6]`). Needs the typed header (FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT). FITTED: the PDB has no
    // body; which header word GetWidth returns is taken from the retail read.
    unsigned long GetWidth() const {return Header.FrameWidth;}
#endif
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
