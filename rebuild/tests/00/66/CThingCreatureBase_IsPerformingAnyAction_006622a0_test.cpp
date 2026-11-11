#include <cstdio>
// Behaviour test for CThingCreatureBase::IsPerformingAnyAction() const (0x006622A0), linked against the row's object
// (?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ): PCurrentAction at +0x100, the action's Finished flag at +0x61.

struct ActionState {
    char pad[0x61];
    char finished;                               // +0x61 CCreatureActionBase::Finished
};

class CThingCreatureBase { public:
    char pad[0x100];
    ActionState* action;                         // +0x100 PCurrentAction
    bool IsPerformingAnyAction() const;
};

int main()
{
    CThingCreatureBase c;
    ActionState a;

    // null action -> false
    c.action = 0;
    bool r1 = c.IsPerformingAnyAction();

    // action present, not finished -> true
    a.finished = 0;
    c.action = &a;
    bool r2 = c.IsPerformingAnyAction();

    // action present, finished -> false
    a.finished = 7;
    c.action = &a;
    bool r3 = c.IsPerformingAnyAction();

    if (!r1 && r2 && !r3) {
        printf("ACTION_OK\n");
        return 0;
    }
    printf("ACTION_FAIL %d %d %d\n", r1, r2, r3);
    return 1;
}
