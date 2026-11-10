// C3DAnimationInfo::CAnimationData::BuildSharedAnimation -- load and initialise the bank's shared animation with memory recovery.
// Retail: 0x00A24D70, 414 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?BuildSharedAnimation@CAnimationData@C3DAnimationInfo@@QAEXXZ
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
namespace NProgressDisplay {void FABLE_FASTCALL DisplayProgress(const CCharString&,float,bool,bool);}
void C3DAnimationInfo::CAnimationData::BuildSharedAnimation() {
 if(!FileData->IsFinished()) {
  NProgressDisplay::DisplayProgress("Asset Wait: Animation",-1.0f,false,false);
  FileData->WaitUntilFinished();
 }
 CMeshDataBank* bank=AnimInfo->GetBank();
 CCountedPointer<C3DAnimFile> anim_file=AnimInfo->GetAnimLoader().LoadAnimFileCompressed(FileData->GetData(),FileData->GetLength(),*bank->GetAnimationMemoryPool());
 SharedAnimation.Reset(new C3DAnimation2);
 SharedAnimation->SetMemoryPool(bank->GetAnimationMemoryPool());
 unsigned long datatype=AnimInfo->GetDataType();
 EAnimationType animtype=bank->GetAnimationType(datatype);
 for(long c_step=0;;++c_step) {
  if(SharedAnimation->InitialiseSharedAnimation(*anim_file,animtype))break;
  if(!AnimInfo->RecoverAnimationMemory(c_step))break;
 }
 FileData.Reset(0);
 AnimInfo->GetBank()->GetAnimationResourceBank().AddResource(*this);
}