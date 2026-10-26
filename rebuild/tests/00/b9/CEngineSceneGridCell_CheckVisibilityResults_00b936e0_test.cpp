// CEngineSceneGridCell::CheckVisibilityResults -- retain visible primitives and detach newly visible group primitives. Retail00b936e0 complete181bytes; ignored args preserve ABI. Shared fields native, calleeB2FAC0 occluded count, B8FE60 remove, B92450 delete group. Shared flattened primitive headers restore native fields; virtual-only interface restores slot27. No cell/handle header. Callee00b2fa40 resets occlusion record through manager01436e58.
#ifndef _MSC_VER
#define __forceinline inline
class CEngineSceneGridCell;
class CEngineInternalPrimitiveBase { public:void* __vftable;unsigned char _pad_0x04[sizeof(long)],_pad_0x08[sizeof(long)];unsigned long EnginePrimitiveFlags;float AlphaMipBias,FadeDistance_FadeBeginDistance,FadeDistance_MaxDrawDistance;unsigned long OcclusionLayerBitMask;CEngineSceneGridCell* GridCell;unsigned char Other[16];CEngineInternalPrimitiveBase* NextPrimitive;void* RefPrimitive;CEngineInternalPrimitiveBase* NextLayerMask;void* RefLayerMask;};
#else
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineInternalPrimitiveOcclusionGroup.h"
#endif
class CEnginePrimitiveHandle;
struct CPersistentPrimitiveMethods {virtual ~CPersistentPrimitiveMethods();virtual void Delete()const;virtual void ReservedSlot2();virtual void NotifyReleased(const CEnginePrimitiveHandle&);long IVRefCount;long Type;};
template<class T> class CIVCountedPointer { public:T* m_p;
 long GetRefCount()const{return m_p?m_p->IVRefCount:0;}
 T* operator->()const{return m_p;}
 void Release(){T* p=m_p;if(p){if(--p->IVRefCount==0)p->Delete();m_p=0;}}
 CIVCountedPointer& operator=(T* p){Release();m_p=p;return *this;}
};
class CEnginePrimitiveHandle { public:void* VTable;CIVCountedPointer<CPersistentPrimitiveMethods> Data;
 __forceinline void Clear(){if(Data.GetRefCount()==1)Data->NotifyReleased(*this);Data=0;}
};
class CEnginePrimitiveOcclusionInfo;
struct CNativeOcclusionBase {CEngineInternalPrimitiveBase Base;CEnginePrimitiveOcclusionInfo* OcclusionInfo[2];};
struct CPrimitiveVirtuals {
 virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual void s16();virtual void s17();virtual void s18();virtual void s19();virtual void s20();virtual void s21();virtual void s22();virtual void s23();virtual void s24();virtual void s25();virtual void s26();virtual CNativeOcclusionBase* GetAsOcclusionPrimitive();
};
#ifndef _MSC_VER
class CEnginePrimitiveOcclusionManager { public:unsigned char Header[12];unsigned long PrimitiveOccludeDelay;};
#endif
struct CEnginePrimitiveOcclusionManagerMethods:CEnginePrimitiveOcclusionManager {unsigned long GetOccludedFrames(CEnginePrimitiveOcclusionInfo*); //00b2fac0
 __forceinline bool IsVisible(CEnginePrimitiveOcclusionInfo* info){if(GetOccludedFrames(info)>PrimitiveOccludeDelay)return false;return true;}
};
extern CEnginePrimitiveOcclusionManagerMethods* g_pPrimitiveOcclusionManager; //01436e58
class CEngineSceneGridCell { public:CEngineSceneGridCell* Parent;CEngineInternalPrimitiveBase* FirstPrimitive;float BoundingBox[6],DrawDistance;CEnginePrimitiveHandle GroupPrimitiveHandle[2];long LocalCount;unsigned long TotalCount,RenderMask,OccludedLayers;unsigned char Flags,OccludedFrames,PreloadFrames;void DeleteOcclusionGroup(long layer,bool reset);bool CheckVisibilityResults(long layer,unsigned long,unsigned long,bool force);};
struct CPrimitiveMethods:CEngineInternalPrimitiveBase {void RemoveFromList();}; //00b8fe60
struct CNativeOcclusionGroup {CNativeOcclusionBase Base;CEngineSceneGridCell* GridCell;void* Grid;unsigned long GridX,GridY,FramesRendered;};
static __forceinline bool IsLayerVisible(CNativeOcclusionBase* p,long layer){if(layer!=-1){CEnginePrimitiveOcclusionInfo* info=p->OcclusionInfo[layer];if(info && g_pPrimitiveOcclusionManager->IsVisible(info))return true;}return false;}
bool CEngineSceneGridCell::CheckVisibilityResults(long layer,unsigned long,unsigned long,bool force){
 bool visible=false;
 for(CEngineInternalPrimitiveBase* p=FirstPrimitive;p;){
  CEngineInternalPrimitiveBase* next=p->NextPrimitive;
  CNativeOcclusionBase* occ=reinterpret_cast<CPrimitiveVirtuals*>(p)->GetAsOcclusionPrimitive();
  if(occ && (force || (occ->Base.EnginePrimitiveFlags&0x4000) || IsLayerVisible(occ,layer))){
   visible=true;OccludedFrames=0;
   if(reinterpret_cast<CPersistentPrimitiveMethods*>(p)->Type==0x29){
    CNativeOcclusionGroup* group=reinterpret_cast<CNativeOcclusionGroup*>(p);CEngineSceneGridCell* cell=group->GridCell;
    reinterpret_cast<CPrimitiveMethods*>(p)->RemoveFromList();cell->DeleteOcclusionGroup(layer,group->FramesRendered>1);
   }
  }
  p=next;
 }
 if(visible)return true;
 ++OccludedFrames;return false;
}

#include <cstdio>
#include <cstring>
#include <cstddef>
class CEnginePrimitiveOcclusionInfo { public:unsigned long Frames;};
static int fail=0,calls=0,removes=0,deletes=0;static bool expectedReset=false;static long expectedLayer=0;static CEngineSceneGridCell* expectedCell=0;static CEnginePrimitiveOcclusionManagerMethods* swapManager=0;static CEnginePrimitiveOcclusionInfo* swapInfo=0;
CEnginePrimitiveOcclusionManagerMethods* g_pPrimitiveOcclusionManager=0;
static void check(bool b){if(!b)++fail;}
unsigned long CEnginePrimitiveOcclusionManagerMethods::GetOccludedFrames(CEnginePrimitiveOcclusionInfo* p){++calls;if(swapManager&&p==swapInfo)g_pPrimitiveOcclusionManager=swapManager;return p->Frames;}
void CEngineSceneGridCell::DeleteOcclusionGroup(long layer,bool reset){check(this==expectedCell&&layer==expectedLayer&&reset==expectedReset);++deletes;}
void CPrimitiveMethods::RemoveFromList(){++removes;NextPrimitive=0;}
struct TestPrimitive {CNativeOcclusionGroup Group;CNativeOcclusionBase* Result;int Gets;};
void CPrimitiveVirtuals::s00(){}
void CPrimitiveVirtuals::s01(){}
void CPrimitiveVirtuals::s02(){}
void CPrimitiveVirtuals::s03(){}
void CPrimitiveVirtuals::s04(){}
void CPrimitiveVirtuals::s05(){}
void CPrimitiveVirtuals::s06(){}
void CPrimitiveVirtuals::s07(){}
void CPrimitiveVirtuals::s08(){}
void CPrimitiveVirtuals::s09(){}
void CPrimitiveVirtuals::s10(){}
void CPrimitiveVirtuals::s11(){}
void CPrimitiveVirtuals::s12(){}
void CPrimitiveVirtuals::s13(){}
void CPrimitiveVirtuals::s14(){}
void CPrimitiveVirtuals::s15(){}
void CPrimitiveVirtuals::s16(){}
void CPrimitiveVirtuals::s17(){}
void CPrimitiveVirtuals::s18(){}
void CPrimitiveVirtuals::s19(){}
void CPrimitiveVirtuals::s20(){}
void CPrimitiveVirtuals::s21(){}
void CPrimitiveVirtuals::s22(){}
void CPrimitiveVirtuals::s23(){}
void CPrimitiveVirtuals::s24(){}
void CPrimitiveVirtuals::s25(){}
void CPrimitiveVirtuals::s26(){}
CNativeOcclusionBase* CPrimitiveVirtuals::GetAsOcclusionPrimitive(){TestPrimitive* p=reinterpret_cast<TestPrimitive*>(this);++p->Gets;return p->Result;}
int main(){CPrimitiveVirtuals vt;CEngineSceneGridCell cell,target;std::memset(&cell,0,sizeof(cell));CEnginePrimitiveOcclusionManagerMethods mgr,other;mgr.PrimitiveOccludeDelay=3;other.PrimitiveOccludeDelay=0;g_pPrimitiveOcclusionManager=&mgr;cell.OccludedFrames=254;
 check(!cell.CheckVisibilityResults(0,9,10,false)&&cell.OccludedFrames==255);check(!cell.CheckVisibilityResults(0,9,10,false)&&cell.OccludedFrames==0);
 TestPrimitive p[4];CEnginePrimitiveOcclusionInfo info[4];for(int i=0;i<4;++i){std::memset(&p[i],0,sizeof(p[i]));p[i].Group.Base.Base.__vftable=*reinterpret_cast<void**>(&vt);p[i].Result=&p[i].Group.Base;reinterpret_cast<CPersistentPrimitiveMethods*>(&p[i].Group.Base.Base)->Type=i==1?0x29:7;p[i].Group.Base.Base.NextPrimitive=i==3?0:&p[i+1].Group.Base.Base;info[i].Frames=4;p[i].Group.Base.OcclusionInfo[0]=&info[i];p[i].Group.Base.OcclusionInfo[1]=&info[i];p[i].Group.GridCell=&target;p[i].Group.FramesRendered=2;}
 cell.FirstPrimitive=&p[0].Group.Base.Base;check(!cell.CheckVisibilityResults(0,11,12,false)&&cell.OccludedFrames==1&&calls==4&&deletes==0);
 info[1].Frames=3;expectedCell=&target;expectedLayer=0;expectedReset=true;swapManager=&other;swapInfo=&info[1];calls=0;check(cell.CheckVisibilityResults(0,2,3,false)&&cell.OccludedFrames==0&&deletes==1&&removes==1);check(p[2].Gets==2&&p[3].Gets==2);swapManager=0;
 p[1].Group.Base.Base.NextPrimitive=&p[2].Group.Base.Base;g_pPrimitiveOcclusionManager=&mgr;calls=0;p[0].Result=0;p[2].Group.Base.OcclusionInfo[1]=0;p[3].Group.Base.Base.EnginePrimitiveFlags=0x4000;expectedLayer=1;expectedReset=false;p[1].Group.FramesRendered=1;info[1].Frames=4;
 check(cell.CheckVisibilityResults(1,0,0,false)&&calls==1&&deletes==1&&cell.OccludedFrames==0);
 calls=0;check(cell.CheckVisibilityResults(1,0,0,true)&&calls==0&&deletes==2&&removes==2&&cell.OccludedFrames==0);
 p[1].Group.Base.Base.NextPrimitive=&p[2].Group.Base.Base;calls=0;p[3].Group.Base.Base.EnginePrimitiveFlags=0;check(!cell.CheckVisibilityResults(-1,0,0,false)&&calls==0&&cell.OccludedFrames==1);
#ifndef _MSC_VER
 check(offsetof(CEngineInternalPrimitiveBase,EnginePrimitiveFlags)>=sizeof(void*)+2*sizeof(long));
#endif
#ifdef _MSC_VER
 check(offsetof(CNativeOcclusionGroup,GridCell)==0x50&&offsetof(CNativeOcclusionGroup,FramesRendered)==0x60&&offsetof(CEngineSceneGridCell,OccludedFrames)==0x45);
#endif
 if(fail){std::printf("00b936e0_TEST FAIL %d\n",fail);return 1;}std::printf("00b936e0_TEST PASS\n");return 0;}