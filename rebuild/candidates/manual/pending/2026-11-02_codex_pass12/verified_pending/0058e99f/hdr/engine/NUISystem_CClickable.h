#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_NUISYSTEM_CCLICKABLE_H
#define FABLE_ENGINE_NUISYSTEM_CCLICKABLE_H
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CWideString.h"
#include "engine/NUISystem_EActionType.h"
class C2DBoxI;
namespace NUISystem {
// Layout: original PDB bases and fields, native constructors55ba20/55b460.
class CHoverable : public CChangingStateComponent {
public:
    CHoverable(long);
    CHoverable(const CHoverable&);
    virtual ~CHoverable();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ProcessEvent(EEvent);
    virtual void AddActionOnHovered(CCountedPointer<CAction>&);
    virtual void AddActionOnUnhovered(CCountedPointer<CAction>&);
    virtual void IgnoreEvent(EEvent);
    virtual void ChangePosition(const C2DVector&,float,bool);
    virtual void RemoveFromHoveredList();
protected:
    virtual bool IsHovered(C2DBoxI&) const;
    virtual void OnHovered();
    virtual void OnUnhovered();
    virtual void UpdateHovering();
    virtual void OnDeactivate();
    void SetWasHovered(bool);
    long StateBeforeHovered;
    bool WasHovered;
    static std::list<CHoverable*> GAlreadyHovered;
    std::list<CCountedPointer<CAction> >* PActionsOnHovered;
    std::list<CCountedPointer<CAction> >* PActionsOnUnhovered;
};
class CClickable : public CHoverable {
public:
    CClickable(long);
    CClickable(const CClickable&);
    virtual ~CClickable();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ProcessEvent(EEvent);
    virtual void AddActionOnLeftClicked(CCountedPointer<CAction>&);
    virtual void AddActionOnLeftHeld(CCountedPointer<CAction>&);
    virtual void AddActionOnRightClicked(CCountedPointer<CAction>&);
    virtual void AddActionOnLeftUnclicked(CCountedPointer<CAction>&);
    virtual bool IsClickable() const;
    virtual void OnLeftClicked();
    virtual void OnLeftUnclicked();
    virtual void OnRightClicked();
    virtual void OnRightUnclicked();
protected:
    virtual void OnHovered();
    virtual void OnUnhovered();
    virtual void OnDeactivate();
    long StateBeforeLeftClicked;
    bool WasLeftClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftHeld;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftUnclicked;
    long StateBeforeRightClicked;
    bool WasRightClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnRightClicked;
    float InputDelay;
    float LastInputTime;
};
// Layout: PDB original fields with retail list size4; native new size0x10.
class CAction {
public:
    CAction();
    ~CAction();
    void AddParam(CActionParam* param) { UIActionParams.push_back(param); }
#if defined(FABLE_RETAIL_ITEM_ENTRY) || defined(FABLE_RETAIL_ITEMS_LIST)
    void AddParam(CActionParam*,CCountedPointer<CAction>&);
#endif
    EActionType ActionType;
    std::list<CActionParam*> UIActionParams;
    std::list<long> ThingActionParams;
protected:
    bool Valid;
};
// Layout: PDB Value+8; native new size12 and sole virtual dtor slot.
class CActionParamString : public CActionParam {
public:
#if defined(FABLE_RETAIL_PHOTOJOURNAL) || defined(FABLE_RETAIL_ITEM_ENTRY)
    CActionParamString(CWideString&);
#else
    CActionParamString(CWideString& value)
        : CActionParam(UI_ACTION_PARAM_TYPE_STRING),Value(value) { }
#endif
    virtual ~CActionParamString();
    CWideString Value;
};
FABLE_STATIC_ASSERT(sizeof(CHoverable)==0x16c);
FABLE_STATIC_ASSERT(sizeof(CClickable)==0x194);
FABLE_STATIC_ASSERT(sizeof(CAction)==0x10);
FABLE_STATIC_ASSERT(sizeof(CActionParamString)==0xc);
}
#endif

#else
#ifndef FABLE_ENGINE_NUISYSTEM_CCLICKABLE_H
#define FABLE_ENGINE_NUISYSTEM_CCLICKABLE_H
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CWideString.h"
#include "engine/NUISystem_EActionType.h"
class C2DBoxI;
namespace NUISystem {
// Layout: original PDB bases and fields, native constructors55ba20/55b460.
class CHoverable : public CChangingStateComponent {
public:
    CHoverable(long);
    CHoverable(const CHoverable&);
    virtual ~CHoverable();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ProcessEvent(EEvent);
    virtual void AddActionOnHovered(CCountedPointer<CAction>&);
    virtual void AddActionOnUnhovered(CCountedPointer<CAction>&);
    virtual void IgnoreEvent(EEvent);
    virtual void ChangePosition(const C2DVector&,float,bool);
    virtual void RemoveFromHoveredList();
protected:
    virtual bool IsHovered(C2DBoxI&) const;
    virtual void OnHovered();
    virtual void OnUnhovered();
    virtual void UpdateHovering();
    virtual void OnDeactivate();
    void SetWasHovered(bool);
    long StateBeforeHovered;
    bool WasHovered;
    static std::list<CHoverable*> GAlreadyHovered;
    std::list<CCountedPointer<CAction> >* PActionsOnHovered;
    std::list<CCountedPointer<CAction> >* PActionsOnUnhovered;
};
class CClickable : public CHoverable {
public:
    CClickable(long);
    CClickable(const CClickable&);
    virtual ~CClickable();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Update(float);
    virtual void ProcessEvent(EEvent);
    virtual void AddActionOnLeftClicked(CCountedPointer<CAction>&);
    virtual void AddActionOnLeftHeld(CCountedPointer<CAction>&);
    virtual void AddActionOnRightClicked(CCountedPointer<CAction>&);
    virtual void AddActionOnLeftUnclicked(CCountedPointer<CAction>&);
    virtual bool IsClickable() const;
    virtual void OnLeftClicked();
    virtual void OnLeftUnclicked();
    virtual void OnRightClicked();
    virtual void OnRightUnclicked();
protected:
    virtual void OnHovered();
    virtual void OnUnhovered();
    virtual void OnDeactivate();
    long StateBeforeLeftClicked;
    bool WasLeftClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftHeld;
    std::list<CCountedPointer<CAction> >* PActionsOnLeftUnclicked;
    long StateBeforeRightClicked;
    bool WasRightClicked;
    std::list<CCountedPointer<CAction> >* PActionsOnRightClicked;
    float InputDelay;
    float LastInputTime;
};
// Layout: PDB original fields with retail list size4; native new size0x10.
class CAction {
public:
    CAction();
    ~CAction();
    void AddParam(CActionParam* param) { UIActionParams.push_back(param); }
    EActionType ActionType;
    std::list<CActionParam*> UIActionParams;
    std::list<long> ThingActionParams;
protected:
    bool Valid;
};
// Layout: PDB Value+8; native new size12 and sole virtual dtor slot.
class CActionParamString : public CActionParam {
public:
    CActionParamString(CWideString& value)
        : CActionParam(UI_ACTION_PARAM_TYPE_STRING),Value(value) { }
    virtual ~CActionParamString();
    CWideString Value;
};
FABLE_STATIC_ASSERT(sizeof(CHoverable)==0x16c);
FABLE_STATIC_ASSERT(sizeof(CClickable)==0x194);
FABLE_STATIC_ASSERT(sizeof(CAction)==0x10);
FABLE_STATIC_ASSERT(sizeof(CActionParamString)==0xc);
}
#endif

#endif
