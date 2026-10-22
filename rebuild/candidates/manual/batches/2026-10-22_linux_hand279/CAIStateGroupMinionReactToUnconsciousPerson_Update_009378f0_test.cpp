// Behaviour test for CAIStateGroup_MinionReactToUnconsciousPerson::Update (0x009378F0): model copy, scripted stubs.
#include <cstdio>

struct CThing { bool dead; bool hasDiscovery; bool discovered; CThing* discoverer; int lifeState; };
static bool IsAlive(const CThing* t) { return t && !t->dead; }
static int g_moveResult, g_lookResult, g_actionResult, g_actions, g_messages, g_terminated, g_defCount, g_defFound;
static int g_released;
struct CAIStateGroup_MinionReactToUnconsciousPerson {
    CThing* person;
    CThing* creature;
    void Update();
};

void CAIStateGroup_MinionReactToUnconsciousPerson::Update()
{
    if (IsAlive(person)) {
        if (g_moveResult == 2 && IsAlive(person)) {
            if (IsAlive(person) && person->hasDiscovery && !person->discovered) {
                person->discovered = true;
                person->discoverer = creature;
            }
            if (creature->lifeState == 3) {
                if (g_lookResult == 2) {
                    ++g_actions;
                    if (g_actionResult != 2) {
                        ++g_terminated;
                        return;
                    }
                    ++g_terminated;
                    return;
                }
            } else {
                int* def = g_defFound ? &g_defCount : 0;
                if (def) ++*def;
                if (g_defFound)
                    ++g_messages;
                if (def) {
                    if (--*def == 0)
                        ++g_released;
                }
            }
        }
    }
    ++g_terminated;
}

int main()
{
    int ok = 1;
    CThing me = { false, false, false, 0, 1 };
    CThing person = { false, true, false, 0, 0 };
    CAIStateGroup_MinionReactToUnconsciousPerson s = { &person, &me };
    // move fails: only terminates
    g_moveResult = 3;
    s.Update();
    if (g_terminated != 1 || person.discovered) ok = 0;
    // reached, not alerted state: discovered, message posted, the def released
    g_moveResult = 2; g_defFound = 1;
    s.Update();
    if (!person.discovered || person.discoverer != &me || g_messages != 1 || g_released != 1 || g_terminated != 2) ok = 0;
    // no def: no message
    g_defFound = 0;
    s.Update();
    if (g_messages != 1 || g_terminated != 3) ok = 0;
    // life state 3: look, then the alert action
    me.lifeState = 3; g_lookResult = 2; g_actionResult = 2;
    s.Update();
    if (g_actions != 1 || g_terminated != 4) ok = 0;
    // look fails: terminates without the action
    g_lookResult = 1;
    s.Update();
    if (g_actions != 1 || g_terminated != 5) ok = 0;
    // dead person
    person.dead = true;
    s.Update();
    if (g_actions != 1 || g_terminated != 6) ok = 0;
    if (!ok) { std::printf("009378f0_TEST FAIL\n"); return 1; }
    std::printf("009378f0_TEST PASS\n");
    return 0;
}
