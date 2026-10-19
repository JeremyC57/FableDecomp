// RenderType: bookmark render state, update alpha comparison, dispatch sorted list.
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
#include <string.h>
struct CSubState {unsigned Current,Desired,BookmarkMask,Index;bool Dirty;unsigned char Type,Stage,Pad;};
struct CSubStateStack {CSubState* State;unsigned OldValue;};
struct CSubRenderStateManager {
 void* Vtable;void* Device;CSubStateStack Stack[1024];CSubState* Updates[256];CSubState* AllStates[256];int StackSize,UpdateSize,AllSize;unsigned BookmarkMask;
 CSubState DeviceStates[231];CSubState BookmarkDummy;unsigned InitFlags;
 void PopBookmark(); //A05840
 void PushBookmark(){Stack[StackSize].State=&BookmarkDummy;++StackSize;BookmarkMask<<=1;}
 __forceinline void Set(CSubState& state,unsigned value){if(state.Desired!=value){if(!(state.BookmarkMask&BookmarkMask)){state.BookmarkMask|=BookmarkMask;Stack[StackSize].State=&state;Stack[StackSize].OldValue=state.Desired;++StackSize;}state.Desired=value;if(!state.Dirty){state.Dirty=true;Updates[UpdateSize]=&state;++UpdateSize;}}}
};
struct CSubPrimitiveManager {void* Vtable;void* Scenes;bool Enable;};
struct CSubPrimitive {void* Vtable;unsigned RenderFlags;unsigned char Alpha;void* Parent;CSubPrimitive* Next;CSubPrimitiveManager* Manager;};
struct CSubSortElement {unsigned SortValue;CSubPrimitive* Primitive;};
struct CSubList {CSubSortElement* Begin;CSubSortElement* End;CSubSortElement* Capacity;int RenderPass,PreRenderPass;bool BoundingVolumePass,Stable,Sorted,Enable;unsigned Size()const{return unsigned(End-Begin);}};
struct CEngineSubPrimitiveRenderer {
 unsigned char Prefix[0x18];CSubList Lists[15];
 void SortList(CSubList&,bool);void RenderList(CSubList&,int);void RenderListTwoPass(CSubList&,int,int);void DeleteList(CSubList&);
 void RenderType(int,bool);
};
extern CSubRenderStateManager* gSubBookmarkManager; //13CB508
extern CSubRenderStateManager* gSubRenderManager; //1436E18
extern unsigned gSubAlphaTestFunc; //1396DE8
// Real virtual thiscall slot6 RenderBoundingVolume; full host PMF representation.
typedef void(CSubPrimitiveManager::*CSubRenderVolumeCall)(CSubPrimitive&);
#ifdef _MSC_VER
typedef char State20[sizeof(CSubState)==20?1:-1];
typedef char List24[sizeof(CSubList)==24?1:-1];
typedef char Stack2808[offsetof(CSubRenderStateManager,StackSize)==0x2808?1:-1];
typedef char Dummy3A24[offsetof(CSubRenderStateManager,BookmarkDummy)==0x3a24?1:-1];
typedef char PMF4[sizeof(CSubRenderVolumeCall)==4?1:-1];
#endif
void CEngineSubPrimitiveRenderer::RenderType(int type,bool clear){
 gSubBookmarkManager->PushBookmark();gSubRenderManager->Set(gSubRenderManager->DeviceStates[7],gSubAlphaTestFunc);
 CSubList& list=Lists[type];if(list.Enable){if(list.Sorted)SortList(list,list.Stable);
 if(list.BoundingVolumePass){unsigned size=list.Size();for(unsigned i=0;i<size;++i){CSubPrimitive* p=list.Begin[i].Primitive;CSubPrimitiveManager* manager=p->Manager;CSubRenderVolumeCall call=0;memcpy(&call,static_cast<const CSubRenderVolumeCall*>(manager->Vtable)+6,sizeof(call));(manager->*call)(*p);}}
 else if(list.PreRenderPass)RenderListTwoPass(list,list.PreRenderPass,list.RenderPass);else RenderList(list,list.RenderPass);}
 if(clear)DeleteList(list);gSubBookmarkManager->PopBookmark();
}