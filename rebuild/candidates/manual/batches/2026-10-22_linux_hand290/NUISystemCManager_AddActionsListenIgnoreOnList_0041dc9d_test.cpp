// Behaviour test for NUISystem::CManager::AddActionsListenIgnoreOnList (0x0041DC9D): build a listen (0xF5) and an ignore
// (0xF9) action for each of the two components, each carrying the event parameter (type 5); the list listens with the first
// component's listen action and the second's ignore action, and ignores with the others.
#include <cstdio>

struct Comp;
struct Action { int type; Comp* target; int paramType; int paramValue; };
struct Comp {
    int nListen, nIgnore; Action listen[4], ignore[4];
    Comp() : nListen(0), nIgnore(0) {}
    void AddListen(const Action& a) { listen[nListen++] = a; }
    void AddIgnore(const Action& a) { ignore[nIgnore++] = a; }
};
static Action Make(int type, Comp* target, int event) { Action a; a.type = type; a.target = target; a.paramType = 5; a.paramValue = event; return a; }

static void AddActionsListenIgnoreOnList(Comp* list, Comp* first, Comp* second, int event)
{
    Action listenFirst = Make(0xf5, first, event);
    Action ignoreFirst = Make(0xf9, first, event);
    list->AddListen(listenFirst);
    list->AddIgnore(ignoreFirst);
    Action listenSecond = Make(0xf5, second, event);
    Action ignoreSecond = Make(0xf9, second, event);
    list->AddListen(ignoreSecond);
    list->AddIgnore(listenSecond);
}

int main()
{
    Comp list, a, b;
    AddActionsListenIgnoreOnList(&list, &a, &b, 17);
    bool ok = list.nListen == 2 && list.nIgnore == 2
        && list.listen[0].type == 0xf5 && list.listen[0].target == &a && list.ignore[0].type == 0xf9 && list.ignore[0].target == &a
        && list.listen[1].type == 0xf9 && list.listen[1].target == &b && list.ignore[1].type == 0xf5 && list.ignore[1].target == &b
        && list.listen[1].paramType == 5 && list.ignore[1].paramValue == 17;
    std::printf(ok ? "0041dc9d_TEST PASS\n" : "0041dc9d_TEST FAIL\n");
    return ok ? 0 : 1;
}
