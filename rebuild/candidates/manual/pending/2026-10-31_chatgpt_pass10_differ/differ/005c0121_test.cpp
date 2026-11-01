// Behavior-only fixture. Target and GetDef bodies are pasted unchanged.
// Unused declarations/layout tails are reduced; real objects and owning handles
// exercise category retention and temporary destruction on both compilers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __declspec(x)
#endif
static int cases,categoryCtors,selectedCalls,defLookups;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
class CDefPointeeBase{public:unsigned long RefCount;CDefPointeeBase():RefCount(1){}virtual~CDefPointeeBase(){}void AddRef(){++RefCount;}long Release(){CHECK(RefCount>0);return(long)--RefCount;}virtual void Delete()const{CHECK(false);}};
class CDefClassBase:public CDefPointeeBase{};class CInventoryCategoryDef:public CDefClassBase{public:long Inventory;bool AddCategoryOnCreate;};
enum ENullDef{NULL_DEF};template<class T>class CDefPointer{public:CDefPointeeBase*Object;CDefPointer(ENullDef=NULL_DEF):Object(0){}CDefPointer(const CDefPointer&o):Object(o.Object){if(Object)Object->AddRef();}~CDefPointer(){Release();}void Release(){if(Object){CDefPointeeBase*object=Object;if(object->Release()==0)object->Delete();Object=0;}}CDefPointer&operator=(const CDefPointer&o){if(Object!=o.Object){Release();Object=o.Object;if(Object)Object->AddRef();}return*this;}CDefPointeeBase*InternalGetObject()const{return Object;}bool IsNull()const{return Object==0;}template<class U>void SetDynamicCast(const CDefPointer<U>&o){if(Object!=o.Object){Release();Object=o.Object;if(Object)Object->AddRef();}}};
class CCharString{public:const char*Text;CCharString(const char*p,long n=-1):Text(p){CHECK(n==-1&&strcmp(p,"INVENTORY_CATEGORY")==0);}~CCharString(){}};
class CDefinitionManager{public:int N;CInventoryCategoryDef*Defs;long GetNoInstantiatedDefsOfType(const CCharString&)const{return N+1;}long GetGlobalIndexFromClassIndex(const CCharString&,long i)const{CHECK(i>=1&&i<=N);return i;}CDefPointer<const CDefClassBase>GetPDefFromGlobalIndex(long i)const{CHECK(i>=1&&i<=N);++defLookups;CDefPointer<const CDefClassBase>r;r.Object=Defs+i-1;r.Object->AddRef();return r;}template<class T>bool GetDef(long,CDefPointer<const T>*)const;};class CGameDefinitionManager:public CDefinitionManager{};
static CGameDefinitionManager*expectedManager;
class CThing{public:short CurrentMapNumber;long GetCurrentMapNumber()const{return CurrentMapNumber;}};
class CTCBase{protected:CThing&ParentThing;public:CTCBase(CThing&t):ParentThing(t){}CThing&GetParentThing(){return ParentThing;}protected:const CGameDefinitionManager&GetDefinitionManager()const{return *expectedManager;}};
namespace NInventory{class CTCInventoryBase;class CCategory{public:CDefPointer<const CInventoryCategoryDef>Def;CTCInventoryBase*Owner;long Index;bool Selected;CCategory():Owner(0),Index(0),Selected(false){}CCategory(CDefPointer<const CInventoryCategoryDef>d,CTCInventoryBase*o,long i):Def(d),Owner(o),Index(i),Selected(false){++categoryCtors;}void SetAsSelected(bool x){CHECK(x);Selected=x;++selectedCalls;}};}
namespace std{template<class T>class vector{public:T Values[12];int Count;vector():Count(0){}void push_back(const T&x){CHECK(Count<12);Values[Count++]=x;}T*begin(){return Values;}bool empty()const{return Count==0;}};}
namespace NInventory{class CTCInventoryBase:public CTCBase{public:enum EInventorySelectionState{};EInventorySelectionState InventorySelectionState;std::vector<CCategory>Categories;long HighlightedItemNumOwned,HighlightedItemMaxOwned;bool CanItemBeQuickAssigned,CanItemBeUsed,InventoryOpen;long GlobalDefIndex,SelectedCategoryIndex;CThing*pBuildingThing;bool EnableAnimXAxis,EnableAnimZAxis;long ParentMapNumber;CTCInventoryBase(CThing&t):CTCBase(t){}virtual void OnCreate();};}
template<class T>__declspec(noinline)bool CDefinitionManager::GetDef(long index,CDefPointer<const T>*out)const{
 if(index<=0)return false;CDefPointer<const CDefClassBase>value=GetPDefFromGlobalIndex(index);if(value.IsNull())return false;out->SetDynamicCast(value);return true;
}
void NInventory::CTCInventoryBase::OnCreate(){
 InventorySelectionState=static_cast<EInventorySelectionState>(0);
 const CGameDefinitionManager&definitions=GetDefinitionManager();
 long count=definitions.GetNoInstantiatedDefsOfType("INVENTORY_CATEGORY")-1;
 for(long i=0;i<count;++i){
 long index=definitions.GetGlobalIndexFromClassIndex("INVENTORY_CATEGORY",i+1);
 CDefPointer<const CInventoryCategoryDef>def;definitions.GetDef(index,&def);
 const CInventoryCategoryDef*category=reinterpret_cast<const CInventoryCategoryDef*>(def.InternalGetObject());
 if(category->Inventory==GlobalDefIndex&&category->AddCategoryOnCreate)Categories.push_back(CCategory(def,this,index));
 CDefPointeeBase*object=def.InternalGetObject();if(object->Release()==0)object->Delete();def.Object=0;
 }
 SelectedCategoryIndex=0;if(!Categories.empty())Categories.begin()->SetAsSelected(true);
 pBuildingThing=0;HighlightedItemNumOwned=0;HighlightedItemMaxOwned=0;CanItemBeQuickAssigned=false;CanItemBeUsed=false;EnableAnimXAxis=false;EnableAnimZAxis=false;long mapNumber=GetParentThing().GetCurrentMapNumber();InventoryOpen=false;ParentMapNumber=mapNumber;
}

int main(){
 const short maps[]={-32768,-1,0,1,32767};
 for(int n=0;n<=6;++n)for(int mask=0;mask<16;++mask)for(int global=0;global<3;++global)for(int preexisting=0;preexisting<2;++preexisting)for(int m=0;m<5;++m){
  CInventoryCategoryDef defs[6];CGameDefinitionManager manager;manager.N=n;manager.Defs=defs;expectedManager=&manager;CThing thing;thing.CurrentMapNumber=maps[m];int expected=preexisting;long indices[8];if(preexisting)indices[0]=99;
  for(int j=0;j<n;++j){defs[j].Inventory=j%3;defs[j].AddCategoryOnCreate=(mask&(1<<(j%4)))!=0;if(defs[j].Inventory==global&&defs[j].AddCategoryOnCreate)indices[expected++]=j+1;}
  {
   NInventory::CTCInventoryBase inventory(thing);inventory.GlobalDefIndex=global;inventory.Categories.Count=preexisting;if(preexisting){inventory.Categories.Values[0].Index=99;inventory.Categories.Values[0].Owner=&inventory;}
   inventory.InventorySelectionState=(NInventory::CTCInventoryBase::EInventorySelectionState)3;inventory.SelectedCategoryIndex=9;inventory.pBuildingThing=&thing;inventory.HighlightedItemNumOwned=9;inventory.HighlightedItemMaxOwned=11;inventory.CanItemBeQuickAssigned=inventory.CanItemBeUsed=inventory.InventoryOpen=inventory.EnableAnimXAxis=inventory.EnableAnimZAxis=true;inventory.ParentMapNumber=123;
   categoryCtors=selectedCalls=defLookups=0;inventory.OnCreate();
   CHECK(defLookups==n&&categoryCtors==expected-preexisting&&inventory.Categories.Count==expected&&selectedCalls==(expected?1:0));
   for(int k=0;k<expected;++k){const NInventory::CCategory&category=inventory.Categories.Values[k];CHECK(category.Index==indices[k]&&category.Owner==&inventory&&category.Selected==(k==0));if(k>=preexisting)CHECK(category.Def.Object==defs+indices[k]-1);}
   for(int t=0;t<n;++t)CHECK(defs[t].RefCount==(defs[t].Inventory==global&&defs[t].AddCategoryOnCreate?2UL:1UL));
   CHECK(inventory.InventorySelectionState==0&&inventory.SelectedCategoryIndex==0&&!inventory.pBuildingThing&&inventory.HighlightedItemNumOwned==0&&inventory.HighlightedItemMaxOwned==0&&!inventory.CanItemBeQuickAssigned&&!inventory.CanItemBeUsed&&!inventory.EnableAnimXAxis&&!inventory.EnableAnimZAxis&&!inventory.InventoryOpen&&inventory.ParentMapNumber==maps[m]);
  }
  for(int z=0;z<n;++z)CHECK(defs[z].RefCount==1);++cases;
 }
 CHECK(cases==3360);puts("005c0121_TEST PASS");return 0;
}
