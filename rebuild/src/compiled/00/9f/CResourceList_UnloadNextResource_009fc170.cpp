// 009fc170 CResourceList::UnloadNextResource
// Leaf: ?UnloadNextResource@CResourceList@@QAE_NK@Z
// Flags: 
// NF143 (2026-11-20, q4942 / q4943; one definition): CResource::ListRemove (0x009FBFB0) is declared only (the strong same-TU copy under
//   auto_inline(off) is gone: the call grades the same), and CResource::ListInsert (0x009FBF90) is written out at its one use (retail
//   expands it there; the in-class body emitted a pick-any copy of the owner's leaf ?ListInsert@CResource@@AAEXPAV1@0@Z). The object
//   now defines only its own leaf, so the self rows 0x009FBFB0 / 0x009FBF90 can carry the PDB leaves without LNK2005.
#include <stddef.h>
#include "rebuild_abi.h"
#define CResource CResourceFields
#include "engine/CResource.h"
#undef CResource
#define CResourceList CResourceListFields
#include "engine/CResourceList.h"
#undef CResourceList
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
class CResourceList;
// Layout: shared flattened CResource/CResourceList are checked below; original
// polymorphic bases and private field access from Ego_r TPI resource_types.json.
class CResource:public CIVCountedPointeeBase{public:
 virtual bool UnloadResource();
private:friend class CResourceList;
 CResourceList*ResourceList;CResource*PrevResource,*NextResource;unsigned long ResourceSize,LastUsedFrame;
private:friend class CResourceList;void ListRemove();
};
class CFailedAllocationHandler{public:virtual~CFailedAllocationHandler();virtual bool HandleFailedAllocation(unsigned long)=0;};
class CResourceList:public CFailedAllocationHandler{public:virtual bool HandleFailedAllocation(unsigned long);private:CResource Head;unsigned long ResourceCount;long AllocatedMemory,MaximumMemory;unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;public:bool UnloadNextResource(unsigned long);};
FABLE_STATIC_ASSERT(sizeof(CResource)==sizeof(CResourceFields));
FABLE_STATIC_ASSERT(sizeof(CResourceList)==sizeof(CResourceListFields));
bool CResourceList::UnloadNextResource(unsigned long min_size){
 bool unloaded;
 CResource*resource=Head.NextResource;
 if(resource==&Head||UnloadedThisFrame>=ResourceCount)return false;
 unloaded=false;
 bool exists=true;
 unsigned long unused_frames=CurrentFrame-resource->LastUsedFrame;
 if(unused_frames>=UnloadDelay&&resource->ResourceSize>=min_size){
  resource->AddRef();unloaded=resource->UnloadResource();
  if(resource->Release()==0){resource->Delete();exists=false;}
 }
 if(exists&&resource->ResourceList==this){
  resource->ListRemove();
  // CResource::ListInsert(Head.PrevResource, &Head) (0x009FBF90), expanded here as retail does:
  resource->PrevResource=Head.PrevResource;resource->NextResource=&Head;resource->PrevResource->NextResource=resource;resource->NextResource->PrevResource=resource;
  if(unloaded)resource->LastUsedFrame=CurrentFrame;
  ++UnloadedThisFrame;
 }
 return true;
}