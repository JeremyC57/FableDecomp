// 0056ef2d CTCInventoryExperience::UpdateMenuScrollButtonsItem
// Codex parked-50 best attempt: DIFFER(66v66); residue: 66/66 bytes; maximum/index/boolean register allocation differs (retail ESI/EDI and DL/AL; compiler ESI/EBX and EAX).
// Flags:
// Leaf: ?UpdateMenuScrollButtonsItem@CTCInventoryExperience@@UAEXXZ
#pragma optimize("s",on)
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#define CTCInventoryExperience CTCInventoryExperienceFields
#include "engine/CTCInventoryExperience.h"
#undef CTCInventoryExperience
#define CTCInventoryBase CTCInventoryBaseFields
#include "engine/_quarantine/CTCInventoryBase.h"
#undef CTCInventoryBase
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
class CThing;
// Layout: true component base has reference ParentThing4 and flag bytes8/9.
class CTCBase:public CBaseClassNonCopyable {
protected:CThing&ParentThing;
private:bool ToBeRemoved,IsInGlobalUpdateTCs;
};
namespace std {
template<class T>class allocator {};
template<class P,class T,class A>struct _STLP_alloc_proxy:public A{P _M_data;};
template<class T,class A>class _Vector_base {
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:unsigned int size()const{return unsigned(this->_M_finish-this->_M_start);}
const T&operator[](unsigned int index)const{return *(this->_M_start+index);}
};
}
namespace NInventory {
// Layout: shared native inventory base130, enum at1c and selection enums38..44.
// Unused virtual slots/data omitted from this field-access projection.
class CTCInventoryBase:public CTCBase {
public:
 enum EInventorySelectionState {INVENTORY_SELECTION_STATE_CATEGORIES,INVENTORY_SELECTION_STATE_ITEMS,INVENTORY_SELECTION_STATE_MAX_STATES};
 enum ESelectionState {SELECTION_STATE_NONE,SELECTION_STATE_OFF,SELECTION_STATE_ON,SELECTION_STATE_SEL,SELECTION_STATE_MAX};
 virtual void UpdateMenuScrollButtonsItem();
private:unsigned char unreadBeforeSelection[0x1c-sizeof(CTCBase)];
protected:EInventorySelectionState InventorySelectionState;
private:unsigned char unreadBeforeArrows[0x18];
protected:ESelectionState MenuArrowHighlightsCategoryUp,MenuArrowHighlightsCategoryDn,MenuArrowHighlightsItemUp,MenuArrowHighlightsItemDn;
private:unsigned char unreadInventoryTail[0x130-0x48];
};
}
class CWideString;enum EPlayerMode{};
namespace NHeroInformationScreens {
enum EScreenType{};
class CBase {
public:virtual CWideString GetScreenName()const=0;virtual EPlayerMode GetPlayerMode()const=0;virtual EScreenType GetScreenType()const=0;virtual bool IsInventoryScreen()const;
};
}
// Layout: real secondary screen interface at130; shared headerstatMax1c0/vector208.
class CTCInventoryExperience:public NInventory::CTCInventoryBase,public NHeroInformationScreens::CBase {
 unsigned char unreadExperiencePrefix[0x1c0-0x134];
 long statIndicatorMax;
 unsigned char unreadBeforeList[0x44];
 std::vector<long> MenuDisplayListExperienceItem;
 unsigned char unreadExperienceTail[0x270-0x214];
public:virtual void UpdateMenuScrollButtonsItem();
};
void CTCInventoryExperience::UpdateMenuScrollButtonsItem(){
 if(InventorySelectionState==INVENTORY_SELECTION_STATE_ITEMS){
  MenuArrowHighlightsCategoryUp=SELECTION_STATE_OFF;
  MenuArrowHighlightsCategoryDn=SELECTION_STATE_OFF;
  long lastIndex=statIndicatorMax-1;
  MenuArrowHighlightsItemUp=MenuDisplayListExperienceItem[0]>0?SELECTION_STATE_ON:SELECTION_STATE_OFF;
  MenuArrowHighlightsItemDn=MenuDisplayListExperienceItem[MenuDisplayListExperienceItem.size()-1]<lastIndex?SELECTION_STATE_ON:SELECTION_STATE_OFF;
 }
}
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseFields));
FABLE_STATIC_ASSERT(sizeof(NInventory::CTCInventoryBase)==sizeof(CTCInventoryBaseFields));
FABLE_STATIC_ASSERT(sizeof(CTCInventoryExperience)==sizeof(CTCInventoryExperienceFields));
