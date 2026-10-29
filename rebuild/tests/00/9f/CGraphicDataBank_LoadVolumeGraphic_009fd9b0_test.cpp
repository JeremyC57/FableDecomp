#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __declspec(x)
#endif
#define PASS9_TEST
// 009fd9b0 CGraphicDataBank::LoadVolumeGraphic
// Leaf: ?LoadVolumeGraphic@CGraphicDataBank@@AAE_NAAVCManagedTexture@@AAVCBankFileAsyncData@@ABVCGraphicHeader@1@_NK3@Z
// Flags:
#include <stddef.h>
#include "rebuild_abi.h"
#define CBankFile CBankFileFields
#include "engine/CBankFile.h"
#undef CBankFile
#define CGraphicDataBank CGraphicDataBankFields
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
#define CManagedTextureData CManagedTextureDataFields
#include "engine/CManagedTextureData.h"
#undef CManagedTextureData
#define CResource CResourceFields
#include "engine/CResource.h"
#undef CResource
// Layout: quarantine/raw headers are physical comparisons. The original bank and managed-data classes below restore their genuine RTTI bases and typed used members; unread state remains explicit storage.
class CBaseClass{public:virtual~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
// Layout: unread original primary-bank fields retained as byte projections; only the bank receiver is used here.
class CBankFile:public CBaseClassNonCopyable{unsigned char unread[0x10c];public:unsigned long GetEntryDataType(unsigned long)const;};
class CBankFileAsync:public CBankFile{unsigned char unread[0x54];};
class CResourceBank:public CBaseClassNonCopyable{unsigned char unread[0x78];};
class CManagedTexture;class CBankFileAsyncData;class CGraphicInfoFrame;
class CGraphicDataBank:public CBankFileAsync,public CResourceBank{unsigned char unread[0x12c];public:
 class CGraphicHeader{unsigned char unread[0x1c];};
 enum ETextureType{};
private:friend class CGraphicInfoFrame;
#ifdef PASS9_TEST
 friend int main();
#endif

 bool LoadVolumeGraphic(CManagedTexture&,CBankFileAsyncData&,const CGraphicHeader&,bool,unsigned long,bool);
};
#include <string.h>
class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void*Data;};
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;CCountedPointer():Data(0),Info(0){}CCountedPointer(const CCountedPointer&o):Data(o.Data),Info(o.Info){if(Info)++Info->RefCount;}~CCountedPointer(){if(Info){if(--Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=0;Info=0;}T*operator->()const{return Data;}};
#define CIVCountedPointeeBase CIVCountedPointeeBaseFields
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CMovableResource CMovableResourceFields
#include "engine/CMovableResource.h"
#undef CMovableResource
class CIVCountedPointeeBase{public:virtual~CIVCountedPointeeBase();virtual void Delete()const;mutable unsigned long IVRefCount;};
class CResourceList;class CResource:public CIVCountedPointeeBase{public:CResourceList*ResourceList;CResource*PrevResource,*NextResource;unsigned long ResourceSize,LastUsedFrame;};
class CTexture;class CVolumeTexture;
class CManagedTextureData:public CResource{public:unsigned short Width,Height;CCountedPointer<CTexture>Texture;CCountedPointer<CVolumeTexture>VolumeTexture;unsigned char unread[0x1c];CCountedPointer<CVolumeTexture>GetVolumeTexture(){return VolumeTexture;}};
template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase*Object;T*operator->()const{return static_cast<T*>(Object);}};
class CManagedTexture:public CBaseClass{public:CIVCountedPointer<CManagedTextureData>Data;CCountedPointer<CVolumeTexture>GetVolumeTexture(){return Data->GetVolumeTexture();}};
class CThreadedFileCallback:public CBaseClassNonCopyable{};
class CMovableResource{public:virtual~CMovableResource();};
class CBankFileAsyncData:public CThreadedFileCallback,public CMovableResource{unsigned char unread[0x40];public:void*GetData();};
#define CPixelFormat CPixelFormatFields
#include "engine/CPixelFormat.h"
#undef CPixelFormat
class CPixelFormat:public CPixelFormatFields{public:unsigned long GetByteColourDepth()const;};
// Layout: the original nonpolymorphic parent of C3DExtentsI is not proven. Its first two long words are an implementation-only projection of SDK Width/Height; only Z at+8 has an exported original member name.
class C3DExtentsI{public:unsigned char prefix[8];long Z;C3DExtentsI(){}C3DExtentsI(const C3DExtentsI&o){memcpy(prefix,o.prefix,8);Z=o.Z;}};
class CVolumeLockedBox{public:C3DExtentsI Dimensions;long RowPitch,SlicePitch;unsigned char*PMem;CVolumeLockedBox(){}CVolumeLockedBox(const CVolumeLockedBox&o):Dimensions(o.Dimensions),RowPitch(o.RowPitch),SlicePitch(o.SlicePitch),PMem(o.PMem){}};
struct IDirect3DVolume9;
class CVolume:public CBaseClass{public:IDirect3DVolume9*PD3DVolume;virtual~CVolume(){Uninitialise(true);}unsigned long GetByteLength()const;CVolumeLockedBox Lock(unsigned long);CPixelFormat GetPixelFormat()const;C3DExtentsI GetDimensions()const;void Swizzle(CVolumeLockedBox&);void Unlock()const;void Uninitialise(bool);};
struct IDirect3DVolumeTexture9;
class CVolumeTexture:public CBaseClass{public:IDirect3DVolumeTexture9*PD3DVolumeTexture;long ByteLength;void*PData;CVolume GetVolumeLevel(long)const;};
bool CGraphicDataBank::LoadVolumeGraphic(CManagedTexture&managedTexture,CBankFileAsyncData&data,const CGraphicHeader&,bool,unsigned long,bool){
 CCountedPointer<CVolumeTexture>texture=managedTexture.GetVolumeTexture();CVolume volume=texture->GetVolumeLevel(0);volume.GetByteLength();CVolumeLockedBox box=volume.Lock(0);unsigned char*slice=box.PMem;const unsigned char*source=(const unsigned char*)data.GetData();unsigned long depth=volume.GetPixelFormat().GetByteColourDepth();
 for(long z=0;z<volume.GetDimensions().Z;++z){for(long y=0;y<reinterpret_cast<const long*>(volume.GetDimensions().prefix)[1];++y){long rowBytes=reinterpret_cast<const long*>(volume.GetDimensions().prefix)[0]*depth;memcpy(slice+box.RowPitch*y,source,rowBytes);source+=reinterpret_cast<const long*>(volume.GetDimensions().prefix)[0]*depth;}slice+=box.SlicePitch;}
 texture->GetVolumeLevel(0).Swizzle(box);volume.Unlock();return true;
}

FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase)==sizeof(CIVCountedPointeeBaseFields));
FABLE_STATIC_ASSERT(sizeof(CMovableResource)==sizeof(CMovableResourceFields));
FABLE_STATIC_ASSERT(sizeof(CBankFileAsyncData)==72);

FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank)==sizeof(CGraphicDataBankFields));
FABLE_STATIC_ASSERT(sizeof(CManagedTextureData)==sizeof(CManagedTextureDataFields));
FABLE_STATIC_ASSERT(sizeof(CResource)==sizeof(CResourceFields));

static CVolume testPrototype;
static unsigned char testSource[512],testDestination[512],testExpected[512];
static long testWidth,testHeight,testDepth,testRowPitch,testSlicePitch;static unsigned long testBytes;
static int testGets,testLocks,testByteLengths,testPixelGets,testSwizzles,testUnlocks,testUninitialises,testDataGets,testDeletes,testError,testPhase;
static CBankFileAsyncData*testAsync;
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CMovableResource::~CMovableResource(){}CIVCountedPointeeBase::~CIVCountedPointeeBase(){}void CIVCountedPointeeBase::Delete()const{testError=1;}
static void __fastcall testDelete(void*){++testDeletes;}
CVolume CVolumeTexture::GetVolumeLevel(long level)const{if(level!=0)testError=2;++testGets;return testPrototype;}
unsigned long CVolume::GetByteLength()const{++testByteLengths;return testWidth*testHeight*testDepth*testBytes;}
CVolumeLockedBox CVolume::Lock(unsigned long flags){if(flags!=0)testError=3;++testLocks;CVolumeLockedBox box;reinterpret_cast<long*>(box.Dimensions.prefix)[0]=testWidth;reinterpret_cast<long*>(box.Dimensions.prefix)[1]=testHeight;box.Dimensions.Z=testDepth;box.RowPitch=testRowPitch;box.SlicePitch=testSlicePitch;box.PMem=testDestination+16;return box;}
CPixelFormat CVolume::GetPixelFormat()const{++testPixelGets;CPixelFormat format;format.FormatIndex=testBytes;return format;}
unsigned long CPixelFormat::GetByteColourDepth()const{return FormatIndex;}
C3DExtentsI CVolume::GetDimensions()const{C3DExtentsI dims;reinterpret_cast<long*>(dims.prefix)[0]=testWidth;reinterpret_cast<long*>(dims.prefix)[1]=testHeight;dims.Z=testDepth;return dims;}
void CVolume::Swizzle(CVolumeLockedBox&box){if(testUnlocks||reinterpret_cast<long*>(box.Dimensions.prefix)[0]!=testWidth||reinterpret_cast<long*>(box.Dimensions.prefix)[1]!=testHeight||box.Dimensions.Z!=testDepth||box.RowPitch!=testRowPitch||box.SlicePitch!=testSlicePitch||box.PMem!=testDestination+16)testError=4;++testSwizzles;testPhase=1;}
void CVolume::Unlock()const{if(testPhase!=1||testUninitialises!=1)testError=5;testPhase=2;++testUnlocks;}
void CVolume::Uninitialise(bool release){if(!release)testError=6;++testUninitialises;}
void*CBankFileAsyncData::GetData(){if(this!=testAsync)testError=7;++testDataGets;return testSource+16;}
int main(){
 CGraphicDataBank bank;CGraphicDataBank::CGraphicHeader header;CManagedTexture managed;CManagedTextureData data;CVolumeTexture texture;CCPPointerInfo info;CBankFileAsyncData async;
 testAsync=&async;managed.Data.Object=&data;data.VolumeTexture.Data=&texture;data.VolumeTexture.Info=&info;info.DeleteFunc=testDelete;info.Data=&texture;testPrototype.PD3DVolume=0;
 for(long w=0;w<=5;++w)for(long h=0;h<=4;++h)for(long z=0;z<=3;++z)for(unsigned long bytes=1;bytes<=4;++bytes)for(long rowPad=0;rowPad<=2;++rowPad)for(long slicePad=0;slicePad<=2;++slicePad){
  testWidth=w;testHeight=h;testDepth=z;testBytes=bytes;testRowPitch=w*bytes+rowPad;testSlicePitch=testRowPitch*h+slicePad;
  for(int i=0;i<512;++i)testSource[i]=(unsigned char)(i*17+w*3+h*5+z*7+bytes);
  memset(testDestination,0xcd,sizeof(testDestination));memset(testExpected,0xcd,sizeof(testExpected));long position=16;
  for(long iz=0;iz<z;++iz)for(long iy=0;iy<h;++iy){memcpy(testExpected+16+iz*testSlicePitch+iy*testRowPitch,testSource+position,w*bytes);position+=w*bytes;}
  testGets=testLocks=testByteLengths=testPixelGets=testSwizzles=testUnlocks=testUninitialises=testDataGets=testDeletes=testError=testPhase=0;info.RefCount=3;
  bool result=bank.LoadVolumeGraphic(managed,async,header,false,37,true);
  if(!result||testError||info.RefCount!=3||testDeletes||testGets!=2||testLocks!=1||testByteLengths!=1||testPixelGets!=1||testSwizzles!=1||testUnlocks!=1||testUninitialises!=2||testDataGets!=1)return 1;
  if(memcmp(testDestination,testExpected,sizeof(testDestination)))return 2;
  for(int i=0;i<512;++i)if(testSource[i]!=(unsigned char)(i*17+w*3+h*5+z*7+bytes))return 3;
 }
 data.VolumeTexture.Data=0;data.VolumeTexture.Info=0;puts("009fd9b0_TEST PASS");return 0;
}