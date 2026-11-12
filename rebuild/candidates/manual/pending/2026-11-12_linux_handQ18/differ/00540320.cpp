// NUISystem::CEditBox::KeyPressed(wchar_t key) -- the edit box's key-press handler (virtual override; body in the prior draft below)
// Grade: DIFFER(ours v retail) -- DIFFER(755v755) (Q18 probe of the prior draft) -- Q18: NOT PORTED, still a local model of classes that have shared headers (not landable even if it matched); the prior draft's residue is on the Earlier grade note line
// Earlier grade note: DIFFER(755v755) -- 18 diff lines: three vptr-load scratch registers swapped (eax/edx at slots 0x24c, 0x240, 0x250) and sub edi,5 vs retail add edi,-5 in the 0x6c Adjustment tail; 14 source forms of the -5 (signed/unsigned, local/member, -5 first/last) all canonicalise to sub
// Next step: the add -5 survives only when the constant reaches the optimizer late (inlined helper parameter or named constant through a function); look for an EditBox helper in FableWin locals around 0x540501, then revisit the vcall declarations (GetTextComponent/UpdateDisplayText slots) for the scratch swaps
// Leaf: ?KeyPressed@CEditBox@NUISystem@@UAEX_W@Z
// Flags: /Zc:wchar_t
#include "engine/CCountedPointer.h"
#define CComponent CComponentFields
#include "engine/_quarantine/CComponent.h"
#undef CComponent
#define CManager CManagerFields
#include "engine/CManager.h"
#undef CManager
#define CText CTextFields
#include "engine/CText.h"
#undef CText
class CCharString;class CWideStringData;
class CWideString{protected:CWideStringData*PStringData;public:CWideString();CWideString(const unsigned short*);CWideString(const CWideString&);~CWideString();long GetLength()const;const CWideString&operator=(const unsigned short*);const CWideString&operator=(const CWideString&);operator const unsigned short*()const;CWideString GetSubString(unsigned long,unsigned long)const;};
CWideString __fastcall operator+(const CWideString&,const CWideString&);
class C2DBoxF{public:float TLX,TLY,BRX,BRY;};
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
class CChangingStateComponent : public CComponent{private:unsigned char unreadChangingState[0x15c-0x134];
public:virtual NUISystem::CUIState* GetState(unsigned long);
public:virtual const NUISystem::CUIState* GetState(unsigned long) const;
public:virtual void UpdateStateChange();
public:virtual bool ChangedStateLastUpdate();
public:virtual bool InternalChanged();
protected:virtual bool AllChildrenChangedState();
protected:virtual void OnActivate();
protected:virtual void OnDeactivate();
};
class CText : public CChangingStateComponent{public:static unsigned int __fastcall GetCharWidthW(wchar_t);
public:virtual const CWideString& GetValue();
public:virtual void SetValue(const CCharString&,bool,bool);
public:virtual void SetValue(const CWideString&,bool,bool);
public:virtual void SetValue(unsigned long,const CWideString&,bool);
public:virtual CCountedPointer<CFontBank>&GetFont();virtual void SetTextWindow(C2DBoxF&);virtual C2DBoxF&GetTextWindow();};
}

namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}void _M_incr(){_M_node=_M_node->_M_next;}bool operator!=(const _List_iterator_base&o)const{return _M_node!=o._M_node;}};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{typedef _List_iterator<T,Traits>_Self;typedef typename Traits::reference reference;_List_iterator(_List_node<T>*n):_List_iterator_base(n){}reference operator*()const{return static_cast<_List_node<T>*>(_M_node)->_M_data;}_Self&operator++(){_M_incr();return *this;}};
template<class T,class A>class _List_base{protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A>class list:public _List_base<T,A>{public:typedef _List_iterator<T,_Nonconst_traits<T> >iterator;iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}};
}
namespace NUISystem{
class CObserver;enum EEvent{};
class CObservable{public:virtual void SetEvent(EEvent);virtual void SetOwnedEvent(CObserver*,EEvent);virtual void AddObserver(CObserver*);virtual void AddExclusiveObserver(CObserver*);virtual void RemoveExclusiveObserver(CObserver*);virtual void RemoveObserver(CObserver*);virtual void ClearObservers();virtual CObserver*GetExclusiveObserver();virtual bool IsExclusivelyObserved();virtual void AddConcurrentExclusiveObserver(CObserver*);virtual void RemoveConcurrentExclusiveObserver(CObserver*);};
// Layout: the shared manager header grounds the native singleton; only its original first virtual prefix is called here.
class CManager:public CObservable{unsigned char unreadManager[sizeof(CManagerFields)-4];public:virtual bool GetMouseDisabled();virtual void SetKeyPressed(wchar_t);virtual wchar_t GetKeyPressed();virtual void Action(CCountedPointer<CAction>&);static CManager*__fastcall GetInstance();};
class CIME{public:static CIME&__fastcall Get();bool IsIMEActive()const;};
// Layout: native changing-state prefix15c followed by two4-byte STLport lists and shared original text/cursor fields.
class CEditBox:public CChangingStateComponent{public:virtual CWideString GetText();virtual void SetText(const CWideString&);virtual CComponent*GetTextComponent();virtual void AddCharacter(wchar_t);virtual void KeyPressed(wchar_t);virtual void UpdateDisplayText();
protected:std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >ActionsOnEnter,ActionsOnEscape;CWideString Text;long CursorPos,LeftMostCharacter,RightMostCharacter,Adjustment;bool Active,IsPasswordBox,UsesIME,DisallowSpaceAsFirstChar,DeleteAllOnKeyPress;long MaxLength;CWideString AllowedCharacters,ForbiddenCharacters;
};
void CEditBox::KeyPressed(wchar_t key)
{
    if (key == 0x6f) {
        if (--CursorPos < 0)
            CursorPos = 0;
        if (CursorPos < LeftMostCharacter)
            Adjustment -= 6;
    } else if (key == 0x70) {
        ++CursorPos;
        if (CursorPos > (long)Text.GetLength())
            CursorPos = Text.GetLength();
        if (CursorPos > RightMostCharacter - 1)
            Adjustment += 6;
    } else {
        if (CIME::Get().IsIMEActive())
            goto refresh;
        if (key == 1) {
            for(std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >::iterator i=ActionsOnEscape.begin();i!=ActionsOnEscape.end();++i)CManager::GetInstance()->Action(*i);
        } else if (key == 0x75) {
            if (DeleteAllOnKeyPress) {
                Text = reinterpret_cast<const unsigned short*>(L"");
                CursorPos = 0;
            } else {
                {
                    CWideString before;
                    CWideString after;
                    if (CursorPos > 0)
                        before = Text.GetSubString(0, CursorPos);
                    if (CursorPos < (long)Text.GetLength())
                        after = Text.GetSubString(CursorPos + 1, Text.GetLength());
                    Text = before + after;
                }
                UpdateDisplayText();
                return;
            }
        } else if (key == 0x6c) {
            CursorPos = 0;
            float width = 0.0f;
            CText* parent = static_cast<CText*>(GetTextComponent());
            long i = 0;
            if (Text.GetLength() != 0) {
                do {
                    width += (float)CText::GetCharWidthW(((const unsigned short*)Text)[i]);
                    if (width > parent->GetTextWindow().BRX)
                        break;
                    ++i;
                } while (i != Text.GetLength());
            }
            long adjustment = i;
            adjustment -= Text.GetLength();
            adjustment += -5;
            Adjustment = adjustment;
            if (adjustment < 0)
                Adjustment = 0;
        } else if (key == 0x71) {
            CursorPos = Text.GetLength();
            Adjustment = 0;
        } else if (key == 0x1c) {
            for(std::list<CCountedPointer<CAction>,std::allocator<CCountedPointer<CAction> > >::iterator i=ActionsOnEnter.begin();i!=ActionsOnEnter.end();++i)CManager::GetInstance()->Action(*i);
        } else {
            goto refresh;
        }
    }
    DeleteAllOnKeyPress = false;
refresh:
    UpdateDisplayText();
}

}
