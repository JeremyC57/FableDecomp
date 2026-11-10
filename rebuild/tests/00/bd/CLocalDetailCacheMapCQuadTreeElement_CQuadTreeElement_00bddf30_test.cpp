// 00bddf30 exact constructor body and original data types; public test observation.
#include <stdio.h>
#include <string.h>
#include <new>
#include <stddef.h>
#if defined(_MSC_VER)
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
};
class C3DBoundingSphere {
public:
    C3DBoundingSphere() : Center(0.0f,0.0f,0.0f), Radius(0.0f) { }
    C3DVector Center;
    float Radius;
};
class CMovableResource {
public:
    CMovableResource() {}
    virtual ~CMovableResource() {}
    virtual void RelocateData(long,void*,void*,long)=0;
    virtual bool CanRelocateData(long) const;
    virtual unsigned long Verify() const;
};
class CEngineMap;
class CEngineStaticMapFileBlockManager;
class CLocalDetailCacheMap {
public:
    class CQuadTreeElement;
    class CObjectCacheGroupCollection;
    bool LockedFromMemoryRelocation;
    unsigned char _pad_0x01[3];
    CEngineStaticMapFileBlockManager* FileBlockManager;
    CEngineMap* Map;
    unsigned char QuadTree[0x4c];
    unsigned char AreaUpdateList[0xc];
    unsigned char CollectionTypePalette[0xc];
};
class CLocalDetailCacheMap::CQuadTreeElement : public CMovableResource {
public:
    CQuadTreeElement(CLocalDetailCacheMap* map,long x,long y,long width,long height);
    virtual ~CQuadTreeElement();
    virtual void RelocateData(long,void*,void*,long);
    virtual bool CanRelocateData(long) const;
public:
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;
    CQuadTreeElement* Children[4];
    C3DBoundingSphere BoundingSphere;
    CLocalDetailCacheMap* Map;
    float MaxFadeDistance;
    unsigned long PrimitiveTypeMask;
    long FileBlockPos;
    long FileBlockSize;
    long OffsetIntoFileBlock;
    unsigned short MapX;
    unsigned short MapY;
    unsigned short Width;
    unsigned short Height;
    bool HasDynamicAreasFlag:1;
    bool RenderUpdateNeeded:1;
    bool StaticContentsLoaded:1;
    bool Loading:1;
};
CLocalDetailCacheMap::CQuadTreeElement::CQuadTreeElement(CLocalDetailCacheMap* map,long mapx,long mapy,long width,long height)
    : Map(map)
{
    Children[0]=0;
    Children[1]=0;
    Children[2]=0;
    Children[3]=0;
    ObjectCacheGroupCollections=0;
    RenderUpdateNeeded=false;
    MapX=mapx;
    MapY=mapy;
    Width=width;
    Height=height;
    RenderUpdateNeeded=false;
    Loading=false;
    StaticContentsLoaded=false;
    HasDynamicAreasFlag=false;
    MaxFadeDistance=0.0f;
    PrimitiveTypeMask=0;
    BoundingSphere.Center.Initialise(0.0f,0.0f,0.0f);
    BoundingSphere.Radius=0.0f;
    FileBlockPos=0;
    FileBlockSize=0;
    OffsetIntoFileBlock=0;
    PrimitiveTypeMask=0;
}

// Controlled virtual service boundaries; none is called by the target constructor.
bool CMovableResource::CanRelocateData(long) const {return true;}
unsigned long CMovableResource::Verify() const {return 123;}
CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement() {}
void CLocalDetailCacheMap::CQuadTreeElement::RelocateData(long,void*,void*,long) {}
bool CLocalDetailCacheMap::CQuadTreeElement::CanRelocateData(long) const {return true;}
int main(){
    typedef CLocalDetailCacheMap::CQuadTreeElement Element;
    union Storage {double alignment;void* pointer;unsigned char bytes[sizeof(Element)];} storage;
    CLocalDetailCacheMap map;
    memset(&map,0,sizeof(map));
    const long values[]={0,1,255,32767,65535,65536,65537,-1,-65537};
    for(int pattern=0;pattern<256;++pattern){
        for(int i=0;i<9;++i){
            for(int use_map=0;use_map<2;++use_map){
                memset(storage.bytes,pattern,sizeof(storage.bytes));
                CLocalDetailCacheMap* owner=use_map?&map:0;
                Element* e=new(storage.bytes) Element(owner,values[i],values[(i+2)%9],values[(i+4)%9],values[(i+6)%9]);
                if(e->Map!=owner || e->ObjectCacheGroupCollections)return 1;
                for(int j=0;j<4;++j)if(e->Children[j])return 2;
                if(e->MapX!=(unsigned short)values[i] || e->MapY!=(unsigned short)values[(i+2)%9] || e->Width!=(unsigned short)values[(i+4)%9] || e->Height!=(unsigned short)values[(i+6)%9])return 3;
                if(e->RenderUpdateNeeded || e->Loading || e->StaticContentsLoaded || e->HasDynamicAreasFlag)return 4;
                if(e->MaxFadeDistance!=0.0f || e->BoundingSphere.Center.X!=0.0f || e->BoundingSphere.Center.Y!=0.0f || e->BoundingSphere.Center.Z!=0.0f || e->BoundingSphere.Radius!=0.0f)return 5;
                if(e->PrimitiveTypeMask || e->FileBlockPos || e->FileBlockSize || e->OffsetIntoFileBlock)return 6;
                const unsigned char* bits=reinterpret_cast<const unsigned char*>(&e->Height)+sizeof(e->Height);
                if(*bits!=(unsigned char)(pattern&0xf0))return 7;
#if defined(_MSC_VER)
                if(sizeof(Element)!=76 || reinterpret_cast<const unsigned char*>(&e->Map)-storage.bytes!=40 || bits-storage.bytes!=72)return 8;
#endif
                e->~Element();
            }
        }
    }
    puts("00bddf30_TEST PASS");
    return 0;
}