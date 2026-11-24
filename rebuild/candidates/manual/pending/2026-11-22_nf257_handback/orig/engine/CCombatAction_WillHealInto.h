#ifndef FABLE_ENGINE_CCOMBATACTION_WILLHEALINTO_H
#define FABLE_ENGINE_CCOMBATACTION_WILLHEALINTO_H
#include "engine/CCombatActionBase.h"

// Ego_r and FableWin class records. SpellLevel follows the retail CCombatActionBase.
class CCombatAction_WillHealInto : public CCombatActionBase {
public:
    CCombatAction_WillHealInto(CThingCreatureBase&, long);                // 0x00956870
private:
    long SpellLevel;
};
FABLE_STATIC_ASSERT_AT(13, sizeof(CCombatAction_WillHealInto) == 0x118);
#endif
