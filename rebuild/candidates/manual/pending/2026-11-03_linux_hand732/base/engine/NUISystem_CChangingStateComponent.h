#ifndef FABLE_ENGINE_NUISYSTEM_CCHANGINGSTATECOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CCHANGINGSTATECOMPONENT_H
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
// Layout: original public base, ctor/dtor and fields; native complete size0x15c.
class CChangingStateComponent : public CComponent {
public:
    enum EChangingStateComponentState {UI_NO_CHANGING=0,UI_CHILDREN_CHANGING=1,UI_INTERNAL_CHANGING=2,UI_LAST=4};
    CChangingStateComponent(long);
    virtual ~CChangingStateComponent();
    virtual void Initialise(); // slot inherited
    virtual void Update(float); // slot inherited
    virtual void ForceState(unsigned long, bool); // slot inherited
    virtual void ChangeState(unsigned long, float); // slot inherited
    virtual void ChangeState(unsigned long); // slot inherited
    virtual NUISystem::CUIState * GetCurrentState(); // slot inherited
    virtual bool HasState(unsigned long); // slot inherited
    virtual bool ChangedState(); // slot inherited
    virtual unsigned long GetCurrentStateIndex(); // slot inherited
    virtual unsigned long GetTargetStateIndex() const; // slot inherited
    virtual float GetUpdateTime() const; // slot inherited
    virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &); // slot inherited
    virtual bool IsPositionIndependent() const; // slot inherited
    virtual bool IsZoomIndependent() const; // slot inherited
protected:
    virtual void ProcessChangeState(); // slot inherited
public:
    virtual NUISystem::CUIState * GetState(unsigned long); // slot 135
    virtual const NUISystem::CUIState * GetState(unsigned long) const; // slot 134
    virtual void UpdateStateChange(); // slot 136
    virtual bool ChangedStateLastUpdate(); // slot 137
    virtual bool InternalChanged(); // slot 138
protected:
    virtual bool AllChildrenChangedState(); // slot 139
    virtual void OnActivate(); // slot 140
    virtual void OnDeactivate(); // slot 141
protected:
    NUISystem::CChangingStateComponent::EChangingStateComponentState StatesBeingDone; // +134
    NUISystem::CChangingStateComponent::EChangingStateComponentState StatesDone; // +138
    std::list<enum NUISystem::CChangingStateComponent::EChangingStateComponentState,std::allocator<enum NUISystem::CChangingStateComponent::EChangingStateComponentState> > StatesToDo; // +13c
    float ParentUpdateTime; // +140
    unsigned long CurrentState; // +144
    unsigned long TargetState; // +148
    unsigned long RequestedState; // +14c
    float UpdateTime; // +150
    bool PreviousUpdateChanged; // +154
    unsigned long PreviousState; // +158
};
FABLE_STATIC_ASSERT(sizeof(CChangingStateComponent)==0x15c);
}
#endif
