// NUISystem::CSpellContainerList::ProcessEvent(NUISystem::EEvent event) -- the spell-assignment list's input observer: rate-limited by InputDelay; left / back releases the exclusive focus (accepting every container's assignments and firing UI action RESET_ASSIGNED_SPELLS), list up / down scrolls, rotate left / right moves the spell selection marker within the selected container, assign creates a UI_SPELL_TEMPLATE sprite in the selected slot (UNASSIGN_SPELL for the slot it left, then ASSIGN_SPELL, each with the flattened slot index), unassign clears it (UNASSIGN_SPELL)
// Leaf: ?ProcessEvent@CSpellContainerList@NUISystem@@UAEXW4EEvent@2@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CSPELLCONTAINER_VIRTUALS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CChangingStateComponent.h"
template<> void std::list<NUISystem::CActionParam*>::push_back(NUISystem::CActionParam* const& x);   // 0x0042B646
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CSpellContainer.h"
#include "engine/NUISystem_CManager.h"

namespace NUISystem {
// Ego_r.pdb / FableWin.pdb ??0CActionParamULONG@NUISystem@@QAE@K@Z (inline in retail: vptr 0x0122F844, type 5 = ULONG, value +8
// after new(0xc)); no shared header carries it.
class CActionParamULONG : public CActionParam { public:
    CActionParamULONG(unsigned long value) : CActionParam(UI_ACTION_PARAM_TYPE_ULONG), Value(value) { }
    virtual ~CActionParamULONG();
    unsigned long Value;                                                // +0x08
};
// RTTI .?AVCSpellContainerList@NUISystem@@ (single base CChangingStateComponent, rtti_bases.tsv); vtable 0x012475C4. New virtuals
// in slot order (rtti-vtables.tsv 142..152; names Ego_r.pdb ?..@CSpellContainerList@NUISystem@@, slots 144 / 147 / 149 / 150 by
// their bodies: +0x17C store, CCountedPointer& argument, +0x18C store, +0x190 C2DVector store). Layout Ego_r.pdb - 0x30, minus 4
// after the vector.
class CSpellContainerList : public CChangingStateComponent { public:
    virtual void ProcessEvent(EEvent event);
    virtual void AddSpellSelectionChild(CCountedPointer<CComponent>&);              // slot 142 0x00544310
    virtual void AddSpellIndicationChild(CCountedPointer<CComponent>&);             // slot 143 0x00544330
    virtual void SetSpellToAssign(long);                                             // slot 144 0x00544100
    virtual void ShowSpellSelectionChild();                                          // slot 145 0x00544110
    virtual void HideSpellSelectionChild();                                          // slot 146 0x00544190
    virtual void AddChildAndSetOffsets(CCountedPointer<CComponent>&);               // slot 147 0x005453D0
    virtual void InitialiseOffsets(unsigned long);                                   // slot 148 0x00544210
    virtual void SetNextSpellContainerAlpha(float);                                  // slot 149 0x005455A0
    virtual void SetNextSpellContainerPosition(C2DVector&);                          // slot 150 0x005455B0
protected:
    virtual void ScrollUp();                                                         // slot 151 0x00544520
    virtual void ScrollDown();                                                       // slot 152 0x00544810
public:
    float LastInputTime;                                                // +0x15c
    unsigned long SpellIndex;                                           // +0x160
    unsigned long SelectedChild;                                        // +0x164
    CComponent* SpellSelectionChild;                                    // +0x168
    CComponent* SpellIndicationChild;                                   // +0x16c
    std::vector<CComponent*> SpellContainers;                           // +0x170
    long SpellToAssign;                                                 // +0x17c
    C2DVector PositionOffset;                                           // +0x180
    float AlphaOffset;                                                  // +0x188
    float NextSpellContainerAlpha;                                      // +0x18c
    C2DVector NextSpellContainerPosition;                               // +0x190
    C2DVector PositionOffsetToUse;                                      // +0x198
    float AlphaOffsetToUse;                                             // +0x1a0
    float InputDelay;                                                   // +0x1a4
};
}
template<> CCountedPointer<NUISystem::CAction>::CCountedPointer(NUISystem::CAction* data);    // 0x0042AA29
template<> CCountedPointer<NUISystem::CAction>::~CCountedPointer();                           // 0x00429B72
template<> CCountedPointer<NUISystem::CComponent>::CCountedPointer(NUISystem::CComponent* data);   // 0x00429C15
template<> CCountedPointer<NUISystem::CComponent>::~CCountedPointer();                        // 0x00429C53

void NUISystem::CSpellContainerList::ProcessEvent(EEvent event)
{
    if (Time - LastInputTime < InputDelay)
        return;
    switch (event) {
    case UI_EVENT_LEFT:
    case UI_EVENT_BACK:
        if (this == CManager::GetInstance()->GetExclusiveObserver()) {
            CManager::GetInstance()->RemoveExclusiveObserver(this);
            CManager::GetInstance()->SetEvent(UI_EVENT_BACK);
            HideSpellSelectionChild();
            LastInputTime = Time;
            for (unsigned long i = 0; i < SpellContainers.size(); ++i)
                ((CSpellContainer*)SpellContainers[i])->AcceptAssignments();
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = UI_ACTION_TYPE_RESET_ASSIGNED_SPELLS;
            CManager::GetInstance()->Action(action);
        }
        break;
    case UI_EVENT_SPELL_LIST_UP:
        ScrollDown();
        LastInputTime = Time;
        break;
    case UI_EVENT_SPELL_LIST_DOWN:
        ScrollUp();
        LastInputTime = Time;
        break;
    case UI_EVENT_SPELL_ROTATE_LEFT: {
        unsigned long previous = SpellIndex;
        if (previous == 0)
            SpellIndex = SpellContainers[SelectedChild]->GetChildren().size() - 1;
        else
            SpellIndex = previous - 1;
        if (SpellSelectionChild) {
            CComponent* container = SpellContainers[SelectedChild];
            CComponent* from = container->GetChildren()[previous].Data;
            CComponent* to = container->GetChildren()[SpellIndex].Data;
            SpellSelectionChild->ChangePositionDelta(to->GetTargetPosition() - from->GetTargetPosition(), 0.2f, false);
        }
        LastInputTime = Time;
        break;
    }
    case UI_EVENT_SPELL_ROTATE_RIGHT: {
        unsigned long previous = SpellIndex;
        if (SpellIndex == SpellContainers[SelectedChild]->GetChildren().size() - 1)
            SpellIndex = 0;
        else
            ++SpellIndex;
        if (SpellSelectionChild) {
            CComponent* container = SpellContainers[SelectedChild];
            CComponent* from = container->GetChildren()[previous].Data;
            CComponent* to = container->GetChildren()[SpellIndex].Data;
            SpellSelectionChild->ChangePositionDelta(to->GetTargetPosition() - from->GetTargetPosition(), 0.2f, false);
        }
        LastInputTime = Time;
        break;
    }
    case UI_EVENT_SPELL_ASSIGN: {
        CSpellContainer* container = (CSpellContainer*)SpellContainers[SelectedChild];
        CCountedPointer<CComponent> spell(CManager::GetInstance()->CreateComponent("UI_SPELL_TEMPLATE", false));
        ((CSprite*)spell.Data)->SetGraphicIndex(SpellToAssign);
        long slot = container->AssignSpell(spell, SpellIndex);
        if (slot != -1) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = UI_ACTION_TYPE_UNASSIGN_SPELL;
            action->AddParam(new CActionParamULONG((SpellContainers.size() - SelectedChild - 1) * container->GetBaseSpells().size() + slot));
            CManager::GetInstance()->Action(action);
        }
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_ASSIGN_SPELL;
        action->AddParam(new CActionParamULONG((SpellContainers.size() - SelectedChild - 1) * container->GetBaseSpells().size() + SpellIndex));
        CManager::GetInstance()->Action(action);
        LastInputTime = Time;
        break;
    }
    case UI_EVENT_SPELL_UNASSIGN: {
        CSpellContainer* container = (CSpellContainer*)SpellContainers[SelectedChild];
        container->UnAssignSpell(SpellIndex);
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_UNASSIGN_SPELL;
        action->AddParam(new CActionParamULONG((SpellContainers.size() - SelectedChild - 1) * container->GetBaseSpells().size() + SpellIndex));
        CManager::GetInstance()->Action(action);
        LastInputTime = Time;
        break;
    }
    }
}