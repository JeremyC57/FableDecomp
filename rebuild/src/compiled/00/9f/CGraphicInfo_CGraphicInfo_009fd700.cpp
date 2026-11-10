// CGraphicInfo::CGraphicInfo(unsigned long,CGraphicDataBank*,const CGraphicDataBank::CGraphicHeader&) -- initialize bank graphic metadata and texture type.
// Retail: 0x009fd700,126 bytes,thiscall.
// Leaf: ??0CGraphicInfo@@QAE@KPAVCGraphicDataBank@@ABVCGraphicHeader@1@@Z
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#include "engine/CGraphicInfo.h"
CGraphicInfo::CGraphicInfo(unsigned long index,CGraphicDataBank* bank,const CGraphicDataBank::CGraphicHeader& header)
 :Bank(bank),Index(index),Frames(0) {
 Header=header;
 TextureType=GetBank()->GetTextureType(Header,IsBump());
}