// Behavior test: exact row body, external game lookups stubbed; host containers supply storage.
#include <map>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
static int callsGetDef,callsSubDef,callsList,callsManager,callsDelete;
static void check(bool b) { if(!b) { puts("00868e50_TEST FAIL"); exit(1); } }
class CDefPointeeBase { public: mutable unsigned long IVRefCount; CDefPointeeBase():IVRefCount(1){} virtual ~CDefPointeeBase(){} virtual void Delete() const {++callsDelete;} void AddRef()const{++IVRefCount;} };
template<class T> class CDefPointer {
public:
 CDefPointeeBase* Object;
 CDefPointer():Object(0){} CDefPointer(const CDefPointer& x):Object(x.Object){if(Object)Object->AddRef();}
 ~CDefPointer(){Release();}
 CDefPointer& operator=(const CDefPointer& x){if(Object!=x.Object){Release();Object=x.Object;if(Object)Object->AddRef();}return *this;}
 void Release(){if(Object){CDefPointeeBase* object=Object;if(--object->IVRefCount==0)object->Delete();Object=0;}}
 T* Get()const{return static_cast<T*>(Object);} T* operator->()const{return Get();}
};
class CAppearanceModifierDef:public CDefPointeeBase {public:long ArmourDefIndex;float OveralProtectionWeighting;};
class CArmourDef:public CDefPointeeBase {};
class CThingObjectDef:public CDefPointeeBase {public:CAppearanceModifierDef* modifier;
 template<class T>bool GetSubDef(CDefPointer<const T>* out)const {++callsSubDef;out->Object=modifier;modifier->AddRef();return true;}
};
static CDefPointeeBase* definitions[64];
class CDefinitionManager {public:template<class T>bool GetDef(long index,CDefPointer<const T>* out)const {++callsGetDef;check(index>=0&&index<64);out->Object=definitions[index];check(out->Object!=0);out->Object->AddRef();return true;}};
class CGameDefinitionManager:public CDefinitionManager{};
enum EHeroAttachableAppearanceModifierType { APPEARANCE_HAIR=0, APPEARANCE_HORN=1, APPEARANCE_CLOTHING=2, NO_OF_APPEARANCE_MODIFIER_TYPES=3 };enum ETCInterfaceType { TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS=94 };
class CTCBase {public:virtual ~CTCBase(){}};
class CTCHeroAttachableAppearanceModifiers:public CTCBase {public:std::list<long> worn;
 void GetAppearanceModifierList(std::list<long>& out,EHeroAttachableAppearanceModifierType type)const{++callsList;check((int)type==2);out=worn;}
};
class CThing {public:bool has;CGameDefinitionManager* defs;CTCHeroAttachableAppearanceModifiers* modifiers;
 const CGameDefinitionManager& PeekDefinitionManager()const{++callsManager;return *defs;}
 template<class T>bool PeekTC(ETCInterfaceType id,T** out)const{if(HasTC(id)){*out=static_cast<T*>(modifiers);return true;}return false;}
 bool HasTC(ETCInterfaceType type)const{check((int)type==0x5e);return has;}
 const CTCBase& GetTC(ETCInterfaceType type)const{check((int)type==0x5e&&has);return *modifiers;}
};
template<class T,class V>class CCompareDefPointer {public:bool operator()(const CDefPointer<T>& a,const CDefPointer<T>& b)const{return a.Get()<b.Get();}};
namespace {
void FABLE_FASTCALL GetArmourWeightsFromThing(const CThing& thing,
    std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> >& armour_weights,
    float& total_armour_weight)
{
    const CGameDefinitionManager& def_man=thing.PeekDefinitionManager();
    const CTCHeroAttachableAppearanceModifiers* ptc_armour_clothes;
    if(thing.PeekTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS,&ptc_armour_clothes)) {
        std::list<long> clothes_list;
        ptc_armour_clothes->GetAppearanceModifierList(clothes_list,APPEARANCE_CLOTHING);
        for(std::list<long>::iterator it=clothes_list.begin();it!=clothes_list.end();++it) {
            CDefPointer<const CThingObjectDef> pdef;
            long clothing_item_def_index=*it;
            def_man.GetDef(clothing_item_def_index,&pdef);
            CDefPointer<const CAppearanceModifierDef> pclothing_def;
            pdef->GetSubDef(&pclothing_def);
            if(pclothing_def->ArmourDefIndex>=0) {
                CDefPointer<const CArmourDef> parmour_def;
                def_man.GetDef(pclothing_def->ArmourDefIndex,&parmour_def);
                armour_weights.insert(std::make_pair(parmour_def,pclothing_def->OveralProtectionWeighting));
                total_armour_weight+=pclothing_def->OveralProtectionWeighting;
            }
        }
    }
}
}


int main(){
 for(int mask=0;mask<16;++mask)for(int pattern=0;pattern<8;++pattern)for(int present=0;present<2;++present){
  CArmourDef armour[2];CAppearanceModifierDef appearance[4];CThingObjectDef objects[4];
  CGameDefinitionManager manager;CTCHeroAttachableAppearanceModifiers modifiers;
  definitions[20]=&armour[0];definitions[21]=&armour[1];
  float expected=2.0f;int expectedCount=1,expectedLookups=0,expectedSubs=0;
  for(int k=0;k<4;++k){definitions[k+1]=&objects[k];objects[k].modifier=&appearance[k];
   appearance[k].ArmourDefIndex=(mask&(1<<k))?20+(k%2):-1;
   appearance[k].OveralProtectionWeighting=(float)((pattern+k)%7-3)*0.25f;
   modifiers.worn.push_back(k+1);
   if(k==1)modifiers.worn.push_back(k+1);
  }
  if(present)for(std::list<long>::const_iterator it=modifiers.worn.begin();it!=modifiers.worn.end();++it){
   ++expectedLookups;++expectedSubs;const CAppearanceModifierDef& a=appearance[*it-1];
   if(a.ArmourDefIndex>=0){++expectedLookups;++expectedCount;expected+=a.OveralProtectionWeighting;}}
  CThing thing;thing.has=present!=0;thing.defs=&manager;thing.modifiers=&modifiers;
  callsGetDef=callsSubDef=callsList=callsManager=callsDelete=0;
  {
   typedef std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> > Weights;
   Weights weights;CDefPointer<const CArmourDef> initial;initial.Object=&armour[0];armour[0].AddRef();weights.insert(std::make_pair(initial,2.0f));
   float total=2.0f;GetArmourWeightsFromThing(thing,weights,total);
   check(total==expected);check((int)weights.size()==expectedCount);
   float sum=0;for(Weights::const_iterator w=weights.begin();w!=weights.end();++w){check(w->first.Get()==&armour[0]||w->first.Get()==&armour[1]);sum+=w->second;}
   check(sum==expected);check(callsGetDef==expectedLookups&&callsSubDef==expectedSubs&&callsList==present&&callsManager==1&&callsDelete==0);
  }
  for(int k=0;k<4;++k)check(appearance[k].IVRefCount==1&&objects[k].IVRefCount==1);
  check(armour[0].IVRefCount==1&&armour[1].IVRefCount==1&&callsDelete==0);
 }
 puts("00868e50_TEST PASS");return 0;
}