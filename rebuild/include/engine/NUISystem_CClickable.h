#ifndef FABLE_ENGINE_NUISYSTEM_CCLICKABLE_H
#define FABLE_ENGINE_NUISYSTEM_CCLICKABLE_H
#include "engine/NUISystem_CChangingStateComponent.h"
#ifdef FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
// Retail opt-in (chunk 886, row 0x0041E6D3 CManager::Action): take CAction / CActionParamString from their shared headers
// (engine/NUISystem_CAction.h, engine/NUISystem_CActionParamString.h) instead of the local copies below, so this header can sit in
// a TU that already includes them. No layout change.
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamString.h"
#endif
#include "fable_string.h"
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
#ifndef FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
// Layout: PDB original fields with retail list size4; native new size0x10.
class CAction {
public:
    CAction();
    ~CAction();
#ifdef FABLE_RETAIL_CACTION
    // Retail opt-in (chunk 735): both Ego_r.pdb AddParam overloads are out of line in retail: ?AddParam@CAction@NUISystem@@
    // QAEXPAVCActionParam@2@@Z 0x0053FDB0 (labels_familypass7 M088) and ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@
    // AAV?$CCountedPointer@VCAction@NUISystem@@@@@Z 0x0053FDE0 (the component parameter, which also learns the action).
    void AddParam(CActionParam* param);
    void AddParam(CActionParam* param, CCountedPointer<CAction>& action);
#else
    void AddParam(CActionParam* param) { UIActionParams.push_back(param); }
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
    CActionParamString(CWideString& value)
        : CActionParam(UI_ACTION_PARAM_TYPE_STRING),Value(value) { }
    virtual ~CActionParamString();
    CWideString Value;
};
#endif
FABLE_STATIC_ASSERT(sizeof(CHoverable)==0x16c);
FABLE_STATIC_ASSERT(sizeof(CClickable)==0x194);
FABLE_STATIC_ASSERT(sizeof(CAction)==0x10);
FABLE_STATIC_ASSERT(sizeof(CActionParamString)==0xc);
}
#endif
