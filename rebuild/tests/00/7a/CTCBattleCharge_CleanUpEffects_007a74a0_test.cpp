// Behaviour test for CTCBattleCharge::CleanUpEffects (0x007A74A0): kill the shield, re-attach both feet effects.
#include <cstdio>
struct Thing;
struct Emitter { Thing* attachedTo; int attachType; };
struct Thing { bool dead; bool killed; Emitter* emitter; };
struct Charge {
    Thing* ParentThing; Thing* ShieldEffect; Thing* LeftFeetEffect; Thing* RightFeetEffect;
    static bool Alive(Thing* t) { return t && !t->dead; }
    void CleanUpEffects()
    {
        if (Alive(ShieldEffect)) ShieldEffect->killed = true;
        Thing* owner = ParentThing;
        Emitter* emitter = 0;
        if (Alive(LeftFeetEffect)) { if (LeftFeetEffect->emitter) emitter = LeftFeetEffect->emitter; emitter->attachedTo = owner; emitter->attachType = 1; }
        if (Alive(RightFeetEffect)) { if (RightFeetEffect->emitter) emitter = RightFeetEffect->emitter; emitter->attachedTo = owner; emitter->attachType = 1; }
    }
};
int main()
{
    Thing hero = { false, false, 0 };
    Emitter el = { 0, 0 }, er = { 0, 0 };
    Thing shield = { false, false, 0 }, left = { false, false, &el }, right = { true, false, &er };
    Charge c = { &hero, &shield, &left, &right };
    c.CleanUpEffects();
    if (!shield.killed || el.attachedTo != &hero || el.attachType != 1 || er.attachedTo != 0) { std::printf("007a74a0_TEST FAIL\n"); return 1; }
    Charge d = { &hero, 0, 0, 0 }; d.CleanUpEffects();
    std::printf("007a74a0_TEST PASS\n");
    return 0;
}