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