// CEngineSceneGridCell::DeleteOcclusionGroup -- clear layer group handle and optionally reset member primitive occlusion histories. Retail00b92450 complete172bytes. Shared flattened primitive headers restore native fields; virtual-only interface restores slot27. No cell/handle header. Callee00b2fa40 resets occlusion record through manager01436e58.
#ifndef _MSC_VER
#define __forceinline inline
struct CEngineSceneGridCell;
struct CEngineInternalPrimitiveBase {void* __vftable;unsigned char _pad_0x04[sizeof(long)],_pad_0x08[sizeof(long)];unsigned long EnginePrimitiveFlags;float AlphaMipBias,FadeDistance_FadeBeginDistance,FadeDistance_MaxDrawDistance;unsigned long OcclusionLayerBitMask;CEngineSceneGridCell* GridCell;unsigned char Other[16];CEngineInternalPrimitiveBase* NextPrimitive;void* RefPrimitive;CEngineInternalPrimitiveBase* NextLayerMask;void* RefLayerMask;};
#else
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#endif
struct CEnginePrimitiveHandle;
struct CPersistentPrimitiveMethods {virtual ~CPersistentPrimitiveMethods();virtual void Delete()const;virtual void ReservedSlot2();virtual void NotifyReleased(const CEnginePrimitiveHandle&);long IVRefCount;long Type;};
template<class T> struct CIVCountedPointer {T* m_p;
 long GetRefCount()const{return m_p?m_p->IVRefCount:0;}
 T* operator->()const{return m_p;}
 void Release(){T* p=m_p;if(p){if(--p->IVRefCount==0)p->Delete();m_p=0;}}
 CIVCountedPointer& operator=(T* p){Release();m_p=p;return *this;}
};
struct CEnginePrimitiveHandle {void* VTable;CIVCountedPointer<CPersistentPrimitiveMethods> Data;
 __forceinline void Clear(){if(Data.GetRefCount()==1)Data->NotifyReleased(*this);Data=0;}
};
struct CEnginePrimitiveOcclusionInfo;
struct CNativeOcclusionBase {CEngineInternalPrimitiveBase Base;CEnginePrimitiveOcclusionInfo* OcclusionInfo[2];};
struct CPrimitiveVirtuals {
 virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual void s16();virtual void s17();virtual void s18();virtual void s19();virtual void s20();virtual void s21();virtual void s22();virtual void s23();virtual void s24();virtual void s25();virtual void s26();virtual CNativeOcclusionBase* GetAsOcclusionPrimitive();
};
#ifndef _MSC_VER
struct CEnginePrimitiveOcclusionManager {unsigned char Header[12];unsigned long PrimitiveOccludeDelay;};
#endif
struct CEnginePrimitiveOcclusionManagerMethods:CEnginePrimitiveOcclusionManager {void Reset(CEnginePrimitiveOcclusionInfo*);}; //00b2fa40
extern CEnginePrimitiveOcclusionManagerMethods* g_pPrimitiveOcclusionManager; //01436e58
struct CEngineSceneGridCell {CEngineSceneGridCell* Parent;CEngineInternalPrimitiveBase* FirstPrimitive;float BoundingBox[6],DrawDistance;CEnginePrimitiveHandle GroupPrimitiveHandle[2];long LocalCount;unsigned long TotalCount,RenderMask,OccludedLayers;unsigned char Flags,OccludedFrames,PreloadFrames;void DeleteOcclusionGroup(long layer,bool reset);};
void CEngineSceneGridCell::DeleteOcclusionGroup(long layer,bool reset){
 unsigned long mask;OccludedLayers=OccludedLayers&~(mask=1UL<<(layer+1));GroupPrimitiveHandle[layer].Clear();
 if(reset){for(CEngineInternalPrimitiveBase* p=FirstPrimitive;p;p=p->NextPrimitive){
  if(reinterpret_cast<CPersistentPrimitiveMethods*>(p)->Type!=0x29){
   CNativeOcclusionBase* occ=reinterpret_cast<CPrimitiveVirtuals*>(p)->GetAsOcclusionPrimitive();
   if(occ && (p->OcclusionLayerBitMask&mask) && layer!=-1){CEnginePrimitiveOcclusionInfo* info=occ->OcclusionInfo[layer];if(info)g_pPrimitiveOcclusionManager->Reset(info);}
  }
 }}
}

#include <cstdio>
#include <cstring>
#include <cstddef>
struct CEnginePrimitiveOcclusionInfo {int ID;};
static int fail=0,notified=0,deleted=0,resets=0,ids[20];
static CPersistentPrimitiveMethods* replaceTarget=0;
CPersistentPrimitiveMethods::~CPersistentPrimitiveMethods(){}
void CPersistentPrimitiveMethods::Delete()const{++deleted;}
void CPersistentPrimitiveMethods::ReservedSlot2(){}
void CPersistentPrimitiveMethods::NotifyReleased(const CEnginePrimitiveHandle& h){++notified;if(replaceTarget)const_cast<CEnginePrimitiveHandle&>(h).Data.m_p=replaceTarget;}
struct TestPrimitive {CEngineInternalPrimitiveBase Base;CNativeOcclusionBase* Result;int Gets;};
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
CEnginePrimitiveOcclusionManagerMethods* g_pPrimitiveOcclusionManager=0;
void CEnginePrimitiveOcclusionManagerMethods::Reset(CEnginePrimitiveOcclusionInfo* p){ids[resets++]=p->ID;}
static void check(bool b){if(!b)++fail;}
int main(){CPrimitiveVirtuals vt;CEnginePrimitiveOcclusionManagerMethods mgr;g_pPrimitiveOcclusionManager=&mgr;CEngineSceneGridCell cell;std::memset(&cell,0,sizeof(cell));TestPrimitive p[5];CNativeOcclusionBase occ[5];CEnginePrimitiveOcclusionInfo infos[5];
 for(int i=0;i<5;++i){std::memset(&p[i],0,sizeof(p[i]));std::memset(&occ[i],0,sizeof(occ[i]));p[i].Base.__vftable=*reinterpret_cast<void**>(&vt);p[i].Base.NextPrimitive=i==4?0:&p[i+1].Base;p[i].Result=&occ[i];reinterpret_cast<CPersistentPrimitiveMethods*>(&p[i].Base)->Type=i==0?0x29:7;p[i].Base.OcclusionLayerBitMask=i==3?0:6;infos[i].ID=i;occ[i].OcclusionInfo[0]=&infos[i];occ[i].OcclusionInfo[1]=&infos[i];}p[2].Result=0;occ[4].OcclusionInfo[0]=0;
 cell.FirstPrimitive=&p[0].Base;CPersistentPrimitiveMethods old,other;old.IVRefCount=2;other.IVRefCount=2;cell.GroupPrimitiveHandle[0].Data.m_p=&old;cell.GroupPrimitiveHandle[1].Data.m_p=&other;cell.OccludedLayers=31;
 cell.DeleteOcclusionGroup(0,false);check(cell.OccludedLayers==29&&old.IVRefCount==1&&other.IVRefCount==2&&!cell.GroupPrimitiveHandle[0].Data.m_p&&resets==0&&deleted==0&&notified==0);
 cell.GroupPrimitiveHandle[0].Data.m_p=&old;cell.OccludedLayers=31;cell.DeleteOcclusionGroup(0,true);check(notified==1&&deleted==1&&old.IVRefCount==0&&resets==1&&ids[0]==1&&cell.OccludedLayers==29);check(p[0].Gets==0&&p[1].Gets==1&&p[2].Gets==1&&p[3].Gets==1&&p[4].Gets==1);
 resets=0;cell.DeleteOcclusionGroup(1,true);check(other.IVRefCount==1&&cell.OccludedLayers==25&&resets==2&&ids[0]==1&&ids[1]==4&&!cell.GroupPrimitiveHandle[1].Data.m_p);
 old.IVRefCount=1;other.IVRefCount=3;replaceTarget=&other;cell.GroupPrimitiveHandle[0].Data.m_p=&old;cell.DeleteOcclusionGroup(0,false);check(old.IVRefCount==1&&other.IVRefCount==2&&notified==2&&deleted==1&&!cell.GroupPrimitiveHandle[0].Data.m_p);replaceTarget=0;
 cell.FirstPrimitive=0;cell.DeleteOcclusionGroup(0,true);
#ifndef _MSC_VER
 check(offsetof(CEngineInternalPrimitiveBase,EnginePrimitiveFlags)>=sizeof(void*)+2*sizeof(long));
#endif
#ifdef _MSC_VER
 check(sizeof(CEnginePrimitiveHandle)==8&&offsetof(CEngineSceneGridCell,OccludedLayers)==0x40&&offsetof(CNativeOcclusionBase,OcclusionInfo)==0x48);
#endif
 if(fail){std::printf("00b92450_TEST FAIL %d\n",fail);return 1;}std::printf("00b92450_TEST PASS\n");return 0;}