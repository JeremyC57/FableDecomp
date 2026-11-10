// C3DAnimationInfo::BuildAnimationInstance -- create and cache a per-mesh animation with memory recovery.
// Retail: 0x00A26AD0, 383 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?BuildAnimationInstance@C3DAnimationInfo@@ABE?AV?$CCountedPointer@VC3DAnimation2@@@@ABVC3DMesh2@@@Z
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DANIMATIONINFO_BUILD
#define FABLE_RETAIL_C3DANIMATIONINFO
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCE_RESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/CBankFileAsyncData.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/C3DAnimation2.h"
#include "engine/C3DAnimFileXLoader.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/C3DMesh2.h"
inline bool C3DAnimationInfo::RecoverAnimationMemory(unsigned long step)const {
 switch(step){
 case 0:{CMeshDataBank* pbank=GetBank();pbank->GetAnimationResourceBank().GetResourceList().ShrinkResources(0);return true;}
 case 1:{CMeshDataBank* pbank=GetBank();CMovableResourceMemoryPool* panimation_pool=pbank->GetAnimationMemoryPool();panimation_pool->Defragment();return true;}
 }
 return false;
}
CCountedPointer<C3DAnimation2> C3DAnimationInfo::BuildAnimationInstance(const C3DMesh2& mesh)const {
 if(!IsLoaded())BuildSharedAnimation();
 unsigned long datatype=GetDataType();
 bool fit_to_mesh=(datatype&0x80)==0x80;
 CCountedPointer<C3DAnimation2> animation;
 float match;
 animation.Reset(new C3DAnimation2);
 animation->SetMemoryPool(GetBank()->GetAnimationMemoryPool());
 for(long c_step=0;;++c_step) {
  if(animation->InitialiseAnimationInstance(*Data->SharedAnimation,mesh,fit_to_mesh,&match))break;
  if(!RecoverAnimationMemory(c_step))break;
 }
 Data->Animations[mesh.GetMeshID()]=animation;
 return animation;
}