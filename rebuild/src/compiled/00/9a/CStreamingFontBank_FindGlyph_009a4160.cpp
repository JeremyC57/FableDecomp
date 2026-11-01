// CStreamingFontBank::FindGlyph(unsigned short character) -- glyph record for a character (NBSP mapped to space): its 64-glyph bank is the cached one, or is LZO-decompressed into the cache when the packed bank's high size bit says compressed, or is used in place; null when the bank or the glyph is absent
// Leaf: ?FindGlyph@CStreamingFontBank@@IBEPBVCStreamingGlyph@@G@Z
// Retail: Fable.exe @ 0x009A4160, 142 bytes (`ret 4`). Ego_r/FableWin: protected const ?FindGlyph@CStreamingFontBank@@IBEPBV
//   CStreamingGlyph@@G@Z. Callee: _lzo1x_decompress_safe 0x00C08170 (landed LZO 1.08 C body; cdecl, `add esp,14h`).
// Shape: two levers ChatGPT pass 10 lacked. (1) the output size is declared at function scope and assigned inside the
//   compressed arm: VC7.1 then keeps the 0x104 store ahead of the src_len load (`mov [esp+14h],104h ; mov dx,[eax]`); a
//   block-scope initialiser is sunk below the pushes. (2) with that store between them, the flag test `*packed & 0x8000`
//   and the size read `*packed & 0x7FFF` are no longer CSE'd, so the flag stays `test byte ptr [eax+1],80h ; je` (the
//   byte-cast form ChatGPT used becomes `mov cl ; test cl,cl ; jns`).
// Layout: engine/CStreamingFontBank.h (glyphBankCache +0x14, glyphBankCacheIndex +0x18, GlyphBanks[1024] +0x2c); GlyphBank /
//   CStreamingGlyph are the Ego_r.pdb structs (260 / 4 bytes, GlyphIndex:24). DBI locals (ChatGPT's read): bankIndex, index,
//   bank, output_size.
#define CStreamingFontBank CStreamingFontBankFields
#include "engine/CStreamingFontBank.h"
#undef CStreamingFontBank
class CStreamingGlyph{public:unsigned long GlyphIndex:24;unsigned long AdjustmentIndex:8;};
typedef unsigned char lzo_byte;                          // LZO 1.08 lzoconf.h (32-bit x86 configuration, as landed 0x00C08170)
typedef unsigned int lzo_uint;
typedef lzo_uint* lzo_uintp;
typedef void* lzo_voidp;
extern "C" int __cdecl lzo1x_decompress_safe(const lzo_byte* src, lzo_uint src_len, lzo_byte* dst, lzo_uintp dst_len, lzo_voidp wrkmem);   // 0x00C08170 (lzo1x.h)
// Layout: supplied flat font bank; original protected nested GlyphBank is260 bytes.
class CStreamingFontBank:public CStreamingFontBankFields{
protected:
 struct GlyphBank{unsigned short FirstGlyph,NoGlyphs;CStreamingGlyph Glyphs[64];};
 const CStreamingGlyph*FindGlyph(unsigned short)const;
};
const CStreamingGlyph*CStreamingFontBank::FindGlyph(unsigned short character)const{
 lzo_uint output_size;
 if(character==0xa0)character=0x20;
 GlyphBank*bank=reinterpret_cast<GlyphBank*>(glyphBankCache);
 unsigned long index=character;
 unsigned long bankIndex=index>>6;
 if(glyphBankCacheIndex!=bankIndex){
  const unsigned short*packed=static_cast<const unsigned short*>(GlyphBanks[bankIndex]);
  if(!packed)return 0;
  if(*packed&0x8000){
   output_size=0x104;
   lzo1x_decompress_safe(reinterpret_cast<const lzo_byte*>(packed+1),*packed&0x7fff,reinterpret_cast<lzo_byte*>(bank),&output_size,0);
   const_cast<CStreamingFontBank*>(this)->glyphBankCacheIndex=bankIndex;
  }else bank=reinterpret_cast<GlyphBank*>(const_cast<unsigned short*>(packed+1));
 }
 unsigned long slot=(index&0x3f)-bank->FirstGlyph;
 if(slot<bank->NoGlyphs){const CStreamingGlyph*glyph=&bank->Glyphs[slot];if(glyph->GlyphIndex)return glyph;}
 return 0;
}