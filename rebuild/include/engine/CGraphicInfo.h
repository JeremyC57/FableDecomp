#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfoFrame.h"
class CGraphicInfo {
public:
    const CGraphicInfoFrame* PeekInfoFrame(unsigned long index) const {return Frames+index;}
    CGraphicDataBank* GetBank() const {return Bank;}
private:
    unsigned char Header[28]; // complete CGraphicDataBank::CGraphicHeader storage
    CGraphicDataBank::ETextureType TextureType;
    CGraphicDataBank* Bank;
    long Index;
    CGraphicInfoFrame* Frames;
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfo)==44);
#endif
#endif
