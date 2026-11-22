#ifndef FABLE_ENGINE_CCOMBATACTION_WILLHEALINTO_H
#define FABLE_ENGINE_CCOMBATACTION_WILLHEALINTO_H
#include "engine/CCombatActionBase.h"

// Ego_r and FableWin class records. SpellLevel follows the retail CCombatActionBase.
class CCombatAction_WillHealInto : public CCombatActionBase {
public:
    CCombatAction_WillHealInto(CThingCreatureBase&, long);                // 0x00956870
#ifdef FABLE_RETAIL_CCOMBATACTION_WILLHEALINTO_OVERRIDES
    // NF257 opt-in (2026-11-22, q12723, rows 0x00956870 / 0x00956A20 / 0x00956940): the class's own overrides, Ego_r.pdb 0x008FAF00 / 0x008F8380 /
    // 0x0097B570 + FableWin.pdb 0x02A11440 / 0x02A113C0 / 0x02BF9AD0 (FinishAction and Initialise public virtual, Clone private virtual const), retail
    // vftable 0x012948AC slot 4 = 0x008C2AD0, slot 12 = 0x008C0560 (landed rows, same spellings) and slot 11 = 0x00956A20. Declarations
    // only; no layout change.
    virtual void FinishAction();                                         // 0x008C2AD0
    virtual void Initialise();                                           // 0x008C0560
private:
    virtual CCreatureActionBase* Clone() const;                          // 0x00956A20
#endif
private:
    long SpellLevel;
};
FABLE_STATIC_ASSERT_AT(13, sizeof(CCombatAction_WillHealInto) == 0x118);
#endif
