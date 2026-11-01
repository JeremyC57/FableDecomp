// Behavior-only fixture: the selected target body is pasted unchanged.
// Unused layout tails and interfaces are reduced. Flattened byte fields are
// represented by pointers to actual constructed objects. Native offsets remain
// separate evidence; this fixture does not assert native parity.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
static int cases,getCalls,queries,prepares,sounds,ctors,dtors,adds,releases;
#define CHECK(x) do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
class CBaseObject{public:virtual ~CBaseObject(){}};
class CThing:public CBaseObject{public:unsigned char CreatedBySerialisation[1];bool IsDead()const{return(CreatedBySerialisation[0]&1)!=0;}};
class CThingGameObject:public CThing{};
class CBaseObjectPointer{public:CBaseObject*Object;};
class CBaseIntelligentPointer{CBaseObjectPointer*PData;public:CBaseIntelligentPointer(CBaseObjectPointer*p):PData(p){}virtual~CBaseIntelligentPointer(){}CBaseObject*GetPItem()const{++getCalls;return PData?PData->Object:0;}};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(CBaseObjectPointer*p):CBaseIntelligentPointer(p){}T*Get()const{return reinterpret_cast<T*>(GetPItem());}T*operator->()const{return Get();}};
class CDefPointeeBase{public:unsigned long RefCount;CDefPointeeBase():RefCount(1){}virtual~CDefPointeeBase(){}virtual void Delete(){CHECK(false);}void AddRef(){++RefCount;++adds;}unsigned long Release(){++releases;return --RefCount;}};
class CCameraModeDef:public CDefPointeeBase{};class CCameraManagerDef:public CDefPointeeBase{};
enum ENullDef{NULL_DEF};
template<class T>class CDefPointer{public:CDefPointeeBase*Object;CDefPointer(ENullDef=NULL_DEF):Object(0){}CDefPointer(const CDefPointer&o):Object(o.Object){if(Object)Object->AddRef();}~CDefPointer(){Release();}CDefPointeeBase*InternalGetObject()const{return Object;}void Release(){if(Object){CDefPointeeBase*object=Object;if(object->Release()==0)object->Delete();Object=0;}}};
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T*iterator;iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}};}
namespace NCameraModes{class CScriptModuleBase{public:virtual~CScriptModuleBase(){}virtual void Update()=0;virtual bool IsFinished()const;};}
class CTestScript:public NCameraModes::CScriptModuleBase{public:bool Finished;void Update(){};};
bool NCameraModes::CScriptModuleBase::IsFinished()const{++queries;return static_cast<const CTestScript*>(this)->Finished;}
class CCharString{public:const char*Text;CCharString(const char*p,long n=-1):Text(p){CHECK(n==-1);++ctors;}~CCharString(){++dtors;}};
class CGameCameraManager;static CGameCameraManager*expectedManager;static CDefPointeeBase*expectedDef;static const CThing*expectedThing;static CCameraManagerDef*expectedManagerDef;static bool expectedError;
namespace{void __fastcall PlayCameraSoundThroughObservedThing(const CThing*thing,const CCameraManagerDef&def,const CCharString&sound){CHECK(thing==expectedThing&&&def==expectedManagerDef);CHECK(strcmp(sound.Text,expectedError?"CAMERA_LIST_PUSH_ERROR":"CAMERA_LIST_PUSH_UP")==0);++sounds;}}
class CGameCameraManager{public:bool BlockModeChange,UsingCameraManually;void*PObservedThing;NCameraModes::CScriptModuleBase*PPendingScriptModule_Data,*PCurrentScriptModule_Data;void*CameraList;void*CameraListIndex;CDefPointeeBase*&PDef_Object;CGameCameraManager(CDefPointer<const CCameraManagerDef>&d):PDef_Object(d.Object){}
void PreparePendingMode(CDefPointer<const CCameraModeDef>mode){CHECK(this==expectedManager&&mode.Object==expectedDef);if(mode.Object)CHECK(mode.Object->RefCount==3);++prepares;}
void PushUpManualCameraList();
__forceinline void PlayCameraSoundThroughObservedThing(const CCharString&sound)const{
 ::PlayCameraSoundThroughObservedThing(reinterpret_cast<const CThing*>(reinterpret_cast<const CBaseIntelligentPointer*>(PObservedThing)->GetPItem()),*reinterpret_cast<const CCameraManagerDef*>(reinterpret_cast<const CDefPointer<const CCameraManagerDef>*>(&PDef_Object)->InternalGetObject()),sound);
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

int main(){
 for(int count=1;count<=6;++count)for(int at=0;at<=count;++at)for(int observed=0;observed<3;++observed)for(int pending=0;pending<2;++pending)for(int current=0;current<3;++current)for(int nullMode=0;nullMode<2;++nullMode){
  CThingGameObject thing;thing.CreatedBySerialisation[0]=(unsigned char)(observed==2?1:0);
  CBaseObjectPointer info;info.Object=observed?&thing:0;CIntelligentPointer<const CThingGameObject>handle(&info);
  CTestScript script;script.Finished=current==2;CCameraModeDef defs[6];CCameraManagerDef managerDef;
  CDefPointer<const CCameraModeDef>entries[6];for(int i=0;i<count;++i){entries[i].Object=nullMode?0:&defs[i];if(entries[i].Object)entries[i].Object->AddRef();}
  std::vector<CDefPointer<const CCameraModeDef> >list;list._M_start=entries;list._M_finish=entries+count;list._M_end_of_storage._M_data=entries+count;
  CDefPointer<const CCameraManagerDef>definition;definition.Object=&managerDef;managerDef.AddRef();
  CDefPointer<const CCameraModeDef>*index=entries+at;CGameCameraManager manager(definition);
  manager.PObservedThing=&handle;manager.PPendingScriptModule_Data=pending?&script:0;manager.PCurrentScriptModule_Data=current?&script:0;manager.CameraList=&list;manager.CameraListIndex=&index;manager.PDef_Object=&managerDef;manager.BlockModeChange=true;manager.UsingCameraManually=false;
  expectedManager=&manager;expectedThing=&thing;expectedManagerDef=&managerDef;expectedError=at==0;expectedDef=at?entries[at-1].Object:0;
  getCalls=queries=prepares=sounds=ctors=dtors=adds=releases=0;
  manager.PushUpManualCameraList();
  bool active=observed==1&&!pending&&current!=1;
  CHECK(!manager.BlockModeChange&&manager.UsingCameraManually);
  CHECK(index==entries+at-(active&&at?1:0));CHECK(prepares==(active&&at?1:0));CHECK(sounds==(active?1:0));CHECK(ctors==sounds&&dtors==sounds);
  CHECK(queries==(observed==1&&!pending&&current?1:0));CHECK(getCalls==(observed?2:1)+(active?1:0));CHECK(adds==(active&&at&&!nullMode?1:0)&&releases==adds);
  for(int j=0;j<count;++j)CHECK(defs[j].RefCount==(nullMode?1UL:2UL));
  CHECK(managerDef.RefCount==2);++cases;
 }
 CHECK(cases==972);puts("00698b50_TEST PASS");return 0;
}
