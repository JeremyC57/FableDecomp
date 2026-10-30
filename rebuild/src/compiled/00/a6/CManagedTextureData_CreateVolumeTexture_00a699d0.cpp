// CManagedTextureData::CreateVolumeTexture(ulong,ulong,ulong,long,const CPixelFormat&) -- round dimensions, replace volume object, initialise it and record allocation result.
// Native00A699D0 length169;9FA620 valid dimensions;99A2D0 base ctor;A6AE60 counted reset;A68370 volume initialise;BFEA1A scalar new16.
#define CManagedTextureData CManagedTextureDataDonor
#include "engine/CManagedTextureData.h"
#undef CManagedTextureData
#define CTexture CTextureDonor
#include "engine/CTexture.h"
#undef CTexture
#include "engine/CPixelFormat.h"
class C2DExtentsI { public:long X,Y;C2DExtentsI(long x,long y):X(x),Y(y){}};
class C3DExtentsI { public:long X,Y,Z;C3DExtentsI(long x,long y,long z):X(x),Y(y),Z(z){}};
class CTexture { public:static C2DExtentsI FABLE_FASTCALL GetNextValidDimensions(const C2DExtentsI&,const CPixelFormat&);}; //009FA620
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
enum EVolumePool {VolumePoolManaged=1}; // Native value1; exact original enumerator spelling unestablished.
class CVolumeTexture:public CBaseClass { public:
 void* PD3DVolumeTexture;long ByteLength;void* PData;
 CVolumeTexture():PD3DVolumeTexture(0),PData(0){}
 virtual ~CVolumeTexture(); //nativeVT0129DC18 scalar deleting slot0 A69590
 bool Initialise(const C3DExtentsI&,long,const CPixelFormat&,unsigned long,EVolumePool,bool); //00A68370
};
template<class T>class CCountedPointer { public:T* Data;CCPPointerInfo* Info;void Reset(T*);}; //00A6AE60 forVolume
class CManagedTextureData:public CManagedTextureDataDonor { public:
 CCountedPointer<CVolumeTexture>& Volume(){return *reinterpret_cast<CCountedPointer<CVolumeTexture>*>(&VolumeTexture_Data);}
 bool CreateVolumeTexture(unsigned long,unsigned long,unsigned long,long,const CPixelFormat&);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CVolumeTexture)==16);
FABLE_STATIC_ASSERT(offsetof(CVolumeTexture,PD3DVolumeTexture)==4);
FABLE_STATIC_ASSERT(offsetof(CVolumeTexture,ByteLength)==8);
FABLE_STATIC_ASSERT(offsetof(CVolumeTexture,PData)==12);
FABLE_STATIC_ASSERT(sizeof(CManagedTextureData)==0x4c);
#endif
bool CManagedTextureData::CreateVolumeTexture(unsigned long width,unsigned long height,unsigned long depth,long levels,const CPixelFormat& format){
 C2DExtentsI size=CTexture::GetNextValidDimensions(C2DExtentsI(width,height),format);
 long x=size.X,y=size.Y;
 Volume().Reset(new CVolumeTexture);
 CVolumeTexture* volume=Volume().Data;
 Width=(unsigned short)x;Height=(unsigned short)y;
 return Allocated=volume->Initialise(C3DExtentsI(x,y,depth),1,format,0,VolumePoolManaged,false);
}