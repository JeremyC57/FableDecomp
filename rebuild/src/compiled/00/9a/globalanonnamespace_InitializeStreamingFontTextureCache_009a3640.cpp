// InitializeStreamingFontTextureCache(CStreamingFontDataBank*) -- initialise glyph texture cache and load bank indices.
// Leaf: ?InitializeStreamingFontTextureCache@?A0xfe46a155@@YIXPAVCStreamingFontDataBank@@@Z
// Flags: /Gr
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSTREAMINGFONTDATABANK
#include "stlport/fable_containers.h"
#include "engine/CDisplayManager.h"
#include "engine/CMemoryAllocator.h"
#include "engine/CSystemManager.h"
#include "engine/CStreamingFontDataBank.h"
#include "engine/CStreamingGlyphData.h"
#include "engine/CStreamingGlyphAdjustment.h"
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace {
extern unsigned char* textureCacheMem;
extern unsigned long textureCacheSize;
extern unsigned long glyphTextureCacheGlyphDataSize;
extern unsigned long glyphTextureCacheGlyphDataNumIndices;
extern unsigned long* glyphTextureCacheGlyphDataIndices;
extern void* glyphTextureCacheGlyphData;
extern unsigned long numTextureCacheGlyphDatasPerChunk;
extern CStreamingGlyphData* glyphTextureCacheGlyphDataCache;
extern unsigned long glyphTextureCacheGlyphDataCacheChunk;
extern CStreamingGlyphAdjustment* glyphTextureCacheGlyphAdjustment;
extern unsigned long textureCacheNumGlyphs;
extern unsigned long textureCacheNumGlyphAdjustments;
extern unsigned long textureCacheMemOffset;
extern CTexture GFontTextureCache;
extern CArray<unsigned char> GFontPixelLookupTable;
extern long GFontTextureBytesPerPixel;
void __fastcall InitializeStreamingFontTextureCache(CStreamingFontDataBank* data_bank)
{
 if(!textureCacheMem) {
  textureCacheSize=512*1024;
  textureCacheMem=(unsigned char*)CMemoryAllocator::Allocate(textureCacheSize,0);
  CPixelFormat pixel_format(D3DFMT_A8);
  GFontTextureBytesPerPixel=1;
  GFontPixelLookupTable.resize(256);
  for(long c_val=0;c_val<256;++c_val)GFontPixelLookupTable[c_val]=(unsigned char)c_val;
  bool success=GFontTextureCache.Initialise(C2DExtentsI(1024,512),1,pixel_format,0,SURFACE_POOL_MANAGED,false,true);
  if(!success) {
   CPixelFormat pixel_format(D3DFMT_A4R4G4B4);
   GFontTextureBytesPerPixel=2;
   GFontPixelLookupTable.resize(512);
   unsigned short* table_ptr=(unsigned short*)GFontPixelLookupTable.GetAsCArray();
   for(long c_val=0;c_val<256;++c_val) {
    unsigned long pixel_val=((c_val*16)/256)<<12;
    pixel_val|=0x0fff;
    table_ptr[c_val]=(unsigned short)pixel_val;
   }
   success=GFontTextureCache.Initialise(C2DExtentsI(1024,512),1,pixel_format,0,SURFACE_POOL_MANAGED,false,false);
  }
  if(!success) {
   CPixelFormat pixel_format;
   GFGetSystemManager()->GetDisplayManager()->GetBestAlphaTextureFormat(32,pixel_format);
   GFontTextureBytesPerPixel=4;
   GFontPixelLookupTable.resize(1024);
   unsigned long* table_ptr=(unsigned long*)GFontPixelLookupTable.GetAsCArray();
   for(long c_val=0;c_val<256;++c_val) {
    unsigned long pixel_val=0xffffffff;
    pixel_val&=~pixel_format.GetAMask();
    pixel_val|=c_val<<pixel_format.GetAPosShift();
    table_ptr[c_val]=pixel_val;
   }
   GFontTextureCache.Initialise(C2DExtentsI(1024,512),1,pixel_format,0,SURFACE_POOL_MANAGED,false,false);
  }
  data_bank->StreamGlyph(&textureCacheNumGlyphs,0,4,true);
  data_bank->StreamGlyph(&textureCacheNumGlyphAdjustments,4,4,true);
  data_bank->StreamGlyph(&textureCacheMemOffset,8,4,true);
  data_bank->StreamGlyph(&glyphTextureCacheGlyphDataSize,12,4,true);
  data_bank->StreamGlyph(&numTextureCacheGlyphDatasPerChunk,16,4,true);
  glyphTextureCacheGlyphDataNumIndices=(textureCacheNumGlyphs+numTextureCacheGlyphDatasPerChunk-1)/numTextureCacheGlyphDatasPerChunk;
  glyphTextureCacheGlyphDataCacheChunk=0xffffffff;
  const unsigned long glyphDataOffset=20;
  const unsigned long glyphIndicesOffset=glyphDataOffset+glyphTextureCacheGlyphDataSize;
  const unsigned long glyphIndicesSize=(glyphTextureCacheGlyphDataNumIndices+1)*sizeof(unsigned long);
  const unsigned long glyphAdjustmentOffset=glyphIndicesOffset+glyphIndicesSize;
  const unsigned long glyphAdjustmentSize=textureCacheNumGlyphAdjustments*sizeof(CStreamingGlyphAdjustment);
  glyphTextureCacheGlyphDataCache=new CStreamingGlyphData[numTextureCacheGlyphDatasPerChunk];
  glyphTextureCacheGlyphData=new unsigned char[glyphTextureCacheGlyphDataSize];
  glyphTextureCacheGlyphDataIndices=new unsigned long[glyphTextureCacheGlyphDataNumIndices+1];
  glyphTextureCacheGlyphAdjustment=new CStreamingGlyphAdjustment[textureCacheNumGlyphAdjustments];
  data_bank->ReadData(glyphTextureCacheGlyphData,glyphDataOffset,glyphTextureCacheGlyphDataSize);
  data_bank->ReadData(glyphTextureCacheGlyphDataIndices,glyphIndicesOffset,glyphIndicesSize);
  data_bank->ReadData(glyphTextureCacheGlyphAdjustment,glyphAdjustmentOffset,glyphAdjustmentSize);
 }
}
}