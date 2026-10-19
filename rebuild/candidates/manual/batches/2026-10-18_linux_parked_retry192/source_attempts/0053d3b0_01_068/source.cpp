// CMenuEntry::ChangeState -- parent selected children, execute state actions, and clear PC hover on unselection.
// Native0053D3B0 length246. Actual donor CComponent retained; vector12 makes nativeChildrenB0 versus donorB8.
// CChangingStateComponent::ChangeState52CF40; nativeMenuVT124662C slots51 SetParent52F330,108 LoadAndReturnDef530EC0,
// 131 ExecuteActions5309B0,144 OnUnhovered53D970. PDB signatures donor5A8EF0,5AA830,5AAFF0,5B6CF0.
// Native CUIDef::PCStyle221 versus donor255; CHoverable::WasHovered160 versus donor190; action pointers1A8/1B4 vs1D8/1E4.
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <stddef.h>
#include "engine/CIVCountedPointeeBase.h"
#define CComponent CComponentDonor
#include "engine/_quarantine/CComponent.h"
#undef CComponent
namespace NUISystem {
struct CUIDef:CIVCountedPointeeBase {
 unsigned char DefinitionData[0x221-8];bool PCStyle;
 typedef void(CUIDef::*DeleteMethod)()const;
 __forceinline void Release()const {CUIDef*p=const_cast<CUIDef*>(this);--p->IVRefCount;if(static_cast<long>(p->IVRefCount)==0)(this->*static_cast<DeleteMethod*>(__vftable)[1])();}
};
struct DefinitionPointer {
 const CUIDef*Object;
 DefinitionPointer(const CUIDef*p=0):Object(p){if(p)++const_cast<CUIDef*>(p)->IVRefCount;}
 DefinitionPointer(const DefinitionPointer&p):Object(p.Object){if(Object)++const_cast<CUIDef*>(Object)->IVRefCount;}
 ~DefinitionPointer(){if(Object){Object->Release();Object=0;}}
 const CUIDef*operator->()const{return Object;}
};
struct CComponent;
struct ChildPointer {CComponent*Data;void*CountInfo;};
struct ChildArray {ChildPointer*First,*Last,*Limit;unsigned long size()const{return Last-First;}};
struct ActionList;
struct CComponent {
 void*VTable;unsigned char ComponentData[0xb0-4];ChildArray Children;
 typedef void(CComponent::*ParentMethod)(CComponent*);
 __forceinline void SetParent(CComponent*p){(this->*static_cast<ParentMethod*>(VTable)[51])(p);}
};
struct CMenuEntry:CComponent {
 unsigned char StateComponentData[0x160-0xbc];bool WasHovered;unsigned char MenuData[0x1a8-0x161];
 ActionList*PActionsOnSelected;ActionList*PActionsOnBack;ActionList*PPreActions;ActionList*PActionsOnUnselected;
 typedef DefinitionPointer(CMenuEntry::*DefinitionMethod)()const;
 typedef void(CMenuEntry::*ActionsMethod)(ActionList*);
 typedef void(CMenuEntry::*UnhoverMethod)();
 void BaseChangeState(unsigned long); //0052CF40 actual CChangingStateComponent::ChangeState(unsigned long)
 void ChangeState(unsigned long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CMenuEntry,Children)==0xb0);
FABLE_STATIC_ASSERT(offsetof(CMenuEntry,WasHovered)==0x160);
FABLE_STATIC_ASSERT(offsetof(CMenuEntry,PActionsOnSelected)==0x1a8);
FABLE_STATIC_ASSERT(offsetof(CMenuEntry,PActionsOnUnselected)==0x1b4);
FABLE_STATIC_ASSERT(offsetof(CUIDef,PCStyle)==0x221);
FABLE_STATIC_ASSERT(sizeof(CMenuEntry::DefinitionMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CComponent::ParentMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CUIDef::DeleteMethod)==4);
#endif
void CMenuEntry::ChangeState(unsigned long state){
 switch(state){
 default:BaseChangeState(state);break;
 case 4:
  (this->*static_cast<ActionsMethod*>(VTable)[131])(PActionsOnUnselected);
  BaseChangeState(4);
  if((this->*static_cast<DefinitionMethod*>(VTable)[108])()->PCStyle){
   (this->*static_cast<UnhoverMethod*>(VTable)[144])();WasHovered=false;
  }
  break;
 case 3:
  for(unsigned long i=0;i<Children.size();++i)Children.First[i].Data->SetParent(this);
  (this->*static_cast<ActionsMethod*>(VTable)[131])(PActionsOnSelected);
  BaseChangeState(3);break;
 }
}
}
