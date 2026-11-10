// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINEINTERNALATTACHEDPRIMITIVE_H
#define FABLE_ENGINE_CENGINEINTERNALATTACHEDPRIMITIVE_H
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitiveAttachmentInfo.h"
#include "engine/CMatrix3x4.h"
inline CEngineInternalPrimitiveBase& FABLE_FASTCALL GFGetPrimitiveFromHandle(const CEnginePrimitiveHandle& handle){return static_cast<CEngineInternalPrimitiveBase&>(handle.GetData());}
class CEngineInternalAttachedPrimitive : public CEngineInternalPrimitiveBase {
public:
 enum EParentType {PARENT_TYPE_NULL,PARENT_TYPE_STATIC_MESH,PARENT_TYPE_ANIMATED_MESH};
 enum EChildType {CHILD_TYPE_NULL,CHILD_TYPE_STATIC_MESH};
 CEngineInternalAttachedPrimitive();
 virtual ~CEngineInternalAttachedPrimitive();
 bool Update(const CEnginePrimitive&,const CEnginePrimitiveAttachmentInfo&,const CEnginePrimitiveHandle&,long,unsigned long);
 EParentType GetParentType(long type)const{
  switch(type){
   case ENGINE_PRIMITIVE_ANIMATING_MESH:
   case ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH:
   case ENGINE_PRIMITIVE_ANIMATING_MESH_FROM_FILE:return PARENT_TYPE_ANIMATED_MESH;
  }
  return PARENT_TYPE_NULL;
 }
 CEnginePersistentPrimitiveBase* GetParentPrimitive(){
  if(ParentGroupIndex!=-1)return GFGetPrimitiveFromHandle(ParentHandle).GetChildPrimitive(ParentGroupIndex);
  return &GFGetPrimitiveFromHandle(ParentHandle);
 }
 void UndoMatrixConversion(CMatrix3x4&)const;
 void RedoMatrixConversion(CMatrix3x4&)const;
 CMatrix3x4 PrimitiveTransform;
 EParentType ParentType;
 EChildType ChildType;
 CEnginePrimitiveHandle ParentHandle;
 long ParentGroupIndex;
 CEnginePrimitiveHandle OutputPrimitive;
 CEnginePrimitiveAttachmentInfo AttachmentInfo;
 CMatrix3x4 ChildInverseTransform;
 bool ChildInverseTransformValid;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalAttachedPrimitive)==220);
#endif
