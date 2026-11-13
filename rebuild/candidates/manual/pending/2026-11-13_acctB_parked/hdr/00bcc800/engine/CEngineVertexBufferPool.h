#ifndef FABLE_ENGINE_CENGINEVERTEXBUFFERPOOL_H
#define FABLE_ENGINE_CENGINEVERTEXBUFFERPOOL_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "engine/CEngineTreeFiler.h"
class CVertexBufferWin32;
#ifdef FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES
// Retail opt-in (row 0x00BCC800 RenderWeaponTrail): the pool's header inlines. Ego_r.pdb CEngineDynamic3DResource<T,3>: the
// resource itself, Key[3] and the free-list link PoolNext after it (CVertexBufferWin32 variant 0x44 bytes in retail).
// Needs FABLE_RETAIL_CENGINETREEFILER_INLINES, CVertexBufferBase's Sizeof / IsBusy (FABLE_RETAIL_CVERTEXBUFFERBASE_ISBUSY_SIZEOF),
// EVertexType and EVertexBufferPool declared before this header.
template<class T,int KeyCount>class CEngineDynamic3DResource:public T {
public:
 unsigned long Key[KeyCount];
 CEngineDynamic3DResource* PoolNext;
};
#else
template<class T,int KeyCount>class CEngineDynamic3DResource;
#endif
template<class T>class CEngineDynamic3DResourceManager {
public:
#ifdef FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES
 class CListHeader { public:T* Head;T* Tail; CListHeader():Head(0),Tail(0){} };
#else
 class CListHeader { public:T* Head;T* Tail; };
#endif
 virtual long GetTotalBytesAllocated()const;
 virtual long GetTotalBytesFree()const;
#ifdef FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES
 // FableWin.pdb ReturnResource QAEXPAV..., GetResource / AddNewResource IAE (protected); out-of-line unreferenced copies of
 // ReturnResource 0x00BCA140 and GetResource 0x00BCA210.
 FABLE_FORCEINLINE void ReturnResource(T* resource)
 {
  unsigned long id = ResourceMappingTree.FindID(resource->Key);
  if (id > 0) {
   CListHeader* header = ResourceMappingTree.GetEntry(id);
   if (!header) {
    header = new CListHeader;
    ResourceMappingTree.SetEntry(id, header);
   }
   if (header->Tail) {
    header->Tail->PoolNext = resource;
    header->Tail = resource;
   } else {
    header->Head = resource;
    header->Tail = resource;
   }
   FreePoolMem += resource->Sizeof();
  }
 }
protected:
 FABLE_FORCEINLINE T* GetResource(unsigned long* key)
 {
  unsigned long id = ResourceMappingTree.AddKey(key);
  CListHeader* header = ResourceMappingTree.GetEntry(id);
  if (!header) {
   header = new CListHeader;
   ResourceMappingTree.SetEntry(id, header);
  }
  if (header->Head && !header->Head->IsBusy()) {
   T* resource = header->Head;
   header->Head = resource->PoolNext;
   resource->PoolNext = 0;
   if (!header->Head)
    header->Tail = 0;
   FreePoolMem -= resource->Sizeof();
   return resource;
  }
  return 0;
 }
 FABLE_FORCEINLINE void AddNewResource(T* resource, unsigned long* key)
 {
  for (long i = 0; i < 3; ++i)
   resource->Key[i] = key[i];
  TotalMemAllocated += resource->Sizeof();
 }
#endif
private:
 virtual void FreeMemory();
 long AllocCount,FreePoolMem,TotalMemAllocated;
 CEngineTreeFiler<CListHeader> ResourceMappingTree;
};
class CEngineVertexBufferPool:public CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32,3> > {
 long SizeGranularity;
#ifdef FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES
public:
 // FableWin.pdb GetKey / GetBuffer QAE (W4EVertexType@@JJKW4EVertexBufferPool@@); BuildNewBuffer 0x00BF3380 out of line;
 // GetBuffer's unreferenced out-of-line copy is 0x00BCA510.
 FABLE_FORCEINLINE void GetKey(unsigned long* key, EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool)
 {
  key[0] = type;
  key[1] = (stride << 16) + count;
  key[2] = (pool << 16) + usage;
 }
 FABLE_FORCEINLINE CEngineDynamic3DResource<CVertexBufferWin32,3>* GetBuffer(EVertexType type, long count, long stride, unsigned long usage,
                                                                         EVertexBufferPool pool)
 {
  long size = ((count - 1) / SizeGranularity + 1) * SizeGranularity;
  count = size < 0x10000 ? size : 0x10000;
  unsigned long key[3];
  GetKey(key, type, count, stride, usage, pool);
  CEngineDynamic3DResource<CVertexBufferWin32,3>* resource = GetResource(key);
  if (!resource) {
   resource = BuildNewBuffer(key, type, count, stride, usage, pool);
   if (resource)
    AddNewResource(resource, key);
  }
  return resource;
 }
 CEngineDynamic3DResource<CVertexBufferWin32,3>* BuildNewBuffer(unsigned long* key, EVertexType type, long count, long stride,
                                                                unsigned long usage, EVertexBufferPool pool);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineVertexBufferPool)==28);
#endif
