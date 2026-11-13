#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const {return Frames+index;}
#ifdef FABLE_RETAIL_CGRAPHICINFO_GETVOLUMETEXTURE
    // Retail opt-in (chunk 2008, row 0x00B54890): Ego_r.pdb 0x00B5D250 ?GetVolumeTexture@CGraphicInfo@@QBE_NPAPAVCVolumeTexture@@K@Z, out of line
    // in retail at 0x009FFB20 (`mov ecx,[ecx+28h] ; jmp 0x009FF710`, the Frames forwarder to CGraphicInfoFrame::GetVolumeTexture). Declaration only.
    bool GetVolumeTexture(class CVolumeTexture** texture, unsigned long level) const;   // 0x009FFB20
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
