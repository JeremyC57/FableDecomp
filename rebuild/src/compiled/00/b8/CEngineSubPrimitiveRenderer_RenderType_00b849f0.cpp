// CEngineSubPrimitiveRenderer::RenderType(int type, bool clear) -- bookmark the render state, update the alpha comparison, dispatch the type's sorted list (bounding-volume pass through each primitive's manager, else the one- or two-pass list render), optionally clear it
// Leaf: ?RenderType@CEngineSubPrimitiveRenderer@@QAEXH_N@Z
// Callees (bounding-volume pass): the virtual CEnginePrimitiveManager::RenderBoundingVolume(CEngineSubPrimitiveBase&), vtable
//   slot 6 (+0x18) (Ego_r.pdb ?RenderBoundingVolume@CEnginePrimitiveManager@@UAEXAAVCEngineSubPrimitiveBase@@@Z), called
//   through the primitive's manager pointer (+0x14) as an ordinary virtual call (the landed source read the slot through a
//   memcpy'd pointer to member).
// Actual headers retained; native list24 vs donor28 from CArray12 vs16.
#define CEngineSubPrimitiveRenderer CSubRendererDonor
#define CRenderStateManager CRenderStateManagerDonor
#define CRenderManagerCore CRenderManagerCoreDonor
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CRenderStateManager.h"
#include "engine/CRenderManagerCore.h"
#undef CRenderManagerCore
#undef CRenderStateManager
#undef CEngineSubPrimitiveRenderer
struct CSubState {unsigned Current,Desired,BookmarkMask,Index;bool Dirty;unsigned char Type,Stage,Pad;};
struct CSubStateStack {CSubState* State;unsigned OldValue;};
struct CSubRenderStateManager {
 void* Vtable;void* Device;CSubStateStack Stack[1024];CSubState* Updates[256];CSubState* AllStates[256];int StackSize,UpdateSize,AllSize;unsigned BookmarkMask;
 CSubState DeviceStates[231];CSubState BookmarkDummy;unsigned InitFlags;
 void PopBookmark(); //A05840
 void PushBookmark(){Stack[StackSize].State=&BookmarkDummy;++StackSize;BookmarkMask<<=1;}
 __forceinline void Set(CSubState& state,unsigned value){if(state.Desired!=value){if(!(state.BookmarkMask&BookmarkMask)){state.BookmarkMask|=BookmarkMask;Stack[StackSize].State=&state;Stack[StackSize].OldValue=state.Desired;++StackSize;}state.Desired=value;if(!state.Dirty){state.Dirty=true;Updates[UpdateSize]=&state;++UpdateSize;}}}
};
class CEngineSubPrimitiveBase;
class CEnginePrimitiveManager { public:virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();virtual void v5();
 virtual void RenderBoundingVolume(CEngineSubPrimitiveBase&); // slot 6 (+0x18)
 void* Scenes;bool Enable;};
class CEngineSubPrimitiveBase {public:void* Vtable;unsigned RenderFlags;unsigned char Alpha;void* Parent;CEngineSubPrimitiveBase* Next;CEnginePrimitiveManager* Manager;};
struct CSubSortElement {unsigned SortValue;CEngineSubPrimitiveBase* Primitive;};
struct CSubList {CSubSortElement* Begin;CSubSortElement* End;CSubSortElement* Capacity;int RenderPass,PreRenderPass;bool BoundingVolumePass,Stable,Sorted,Enable;unsigned Size()const{return unsigned(End-Begin);}};
class CEngineSubPrimitiveRenderer { public:
 unsigned char Prefix[0x18];CSubList Lists[15];
 void SortList(CSubList&,bool);void RenderList(CSubList&,int);void RenderListTwoPass(CSubList&,int,int);void DeleteList(CSubList&);
 void RenderType(int,bool);
};
extern CSubRenderStateManager* gSubBookmarkManager; //13CB508
extern CSubRenderStateManager* gSubRenderManager; //1436E18
extern unsigned gSubAlphaTestFunc; //1396DE8
#ifdef _MSC_VER
typedef char State20[sizeof(CSubState)==20?1:-1];
typedef char List24[sizeof(CSubList)==24?1:-1];
typedef char Stack2808[offsetof(CSubRenderStateManager,StackSize)==0x2808?1:-1];
typedef char Dummy3A24[offsetof(CSubRenderStateManager,BookmarkDummy)==0x3a24?1:-1];
#endif
void CEngineSubPrimitiveRenderer::RenderType(int type,bool clear){
 gSubBookmarkManager->PushBookmark();gSubRenderManager->Set(gSubRenderManager->DeviceStates[7],gSubAlphaTestFunc);
 CSubList& list=Lists[type];if(list.Enable){if(list.Sorted)SortList(list,list.Stable);
 if(list.BoundingVolumePass){unsigned size=list.Size();for(unsigned i=0;i<size;++i){CEngineSubPrimitiveBase* p=list.Begin[i].Primitive;p->Manager->RenderBoundingVolume(*p);}}
 else if(list.PreRenderPass)RenderListTwoPass(list,list.PreRenderPass,list.RenderPass);else RenderList(list,list.RenderPass);}
 if(clear)DeleteList(list);gSubBookmarkManager->PopBookmark();
}