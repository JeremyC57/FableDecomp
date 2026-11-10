#include <stdio.h>
#include <assert.h>
#include <vector>
#ifndef _MSC_VER
#define __fastcall
#endif
enum EEngineOcclusionLayer{OCCLUSION_LAYER_NONE=-1,OCCLUSION_LAYER_PRIMARY_VIEW=0,OCCLUSION_LAYER_SHADOW_BUFFER=1,OCCLUSION_LAYER_MAX=2};
inline unsigned long __fastcall GFGetOcclusionLayerBitMask(EEngineOcclusionLayer layer){return 1UL<<(layer+1);}
class CEnginePrimitiveHandle;class CEngineSceneGridCell;class CEngineSceneGrid{};
static int groups_created,groups_deleted,infos_created,infos_deleted,notified,added,invalidated,occluded;
static bool preset_info,preset_disabled;
class CIVCountedPointeeBase{public:mutable unsigned long IVRefCount;CIVCountedPointeeBase():IVRefCount(1){}virtual~CIVCountedPointeeBase(){}virtual void Delete()const{delete this;}long Release()const{return --IVRefCount;}long GetRefCount()const{return IVRefCount;}};
template<class T>class CIVCountedPointer{CIVCountedPointeeBase*Object;public:CIVCountedPointer():Object(0){}~CIVCountedPointer(){Release();}void Release(){CIVCountedPointeeBase*object=Object;if(object){if(object->Release()==0)object->Delete();Object=0;}}void Reset(T*object){Release();Object=object;}T*operator->()const{return static_cast<T*>(Object);}T&operator*()const{return *static_cast<T*>(Object);}bool IsNull()const{return Object==0;}unsigned long GetRefCount()const{return Object?Object->GetRefCount():0;}};
class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase{public:virtual void NotifyReleased(const CEnginePrimitiveHandle&){++notified;}};
class CEnginePrimitiveHandle{CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;public:virtual~CEnginePrimitiveHandle(){if(!Data.IsNull()&&Data->GetRefCount()==1)Data->NotifyReleased(*this);}bool IsNull()const{return Data.IsNull();}CEnginePersistentPrimitiveBase&GetData()const{return *Data;}void Clear(){if(Data.GetRefCount()==1)Data->NotifyReleased(*this);Data.Reset(0);}void SetData(CEnginePersistentPrimitiveBase*data){Clear();Data.Reset(data);}};
class CEnginePrimitiveOcclusionInfo:public CIVCountedPointeeBase{public:CEnginePrimitiveOcclusionInfo(){++infos_created;}virtual~CEnginePrimitiveOcclusionInfo(){++infos_deleted;}};
class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase{public:unsigned long EnginePrimitiveFlags,OcclusionLayerBitMask;CEngineSceneGridCell*GridCell;CEngineInternalPrimitiveBase*NextPrimitive;void SetGridCell(CEngineSceneGridCell*cell){GridCell=cell;}void AddToList(CEngineInternalPrimitiveBase**list){NextPrimitive=*list;*list=this;++added;}};
class CEngineInternalPrimitiveOcclusionBase:public CEngineInternalPrimitiveBase{public:CIVCountedPointer<CEnginePrimitiveOcclusionInfo>OcclusionInfo[2];void SetupOcclusionInfo(EEngineOcclusionLayer layer){if(layer!=OCCLUSION_LAYER_NONE&&!(EnginePrimitiveFlags&0x4000)&&OcclusionInfo[layer].IsNull())OcclusionInfo[layer].Reset(new CEnginePrimitiveOcclusionInfo);}CEnginePrimitiveOcclusionInfo&GetOcclusionInfo(EEngineOcclusionLayer layer)const{return *OcclusionInfo[layer];}};
class CEngineInternalPrimitiveOcclusionGroup:public CEngineInternalPrimitiveOcclusionBase{public:CEngineSceneGridCell*cell_arg;CEngineSceneGrid*grid_arg;unsigned long x_arg,y_arg;CEngineInternalPrimitiveOcclusionGroup(CEngineSceneGridCell*cell,CEngineSceneGrid*grid,unsigned long x,unsigned long y):cell_arg(cell),grid_arg(grid),x_arg(x),y_arg(y){++groups_created;EnginePrimitiveFlags=preset_disabled?0x4000:0;OcclusionLayerBitMask=0;GridCell=0;NextPrimitive=0;if(preset_info){OcclusionInfo[0].Reset(new CEnginePrimitiveOcclusionInfo);OcclusionInfo[1].Reset(new CEnginePrimitiveOcclusionInfo);}}virtual~CEngineInternalPrimitiveOcclusionGroup(){++groups_deleted;}};
class CEnginePrimitiveOcclusionManager{public:void SetAsOccluded(CEnginePrimitiveOcclusionInfo&)const{++occluded;}};
namespace NEngineInternal{CEnginePrimitiveOcclusionManager manager;CEnginePrimitiveOcclusionManager*GPPrimitiveOcclusionManager=&manager;}
inline CEngineInternalPrimitiveBase&__fastcall GFGetPrimitiveFromHandle(const CEnginePrimitiveHandle&handle){return static_cast<CEngineInternalPrimitiveBase&>(handle.GetData());}
class CEngineSceneGridCell{public:CEngineSceneGridCell*Parent;CEngineInternalPrimitiveBase*FirstPrimitive;CEnginePrimitiveHandle GroupPrimitiveHandle[2];long LocalCount;unsigned long OccludedLayers;unsigned char BoundingBoxValid:1,TotalCountValid:1,RenderMaskValid:1;CEngineSceneGridCell():Parent(0),FirstPrimitive(0),LocalCount(0),OccludedLayers(0),BoundingBoxValid(0),TotalCountValid(0),RenderMaskValid(0){}bool IsBoundingBoxValid()const{return BoundingBoxValid;}bool IsTotalCountValid()const{return TotalCountValid;}bool IsRenderMaskValid()const{return RenderMaskValid;}void InvalidateAll(){++invalidated;BoundingBoxValid=TotalCountValid=RenderMaskValid=0;}CEngineInternalPrimitiveOcclusionGroup&GetGroupPrimitive(EEngineOcclusionLayer layer)const{return static_cast<CEngineInternalPrimitiveOcclusionGroup&>(GroupPrimitiveHandle[layer].GetData());}void AddPrimitive(CEngineInternalPrimitiveBase&primitive){primitive.SetGridCell(this);primitive.AddToList(&FirstPrimitive);++LocalCount;if(IsBoundingBoxValid()||IsTotalCountValid()||IsRenderMaskValid()){BoundingBoxValid=0;TotalCountValid=0;RenderMaskValid=0;if(Parent)Parent->InvalidateAll();}}void CreateOcclusionGroup(EEngineOcclusionLayer,CEngineSceneGrid*,unsigned long,unsigned long);};
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
int main(){for(int layer=0;layer<2;++layer)for(int flags=0;flags<8;++flags)for(int previous=0;previous<2;++previous)for(int existing=0;existing<2;++existing)for(int disabled=0;disabled<2;++disabled){if(disabled&&!previous)continue;
 preset_info=previous!=0;preset_disabled=disabled!=0;int before_groups=groups_created,before_infos=infos_created,before_occluded=occluded,before_add=added,before_invalid=invalidated;
 {CEngineSceneGridCell root,parent,child;CEngineSceneGrid grid;parent.Parent=&root;child.Parent=&parent;parent.BoundingBoxValid=(flags&1)!=0;parent.TotalCountValid=(flags&2)!=0;parent.RenderMaskValid=(flags&4)!=0;child.OccludedLayers=16;
 if(existing){child.GroupPrimitiveHandle[layer].SetData(new CEngineInternalPrimitiveOcclusionGroup(&child,&grid,8,9));before_groups=groups_created;before_infos=infos_created;}
 child.CreateOcclusionGroup((EEngineOcclusionLayer)layer,&grid,8,9);CEngineInternalPrimitiveOcclusionGroup&group=child.GetGroupPrimitive((EEngineOcclusionLayer)layer);assert(group.cell_arg==&child&&group.grid_arg==&grid&&group.x_arg==8&&group.y_arg==9);
 if(existing){assert(groups_created==before_groups&&infos_created==before_infos&&occluded==before_occluded&&added==before_add&&child.OccludedLayers==16);}else{assert(groups_created==before_groups+1);assert(infos_created==before_infos+(previous?2:1));assert(occluded==before_occluded+1&&added==before_add+1);assert(group.GridCell==&parent&&parent.FirstPrimitive==&group&&parent.LocalCount==1);assert(group.OcclusionLayerBitMask==(1UL<<(layer+1)));assert(child.OccludedLayers==(16UL|(1UL<<(layer+1))));assert(!parent.BoundingBoxValid&&!parent.TotalCountValid&&!parent.RenderMaskValid);assert(invalidated==before_invalid+(flags?1:0));}
 }
 assert(groups_created==groups_deleted&&infos_created==infos_deleted);
}puts("00b92300_TEST PASS");return 0;}