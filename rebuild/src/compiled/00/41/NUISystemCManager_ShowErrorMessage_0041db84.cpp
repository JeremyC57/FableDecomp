// 0041db84 NUISystem::CManager::ShowErrorMessage
// Leaf: ?ShowErrorMessage@CManager@NUISystem@@QAEXW4ERRORMESSAGE_TYPE@@@Z
// Flags: /O1 /Oy-
#include "engine/CCountedPointer.h"
#define CManager CManagerFields
#include "engine/CManager.h"
#undef CManager
#define CComponent CComponentFields
#include "engine/_quarantine/CComponent.h"
#undef CComponent
#define CGameJoystickManager CGameJoystickManagerFields
#include "engine/CGameJoystickManager.h"
#undef CGameJoystickManager
class CCharStringData;class CWideStringData;
class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();};
class CWideString{protected:CWideStringData*PStringData;public:static CWideString NullString;CWideString(const CWideString&);~CWideString();bool operator!=(const CWideString&)const;static CWideString __fastcall CreateFromCharString(const CCharString&);};
namespace NGameText{class CDataBank{public:const CWideString GetTextBySymbol(const CCharString&)const;};bool __fastcall ConstructString(CWideString&,long);}
// Layout: original native game component text-bank field14 and static singleton, confirmed original types.
class CMainGameComponent{protected:static CMainGameComponent*PMainGameComponent;unsigned char unreadMainGame[0x14];CCountedPointer<NGameText::CDataBank>PTextBank;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}const NGameText::CDataBank&GetTextBank()const{return *PTextBank;}};
// Layout: original frontend text-bank field60 and private singleton; native text-bank helper00497b40 loads singleton013b871c then field60.
class CNewFrontendGameComponent{private:static CNewFrontendGameComponent*m_pNewFrontendGameComponent;unsigned char unreadFrontend[0x60];CCountedPointer<NGameText::CDataBank>m_pTextBank;public:static CNewFrontendGameComponent*__fastcall Get(){return m_pNewFrontendGameComponent;}const NGameText::CDataBank&GetTextBank()const{return *m_pTextBank;}};
static __declspec(noinline) CWideString __fastcall GetTextBySymbol(const CCharString&symbol){
 if(CMainGameComponent::Get()){CWideString text=CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(symbol);if(text!=CWideString::NullString)return text;}
 if(CNewFrontendGameComponent::Get()){CWideString text=CNewFrontendGameComponent::Get()->GetTextBank().GetTextBySymbol(symbol);if(text!=CWideString::NullString)return text;}
 return CWideString::CreateFromCharString(symbol);
}
class CGameJoystickManager:public CGameJoystickManagerFields{public:static CGameJoystickManager&__fastcall Get();long GetMainPlayerJoystickDeviceNumber()const;};
class C2DBoxF;class C2DVector;class CEnginePrimitiveHandle;class CIEngine;class CRGBColour;class CFontBank;
template<class T>class CDefPointer;
namespace std{template<class T>class allocator;template<class T>struct less;template<class A,class B>struct pair;template<class T,class A>class list;template<class T,class A>class vector;template<class K,class V,class C,class A>class map;}
namespace NUISystem{class CComponent;class CAction;class CDeletion;class CUIState;class CUIDef;enum ETextAlignement{};enum EType{};
class CUIInterface{
public:virtual ~CUIInterface();
public:virtual void Update(float);
public:virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long *,NUISystem::CComponent*);
};
class CComponent : public CUIInterface{
public:virtual NUISystem::CComponent* FindChild(CCharString);
public:virtual NUISystem::CComponent* FindChildForward(CCharString);
public:virtual void FindAllChildren(CCharString,std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >&);
public:virtual void FindAllChildrenOfType(NUISystem::EType,std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >&);
public:virtual void SetPAcceptButton(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* FindChildNonRecursive(CCharString);
public:virtual NUISystem::CComponent* FindFirstChildOfType(NUISystem::EType);
public:virtual bool HasChild(NUISystem::CComponent*) const;
public:virtual void SetZoom(const C2DVector&);
public:virtual C2DVector GetZoom() const;
public:virtual void SetColour(const CRGBColour&);
public:virtual CRGBColour GetColour() const;
public:virtual void SetPosition(const C2DVector&);
public:virtual C2DVector GetPosition() const;
public:virtual C2DVector GetParentPosition() const;
public:virtual void SetParentPosition(C2DVector&);
public:virtual C2DVector GetParentZoom() const;
public:virtual void SetParentZoom(C2DVector&);
public:virtual CRGBColour GetParentColour() const;
public:virtual void SetParentColour(CRGBColour&);
public:virtual C2DVector GetSize() const;
public:virtual C2DBoxF GetBoundingBox() const;
public:virtual const C2DVector& GetRenderPosition() const;
public:virtual const C2DVector& GetRenderZoom() const;
public:virtual const CRGBColour& GetRenderColour() const;
public:virtual void SetRenderPosition(C2DVector&);
public:virtual void SetRenderZoom(C2DVector&);
public:virtual void SetRenderColour(CRGBColour&);
public:virtual void ChangePosition(const C2DVector&,float,bool);
public:virtual void ChangePositionDelta(const C2DVector&,float,bool);
public:virtual void ChangeTargetPositionDelta(const C2DVector&,float,bool);
public:virtual void UpdatePosition(float);
public:virtual void ChangeZoom(const C2DVector&,float,bool);
public:virtual void ChangeZoomDelta(const C2DVector&,float,bool);
public:virtual void UpdateZoom(float);
public:virtual void ChangeColour(const CRGBColour&,float,bool);
public:virtual void ChangeColourDelta(const CRGBColour&,float,bool);
public:virtual void UpdateColour(float);
public:virtual void ResetState();
public:virtual void ForceState(unsigned long,bool);
public:virtual void Initialise();
public:virtual bool HasState(unsigned long);
public:virtual bool HasStateInChildren(unsigned long);
public:virtual void ChangeState(unsigned long);
public:virtual void ChangeState(unsigned long,float);
public:virtual void ChangeState(NUISystem::CUIState&);
public:virtual bool ChangedState();
public:virtual NUISystem::CUIState* GetCurrentState();
public:virtual void SetParent(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* GetParent() const;
public:virtual std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > >& GetChildren();
public:virtual float GetUpdateTime() const;
public:virtual C2DVector GetTargetPosition() const;
public:virtual CRGBColour GetTargetColour() const;
public:virtual C2DVector GetTargetZoom() const;
public:virtual void ProcessChangeState();
public:virtual void AddChild(CCountedPointer<NUISystem::CComponent>&);
public:virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent>&);
public:virtual void AddChildFront(CCountedPointer<NUISystem::CComponent>&);
public:virtual void SetDeletion(NUISystem::CDeletion);
public:virtual NUISystem::CDeletion& GetDeletion();
public:virtual void RemoveChild(unsigned long);
public:virtual NUISystem::EType GetType() const;
public:virtual void AddAction(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnBack(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddPreAction(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnRightClicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDropped(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnHovered(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnUnhovered(CCountedPointer<NUISystem::CAction>&);
public:virtual void AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction>&);
public:virtual void Die();
public:virtual std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > >& GetStates();
public:virtual void SetName(const CCharString&);
public:virtual CCharString GetName();
public:virtual NUISystem::CComponent* FindComponent(NUISystem::CComponent*);
public:virtual unsigned long GetCurrentStateIndex();
public:virtual unsigned long GetTargetStateIndex() const;
public:virtual float CanScroll(bool,C2DBoxF&);
public:virtual void FakeChildDying(NUISystem::CComponent*);
public:virtual void SetParentViewport(NUISystem::CComponent*);
public:virtual void ClearHandle();
public:virtual void MakeUnavailable();
public:virtual void MakeUnavailableDynamically();
public:virtual void MakeAvailable();
public:virtual void MakeAvailableDynamically();
public:virtual void AddActionIAmParamOf(CCountedPointer<NUISystem::CAction>&);
public:virtual NUISystem::CComponent* FindFirstChildOfTypeNonRecursive(NUISystem::EType);
public:virtual bool IsAllowedToScroll() const;
public:virtual NUISystem::CComponent* GetParentViewport();
public:virtual bool IsBastardChild() const;
public:virtual bool IsIndependent() const;
public:virtual bool IsPositionIndependent() const;
public:virtual bool IsZoomIndependent() const;
public:virtual bool IsLayerIndependent() const;
public:virtual bool IsDrawFromViewport() const;
public:virtual bool IsPositionIsCenter() const;
public:virtual long GetDefIndex() const;
public:virtual CDefPointer<NUISystem::CUIDef const > LoadAndReturnDef() const;
public:virtual void OnObserverRemove();
public:virtual C2DVector CanMove(const C2DVector&);
public:virtual C2DVector GetDynamicSize() const;
public:virtual C2DVector GetTargetRenderPosition() const;
public:virtual bool IsPCStyle() const;
public:virtual void SetRelativeParentPosition(C2DVector&);
public:virtual void SetRelativeParentZoom(C2DVector&);
public:virtual bool IsUsingRelativeZoom() const;
public:virtual bool IsUsingRelativePosition() const;
public:virtual C2DVector GetRelativeRenderPosition();
public:virtual C2DVector GetRelativeRenderZoom();
public:virtual void SetRelativeRenderPosition(C2DVector);
public:virtual C2DVector SetRelativeRenderZoom(C2DVector);
public:virtual C2DVector GetTargetRelativeRenderPosition() const;
public:virtual C2DVector GetTargetRelativeRenderZoom() const;
public:virtual void AddPositionChild(NUISystem::CComponent*);
public:virtual void SetPositionParent(NUISystem::CComponent*);
public:virtual NUISystem::CComponent* GetPositionParent();
public:virtual void PositionChildDying(NUISystem::CComponent*);
public:virtual void PositionParentDying(NUISystem::CComponent*);
public:virtual long GetRenderLayer() const;
public:virtual void AddActionOnDesctruction(CCountedPointer<NUISystem::CAction>&);
public:virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >*);
public:virtual bool IsClickable() const;
public:virtual void RemoveFromHoveredList();
private:unsigned char unreadComponentMembers[sizeof(CComponentFields)-4];
};
class CChangingStateComponent : public CComponent{
public:virtual NUISystem::CUIState* GetState(unsigned long);
public:virtual const NUISystem::CUIState* GetState(unsigned long) const;
public:virtual void UpdateStateChange();
public:virtual bool ChangedStateLastUpdate();
public:virtual bool InternalChanged();
protected:virtual bool AllChildrenChangedState();
protected:virtual void OnActivate();
protected:virtual void OnDeactivate();
};
class CText : public CChangingStateComponent{
public:virtual const CWideString& GetValue();
public:virtual void SetValue(const CCharString&,bool,bool);
public:virtual void SetValue(const CWideString&,bool,bool);
public:virtual void SetValue(unsigned long,const CWideString&,bool);
};
}

enum ERRORMESSAGE_TYPE{};
namespace NUISystem{
// Layout: supplied manager fields9c/a0; global flattened component pointer is projected back to the original namespace at each use.
class CManager:public CManagerFields{protected:using CManagerFields::m_bHasErrorMessage;using CManagerFields::m_pErrorMessage;public:CComponent*CreateComponent(const CCharString&,bool);void ShowErrorMessage(ERRORMESSAGE_TYPE);};
void CManager::ShowErrorMessage(ERRORMESSAGE_TYPE error){
 switch(error){case 0:CText*text;
 if(CNewFrontendGameComponent::Get()){
 m_pErrorMessage=reinterpret_cast< ::CComponent*>(CreateComponent("UI_DIALOG_CONTROLLERDISCONNECT",false));
 text=static_cast<CText*>(reinterpret_cast<CComponent*>(m_pErrorMessage)->FindChild("UI_CONTROLLERDISCONNECT_TEXT"));
 }else{
 m_pErrorMessage=reinterpret_cast< ::CComponent*>(CreateComponent("UI_DIALOG_CONTROLLERDISCONNECT_GAME",false));
 text=static_cast<CText*>(reinterpret_cast<CComponent*>(m_pErrorMessage)->FindChild("UI_CONTROLLERDISCONNECT_TEXT_GAME"));
 }
 CWideString message=GetTextBySymbol("TEXT_GUI_CONTROLLER_DISCONNECT");
 NGameText::ConstructString(message,CGameJoystickManager::Get().GetMainPlayerJoystickDeviceNumber()+1);
 text->SetValue(message,true,true);
 reinterpret_cast<CComponent*>(m_pErrorMessage)->Initialise();
 reinterpret_cast<CComponent*>(m_pErrorMessage)->ChangeState(1UL);
 break;}
 m_bHasErrorMessage=true;
}
}