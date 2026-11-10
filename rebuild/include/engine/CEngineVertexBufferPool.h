#ifndef FABLE_ENGINE_CENGINEVERTEXBUFFERPOOL_H
#define FABLE_ENGINE_CENGINEVERTEXBUFFERPOOL_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/CEngineTreeFiler.h"
class CVertexBufferWin32;
template<class T,int KeyCount>class CEngineDynamic3DResource;
template<class T>class CEngineDynamic3DResourceManager {
public:
 class CListHeader { public:T* Head;T* Tail; };
 virtual long GetTotalBytesAllocated()const;
 virtual long GetTotalBytesFree()const;
private:
 virtual void FreeMemory();
 long AllocCount,FreePoolMem,TotalMemAllocated;
 CEngineTreeFiler<CListHeader> ResourceMappingTree;
};
class CEngineVertexBufferPool:public CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32,3> > {
 long SizeGranularity;
};
FABLE_STATIC_ASSERT(sizeof(CEngineVertexBufferPool)==28);
#endif
