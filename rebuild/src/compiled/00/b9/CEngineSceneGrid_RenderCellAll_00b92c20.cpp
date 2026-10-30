// CEngineSceneGrid::RenderCellAll -- mask-guarded primitive dispatch then recurse four child cells and mark rendered. Retail00b92c20 complete237bytes. Uses shared native primitive/typeinfo headers; no scene-grid/cell/stats headers. Layout matches landed GetCellRenderMask B91DC0. Direct calleeB91DC0 and recursiveB92C20, virtualmanager slot7RenderPrimitive. Globals1436e84typeinfo/143b8d8frame.
#ifndef _MSC_VER
class CEngineInternalPrimitiveBase { public:void* __vftable;unsigned char _pad_0x04[sizeof(long)],_pad_0x08[sizeof(long)];unsigned long EnginePrimitiveFlags;float AlphaMipBias,FadeDistance_FadeBeginDistance,FadeDistance_MaxDrawDistance;unsigned long OcclusionLayerBitMask;void* GridCell;unsigned char GridPos[4];unsigned char Other[8];unsigned long RenderLayerMask;unsigned char PostRenderEnable[2];unsigned short LastFrameRendered;CEngineInternalPrimitiveBase* NextPrimitive;void* RefPrimitive;CEngineInternalPrimitiveBase* NextLayerMask;void* RefLayerMask;};
class CEnginePrimitiveManager;class CEnginePrimitiveTypeInfo { public:void* Head;CEnginePrimitiveManager* PrimitiveManagers[47];};
#else
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#endif
enum EEngineRenderLayer {ENGINE_RENDER_LAYER_DEFAULT=0};
enum ERendered {RENDERED_NOTHING=0,RENDERED_SUB_PRIMITIVES=2};
class CEngineSubPrimitiveChain { public:void* First;void* Last;CEngineSubPrimitiveChain():First(0),Last(0){}};
class CEnginePrimitiveManager { public:virtual void v00();virtual void v01();virtual void v02();virtual void v03();virtual void v04();virtual void v05();virtual void v06();virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo; //01436e84
extern unsigned short g_engineFrameNumber; //0143b8d8
class CEngineSceneRenderStats;
class CEngineSceneGridCell { public:CEngineSceneGridCell* Parent;CEngineInternalPrimitiveBase* FirstPrimitive;unsigned char BoundsAndGroups[0x2c];unsigned long LocalCount,TotalCount,RenderMask,OccludedLayers;unsigned char Flags,OccludedFrames,PreloadFrames;};
template<class T>class CArray { public:T* Data;long Size,Capacity;};
class CEngineSceneGrid { public:unsigned long Base,Level;CEngineSceneGrid *LevelUp,*LevelDown;unsigned long Size;CArray<CEngineSceneGridCell*> GridCells;
private: unsigned long GetCellRenderMask(unsigned long,unsigned long); public: //00b91dc0 (Ego_r: private)
 void RenderCellAll(unsigned long x,unsigned long y,EEngineRenderLayer layer,CEngineSceneRenderStats& stats);
};
void CEngineSceneGrid::RenderCellAll(unsigned long x,unsigned long y,EEngineRenderLayer layer,CEngineSceneRenderStats& stats){
 CEngineSceneGridCell* cell=GridCells.Data[y*Size+x];
 if(cell && (GetCellRenderMask(x,y)&layer)){
  CEngineInternalPrimitiveBase* p=cell->FirstPrimitive;
  while(p){if(p->RenderLayerMask&layer){
   unsigned long type=*reinterpret_cast<unsigned long*>(p->_pad_0x08);
   CEnginePrimitiveManager* manager=g_pEnginePrimitiveTypeInfo->PrimitiveManagers[type];
   CEngineSubPrimitiveChain chain;
   if(manager->RenderPrimitive(*p,layer,chain)==RENDERED_SUB_PRIMITIVES)p->LastFrameRendered=g_engineFrameNumber;
   p=p->NextPrimitive;
  }else p=p->NextLayerMask;}
  if(LevelDown){for(unsigned long i=0;i<2;++i)for(unsigned long j=0;j<2;++j)LevelDown->RenderCellAll(x*2+j,y*2+i,layer,stats);}
  cell->Flags|=8;
 }
}