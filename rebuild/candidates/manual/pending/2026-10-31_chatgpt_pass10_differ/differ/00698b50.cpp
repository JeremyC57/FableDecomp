// 00698b50 CGameCameraManager::PushUpManualCameraList
// Leaf: ?PushUpManualCameraList@CGameCameraManager@@QAEXXZ
// Flags:
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
 __forceinline void PlayCameraSoundThroughObservedThing(const CCharString&sound)const{
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
    if(list.begin()==index){
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
