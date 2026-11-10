// CEngineInternalAttachedPrimitive::Update
// Inline-budget: plain C++ calls handle assignment and SetData; retail expands both release paths.
// Flags: /d2inlS100
// Retail: 0x00b396e0, 497 bytes, thiscall; original field and call evidence in NAMING_EVIDENCE.md.
// Leaf: ?Update@CEngineInternalAttachedPrimitive@@QAE_NABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABVCEnginePrimitiveHandle@@JK@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#include "engine/CEngineInternalAttachedPrimitive.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;
bool CEngineInternalAttachedPrimitive::Update(const CEnginePrimitive& src,const CEnginePrimitiveAttachmentInfo& attachment_info,const CEnginePrimitiveHandle& parent,long group_index,unsigned long flags)
{
 AttachmentInfo=attachment_info;
 ChildInverseTransformValid=false;
 ParentHandle=parent;
 ParentGroupIndex=group_index;
 if(!GetParentPrimitive())return false;
 ParentType=GetParentType(GetParentPrimitive()->GetType());
 if(ParentType==PARENT_TYPE_NULL)return false;
 EEnginePrimitiveType type=src.GetType();
 if(OutputPrimitive.IsNull()||OutputPrimitive.GetType()!=type){
  CEnginePrimitiveManager* prim_manager=g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(type);
  if(!prim_manager)return false;
  CEngineInternalPrimitiveBase* internal_prim=prim_manager->CreateInternalPrimitive(type);
  OutputPrimitive.SetData(internal_prim);
  prim_manager->AddPrimitive(*internal_prim,0);
 }
 CEngineInternalPrimitiveBase& output_prim=GFGetPrimitiveFromHandle(OutputPrimitive);
 output_prim.Update(src,flags);
 output_prim.SetEnable(false);
 if(!output_prim.GetObjectTransform(PrimitiveTransform))return false;
 UndoMatrixConversion(PrimitiveTransform);
 return true;
}