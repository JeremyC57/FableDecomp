// 008f7860 NCombatActionHelpers::IsToCreateCombatParticleEffects
// Leaf: ?IsToCreateCombatParticleEffects@NCombatActionHelpers@@YI_NABVCThing@@PBV2@@Z
// Flags:
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CDefClassBase.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CClass CClassFields
#include "engine/CClass.h"
#undef CClass
#define CThingBaseDef CThingBaseDefFields
#include "engine/_quarantine/CThingBaseDef.h"
#undef CThingBaseDef
#include "engine/CWorld.h"
class CDefString{long TablePos;public:CDefString(){}bool operator==(const char*)const;};
// Layout: flattened shared CClass storage, original byte Type10 and ulong accessor.
class CClass:public CClassFields{public:unsigned long GetType()const{return Type;}};
// Layout: only original polymorphic base used; shared full def projection documents derived storage.
class CThingBaseDef:public CDefClassBase{};
class CThing:public CThingFields{public:
 bool IsDead()const{return (CreatedBySerialisation[0]&1)!=0;}
 const CThingBaseDef&PeekBaseDef()const{return *reinterpret_cast<const CThingBaseDef*>(PBaseDef_Object);}
};
namespace boost{template<class T>class scoped_ptr{T*ptr;public:T&operator*()const{return *ptr;}};}
// Layout: original PWorld scoped_ptr24; original protected singleton and public accessors.
class CMainGameComponent{protected:static CMainGameComponent*PMainGameComponent;unsigned char _pad_0[0x24];boost::scoped_ptr<CWorld>PWorld;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}CWorld&GetWorld(){return *PWorld;}};
namespace NGlobalConsole{extern bool CombatParticleEffects;}
namespace NCombatActionHelpers{
bool __fastcall IsToCreateCombatParticleEffects(const CThing&thing,const CThing*target){
 if(!NGlobalConsole::CombatParticleEffects)return false;
 if(reinterpret_cast<const CGameScriptInterfaceBase*>(CMainGameComponent::Get()->GetWorld().PGameScriptInterface_ptr)->IsInMovieSequence())return true;
 bool result;
 if(reinterpret_cast<const CClass&>(thing).GetType()==2)result=true;
 else if(!target || target->IsDead() || reinterpret_cast<const CClass&>(*target).GetType()==2)result=true;
 else result=(thing.PeekBaseDef().GetInstantiationName()=="CREATURE_DRAGON_01")?true:false;
 return result;
}
}