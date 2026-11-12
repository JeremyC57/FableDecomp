#ifndef FABLE_ENGINE_CENGINESCENEGRIDCELL_H
#define FABLE_ENGINE_CENGINESCENEGRIDCELL_H
#include "engine/C3DBoundingBox.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEngineInternalPrimitiveOcclusionGroup.h"
class CEngineSceneGrid;
inline CEngineInternalPrimitiveBase& FABLE_FASTCALL GFGetPrimitiveFromHandle(const CEnginePrimitiveHandle& handle){return static_cast<CEngineInternalPrimitiveBase&>(handle.GetData());}
class CEngineSceneGridCell {
public:~CEngineSceneGridCell();bool IsBoundingBoxValid()const{return BoundingBoxValid;}
private:
 CEngineSceneGridCell* Parent;CEngineInternalPrimitiveBase* FirstPrimitive;C3DBoundingBox BoundingBox;float DrawDistance;
 CEnginePrimitiveHandle GroupPrimitiveHandle[2];long LocalCount;unsigned long TotalCount,RenderMask,OccludedLayers;
 unsigned char BoundingBoxValid:1,TotalCountValid:1,RenderMaskValid:1,WasRendered:1,HasOcclusionResults:1;unsigned char OccludedFrames,PreloadFrames;
 bool IsTotalCountValid()const{return TotalCountValid;}bool IsRenderMaskValid()const{return RenderMaskValid;}
 void InvalidateAll();
 CEngineInternalPrimitiveOcclusionGroup& GetGroupPrimitive(EEngineOcclusionLayer layer)const{return static_cast<CEngineInternalPrimitiveOcclusionGroup&>(GroupPrimitiveHandle[layer].GetData());}
 void AddPrimitive(CEngineInternalPrimitiveBase& primitive){primitive.SetGridCell(this);primitive.AddToList(&FirstPrimitive);++LocalCount;if(IsBoundingBoxValid()||IsTotalCountValid()||IsRenderMaskValid()){BoundingBoxValid=0;TotalCountValid=0;RenderMaskValid=0;if(Parent)Parent->InvalidateAll();}}
 void CreateOcclusionGroup(EEngineOcclusionLayer,CEngineSceneGrid*,unsigned long,unsigned long);
#ifdef FABLE_RETAIL_CENGINESCENEGRIDCELL_CHECKVISIBILITYRESULTS
 // Retail opt-in (chunk F2, row 0x00B939E0 CEngineSceneGrid::CheckVisibilityResults): Ego_r.pdb
 // ?CheckVisibilityResults@CEngineSceneGridCell@@AAE_NW4EEngineOcclusionLayer@@KK_N@Z (private; retail 0x00B936E0, landed). The grid
 // reads the cell's private members and calls its private methods (CreateOcclusionGroup is private too), so it is a friend.
 // Declarations only: no layout change.
 bool CheckVisibilityResults(EEngineOcclusionLayer,unsigned long,unsigned long,bool);   // 0x00B936E0
 friend class CEngineSceneGrid;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineSceneGridCell)==0x48);
#endif
