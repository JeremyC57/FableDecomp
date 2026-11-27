#ifndef FABLE_ENGINE_ECOMBATANIMATIONHANDEDNESS_H
#define FABLE_ENGINE_ECOMBATANIMATIONHANDEDNESS_H
#ifdef FABLE_RETAIL_GENERIC_STRIKE_RESPONSE_PASS29
// Ego_d and FableWin enum records; native initialization stores 2.
enum ECombatAnimationHandedness { HANDED_RIGHT=0,HANDED_LEFT=1,HANDED_NONE=2 };
#else
#ifdef FABLE_RETAIL_CREATURE_ONCREATE_PASS27
// Ego_d and FableWin enum records; native initialization stores 2.
enum ECombatAnimationHandedness { HANDED_RIGHT=0,HANDED_LEFT=1,HANDED_NONE=2 };
#endif
#endif
#endif
