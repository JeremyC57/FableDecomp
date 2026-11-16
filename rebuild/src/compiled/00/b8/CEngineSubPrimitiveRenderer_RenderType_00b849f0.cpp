// CEngineSubPrimitiveRenderer::RenderType(int type, bool clear) -- bookmark the render state, update the alpha comparison, dispatch the type's sorted list (bounding-volume pass through each primitive's manager, else the one- or two-pass list render), optionally clear it
// Leaf: ?RenderType@CEngineSubPrimitiveRenderer@@QAEXH_N@Z
// Callees (bounding-volume pass): the virtual CEnginePrimitiveManager::RenderBoundingVolume(CEngineSubPrimitiveBase&), vtable
//   slot 6 (+0x18) (Ego_r.pdb ?RenderBoundingVolume@CEnginePrimitiveManager@@UAEXAAVCEngineSubPrimitiveBase@@@Z), called
//   through the primitive's manager pointer (+0x14) as an ordinary virtual call (the landed source read the slot through a
//   memcpy'd pointer to member).
// Actual headers retained; native list24 vs donor28 from CArray12 vs16.
#include "fable_compiler.h"
// NF45 (round 7): the render-state manager is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderStateManager at offset 0) and the state is
//   the shared inline SetAlphaTestFunc (DeviceState.AlphaTestFunc +0x8C, core +0x28A4) [ALPHA_TEST_GREATER] through D3DAlphaFuncModeTable
//   0x01396DD8 (+0x10); the bookmark push / RestoreCaptureBlock pair on 0x013CB508 is a CRenderStateScopedCaptureBlock
//   (engine/CRenderStateCaptureBlock.h). Was a local CSubRenderStateManager / CSubState model on gSubRenderManager / gSubBookmarkManager,
//   gSubAlphaTestFunc, and three shared headers included under renaming macros (dropped).
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
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
#ifdef _MSC_VER
typedef char List24[sizeof(CSubList)==24?1:-1];
#endif
void CEngineSubPrimitiveRenderer::RenderType(int type,bool clear){
 CRenderStateScopedCaptureBlock capture_block;NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
 CSubList& list=Lists[type];if(list.Enable){if(list.Sorted)SortList(list,list.Stable);
 if(list.BoundingVolumePass){unsigned size=list.Size();for(unsigned i=0;i<size;++i){CEngineSubPrimitiveBase* p=list.Begin[i].Primitive;p->Manager->RenderBoundingVolume(*p);}}
 else if(list.PreRenderPass)RenderListTwoPass(list,list.PreRenderPass,list.RenderPass);else RenderList(list,list.RenderPass);}
 if(clear)DeleteList(list);
}