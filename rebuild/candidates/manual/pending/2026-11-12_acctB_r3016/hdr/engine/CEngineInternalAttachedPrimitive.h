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
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_UPDATE
 bool GetParentWorldSpaceBoundingSphereAnimatedMesh(C3DBoundingSphere&,float);
 bool GetParentWorldSpaceBoundingSphere(C3DBoundingSphere& dest,float interp);
 CEngineInternalPrimitiveBase& GetOutputPrimitive(){return static_cast<CEngineInternalPrimitiveBase&>(OutputPrimitive.GetData());}
 void UpdateOutputPrimitive(float);
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALATTACHEDPRIMITIVE_TRANSFORMS
 // Chunk 2006 (row 0x00B399A0 UpdateOutputPrimitive): Ego_r.pdb / FableWin.pdb members of the same TU (retail order 0x00B38E30
 // RedoMatrixConversion, 0x00B39080 GetParentTransformAnimatedMesh, 0x00B393D0 GetChildInverseTransformStaticMesh, 0x00B398E0
 // GetParentTransform, 0x00B39900 GetChildInverseTransform). Declarations only: no layout change.
 bool GetParentTransform(CMatrix3x4&,float);                       // 0x00B398E0
 bool GetParentTransformAnimatedMesh(CMatrix3x4&,float);           // 0x00B39080
 bool GetChildInverseTransform();                                  // 0x00B39900
 bool GetChildInverseTransformStaticMesh(CMatrix3x4&);             // 0x00B393D0
#ifndef FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_UPDATE
 CEngineInternalPrimitiveBase& GetOutputPrimitive(){return static_cast<CEngineInternalPrimitiveBase&>(OutputPrimitive.GetData());}
 void UpdateOutputPrimitive(float);
#endif
#endif
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
