// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero() -- turn the sheriff's guard on the hero: best-enemy target, killable, damageable, pushable, movement in actions; release this guard's own entity handle and its slot in the parent script's seh_Guards
// Grade: DIFFER(186v187) -- the seh_Guards[ID] address: retail mov edx,[esi+30h] ; mov eax,[ecx+7Ch] ; shl edx,4 ; lea esi,[eax+edx]; built mov esi,[esi+30h] ; mov edx,[ecx+7Ch] ; shl esi,4 ; add esi,edx (register choice, 1 byte).
// Next step: 12 index/pointer/reference forms give the same allocation; try the real CV_MurderTwistFinalScript TU neighbours
//   (the other CMurderTwistSheriffGuard members before AttackHero) and the generated-script macro shape for the release pair.
// Leaf: ?AttackHero@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@AAEXXZ
// Retail: Fable.exe @ 0x00EB3BA0, 187 bytes (tail jmp to CScriptGameResourceObjectScriptedThingBase::Release).
// Model: ChatGPT pass 10 attempt 7 (shared NScript/CScriptThing headers, retail seh_Guards at +0x7C), unchanged apart from this header.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void*Data;};
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;CCountedPointer(const CCountedPointer&other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}};
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
inline CBaseClass::CBaseClass(){}
inline CScriptThing::CScriptThing(const CScriptThing&other):CBaseClass(),PImp(other.PImp){}
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*begin(){return this->_M_start;}T&operator[](unsigned int n){return *(begin()+n);}};
}
namespace NScript{
class CQ_SunnyvaleMasterData;
// Layout: shared level base0x48; donor seh_Guards0x94 reconciles to native0x7c (base -0x14 and Guards vector -4).
class CV_MurderTwistFinalScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{unsigned char _pad_0x48[0x7c-0x48];public:std::vector<CScriptGameResourceObjectScriptedThingBase>seh_Guards;class CMurderTwistSheriffGuard;};
class CV_MurderTwistFinalScript::CMurderTwistSheriffGuard:public CEntityScriptBase{
protected:CV_MurderTwistFinalScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;
private:CScriptGameResourceObjectScriptedThingBase MyEntityHandle;unsigned long NextLine;unsigned long ID;void AttackHero();
};
void CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::AttackHero(){
 Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
 Game->EntitySetAsKillable(Me,true,true);
 Game->EntitySetAsDamageable(Me,true);
 Game->SetIsPushableByHero(Me,true);
 Game->EntitySetAsUseMovementInActions(Me,true);
 if(MyEntityHandle.IsValid())MyEntityHandle.Release();
 CScriptGameResourceObjectScriptedThingBase*handle=&ParentClass.seh_Guards[ID];
 if(handle->IsValid())handle->Release();
}
}
