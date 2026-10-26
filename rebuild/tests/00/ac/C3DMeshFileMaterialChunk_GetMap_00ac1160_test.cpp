#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define FABLE_ENGINE_C3DMESHFILEMATERIALCHUNK_H
class C3DMeshFileMaterialChunk { public:};
#endif
// C3DMeshFileMaterialChunk::GetMap(EMaterialMapType) const -- find the requested MMAP child by its map type
// Retail @ 0x00AC1160, 263 bytes. FindFirstMap @ 0x00AC0E50 wraps FindFirstSubChunkOfType @ 0x00AA7F40
// with the rdata literal "MMAP" at 0x0129EDD4 (bytes 4d 4d 41 50); FindNextMap @ 0x00AC0EE0 wraps FindNextSubChunk @ 0x00AA7FF0.
// MapType is at +0x2c from the retail compare at 0x00AC11C5. The generated map header is absent, so the
// fixture models only the required prefix and field; the CCountedPointer contract follows landed siblings.
struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template<class T> class CCountedPointer { public:
 T* Object; SCountBlock* Count;
 CCountedPointer():Object(0),Count(0){}
 CCountedPointer(const CCountedPointer& o):Object(o.Object),Count(o.Count){if(Count)++Count->references;}
 template<class U> CCountedPointer(const CCountedPointer<U>& o):Object((T*)o.Object),Count(o.Count){if(Count)++Count->references;}
 ~CCountedPointer(){if(Count){--Count->references;if(!Count->references){Count->destroy(Count->object);delete Count;}}Object=0;Count=0;}
 struct CastTag{};
 template<class U> CCountedPointer(const CCountedPointer<U>& o,CastTag):Object(0),Count(0){if(o.Count){Object=(T*)o.Object;Count=o.Count;++Count->references;}}
 CCountedPointer& operator=(const CCountedPointer& o){SCountBlock*c=o.Count;T*p=o.Object;if(Count!=c){if(Count){--Count->references;if(!Count->references){Count->destroy(Count->object);delete Count;}}Object=p;Count=c;if(c)++c->references;}return *this;}
 T* operator->()const{return Object;}
};
class CChunkedFileChunk;
enum EMaterialMapType { MAP_DIFFUSE=0, MAP_BUMP=1, MAP_REFLECTION=2, MAP_ILLUMINATION=3 };
class C3DMeshFileMapChunk { public: unsigned char pad[0x2c]; EMaterialMapType MapType; };
#include "engine/C3DMeshFileMaterialChunk.h"
// Method-only view: this function does not access owner fields, so shared owner storage is sufficient.
// The two inherited superchunk helpers receive the same object address; no secondary base adjustment.
struct CMaterialChunkMethods : C3DMeshFileMaterialChunk {
 CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char*)const; // 00AA7F40
 CCountedPointer<CChunkedFileChunk> FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>&)const; // 00AA7FF0
 CCountedPointer<const C3DMeshFileMapChunk> FindFirstMap()const;
 CCountedPointer<const C3DMeshFileMapChunk> FindNextMap(const CCountedPointer<const C3DMeshFileMapChunk>&)const;
 CCountedPointer<const C3DMeshFileMapChunk> GetMap(EMaterialMapType)const;
};
// Keep the wrappers out of the caller: retail calls these separately, and the helper boundary preserves
// the counted-handle register/lifetime schedule (notably across the caller-clobbered ECX/EDX registers).
#pragma auto_inline(off)
CCountedPointer<const C3DMeshFileMapChunk> CMaterialChunkMethods::FindFirstMap()const{return CCountedPointer<const C3DMeshFileMapChunk>(FindFirstSubChunkOfType("MMAP"),CCountedPointer<const C3DMeshFileMapChunk>::CastTag());}
CCountedPointer<const C3DMeshFileMapChunk> CMaterialChunkMethods::FindNextMap(const CCountedPointer<const C3DMeshFileMapChunk>& current)const{CCountedPointer<CChunkedFileChunk> prev(current);return CCountedPointer<const C3DMeshFileMapChunk>(FindNextSubChunk(prev),CCountedPointer<const C3DMeshFileMapChunk>::CastTag());}
#pragma auto_inline(on)
CCountedPointer<const C3DMeshFileMapChunk> CMaterialChunkMethods::GetMap(EMaterialMapType type)const{CCountedPointer<const C3DMeshFileMapChunk> map=FindFirstMap();while(map.Object&&map->MapType!=type)map=FindNextMap(map);if(map.Object)return map;return CCountedPointer<const C3DMeshFileMapChunk>();}

static C3DMeshFileMapChunk maps[3];static int mapCount,liveBlocks,firstCalls,nextCalls,destroyCalls;
static int destroyed[4];static bool wrong;static CMaterialChunkMethods* expectedMaterial;
static void __fastcall ReleaseMap(void* object){
    int index=-1;for(int i=0;i<mapCount;++i)if(object==&maps[i])index=i;
    if(index<0 || destroyCalls>=4)wrong=true;else destroyed[destroyCalls++]=index;
    --liveBlocks;
}
static CCountedPointer<CChunkedFileChunk> MakeMap(int i){
    CCountedPointer<CChunkedFileChunk> p;
    if(i>=0&&i<mapCount){p.Object=(CChunkedFileChunk*)&maps[i];p.Count=new SCountBlock;
        p.Count->references=1;p.Count->destroy=&ReleaseMap;p.Count->object=p.Object;++liveBlocks;}
    return p;
}
CCountedPointer<CChunkedFileChunk> CMaterialChunkMethods::FindFirstSubChunkOfType(const char* type)const{
    if(this!=expectedMaterial || std::strcmp(type,"MMAP") || firstCalls)wrong=true;
    ++firstCalls;return MakeMap(0);
}
CCountedPointer<CChunkedFileChunk> CMaterialChunkMethods::FindNextSubChunk(const CCountedPointer<CChunkedFileChunk>& prev)const{
    if(this!=expectedMaterial || !prev.Count || prev.Count->references<1)wrong=true;
    ++nextCalls;for(int i=0;i<mapCount;++i)if((void*)prev.Object==&maps[i])return MakeMap(i+1);
    wrong=true;return CCountedPointer<CChunkedFileChunk>();
}
static bool run(int count,EMaterialMapType type,int expectedIndex){
    CMaterialChunkMethods material;expectedMaterial=&material;mapCount=count;
    maps[0].MapType=MAP_DIFFUSE;maps[1].MapType=MAP_BUMP;maps[2].MapType=MAP_REFLECTION;
    firstCalls=nextCalls=destroyCalls=0;wrong=false;
    if(liveBlocks)return false;
    {
        CCountedPointer<const C3DMeshFileMapChunk> p=material.GetMap(type);
        if(expectedIndex>=0){if(p.Object!=&maps[expectedIndex] || !p.Count || p.Count->references!=1 || liveBlocks!=1)return false;}
        else if(p.Object || p.Count || liveBlocks)return false;
    }
    int visited=expectedIndex>=0?expectedIndex+1:count;
    if(wrong || liveBlocks || firstCalls!=1 || nextCalls!=(expectedIndex>=0?expectedIndex:count) || destroyCalls!=visited)return false;
    for(int i=0;i<visited;++i)if(destroyed[i]!=i)return false;
    return true;
}
int main(){
    if(!run(3,MAP_DIFFUSE,0)||!run(3,MAP_BUMP,1)||!run(3,MAP_REFLECTION,2)||!run(3,MAP_ILLUMINATION,-1)||!run(0,MAP_DIFFUSE,-1))return 1;
    std::puts("00ac1160_TEST PASS");return 0;
}