// Behaviour test for 0x0041E10D NUISystem::CManager::IsKeyHeld: locked -> false; otherwise the mapped engine key is queried and the event removed.
#include <cstdio>
struct Event { int state; Event() : state(0) { } };
struct Input {
    int downKey; int removed; int lastState;
    bool IsKeyboardEventInQueue(int type, int key, Event* e) { lastState = e->state; return type == 1 && key == downKey; }
    void RemoveEventFromQueue(Event*) { ++removed; }
};
struct M {
    Input* input; int keyMap[4]; int locked;
    bool IsKeyHeld(int key)
    {
        if (locked == 0) {
            Event ev; Input* in = input; ev.state = 2;
            bool held = in->IsKeyboardEventInQueue(1, keyMap[key], &ev);
            input->RemoveEventFromQueue(&ev);
            return held;
        }
        return false;
    }
};
int main()
{
    Input in; in.downKey = 30; in.removed = 0; in.lastState = 0;
    M m; m.input = &in; m.keyMap[0] = 10; m.keyMap[1] = 30; m.locked = 0;
    bool ok = m.IsKeyHeld(1) && !m.IsKeyHeld(0) && in.removed == 2 && in.lastState == 2;
    m.locked = 1;
    ok = ok && !m.IsKeyHeld(1) && in.removed == 2;
    if (!ok) { std::printf("0041e10d_TEST FAIL\n"); return 1; }
    std::printf("0041e10d_TEST PASS\n");
    return 0;
}
