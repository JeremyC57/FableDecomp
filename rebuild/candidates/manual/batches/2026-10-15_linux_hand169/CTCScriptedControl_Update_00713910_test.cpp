#include <cstdio>
// Behaviour model of CTCScriptedControl::Update (0x00713910): start, update, and retire the front action.
struct Action { bool activated, terminated; int starts, updates, exits; bool finishOnUpdate;
    void Start() { starts++; activated = true; }
    void Update() { updates++; if (finishOnUpdate) terminated = true; } };
struct Control { Action* queue[4]; int count;
    void Update() {
        if (count && !queue[0]->activated) queue[0]->Start();
        if (count && !queue[0]->terminated) queue[0]->Update();
        if (count && queue[0]->terminated) {
            Action* a = count ? queue[0] : 0;
            if (a) { a->exits++; for (int i = 1; i < count; ++i) queue[i - 1] = queue[i]; --count; }
        }
    } };
int main()
{
    Action a = { false, false, 0, 0, 0, true }, b = { false, false, 0, 0, 0, false };
    Control c = { { &a, &b, 0, 0 }, 2 };
    c.Update();
    if (a.starts != 1 || a.updates != 1 || a.exits != 1 || c.count != 1 || c.queue[0] != &b) { std::printf("00713910_TEST FAIL\n"); return 1; }
    c.Update(); c.Update();
    if (b.starts != 1 || b.updates != 2 || c.count != 1) { std::printf("00713910_TEST FAIL\n"); return 1; }
    std::printf("00713910_TEST PASS\n");
    return 0;
}
