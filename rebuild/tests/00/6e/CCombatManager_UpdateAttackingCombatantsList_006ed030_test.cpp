// Behaviour test for 0x006ED030 CCombatManager::UpdateAttackingCombatantsList(CCombatantInfo&, bool): no attackers -> nothing;
// else the four defender updates in order, then per attacker (copied out of the map first): skip when off the wheel, age the
// preferred dolly (clear it when the timer goes negative), skip without a current sequence, run the sequence resets, and finish
// the current action only when the attacker is in melee, its sequence is melee / break-into-melee, the defender is attackable,
// the next action is a strike and the current action is an interruptible strike.
#include <stdio.h>
#include <string.h>

struct Action { bool strike, ready; };
struct Sequence { bool melee, break_into, next_strike; };
struct Attacker { unsigned long uid; bool in_melee; Action* action; int finished; };
struct AttackerInfo {
    unsigned long uid; bool dolly_set; long dolly_timer; bool using_wheel; long current; Sequence* sequences[2]; int resets;
};
struct Info { bool defender_attackable; Attacker* attackers[16]; int attacker_count; };   // attackers kept in key order

static char g_log[256];
static AttackerInfo* g_combatants;
static int g_combatant_count;

static AttackerInfo& Find(unsigned long uid)
{
    for (int i = 0; i < g_combatant_count; ++i)
        if (g_combatants[i].uid == uid)
            return g_combatants[i];
    return g_combatants[0];
}

static void Update(Info& info, bool force)
{
    if (info.attacker_count == 0)
        return;
    strcat(g_log, "pattern,instruction,");
    bool attackable = info.defender_attackable;
    strcat(g_log, force ? "wheel(1)," : "wheel(0),");
    Attacker* list[16];
    int n = 0;
    for (int i = 0; i < info.attacker_count; ++i)
        list[n++] = info.attackers[i];
    for (int i = 0; i < n; ++i) {
        Attacker* a = list[i];
        AttackerInfo& ai = Find(a->uid);
        if (!ai.using_wheel)
            continue;
        if (ai.dolly_set && --ai.dolly_timer < 0)
            ai.dolly_set = false;
        if (ai.current < 0)
            continue;
        ++ai.resets;
        if (a->in_melee && ai.current >= 0) {
            Sequence* s = ai.sequences[ai.current];
            if ((s->melee || s->break_into) && attackable && s->next_strike) {
                const Action* act = a->action;
                if (act && act->strike && act->ready)
                    ++a->finished;
            }
        }
    }
}

static AttackerInfo Make(unsigned long uid, Sequence* s)
{
    AttackerInfo r;
    r.uid = uid; r.dolly_set = false; r.dolly_timer = 0; r.using_wheel = true; r.current = 0;
    r.sequences[0] = s; r.sequences[1] = 0; r.resets = 0;
    return r;
}

int main()
{
    bool ok = true;
    Sequence melee = { true, false, true }, breaker = { false, true, true }, idle = { false, false, true },
             no_strike = { true, false, false };
    Action ready = { true, true }, busy = { true, false };
    Attacker a[8] = { { 1, true, &ready, 0 }, { 2, true, &ready, 0 }, { 3, true, &busy, 0 }, { 4, false, &ready, 0 },
                      { 5, true, &ready, 0 }, { 6, true, 0, 0 }, { 7, true, &ready, 0 }, { 8, true, &ready, 0 } };
    AttackerInfo c[8];
    c[0] = Make(1, &melee); c[0].dolly_set = true; c[0].dolly_timer = 0;     // finishes; dolly expires
    c[1] = Make(2, &breaker); c[1].dolly_set = true; c[1].dolly_timer = 3;   // finishes; dolly ages
    c[2] = Make(3, &melee);                                                  // action not ready
    c[3] = Make(4, &melee);                                                  // not in melee
    c[4] = Make(5, &idle);                                                   // not a melee sequence
    c[5] = Make(6, &melee);                                                  // no current action
    c[6] = Make(7, &no_strike);                                              // next action no strike
    c[7] = Make(8, &melee); c[7].using_wheel = false; c[7].dolly_set = true; // off the wheel
    g_combatants = c; g_combatant_count = 8;

    Info empty; empty.defender_attackable = true; empty.attacker_count = 0;
    g_log[0] = 0;
    Update(empty, true);
    ok = ok && g_log[0] == 0;

    Info info; info.defender_attackable = true; info.attacker_count = 8;
    for (int i = 0; i < 8; ++i)
        info.attackers[i] = &a[i];
    Update(info, false);
    ok = ok && strcmp(g_log, "pattern,instruction,wheel(0),") == 0;
    ok = ok && a[0].finished == 1 && a[1].finished == 1 && a[2].finished == 0 && a[3].finished == 0 && a[4].finished == 0 &&
         a[5].finished == 0 && a[6].finished == 0 && a[7].finished == 0;
    ok = ok && !c[0].dolly_set && c[0].dolly_timer == -1;
    ok = ok && c[1].dolly_set && c[1].dolly_timer == 2;
    ok = ok && c[7].dolly_set && c[7].resets == 0;
    ok = ok && c[0].resets == 1 && c[3].resets == 1;

    // An unattackable defender finishes nothing; no current sequence skips the resets.
    g_log[0] = 0;
    c[0].current = -1;
    info.defender_attackable = false;
    Update(info, true);
    ok = ok && strcmp(g_log, "pattern,instruction,wheel(1),") == 0 && a[1].finished == 1 && c[0].resets == 1 && c[1].resets == 2;

    printf(ok ? "006ed030_TEST PASS\n" : "006ed030_TEST FAIL\n");
    return ok ? 0 : 1;
}