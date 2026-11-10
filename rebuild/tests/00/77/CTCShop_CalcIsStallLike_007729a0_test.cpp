// 007729a0 exact selected body with isolated definition/list dependency fixtures.
#include <stdio.h>
#include <assert.h>
#include <list>
static int reads=0;
enum ETCInterfaceType{TCI_DOOR=38};
class CTCThingOwner;
class CThing { public:
 bool dead,door,hasOwner;CTCThingOwner* Owner;mutable int Lookups;
 CThing():dead(false),door(false),hasOwner(false),Owner(0),Lookups(0){}
 bool IsDead()const{return dead;}bool HasTC(ETCInterfaceType)const{return door;}
 unsigned long GetDefGlobalIndex()const{return 17;}
 bool GetTC(CTCThingOwner** out){++Lookups;if(!hasOwner)return false;*out=Owner;return true;}
};
template<class T>class CIntelligentPointer {T* P;public:
 CIntelligentPointer(T* p):P(p){}T* Get()const{++reads;return P;}
 T* operator->()const{return Get();}bool operator!=(int)const{return Get()!=0;}
};
template<class T>bool IS_THING_ALIVE(const T& x){if(x!=0){if(!x->IsDead())return true;}return false;}
class CTCThingOwner { public:
 std::list<CIntelligentPointer<CThing> > ObjectList;
 std::list<CIntelligentPointer<CThing> >& GetObjectList(){return ObjectList;}
};
struct Counted {mutable int Refs,Releases;Counted():Refs(1),Releases(0){}};
template<class T>class CDefPointer {
 T* Object;
public:
 CDefPointer():Object(0){}~CDefPointer(){if(Object){--Object->Refs;++Object->Releases;}}
 T* operator->()const{return Object;}
 void Set(T* p){assert(!Object);Object=p;if(p)++p->Refs;}
};
class CShopDef:public Counted{ public:bool IsStockDisplayPermanent;};
class CThingBaseDef:public Counted{ public:
 bool Found;const CShopDef* Shop;mutable int Queries;
 CThingBaseDef():Found(false),Shop(0),Queries(0){}
 bool GetSubDef(CDefPointer<const CShopDef>* out)const{++Queries;if(!Found)return false;out->Set(Shop);return true;}
};
class CGameDefinitionManager{ public:
 const CThingBaseDef* Def;mutable int Calls;
 CGameDefinitionManager():Def(0),Calls(0){}
 bool GetDef(long index,CDefPointer<const CThingBaseDef>* out)const{assert(index==17);++Calls;out->Set(Def);return true;}
};
class CTCShop{ public:
 CThing* ParentThing;const CGameDefinitionManager* Manager;
 const CGameDefinitionManager& GetDefinitionManager()const{return *Manager;}
 CThing& GetParentThing(){return *ParentThing;}
 bool CalcIsStallLike();
};
bool CTCShop::CalcIsStallLike()
{
 CDefPointer<const CThingBaseDef> pdef;
 GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(),&pdef);
 CDefPointer<const CShopDef> ShopDef;
 if(pdef->GetSubDef(&ShopDef) && ShopDef->IsStockDisplayPermanent)return false;
 CTCThingOwner* tc_owner;
 if(GetParentThing().GetTC(&tc_owner)){
  const std::list<CIntelligentPointer<CThing> >& objects=tc_owner->GetObjectList();
  for(std::list<CIntelligentPointer<CThing> >::const_iterator i=objects.begin();i!=objects.end();++i){
   if(IS_THING_ALIVE(*i) && (*i)->HasTC(TCI_DOOR))return false;
  }
 }
 return true;
}

int main(){unsigned cases=0;
 for(int n=0;n<=4;++n){int variants=1;for(int a=0;a<n;++a)variants*=4;
 for(int code=0;code<variants;++code)for(int defstate=0;defstate<3;++defstate)for(int owner=0;owner<2;++owner){
  CThing parent,things[4];CTCThingOwner owned;CShopDef def;CThingBaseDef base;CGameDefinitionManager manager;CTCShop shop;
  parent.hasOwner=owner!=0;parent.Owner=&owned;base.Found=defstate!=0;base.Shop=&def;def.IsStockDisplayPermanent=defstate==2;
  manager.Def=&base;shop.Manager=&manager;shop.ParentThing=&parent;
  bool foundDoor=false;int expectedReads=0,value=code;
  for(int k=0;k<n;++k){int state=value%4;value/=4;things[k].dead=state==1;things[k].door=state==3;
   owned.ObjectList.push_back(CIntelligentPointer<CThing>(state?&things[k]:0));
   if(!foundDoor){expectedReads+=state==0?1:(state==1?2:3);if(state==3)foundDoor=true;}
  }
  reads=0;bool result=shop.CalcIsStallLike();bool early=defstate==2;
  assert(result==(!early && (!owner || !foundDoor)));
  assert(reads==(!early&&owner?expectedReads:0));assert(parent.Lookups==(early?0:1));
  assert(manager.Calls==1&&base.Queries==1);assert(base.Refs==1&&base.Releases==1);
  assert(def.Refs==1&&def.Releases==(defstate?1:0));++cases;
 }}
 assert(cases==2046);puts("007729a0_TEST PASS");return 0;
}