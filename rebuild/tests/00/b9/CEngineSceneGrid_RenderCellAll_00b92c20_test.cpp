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
 unsigned long GetCellRenderMask(unsigned long,unsigned long); //00b91dc0
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

#include <cstdio>
#include <cstring>
#include <cstddef>
class CEngineSceneRenderStats { public:unsigned long Guard;};
CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo=0;unsigned short g_engineFrameNumber=0;
static int fail=0,renderCount=0,maskCount=0,ids[20],managerIds[20];static unsigned long coords[20];static CEngineInternalPrimitiveBase* primitives=0;static CEnginePrimitiveManager* expectedManagers[2];static CEnginePrimitiveTypeInfo* replacement=0;
static void check(bool b){if(!b)++fail;}
unsigned long CEngineSceneGrid::GetCellRenderMask(unsigned long x,unsigned long y){coords[maskCount++]=x+10*y;return GridCells.Data[y*Size+x]->RenderMask;}
void CEnginePrimitiveManager::v00(){}
void CEnginePrimitiveManager::v01(){}
void CEnginePrimitiveManager::v02(){}
void CEnginePrimitiveManager::v03(){}
void CEnginePrimitiveManager::v04(){}
void CEnginePrimitiveManager::v05(){}
void CEnginePrimitiveManager::v06(){}
ERendered CEnginePrimitiveManager::RenderPrimitive(CEngineInternalPrimitiveBase& p,EEngineRenderLayer layer,CEngineSubPrimitiveChain& chain){int id=int(&p-primitives);ids[renderCount]=id;managerIds[renderCount++]=this==expectedManagers[0]?0:1;check(layer==1&&chain.First==0&&chain.Last==0);chain.First=&p;chain.Last=&p;if(id==0){g_pEnginePrimitiveTypeInfo=replacement;g_engineFrameNumber=0x3456;}return static_cast<ERendered>(id==0||id==4?2:(id==3?0:1));}
int main(){CEnginePrimitiveTypeInfo ti,ti2;std::memset(&ti,0,sizeof(ti));std::memset(&ti2,0,sizeof(ti2));CEnginePrimitiveManager m0,m1;expectedManagers[0]=&m0;expectedManagers[1]=&m1;ti.PrimitiveManagers[0]=&m0;ti.PrimitiveManagers[1]=&m0;ti2.PrimitiveManagers[0]=&m1;ti2.PrimitiveManagers[1]=&m1;replacement=&ti2;g_pEnginePrimitiveTypeInfo=&ti;g_engineFrameNumber=99;
 CEngineInternalPrimitiveBase p[6];primitives=p;std::memset(p,0,sizeof(p));for(int i=0;i<6;++i){*reinterpret_cast<unsigned long*>(p[i]._pad_0x08)=i%2;p[i].RenderLayerMask=1;p[i].LastFrameRendered=77;}p[0].NextPrimitive=&p[1];p[1].RenderLayerMask=2;p[1].NextPrimitive=&p[2];p[1].NextLayerMask=&p[3];
 CEngineSceneGrid grid,down;std::memset(&grid,0,sizeof(grid));std::memset(&down,0,sizeof(down));CEngineSceneGridCell top,c[4];std::memset(&top,0,sizeof(top));std::memset(c,0,sizeof(c));CEngineSceneGridCell* cells[1]={&top};CEngineSceneGridCell* children[4]={&c[0],&c[1],0,&c[3]};grid.Size=1;grid.GridCells.Data=cells;grid.LevelDown=&down;down.Size=2;down.GridCells.Data=children;top.FirstPrimitive=&p[0];top.RenderMask=1;top.Flags=0xa1;c[0].FirstPrimitive=&p[4];c[0].RenderMask=1;c[0].Flags=0x40;c[1].RenderMask=2;c[1].Flags=3;c[3].FirstPrimitive=&p[5];c[3].RenderMask=1;c[3].Flags=0x10;CEngineSceneRenderStats stats;stats.Guard=0x12345678;
 grid.RenderCellAll(0,0,static_cast<EEngineRenderLayer>(1),stats);check(renderCount==4&&ids[0]==0&&ids[1]==3&&ids[2]==4&&ids[3]==5);check(managerIds[0]==0&&managerIds[1]==1&&managerIds[2]==1&&managerIds[3]==1);check(p[0].LastFrameRendered==0x3456&&p[4].LastFrameRendered==0x3456);check(p[1].LastFrameRendered==77&&p[2].LastFrameRendered==77&&p[3].LastFrameRendered==77&&p[5].LastFrameRendered==77);check(top.Flags==0xa9&&c[0].Flags==0x48&&c[1].Flags==3&&c[3].Flags==0x18);check(maskCount==4&&coords[0]==0&&coords[1]==0&&coords[2]==1&&coords[3]==11&&stats.Guard==0x12345678);
 renderCount=maskCount=0;top.RenderMask=2;grid.RenderCellAll(0,0,static_cast<EEngineRenderLayer>(1),stats);check(maskCount==1&&renderCount==0);cells[0]=0;grid.RenderCellAll(0,0,static_cast<EEngineRenderLayer>(1),stats);check(maskCount==1);
#ifdef _MSC_VER
 check(offsetof(CEngineSceneGrid,GridCells)==0x14&&offsetof(CEngineSceneGridCell,Flags)==0x44&&sizeof(CEngineSubPrimitiveChain)==8);
#endif
 if(fail){std::printf("00b92c20_TEST FAIL %d\n",fail);return 1;}std::printf("00b92c20_TEST PASS\n");return 0;}