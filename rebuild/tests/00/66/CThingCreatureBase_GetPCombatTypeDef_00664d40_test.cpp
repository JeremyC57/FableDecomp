#include <cstdio>
#include <cstring>
static int gAdds,gReleases,gDeletes,gLookupsIndex,gLookupsName,gPeekTC,gCombatTypes,gManagers,gStringCtor,gStringDtor;static long gIndex;static bool gIndexOK,gNameOK;
class CDefPointeeBase{public:mutable long IVRefCount;CDefPointeeBase():IVRefCount(0){}virtual ~CDefPointeeBase(){}void AddRef(){++IVRefCount;++gAdds;}virtual void Delete(){++gDeletes;}};
class CDefClassBase:public CDefPointeeBase{};class CCombatTypeDef:public CDefClassBase{};
template<class T>class CDefPointer{public:CDefPointeeBase* Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer& o):Object(o.Object){if(Object)Object->AddRef();}~CDefPointer(){Release();}CDefPointer& operator=(const CDefPointer& o){if(Object!=o.Object){Release();Object=o.Object;if(Object)Object->AddRef();}return *this;}void Release(){if(Object){CDefPointeeBase* object=Object;++gReleases;if(--object->IVRefCount==0)object->Delete();Object=0;}}void Set(CDefPointeeBase* o){if(Object!=o){Release();Object=o;if(Object)Object->AddRef();}}T* Get()const{return static_cast<T*>(Object);}};
class CCharString{public:const char* Text;CCharString(const char* text):Text(text){++gStringCtor;}~CCharString(){++gStringDtor;}};
static CCombatTypeDef gByIndex,gByName;
class CGameDefinitionManager{public:bool GetDef(long index,CDefPointer<const CCombatTypeDef>* def)const{++gLookupsIndex;gIndex=index;if(index>0&&gIndexOK){def->Set(&gByIndex);return true;}return false;}bool GetDef(const CCharString& name,CDefPointer<const CCombatTypeDef>* def)const{++gLookupsName;if(std::strcmp(name.Text,"COMBAT_TYPE_NULL"))return false;if(gNameOK){def->Set(&gByName);return true;}return false;}};
class CTCCombat{public:CDefPointer<const CCombatTypeDef> PCombatTypeDef;CDefPointer<const CCombatTypeDef> GetPCombatTypeDef()const{return PCombatTypeDef;}};
class CThingCreatureBase{public:bool HasCombat;CTCCombat* Combat;long CombatType;CGameDefinitionManager* Manager;bool PeekTC(const CTCCombat** out)const{++gPeekTC;if(HasCombat){*out=Combat;return true;}return false;}long GetCombatType()const{++gCombatTypes;return CombatType;}const CGameDefinitionManager& PeekDefinitionManager()const{++gManagers;return *Manager;}CDefPointer<const CCombatTypeDef> GetPCombatTypeDef()const;};
CDefPointer<const CCombatTypeDef> CThingCreatureBase::GetPCombatTypeDef()const
{
 CDefPointer<const CCombatTypeDef> pdef;
 const CTCCombat* ptc_combat;
 if(PeekTC(&ptc_combat)){
  pdef=ptc_combat->GetPCombatTypeDef();
  if(pdef.Get())return pdef;
 }
 long combat_type=GetCombatType();
 if(combat_type){
  PeekDefinitionManager().GetDef(combat_type,&pdef);
  return pdef;
 }
 PeekDefinitionManager().GetDef("COMBAT_TYPE_NULL",&pdef);
 return pdef;
}

int main(){int ok=1,cases=0;for(int has=0;has<2;++has)for(int component=0;component<2;++component)for(int kind=-1;kind<3;++kind)for(int indexok=0;indexok<2;++indexok)for(int nameok=0;nameok<2;++nameok){CCombatTypeDef componentDef;CDefPointer<const CCombatTypeDef> indexOwner,nameOwner;indexOwner.Set(&gByIndex);nameOwner.Set(&gByName);CTCCombat combat;if(component)combat.PCombatTypeDef.Set(&componentDef);CGameDefinitionManager manager;CThingCreatureBase thing;thing.HasCombat=has!=0;thing.Combat=&combat;thing.CombatType=kind;thing.Manager=&manager;gIndexOK=indexok!=0;gNameOK=nameok!=0;gAdds=gReleases=gDeletes=gLookupsIndex=gLookupsName=gPeekTC=gCombatTypes=gManagers=gStringCtor=gStringDtor=0;gIndex=99;const CCombatTypeDef* expected=has&&component?&componentDef:(kind?(kind>0&&indexok?&gByIndex:0):(nameok?&gByName:0));bool override=has&&component;{CDefPointer<const CCombatTypeDef> result=thing.GetPCombatTypeDef();ok &= result.Get()==expected&&gPeekTC==1&&gCombatTypes==(override?0:1)&&gManagers==(override?0:1)&&gLookupsIndex==(!override&&kind?1:0)&&gLookupsName==(!override&&!kind?1:0)&&gStringCtor==(!override&&!kind?1:0)&&gStringDtor==gStringCtor;ok &= componentDef.IVRefCount==(component?1:0)+(expected==&componentDef?1:0)&&gByIndex.IVRefCount==1+(expected==&gByIndex?1:0)&&gByName.IVRefCount==1+(expected==&gByName?1:0)&&!gDeletes;if(!override&&kind)ok &= gIndex==kind;}ok &= componentDef.IVRefCount==(component?1:0)&&gByIndex.IVRefCount==1&&gByName.IVRefCount==1&&gAdds==gReleases&&!gDeletes;++cases;}
ok &= cases==64;std::printf(ok?"00664d40_TEST PASS\n":"00664d40_TEST FAIL\n");return ok?0:1;}