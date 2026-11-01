// 009a4160 CStreamingFontBank::FindGlyph
// Leaf: ?FindGlyph@CStreamingFontBank@@IBEPBVCStreamingGlyph@@G@Z
// Flags:
#define CStreamingFontBank CStreamingFontBankFields
#include "engine/CStreamingFontBank.h"
#undef CStreamingFontBank
class CStreamingGlyph{public:unsigned long GlyphIndex:24;unsigned long AdjustmentIndex:8;};
extern "C" int __cdecl lzo1x_decompress_safe(const unsigned char*,unsigned int,unsigned char*,unsigned int*,void*);
// Layout: supplied flat font bank; original protected nested GlyphBank is260 bytes.
class CStreamingFontBank:public CStreamingFontBankFields{
protected:
 struct GlyphBank{unsigned short FirstGlyph,NoGlyphs;CStreamingGlyph Glyphs[64];};
 const CStreamingGlyph*FindGlyph(unsigned short)const;
};
const CStreamingGlyph*CStreamingFontBank::FindGlyph(unsigned short character)const{
 if(character==0xa0)character=0x20;
 GlyphBank*bank=reinterpret_cast<GlyphBank*>(glyphBankCache);
 unsigned long index=character;
 unsigned long bankIndex=index>>6;
 if(glyphBankCacheIndex!=bankIndex){
  const unsigned short*packed=static_cast<const unsigned short*>(GlyphBanks[bankIndex]);
  if(!packed)return 0;
  if(reinterpret_cast<const unsigned char*>(packed)[1]&0x80){
   unsigned int size=0x104;
   lzo1x_decompress_safe(reinterpret_cast<const unsigned char*>(packed+1),*packed&0x7fff,reinterpret_cast<unsigned char*>(bank),&size,0);
   const_cast<CStreamingFontBank*>(this)->glyphBankCacheIndex=bankIndex;
  }else bank=reinterpret_cast<GlyphBank*>(const_cast<unsigned short*>(packed+1));
 }
 unsigned long slot=(index&0x3f)-bank->FirstGlyph;
 if(slot<bank->NoGlyphs){const CStreamingGlyph*glyph=&bank->Glyphs[slot];if(glyph->GlyphIndex)return glyph;}
 return 0;
}
