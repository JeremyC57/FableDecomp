// NUISystem::CEditBox constructor native5407B0,311bytes. Actual Component/Observer/Manager/CIV/string headers.
// Native RTTI1245DE4 confirms base CChangingStateComponent (52CC50), not propagated CDraggableInto.
// Native RTTI1246B8C CEditBox; secondary tables1246B64 observer+4 and1246B5C secondary+18.
// Base ends15C vs donor18C. Lists are single-head4 vs donor12; derived fields thereafter donor-40.
// Manager actualVT1230134 slot2=55CA40 CObservable::AddObserver(CObserver*), PDB5D5E70.
// Observer52DA20 ObserveEvent(EEvent), events33/34; no invented event semantic names.
// Actual LoadAndReturnDef()const530EC0 lowered to hidden result storage pointer/ret4; returned EAX is same storage.
// Definition charlimit1D0/usesIME1D4/disallow1D5/password1CD vs donor1F8/1FC/1FD/1F5.
// CUIDef actual CIV unsignedlong IVRefCount4; GetRefCount()const returns signedlong (FableWin PDB1909400).
// Delete()const slot1; ReleaseReference is a local void helper, not actual signedlong Release().
// Actual CUIDef header absent after full include/repo filename search; raw PDB UIDef projection retained.
// PDB ctor5A77D0 CChangingStateComponent QAE@J and5BA230 CEditBox QAE@J prove long argument.
// Full-width host PMFs retained.
// Leaf: EditBoxNative
#include <stddef.h>
#include "rebuild_abi.h"
#define CComponent CComponentDonor
#include "engine/_quarantine/CComponent.h"
#undef CComponent
#include "engine/CObserver.h"
#include "engine/CManager.h"
#include "engine/CIVCountedPointeeBase.h"
#include "fable_string.h"
namespace NUISystem {enum EEvent {NativeEvent33=33,NativeEvent34=34};}
struct ObserverNative {void*VTable;unsigned char EventsToObserve[12];bool PreventObservation;unsigned char Alignment[3];void ObserveEvent(NUISystem::EEvent);};
struct ManagerMethods:CManager {typedef void(ManagerMethods::*AddObserverMethod)(ObserverNative*);void AddObserver(ObserverNative*p){(this->*static_cast<AddObserverMethod*>(__vftable)[2])(p);}};
ManagerMethods* __fastcall GetUIManager(); //41E5F2 CManager::GetInstance
extern const unsigned char EditBoxVTable[],EditBoxObserverVTable[],EditBoxSecondaryVTable[];
struct EditBoxBaseNative {void*VTable;ObserverNative Observer;void*SecondaryVTable;unsigned char Unrecovered1C[0x15c-0x1c];void InitializeChangingStateComponent(long);EditBoxBaseNative(long def){InitializeChangingStateComponent(def);VTable=(void*)EditBoxVTable;Observer.VTable=(void*)EditBoxObserverVTable;SecondaryVTable=(void*)EditBoxSecondaryVTable;}};
struct ActionNode {ActionNode*Next,*Previous;void*Action;void*CountInfo;};
void* __cdecl AllocateActionListNode(size_t); // genuine CRT malloc BFEA0E, descriptive ABI alias
struct ActionList {ActionNode*Head;ActionList():Head(0){ActionNode*p=static_cast<ActionNode*>(AllocateActionListNode(sizeof(ActionNode)));p->Next=p;p->Previous=p;Head=p;}};
#pragma pack(push,1)
struct UIDefNative:CIVCountedPointeeBase {unsigned char Unrecovered08[0x1cd-8];bool PasswordBox;unsigned char Alignment[2];long EditBoxCharLimit;bool EditBoxUsesIME,DisallowSpaceAsFirstChar;
 typedef void(UIDefNative::*DeleteMethod)()const;void DecRef()const{--const_cast<UIDefNative*>(this)->IVRefCount;}long GetRefCount()const{return IVRefCount;}
 void ReleaseReference()const{DecRef();if(GetRefCount()==0)(this->*static_cast<DeleteMethod*>(__vftable)[1])();}
};
#pragma pack(pop)
struct DefinitionResultStorage {const UIDefNative*Data;~DefinitionResultStorage(){if(Data)Data->ReleaseReference();}const UIDefNative*operator->()const{return Data;}};
struct EditBoxNative:EditBoxBaseNative {ActionList ActionsOnEnter,ActionsOnEscape;CWideString Text;long CursorPos,LeftMostCharacter,RightMostCharacter,Adjustment;bool Active,IsPasswordBox,UsesIME,DisallowSpaceAsFirstChar,DeleteAllOnKeyPress;long MaxLength;CWideString AllowedCharacters,ForbiddenCharacters;
 DefinitionResultStorage*LoadAndReturnDefResultStorage(DefinitionResultStorage*)const;EditBoxNative(long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(ObserverNative)==20);
FABLE_STATIC_ASSERT(sizeof(ActionNode)==16);
FABLE_STATIC_ASSERT(offsetof(EditBoxNative,ActionsOnEnter)==0x15c);
FABLE_STATIC_ASSERT(offsetof(EditBoxNative,Text)==0x164);
FABLE_STATIC_ASSERT(offsetof(EditBoxNative,MaxLength)==0x180);
FABLE_STATIC_ASSERT(offsetof(EditBoxNative,ForbiddenCharacters)==0x188);
FABLE_STATIC_ASSERT(offsetof(UIDefNative,EditBoxCharLimit)==0x1d0);
FABLE_STATIC_ASSERT(sizeof(UIDefNative::DeleteMethod)==4);
FABLE_STATIC_ASSERT(sizeof(ManagerMethods::AddObserverMethod)==4);
#endif
EditBoxNative::EditBoxNative(long def):EditBoxBaseNative(def),CursorPos(0),LeftMostCharacter(0),RightMostCharacter(0),Adjustment(0),Active(true),IsPasswordBox(false),UsesIME(false),DeleteAllOnKeyPress(false),MaxLength(50){
 GetUIManager()->AddObserver(&Observer);Observer.ObserveEvent(NUISystem::NativeEvent33);Observer.ObserveEvent(NUISystem::NativeEvent34);
 DefinitionResultStorage definition;LoadAndReturnDefResultStorage(&definition);
 if(definition->EditBoxCharLimit>0)MaxLength=definition->EditBoxCharLimit;
 UsesIME=definition->EditBoxUsesIME;DisallowSpaceAsFirstChar=definition->DisallowSpaceAsFirstChar;IsPasswordBox=definition->PasswordBox;
}