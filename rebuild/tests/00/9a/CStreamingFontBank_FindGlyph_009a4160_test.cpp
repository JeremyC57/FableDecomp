// Behavior-only fixture; the src/009a4160.cpp body is pasted unchanged (agent680 shape: function-scope output_size). The cache
// points to a constructed GlyphBank, and packed raw records use placement-built
// objects at naturally aligned addresses. Unused bank fields are reduced.
// GNU64 uses 32-bit bitfield storage; VC7.1 retains original unsigned long.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#endif
typedef unsigned char lzo_byte;typedef unsigned int lzo_uint;
static int cases,decodeCalls;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
#ifdef _MSC_VER
typedef unsigned long GlyphWord;
#else
typedef unsigned int GlyphWord;
#endif
class CStreamingGlyph{public:GlyphWord GlyphIndex:24;GlyphWord AdjustmentIndex:8;};
class CStreamingFontBank{public:struct GlyphBank{unsigned short FirstGlyph,NoGlyphs;CStreamingGlyph Glyphs[64];};void*glyphBankCache;unsigned long glyphBankCacheIndex;void*GlyphBanks[1024];const CStreamingGlyph*FindGlyph(unsigned short)const;};
static CStreamingFontBank::GlyphBank*expectedOutput;static const CStreamingFontBank::GlyphBank*decodedBank;static const unsigned char*expectedInput;
extern "C" int __cdecl lzo1x_decompress_safe(const unsigned char*src,unsigned int count,unsigned char*dest,unsigned int*size,void*work){CHECK(src==expectedInput&&count==4&&dest==reinterpret_cast<unsigned char*>(expectedOutput)&&*size==0x104&&work==0);CHECK(src[0]==0x31&&src[1]==0x42&&src[2]==0x53&&src[3]==0x64);memcpy(dest,decodedBank,sizeof(*decodedBank));*size=sizeof(*decodedBank);++decodeCalls;return 0;}
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

int main(){
 const unsigned short chars[]={0,1,32,63,64,65,127,128,0xa0,0xff,0x100,0x7fff,0xffff};const unsigned short firsts[]={0,1,16,32,63};const unsigned short counts[]={0,1,8,32,64};
 CHECK(sizeof(CStreamingGlyph)==4&&sizeof(CStreamingFontBank::GlyphBank)==0x104);
 for(unsigned c=0;c<sizeof(chars)/sizeof(chars[0]);++c)for(unsigned f=0;f<5;++f)for(unsigned n=0;n<5;++n)for(int mode=0;mode<4;++mode)for(int zero=0;zero<2;++zero){
  CStreamingFontBank font;CStreamingFontBank::GlyphBank cache,wanted;
  union{unsigned int Align;unsigned char Bytes[268];}storage;
  unsigned short*header=new(storage.Bytes+2)unsigned short;
  CStreamingFontBank::GlyphBank*raw=new(storage.Bytes+4)CStreamingFontBank::GlyphBank;
  wanted.FirstGlyph=firsts[f];wanted.NoGlyphs=counts[n];for(int i=0;i<64;++i){wanted.Glyphs[i].GlyphIndex=zero?0:(unsigned int)(0x123400+i);wanted.Glyphs[i].AdjustmentIndex=(unsigned int)(i+128);}
  cache=wanted;*raw=wanted;
  unsigned short ch=chars[c]==0xa0?0x20:chars[c];unsigned long bankIndex=(unsigned long)ch>>6;unsigned long slot=((unsigned long)ch&63)-wanted.FirstGlyph;
  for(int j=0;j<1024;++j)font.GlyphBanks[j]=0;
  font.glyphBankCache=&cache;font.glyphBankCacheIndex=mode==0?bankIndex:bankIndex+1024;
  unsigned char compressed[6]={4,128,0x31,0x42,0x53,0x64};
  // The compressed header requires ushort alignment; copy into an actual ushort array.
  unsigned short packed[3];memcpy(packed,compressed,6);
  *header=0x104;if(mode==2)font.GlyphBanks[bankIndex]=header;if(mode==3)font.GlyphBanks[bankIndex]=packed;
  expectedInput=reinterpret_cast<const unsigned char*>(packed+1);expectedOutput=&cache;decodedBank=&wanted;decodeCalls=0;
  const CStreamingGlyph*result=font.FindGlyph(chars[c]);const CStreamingGlyph*expected=0;
  if(mode!=1&&slot<wanted.NoGlyphs&&!zero)expected=(mode==2?raw:&cache)->Glyphs+slot;
  CHECK(result==expected&&decodeCalls==(mode==3?1:0));CHECK(font.glyphBankCacheIndex==((mode==0||mode==3)?bankIndex:bankIndex+1024));
  CHECK(memcmp(&cache,&wanted,sizeof(cache))==0&&memcmp(raw,&wanted,sizeof(*raw))==0);if(result)CHECK(result->GlyphIndex==0x123400+slot&&result->AdjustmentIndex==128+slot);
  raw->~GlyphBank();++cases;
 }
 CHECK(cases==2600);puts("009a4160_TEST PASS");return 0;
}