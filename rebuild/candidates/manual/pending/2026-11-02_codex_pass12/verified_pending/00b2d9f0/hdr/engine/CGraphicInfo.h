#ifndef FABLE_ENGINE_CGRAPHICINFO_H
#define FABLE_ENGINE_CGRAPHICINFO_H
#include "engine/CGraphicDataBank.h"
class CGraphicInfoFrame;
// Ego_r original2c-byte class, native header+0/type+1c/bank+20/index+24.
class CGraphicInfo {
public:
#ifdef FABLE_RETAIL_TEXTURE_CACHE
 bool IsBump()const{return Bank->GetEntryDataType(Index)==2;}
#endif
 const CGraphicDataBank::CGraphicHeader& PeekGraphicHeader() const {return Header;}
 CGraphicDataBank::ETextureType GetTextureType() const {return TextureType;}
private:
 friend class CGraphicInfoFrame;
 CGraphicDataBank* GetBank() const {return Bank;}
 unsigned long GetIndex() const {return Index;}
 CGraphicDataBank::CGraphicHeader Header;
 CGraphicDataBank::ETextureType TextureType;
 CGraphicDataBank* Bank;
 long Index;
 CGraphicInfoFrame* Frames;
};
FABLE_STATIC_ASSERT(sizeof(CGraphicInfo)==0x2c);
#ifdef FABLE_RETAIL_TEXTURE_CACHE
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index)const{return index<GraphicList.size()?&GraphicList[index]:0;}
#endif
#endif
