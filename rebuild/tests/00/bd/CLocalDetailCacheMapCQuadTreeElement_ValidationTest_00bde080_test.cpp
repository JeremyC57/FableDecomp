#include <stdio.h>
#include <string.h>
#include <new>
// 00bde080 CLocalDetailCacheMap::CQuadTreeElement::ValidationTest
// Flags:
// Leaf: ?ValidationTest@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ
#define CLocalDetailCacheMap Pass9LocalDetailMapStorage
#include "engine/CLocalDetailCacheMap.h"
#undef CLocalDetailCacheMap
#define CMovableResource Pass9MovableResourceStorage
#include "engine/CMovableResource.h"
#undef CMovableResource
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CMovableResource {public:
 virtual ~CMovableResource();
 virtual void RelocateData(long,void*,void*,long)=0;
 virtual bool CanRelocateData(long)const;
 virtual unsigned long Verify()const;
};
#pragma pack(push,1)
class C3DVector {public:float X,Y,Z;
 __forceinline C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
};
#pragma pack(pop)
class C3DBoundingSphere {public:C3DVector Center;float Radius;
 __forceinline C3DBoundingSphere():Center(0.0f,0.0f,0.0f),Radius(0.0f){}
};
// Layout: the outer class uses its shared physical storage; only its nested scope is needed.
class CLocalDetailCacheMap:public Pass9LocalDetailMapStorage {public:class CObjectCacheGroupCollection;class CQuadTreeElement;};
// Layout: no engine header exists for this nested type. Donor layout151349..151366
// and native00bddf30 agree: CMovableResource4, collection4, children8,
// sphere24, owner40, fade44, mask48, file52/56/60, rectangle64..70, flags72.
// Layout: original nested collection derives CMovableResource, Next4, unread52.
class CLocalDetailCacheMap::CObjectCacheGroupCollection:public CMovableResource{public:CObjectCacheGroupCollection*Next;unsigned char pass9_unread[52];};
class CLocalDetailCacheMap::CQuadTreeElement:public CMovableResource {public:
 CObjectCacheGroupCollection* ObjectCacheGroupCollections;
 CQuadTreeElement* Children[4];C3DBoundingSphere BoundingSphere;
 CLocalDetailCacheMap* Map;float MaxFadeDistance;unsigned long PrimitiveTypeMask;
 long FileBlockPos,FileBlockSize,OffsetIntoFileBlock;
 unsigned short MapX,MapY,Width,Height;
 bool HasDynamicAreasFlag:1;bool RenderUpdateNeeded:1;
 bool StaticContentsLoaded:1;bool Loading:1;
 virtual ~CQuadTreeElement();
 virtual void RelocateData(long,void*,void*,long);
 virtual bool CanRelocateData(long)const;
 void ValidationTest();
 CQuadTreeElement(CLocalDetailCacheMap*,long,long,long,long);
};
void CLocalDetailCacheMap::CQuadTreeElement::ValidationTest(){
 for(CObjectCacheGroupCollection*collection=ObjectCacheGroupCollections;collection;collection=collection->Next){}
 if(Children[0])Children[0]->ValidationTest();
 if(Children[1]){void(CQuadTreeElement::*validate)()=&CQuadTreeElement::ValidationTest;(Children[1]->*validate)();}
}

CLocalDetailCacheMap::CQuadTreeElement::CQuadTreeElement(CLocalDetailCacheMap*map,long x,long y,long width,long height)
 :Map(map),MapX(static_cast<unsigned short>(x)),MapY(static_cast<unsigned short>(y)),Width(static_cast<unsigned short>(width)),Height(static_cast<unsigned short>(height))
{
 for(long i=0;i<4;++i)Children[i]=0;
 HasDynamicAreasFlag=false;RenderUpdateNeeded=false;StaticContentsLoaded=false;Loading=false;
 ObjectCacheGroupCollections=0;MaxFadeDistance=0.0f;
 BoundingSphere.Center.X=0.0f;BoundingSphere.Center.Y=0.0f;BoundingSphere.Center.Z=0.0f;BoundingSphere.Radius=0.0f;
 FileBlockPos=0;FileBlockSize=0;OffsetIntoFileBlock=0;PrimitiveTypeMask=0;
}

CMovableResource::~CMovableResource(){}
bool CMovableResource::CanRelocateData(long)const{return true;}
unsigned long CMovableResource::Verify()const{return 0;}
CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement(){}
void CLocalDetailCacheMap::CQuadTreeElement::RelocateData(long,void*,void*,long){}
bool CLocalDetailCacheMap::CQuadTreeElement::CanRelocateData(long)const{return true;}
class FixtureCollection:public CLocalDetailCacheMap::CObjectCacheGroupCollection{public:virtual void RelocateData(long,void*,void*,long){}};
int main(){
 typedef CLocalDetailCacheMap::CQuadTreeElement Cell;
 if(sizeof(Cell)!=76||sizeof(FixtureCollection)!=60)return 1;
 union Storage{double align;unsigned char bytes[16*76+32];}storage;
 FixtureCollection collections[24];Cell*cells[16];unsigned int cases=0;
 for(unsigned int seed=0;seed<256;++seed)for(unsigned int empty=0;empty<2;++empty)for(unsigned int start=0;start<8;++start){
  memset(storage.bytes,0xa5,sizeof(storage.bytes));
  for(unsigned int i=0;i<24;++i){collections[i].Next=i<23?collections+i+1:0;memset(collections[i].pass9_unread,seed+i,52);}
  for(unsigned int i=0;i<16;++i)cells[i]=new(storage.bytes+16+i*76)Cell(0,i,i+1,i+2,i+3);
  for(unsigned int i=0;i<16;++i){
   unsigned int a=empty?0:((seed+i*3)%(16-i));unsigned int b=empty?0:(((seed>>2)+i*5)%(16-i));
   cells[i]->Children[0]=a?cells[i+a]:0;cells[i]->Children[1]=b?cells[i+b]:0;
   cells[i]->Children[2]=reinterpret_cast<Cell*>(1);cells[i]->Children[3]=reinterpret_cast<Cell*>(3);
   cells[i]->ObjectCacheGroupCollections=empty?0:collections+(seed+i)%24;
  }
  unsigned char beforeCells[16*76],beforeCollections[sizeof(collections)];
  memcpy(beforeCells,storage.bytes+16,sizeof(beforeCells));memcpy(beforeCollections,collections,sizeof(collections));
  cells[start]->ValidationTest();
  if(memcmp(beforeCells,storage.bytes+16,sizeof(beforeCells))||memcmp(beforeCollections,collections,sizeof(collections)))return 2;
  for(unsigned int i=0;i<16;++i)if(storage.bytes[i]!=0xa5||storage.bytes[16+16*76+i]!=0xa5)return 3;
  for(unsigned int i=0;i<16;++i)cells[i]->~CQuadTreeElement();++cases;
 }
 if(cases!=4096)return 4;puts("00bde080_TEST PASS");return 0;
}