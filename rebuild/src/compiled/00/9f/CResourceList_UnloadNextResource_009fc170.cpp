// 009fc170 CResourceList::UnloadNextResource
// Leaf: ?UnloadNextResource@CResourceList@@QAE_NK@Z
// Flags: 
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
 void ListInsert(CResource*prev,CResource*next){PrevResource=prev;NextResource=next;PrevResource->NextResource=this;NextResource->PrevResource=this;}
};
class CFailedAllocationHandler{public:virtual~CFailedAllocationHandler();virtual bool HandleFailedAllocation(unsigned long)=0;};
class CResourceList:public CFailedAllocationHandler{public:virtual bool HandleFailedAllocation(unsigned long);private:CResource Head;unsigned long ResourceCount;long AllocatedMemory,MaximumMemory;unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;public:bool UnloadNextResource(unsigned long);};
FABLE_STATIC_ASSERT(sizeof(CResource)==sizeof(CResourceFields));
FABLE_STATIC_ASSERT(sizeof(CResourceList)==sizeof(CResourceListFields));
#pragma auto_inline(off)
void CResource::ListRemove(){
 if(NextResource){
  if(PrevResource->NextResource==this){if(NextResource->PrevResource==this){PrevResource->NextResource=NextResource;NextResource->PrevResource=PrevResource;}}
  PrevResource=0;NextResource=0;
 }
}
#pragma auto_inline(on)
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
  resource->ListRemove();resource->ListInsert(Head.PrevResource,&Head);if(unloaded)resource->LastUsedFrame=CurrentFrame;
  ++UnloadedThisFrame;
 }
 return true;
}