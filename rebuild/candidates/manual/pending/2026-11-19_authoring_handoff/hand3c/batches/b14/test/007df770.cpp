// Behaviour test for CTCWallMountEffects::FrameUpdate (0x007DF770), a portable model: nothing happens unless the component waits for
// an answer and a yes/no answer message is pending. A yes (ExtraData 1) to the effect prompt runs the effect; a yes to the remove
// prompt starts the use-wall-mount action on the creature if it is alive; either way the creature pointer and the wait are cleared.
// A no to the effect prompt shows "Do you want to take the item?" (Yes / No) and switches to the remove prompt; a no to the remove
// prompt clears the creature and the wait. Every answered case removes the message.
#include <stdio.h>
#include <string.h>
enum { NONE = 0, EFFECT = 1, REMOVE = 2 };
struct Creature { bool dead; int actions_started; };
struct World { bool has_answer; long extra; int removed; };
struct Effects {
    int waiting; Creature* creature; int effects_run; const char* subtitle;
    void FrameUpdate(World& world)
    {
        if (waiting == NONE || !world.has_answer) return;
        if (world.extra == 1) {
            if (waiting == EFFECT) ++effects_run;
            else if (waiting == REMOVE && creature && !creature->dead) ++creature->actions_started;
            creature = 0; waiting = NONE;
        } else if (waiting == EFFECT) {
            subtitle = "Do you want to take the item?"; waiting = REMOVE;
        } else {
            creature = 0; waiting = NONE;
        }
        world.has_answer = false; ++world.removed;
    }
};
int main()
{
    int bad = 0;
    Creature c = { false, 0 };
    Effects e = { EFFECT, &c, 0, 0 }; World w = { true, 0, 0 };
    e.FrameUpdate(w);                                            // no to the effect: ask about removing
    if (e.waiting != REMOVE || !e.subtitle || strcmp(e.subtitle, "Do you want to take the item?") || w.removed != 1 || !e.creature) ++bad;
    w.has_answer = true; w.extra = 1; e.FrameUpdate(w);          // yes to removing: action on the live creature
    if (c.actions_started != 1 || e.creature || e.waiting != NONE || w.removed != 2) ++bad;
    Effects f = { EFFECT, &c, 0, 0 }; w.has_answer = true; w.extra = 1; f.FrameUpdate(w);
    if (f.effects_run != 1 || f.waiting != NONE || c.actions_started != 1) ++bad;
    Creature dead = { true, 0 };
    Effects g = { REMOVE, &dead, 0, 0 }; w.has_answer = true; w.extra = 1; g.FrameUpdate(w);
    if (dead.actions_started != 0 || g.waiting != NONE) ++bad;
    Effects h = { REMOVE, &c, 0, 0 }; w.has_answer = true; w.extra = 0; h.FrameUpdate(w);
    if (h.creature || h.waiting != NONE || c.actions_started != 1) ++bad;
    Effects idle = { NONE, &c, 0, 0 }; w.has_answer = true; int before = w.removed; idle.FrameUpdate(w);
    if (w.removed != before || !w.has_answer) ++bad;             // not waiting: the message stays
    Effects quiet = { EFFECT, &c, 0, 0 }; w.has_answer = false; quiet.FrameUpdate(w);
    if (quiet.waiting != EFFECT) ++bad;
    printf(bad ? "007df770_TEST FAIL\n" : "007df770_TEST PASS\n");
    return bad ? 1 : 0;
}
