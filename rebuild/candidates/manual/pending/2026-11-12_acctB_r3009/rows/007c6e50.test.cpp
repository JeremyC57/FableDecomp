// Behaviour test for 0x007C6E50 CTCTurncoatSpell::FrameUpdate(): a self-contained host model of the row's logic (will-button timer
// tick; a dead target releases the button; RELEASED removes the spell, JUST_RELEASED also unlocks the ability mutex and finishes the
// caster's action; while held: range test against TurncoatRange[level] squared, the out-of-range effect started / the turncoating
// effects stopped or restarted on range changes, stamina drained by whole points of the accumulated per-frame cost, a turncoated
// target is absorbed (sound, originator, button released), else while the target lives with health and the caster has stamina the
// target's turncoatedness grows by whole points of accumulated resistance damage and every XPFrameCounter frames the caster gains
// the ability's trainable-stat experience; the caster always makes a noise) over host mocks.
#include <cstdio>

enum EButtonHeldState { BEING_HELD_DOWN, JUST_RELEASED_DELAYED, JUST_RELEASED, RELEASED };
struct Vec { float X, Y, Z; };
struct Creature { bool alive; float health; Vec pos; bool turncoated; float turncoatedness; float resistance; bool originatorSet; };
struct Caster { Vec pos; long stamina; long noises; bool hasExperience; long experience; long statGiven; bool hasSound; long absorbSounds;
                bool removed; bool actionFinished; bool mutexLocked; long currentAbility; };
struct Spell {
    EButtonHeldState state;
    Caster* caster; Creature* target;
    float range[4]; long LevelOfSpell;
    bool InRange, outOfRangeEffect, turncoatingEffects;
    float PerFrameStaminaModify, AccModifyStamina; long StaminaModify;
    float PerFrameResistanceModify, AccModifyResistance; long ResistanceModify;
    long CurXPFrameCounter, XPFrameCounter;
    long abilityStat, abilityExperience;
    void FrameUpdate() {
        EButtonHeldState s = state;
        if (!target || !target->alive) state = JUST_RELEASED_DELAYED;
        switch (s) {
        case BEING_HELD_DOWN: {
            if (!target || !target->alive) return;
            float dx = caster->pos.X - target->pos.X, dy = caster->pos.Y - target->pos.Y, dz = caster->pos.Z - target->pos.Z;
            float r = range[LevelOfSpell];
            bool in_range = dx * dx + dz * dz + dy * dy <= r * r;
            if (!InRange && !in_range && !outOfRangeEffect) outOfRangeEffect = true;
            else if (InRange && !in_range) { turncoatingEffects = false; outOfRangeEffect = true; }
            else if (!InRange && in_range) { outOfRangeEffect = false; turncoatingEffects = true; }
            InRange = in_range;
            AccModifyStamina += PerFrameStaminaModify;
            long stamina = StaminaModify;
            while (AccModifyStamina > 1.0f) { AccModifyStamina -= 1.0f; ++stamina; }
            caster->stamina -= stamina;
            if (!target->turncoated) {
                bool give_xp = false;
                if (!target->alive || target->health <= 0.0f || caster->stamina <= 0) state = JUST_RELEASED_DELAYED;
                else if (InRange) {
                    AccModifyResistance += PerFrameResistanceModify;
                    long resist = ResistanceModify;
                    while (AccModifyResistance > 1.0f) { AccModifyResistance -= 1.0f; ++resist; }
                    target->turncoatedness += (float)resist;
                    if (target->turncoatedness >= target->resistance) target->turncoated = true;
                    if (--CurXPFrameCounter == 0) {
                        CurXPFrameCounter = XPFrameCounter;
                        give_xp = true;
                    }
                }
                if (give_xp && caster->hasExperience) { caster->experience += abilityExperience; caster->statGiven = abilityStat; }
            } else {
                if (caster->hasSound) ++caster->absorbSounds;
                target->originatorSet = true;
                state = JUST_RELEASED_DELAYED;
            }
            ++caster->noises;
            break;
        }
        case JUST_RELEASED:
            caster->mutexLocked = false; caster->currentAbility = 0;   // UnLockAbilitiyMutex on the special-abilities component
            caster->actionFinished = true;
            caster->removed = true;
            return;
        case RELEASED:
            caster->removed = true;
            return;
        default: break;
        }
    }
};

static int fail(const char* w) { std::printf("007c6e50_TEST FAIL (%s)\n", w); return 1; }

int main()
{
    Caster c = { {0, 0, 0}, 100, 0, true, 0, 0, true, 0, false, false, true, 5 };
    Creature t = { true, 50.0f, {3, 4, 0}, false, 0.0f, 10.0f, false };
    Spell s = { BEING_HELD_DOWN, &c, &t, {2.0f, 6.0f, 9.0f, 12.0f}, 1, false, false, false,
                0.5f, 0.0f, 1, 1.5f, 0.0f, 2, 3, 3, 7, 25 };
    s.FrameUpdate();                                   // in range (5 <= 6): effects start, drain 1 stamina, resistance 3
    if (!s.InRange || !s.turncoatingEffects || s.outOfRangeEffect) return fail("enter range");
    if (c.stamina != 99 || t.turncoatedness != 3.0f || c.noises != 1) return fail("first frame");
    s.FrameUpdate(); s.FrameUpdate();                  // third frame: XP counter hits 0, experience given and counter reset
    if (c.experience != 25 || c.statGiven != 7 || s.CurXPFrameCounter != 3) return fail("experience");
    if (c.stamina != 96) return fail("stamina accumulation");
    t.pos.X = 30.0f; s.FrameUpdate();                  // out of range: effects stop, out-of-range effect on
    if (s.InRange || s.turncoatingEffects || !s.outOfRangeEffect) return fail("leave range");
    t.pos.X = 3.0f; t.turncoatedness = 9.5f; s.FrameUpdate(); s.FrameUpdate();
    if (!t.turncoated) return fail("turncoat");
    s.FrameUpdate();                                   // turncoated: absorb, originator, button released
    if (!t.originatorSet || c.absorbSounds != 1 || s.state != JUST_RELEASED_DELAYED) return fail("absorb");
    s.state = JUST_RELEASED; s.FrameUpdate();
    if (!c.removed || !c.actionFinished || c.mutexLocked || c.currentAbility != 0) return fail("released");
    Caster c2 = c; c2.removed = false; Creature dead = t; dead.alive = false;
    Spell s2 = s; s2.caster = &c2; s2.target = &dead; s2.state = RELEASED; s2.FrameUpdate();
    if (!c2.removed || s2.state != JUST_RELEASED_DELAYED) return fail("dead target");
    std::printf("007c6e50_TEST PASS\n");
    return 0;
}
