// 009fd9b0 CGraphicDataBank::LoadVolumeGraphic
// Leaf: ?LoadVolumeGraphic@CGraphicDataBank@@AAE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@ABVCGraphicHeader@1@_NK3@Z
// Flags:
// Re-port (2026-11-19): the local C3DExtentsI model replaced by the shared engine/C3DExtentsI.h, the local CIVCountedPointer by engine/CIVCountedPointer.h.
// Re-port (audit h3c2b, 2026-11-20; widened shared-header policy): the fourteen local class models (CBaseClassNonCopyable, CBankFile,
//   CBankFileAsync, CResourceBank, CGraphicDataBank, CIVCountedPointeeBase, CResource, CManagedTextureData, CManagedTexture,
//   CThreadedFileCallback, CMovableResource, CBankFileAsyncData, CPixelFormat, CVolumeTexture) and the renamed-donor includes are gone.
//   The classes are the shared retail headers, with the define set of the sibling 0x009FDE80 CGraphicInfoFrame::BeginLoading where it
//   applies: engine/CGraphicDataBank.h (FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME declares the private LoadVolumeGraphic), engine/CManagedTexture.h
//   / CManagedTextureData.h (new FABLE_RETAIL_CMANAGEDTEXTURE_GETVOLUMETEXTURE: FableWin.pdb's two inline GetVolumeTexture members),
//   engine/CBankFileAsyncData.h (GetData 0x009D5530), engine/CPixelFormat.h (GetByteColourDepth out of line, 0x009E3800) and
//   engine/CVolumeTexture.h (FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST declares GetVolumeLevel 0x00A67EA0). The function text is unchanged.
//   Not shared (no header exists): CVolume and CVolumeLockedBox, as in rows 0x00A68910 / 0x00A685E0.
#define FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CBANKFILEASYNCDATA_WATER
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
#define FABLE_RETAIL_CMANAGEDTEXTURE_GETVOLUMETEXTURE
#define FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST
#include <string.h>
#include "engine/CGraphicInfo.h"
#include "engine/CTextureManager.h"
#include "engine/CBankFileAsyncData.h"
#include "engine/CPixelFormat.h"
#include "engine/C3DExtentsI.h"
#include "engine/CVolumeTexture.h"
class CVolumeLockedBox{public:C3DExtentsI Dimensions;long RowPitch,SlicePitch;unsigned char*PMem;CVolumeLockedBox(){}CVolumeLockedBox(const CVolumeLockedBox&o):Dimensions(o.Dimensions),RowPitch(o.RowPitch),SlicePitch(o.SlicePitch),PMem(o.PMem){}};
struct IDirect3DVolume9;
class CVolume:public CBaseClass{public:IDirect3DVolume9*PD3DVolume;virtual~CVolume(){Uninitialise(true);}unsigned long GetByteLength()const;CVolumeLockedBox Lock(unsigned long);CPixelFormat GetPixelFormat()const;C3DExtentsI GetDimensions()const;void Swizzle(CVolumeLockedBox&);void Unlock()const;void Uninitialise(bool);};
bool CGraphicDataBank::LoadVolumeGraphic(CManagedTexture&managedTexture,CBankFileAsyncData&data,const CGraphicHeader&,bool,unsigned long,bool){
 CCountedPointer<CVolumeTexture>texture=managedTexture.GetVolumeTexture();CVolume volume=texture->GetVolumeLevel(0);volume.GetByteLength();CVolumeLockedBox box=volume.Lock(0);unsigned char*slice=box.PMem;const unsigned char*source=(const unsigned char*)data.GetData();unsigned long depth=volume.GetPixelFormat().GetByteColourDepth();
 for(long z=0;z<volume.GetDimensions().Z;++z){for(long y=0;y<volume.GetDimensions().Y;++y){long rowBytes=volume.GetDimensions().X*depth;memcpy(slice+box.RowPitch*y,source,rowBytes);source+=volume.GetDimensions().X*depth;}slice+=box.SlicePitch;}
 texture->GetVolumeLevel(0).Swizzle(box);volume.Unlock();return true;
}