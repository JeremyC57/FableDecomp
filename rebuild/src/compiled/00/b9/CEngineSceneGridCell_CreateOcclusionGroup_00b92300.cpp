// CEngineSceneGridCell::CreateOcclusionGroup -- attach an occlusion group to the parent cell.
// Inline-budget: S91 keeps SetData as a call (274 bytes); S92 first expands SetData and SetupOcclusionInfo exactly as native (335 bytes).
// Flags: /d2inlS92
// Retail: 00b92300,335 bytes,private thiscall with layer, grid and two coordinates.
// Leaf: ?CreateOcclusionGroup@CEngineSceneGridCell@@AAEXW4EEngineOcclusionLayer@@PAVCEngineSceneGrid@@KK@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_OCCLUSION
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_SET
#include "engine/CEngineSceneGridCell.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
namespace NEngineInternal {extern CEnginePrimitiveOcclusionManager* GPPrimitiveOcclusionManager;}
void CEngineSceneGridCell::CreateOcclusionGroup(EEngineOcclusionLayer occ_layer,CEngineSceneGrid* grid,unsigned long x,unsigned long y){
 CEnginePrimitiveHandle& group_handle=GroupPrimitiveHandle[occ_layer];
 if(group_handle.IsNull()){
  unsigned long occlusion_mask=GFGetOcclusionLayerBitMask(occ_layer);
  group_handle.SetData(new CEngineInternalPrimitiveOcclusionGroup(this,grid,x,y));
  CEngineInternalPrimitiveOcclusionGroup& group=GetGroupPrimitive(occ_layer);
  group.SetupOcclusionInfo(occ_layer);
  NEngineInternal::GPPrimitiveOcclusionManager->SetAsOccluded(group.GetOcclusionInfo(occ_layer));
  group.OcclusionLayerBitMask=occlusion_mask;
  Parent->AddPrimitive(GFGetPrimitiveFromHandle(group_handle));
  OccludedLayers|=occlusion_mask;
 }
}