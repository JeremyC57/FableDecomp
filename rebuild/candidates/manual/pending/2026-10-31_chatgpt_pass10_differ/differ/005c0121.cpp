// 005c0121 NInventory::CTCInventoryBase::OnCreate
// Leaf: ?OnCreate@CTCInventoryBase@NInventory@@UAEXXZ
// Flags: /O1 /Oy-
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCInventoryBase CTCInventoryBaseFields
#include "engine/_quarantine/CTCInventoryBase.h"
#define CCategory CCategoryFields
#include "engine/CCategory.h"
#undef CCategory
#undef CTCInventoryBase
#include "engine/_quarantine/CInventoryCategoryDef.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
class CThing:public CThingFields{public:long GetCurrentMapNumber()const{return CurrentMapNumber;}};
#include "engine/CBaseClassNonCopyable.h"
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CDefinitionManager:public CDefinitionManagerFields{public:
 long GetNoInstantiatedDefsOfType(const CCharString&)const;
 long GetGlobalIndexFromClassIndex(const CCharString&,long)const;
 CDefPointer<const CDefClassBase>GetPDefFromGlobalIndex(long)const;
 template<class T>bool GetDef(long,CDefPointer<const T>*)const;
};
class CGameDefinitionManager:public CDefinitionManager{};
// Layout: original real polymorphic base restores shared flattened CTCBase prefix.
class CTCBase:public CBaseClassNonCopyable{public:CThing&GetParentThing(){return ParentThing;}protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;protected:const CGameDefinitionManager&GetDefinitionManager()const;};
namespace NInventory{class CTCInventoryBase;
// Layout: shared original category fields, 44 bytes in native STLport layout.
class CCategory:public CCategoryFields{public:CCategory(CDefPointer<const CInventoryCategoryDef>,CTCInventoryBase*,long);~CCategory();void SetAsSelected(bool);};
}
namespace std{template<class T>class allocator{};template<class T,class A=allocator<T> >class vector{protected:T*_M_start;T*_M_finish;T*_M_end_of_storage;public:void push_back(const T&);T*begin(){return _M_start;}bool empty()const{return _M_start==_M_finish;}};}
namespace NInventory{
// Layout: shared inventory base fields, restoring original real CTCBase prefix.
class CTCInventoryBase:public CTCBase{
 unsigned char unreadComponents[0x1c-0xc];
 protected:enum EInventorySelectionState{};EInventorySelectionState InventorySelectionState;
 std::vector<CCategory>Categories;long HighlightedItemNumOwned,HighlightedItemMaxOwned;bool CanItemBeQuickAssigned,CanItemBeUsed;
 unsigned char unreadMenuState[0x58-0x36];bool InventoryOpen;
 unsigned char unreadDefinition[0x6c-0x59];
 private:long GlobalDefIndex,SelectedCategoryIndex;
 unsigned char unreadItemState[0xb8-0x74];CThing*pBuildingThing;
 unsigned char unreadAnimation[0x120-0xbc];bool EnableAnimXAxis,EnableAnimZAxis;
 unsigned char unreadIndexState[0x12c-0x122];long ParentMapNumber;
 public:virtual void OnCreate();
};
}
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
