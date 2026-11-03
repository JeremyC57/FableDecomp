#ifndef FABLE_ENGINE_CGRAPHICDATABANK_H
#define FABLE_ENGINE_CGRAPHICDATABANK_H
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFileAsync.h"
#include "engine/CResourceBank.h"
#if defined(FABLE_RETAIL_FRAME_LOADING) || defined(FABLE_RETAIL_TEXTURE_CACHE)
#include "engine/CPixelFormat.h"
#include "engine/CTextureManager.h"
#endif
// Layout: native ctor9fea20 and RTTI: async0..163, resource164..1df,
// own fields1e0..30b including11 eight-byte BlankTextures at2b4.
class CGraphicInfo;
class CGraphicDataBank:public CBankFileAsync,public CResourceBank {
public:CGraphicDataBank();virtual ~CGraphicDataBank();
 virtual unsigned long GetNoFramesInGraphic(unsigned long) const;
 virtual unsigned long GetGraphicFrameWidth(unsigned long,unsigned long) const;
 virtual unsigned long GetGraphicFrameHeight(unsigned long,unsigned long) const;
 virtual unsigned long GetGraphicMaxWidth(unsigned long) const;
 virtual unsigned long GetGraphicMaxHeight(unsigned long) const;
 virtual bool IsUsingSurfaces() const;
public: virtual void Close();
protected: virtual unsigned long GetDataFormatVersion();
protected: virtual void NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount> &);
protected: virtual void NotifyBankOpened();
protected: virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char> &);
protected: virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char> &);
protected: virtual void NotifyEntryRemoved(unsigned long);
protected: virtual void PrepareDataForInclusion(unsigned long,const CArray<CCharString> &,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream &,CDataOutputStream &);
#if defined(FABLE_RETAIL_FRAME_LOADING) || defined(FABLE_RETAIL_TEXTURE_CACHE)
public:
 enum ETextureType {TEXTURE_TYPE_OPAQUE=0,TEXTURE_TYPE_ALPHA=1,TEXTURE_TYPE_BOOLEAN_ALPHA=2,TEXTURE_TYPE_INTERPOLATED_ALPHA=3,TEXTURE_TYPE_BUMP=4,TEXTURE_TYPE_BUMP_ALPHA=5,TEXTURE_TYPE_UNCOMPRESSED_OPAQUE=6,TEXTURE_TYPE_UNCOMPRESSED_ALPHA=7,TEXTURE_TYPE_UNCOMPRESSED_BUMP=8,TEXTURE_TYPE_UNCOMPRESSED_BUMP_ALPHA=9,TEXTURE_TYPE_SIGNED=10,TEXTURE_TYPE_COUNT=11};
 class CGraphicHeader {public:
  unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;
  CPixelFormat PixelFormat;
  unsigned char TransparencyType,MipmapLevels;
  unsigned char IsFlatAnimatedGraphic:1;
  unsigned long FrameDataSize,CompressedMipmapSizes[1];
 };
#ifdef FABLE_RETAIL_TEXTURE_CACHE
 unsigned long GetUnloadFrameDelay()const{return UnloadFrameDelay;}
 unsigned long GetReduceSizeFrameDelay()const{return ReduceSizeFrameDelay;}
 void SetUnloadFrameDelay(unsigned long);void SetReduceSizeFrameDelay(unsigned long);
 const CGraphicInfo* PeekGraphic(unsigned long)const;
#endif
 CTextureManager& GetTextureManager() {return *TextureManager;}
 CPixelFormat GetPixelFormat(ETextureType type) const {return PixelFormats[type];}
private:
#ifdef FABLE_RETAIL_TEXTURE_CACHE
 CArray<CGraphicInfo> GraphicList;
#else
 unsigned char GraphicList[12]; // +1e0 CArray<CGraphicInfo>
#endif
 unsigned char GraphicFrameBuffer[12]; // +1ec CArray<CGraphicInfoFrame>
 bool Initialised;
 unsigned char _pad_0x1f9[3];
 unsigned char StateBlock[100]; // +1fc CGraphicBankStateBlock
 long TextureSizeWarningMaxWidth,TextureSizeWarningMaxHeight;
 unsigned long UnloadFrameDelay,ReduceSizeFrameDelay,TexturePointersValidForFrames;
 long MinAvailablePreloadMemory,AvailableMemory;
 bool FreezeGraphics;
 CCountedPointer<CTextureManager> TextureManager; // +280 native9fdfc4
 CPixelFormat PixelFormats[11]; // +288 native9fdfcd
 unsigned char BlankTextures[88]; // +2b4 CTexture[11]
#else
private:unsigned char _pad_0x1e0[0x12c];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank)==0x30c);
#endif
