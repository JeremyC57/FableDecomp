// CGameCameraManager::PushUpManualCameraList() -- manual camera control: while the observed thing is alive and no script module is pending or running, step the camera-list index back one mode (error sound at the start of the list, push-up sound otherwise)
// Leaf: ?PushUpManualCameraList@CGameCameraManager@@QAEXXZ
// Retail: Fable.exe @ 0x00698B50, 218 bytes. Ego_r: ?PushUpManualCameraList@CGameCameraManager@@QAEXXZ (sibling PushDown 0x00698C30 landed).
//   Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, PreparePendingMode 0x00698840, CCharString(const char*, long) / dtor,
//   the anonymous-namespace PlayCameraSoundThroughObservedThing 0x00698A30 (fastcall).
// Shape: retail loads the list's begin into ecx before the index (mov ecx,[esi+108h] ; mov eax,[esi+114h] ; cmp eax,ecx): the begin
//   iterator is a materialised temporary compared by const reference, as an iterator object's operator==(const iterator&) does. With
//   STLport 4.6's pointer iterator that is "iterator const& first = CameraList.begin(); if (CameraListIndex == first)"; comparing
//   begin() directly folds it into the memory operand (216v218, ChatGPT pass 10).
// Flags:
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#define CGameCameraManager CGameCameraManagerLayout
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#define CThing CThingLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
class CThing:public CThingLayout{public:
 // Layout: Dead is bit0 of native91; the shared byte groups it under CreatedBySerialisation.
 bool IsDead()const{return (CreatedBySerialisation[0]&1)!=0;}
};
class CThingGameObject;class CBaseObject;class CBaseObjectPointer;
class CBaseIntelligentPointer{CBaseObjectPointer*PData;public:virtual ~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*Get()const{return reinterpret_cast<T*>(GetPItem());}T*operator->()const{return Get();}};
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
namespace NCameraModes{class CScriptModuleBase{public:virtual ~CScriptModuleBase();virtual void Update()=0;virtual bool IsFinished()const;};}
class CCameraModeDef;class CCameraManagerDef;
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T*iterator;iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}};
}
namespace {void __fastcall PlayCameraSoundThroughObservedThing(const CThing*,const CCameraManagerDef&,const CCharString&);}
// Layout: all storage comes from supplied CGameCameraManager; original private PreparePendingMode restored.
class CGameCameraManager:public CGameCameraManagerLayout{void PreparePendingMode(CDefPointer<const CCameraModeDef>);public:void PushUpManualCameraList();
 FABLE_FORCEINLINE void PlayCameraSoundThroughObservedThing(const CCharString&sound)const{
 ::PlayCameraSoundThroughObservedThing(
 reinterpret_cast<const CThing*>(reinterpret_cast<const CBaseIntelligentPointer*>(PObservedThing)->GetPItem()),
 *reinterpret_cast<const CCameraManagerDef*>(reinterpret_cast<const CDefPointer<const CCameraManagerDef>*>(&PDef_Object)->InternalGetObject()),sound);
 }
};
void CGameCameraManager::PushUpManualCameraList(){
 BlockModeChange=false;UsingCameraManually=true;
 CIntelligentPointer<const CThingGameObject>&observed=*reinterpret_cast<CIntelligentPointer<const CThingGameObject>*>(PObservedThing);
 if(observed.Get()&&!reinterpret_cast<const CThing*>(observed.Get())->IsDead()){
  if(!PPendingScriptModule_Data){
   if(!PCurrentScriptModule_Data||reinterpret_cast<const NCameraModes::CScriptModuleBase*>(PCurrentScriptModule_Data)->IsFinished()){
    typedef CDefPointer<const CCameraModeDef>Mode;
    std::vector<Mode>&list=*reinterpret_cast<std::vector<Mode>*>(CameraList);
    Mode*&index=*reinterpret_cast<Mode**>(CameraListIndex);
    Mode*const&first=list.begin();
    if(index==first){
     CCharString sound("CAMERA_LIST_PUSH_ERROR",-1);
     PlayCameraSoundThroughObservedThing(sound);
    }else{
     --index;PreparePendingMode(*index);
     CCharString sound("CAMERA_LIST_PUSH_UP",-1);
     PlayCameraSoundThroughObservedThing(sound);
    }
   }
  }
 }
}