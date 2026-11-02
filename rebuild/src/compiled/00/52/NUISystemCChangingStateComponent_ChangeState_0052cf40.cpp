// NUISystem::CChangingStateComponent::ChangeState(unsigned long) -- queue and propagate a requested UI state.
// Leaf: ?ChangeState@CChangingStateComponent@NUISystem@@UAEXK@Z
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CUIState.h"
namespace NUISystem {
void CChangingStateComponent::ChangeState(unsigned long state)
{
    if (RequestedState==state) return;
    RequestedState=state;
    StatesDone=UI_NO_CHANGING;
    StatesBeingDone=UI_NO_CHANGING;
    StatesToDo.clear();
    PreviousState=CurrentState;
    CurrentState=TargetState;
    switch(state) {
    case UI_STATE_INACTIVE:
    case UI_STATE_LIST_INACTIVE: OnDeactivate(); break;
    case UI_STATE_ACTIVE:
    case UI_STATE_LIST_ACTIVE: OnActivate(); break;
    }
    CUIState* ptarget=GetState(state);
    if(!HasState(state)) {
        UpdateTime=ParentUpdateTime;
        StatesToDo.push_back(UI_CHILDREN_CHANGING);
        for (unsigned long count=0; count<Children.size(); ++count) {
            if (Children[count]->GetParent()==this) {
                if (Children[count]->GetType()==UI_TYPE_LIST &&
                    (RequestedState==UI_STATE_ACTIVE || RequestedState==UI_STATE_SELECTED || RequestedState==UI_STATE_UNSELECTED))
                    continue;
                Children[count]->ChangeState(RequestedState,UpdateTime);
            }
        }
        return;
    }
    TargetState=state;
    UpdateTime=ptarget->UpdateTime>=0.0f ? ptarget->UpdateTime : ParentUpdateTime;
    switch(ptarget->StateChangeType) {
    case STATE_CHANGE_SIMULTANEOUS:
        StatesToDo.push_back((EChangingStateComponentState)(UI_CHILDREN_CHANGING|UI_INTERNAL_CHANGING));
        for (unsigned long count=0; count<Children.size(); ++count) {
            if (Children[count]->GetParent()==this) {
                if (Children[count]->GetType()==UI_TYPE_LIST &&
                    (RequestedState==UI_STATE_ACTIVE || RequestedState==UI_STATE_SELECTED || RequestedState==UI_STATE_UNSELECTED))
                    continue;
                Children[count]->ChangeState(RequestedState,UpdateTime);
            }
        }
        break;
    case STATE_CHANGE_PARENT_FIRST:
        StatesToDo.push_back(UI_INTERNAL_CHANGING);
        StatesToDo.push_back(UI_CHILDREN_CHANGING);
        break;
    case STATE_CHANGE_CHILDREN_FIRST:
        StatesToDo.push_back(UI_CHILDREN_CHANGING);
        StatesToDo.push_back(UI_INTERNAL_CHANGING);
        for (unsigned long count=0; count<Children.size(); ++count) {
            if (Children[count]->GetParent()==this) {
                if (Children[count]->GetType()==UI_TYPE_LIST &&
                    (RequestedState==UI_STATE_ACTIVE || RequestedState==UI_STATE_SELECTED || RequestedState==UI_STATE_UNSELECTED))
                    continue;
                Children[count]->ChangeState(RequestedState,UpdateTime);
            }
        }
        break;
    case STATE_CHANGE_CHILDREN_ONLY:
        StatesToDo.push_back(UI_CHILDREN_CHANGING);
        for (unsigned long count=0; count<Children.size(); ++count) {
            if (Children[count]->GetParent()==this) {
                if (Children[count]->GetType()==UI_TYPE_LIST &&
                    (RequestedState==UI_STATE_ACTIVE || RequestedState==UI_STATE_SELECTED || RequestedState==UI_STATE_UNSELECTED))
                    continue;
                Children[count]->ChangeState(RequestedState,UpdateTime);
            }
        }
        break;
    case STATE_CHANGE_PARENT_ONLY:
        StatesToDo.push_back(UI_INTERNAL_CHANGING);
        break;
    }
}
}