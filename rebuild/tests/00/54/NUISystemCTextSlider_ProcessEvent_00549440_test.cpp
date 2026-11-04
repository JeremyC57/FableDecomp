// Behaviour test for NUISystem::CTextSlider::ProcessEvent (0x00549440): a model of the slider's input handling. Inside the
// input delay nothing happens; SLIDER_LEFT / SLIDER_RIGHT step the selection with wrap-around (0 <-> MaxChild), push the new
// value into every action's first parameter, run the actions, make the accept button observe mouse movement with an opaque
// colour, mark the slider moved, restart the delay and play the forward sound; a step that does not move (MaxChild 0) does
// none of that. BACK / SELECT pop the previous-selected list when it holds more than one list (6 to the popped front, 3 to
// the new front, the slider stops observing), set the manager's change (REJECT / ACCEPT), run the back / select actions and
// restart the delay.
#include <cstdio>
#include <list>
#include <vector>
enum { EV_SELECT = 4, EV_BACK = 5, EV_LEFT = 20, EV_RIGHT = 21, EV_MOUSE = 25 };
struct Manager { int actions; int sounds; int change; int removed; std::vector<int> executedValues; };
static Manager g_manager;
struct List { int lastState; };
static std::list<List*> g_previous;
struct Button { bool observing; unsigned char alpha; };
struct Param { long value; };
struct Slider {
    float time, lastInput, delay;
    unsigned long selected, previous, maxChild;
    bool move;
    std::vector<Param*> actions;
    std::vector<int> onBack, onSelect;
    Button* accept;
    int baseEvents;
    void Step(bool left)
    {
        previous = selected;
        if (left) { if (selected == 0) selected = maxChild; else --selected; }
        else { if (selected == maxChild) selected = 0; else ++selected; }
        if (previous == selected) return;
        if (accept && !accept->observing) { accept->observing = true; accept->alpha = 255; }
        for (unsigned i = 0; i < actions.size(); ++i) { actions[i]->value = (long)selected; ++g_manager.actions; g_manager.executedValues.push_back((int)selected); }
        move = true;
        lastInput = time;
        ++g_manager.sounds;
    }
    void Leave(int change, std::vector<int>& list)
    {
        if (g_previous.size() > 1) {
            g_previous.front()->lastState = 6;
            g_previous.pop_front();
            g_previous.front()->lastState = 3;
            ++g_manager.removed;
        }
        g_manager.change = change;
        g_manager.actions += (int)list.size();
        lastInput = time;
    }
    void ProcessEvent(int event)
    {
        if (time - lastInput < delay) return;
        ++baseEvents;
        switch (event) {
        case EV_LEFT: Step(true); break;
        case EV_RIGHT: Step(false); break;
        case EV_BACK: Leave(1, onBack); break;
        case EV_SELECT: Leave(0, onSelect); break;
        }
    }
};
static int Fail(int n) { std::printf("00549440_TEST FAIL %d\n", n); return 1; }
int main()
{
    Param p1 = { -1 }, p2 = { -1 };
    Button button = { false, 0x40 };
    Slider s;
    s.time = 1.0f; s.lastInput = 0.9f; s.delay = 0.5f; s.selected = 0; s.previous = 0; s.maxChild = 3; s.move = false;
    s.actions.push_back(&p1); s.actions.push_back(&p2); s.accept = &button; s.baseEvents = 0;
    s.ProcessEvent(EV_LEFT);                                   // inside the delay: ignored
    if (s.baseEvents != 0 || s.selected != 0) return Fail(1);
    s.time = 2.0f;
    s.ProcessEvent(EV_LEFT);                                   // 0 wraps to MaxChild
    if (s.selected != 3 || s.previous != 0 || p1.value != 3 || p2.value != 3 || !s.move || s.lastInput != 2.0f) return Fail(2);
    if (!button.observing || button.alpha != 255 || g_manager.sounds != 1 || g_manager.actions != 2) return Fail(3);
    s.time = 3.0f;
    s.ProcessEvent(EV_RIGHT);                                  // MaxChild wraps to 0
    if (s.selected != 0 || p1.value != 0 || g_manager.sounds != 2) return Fail(4);
    s.maxChild = 0; s.time = 4.0f;
    s.ProcessEvent(EV_RIGHT);                                  // no movement: nothing else happens
    if (g_manager.sounds != 2 || s.lastInput != 3.0f) return Fail(5);
    List a = { 0 }, b = { 0 };
    g_previous.push_back(&a); g_previous.push_back(&b);
    s.onBack.push_back(1); s.time = 5.0f;
    s.ProcessEvent(EV_BACK);
    if (a.lastState != 6 || b.lastState != 3 || g_previous.size() != 1 || g_manager.change != 1 || g_manager.removed != 1) return Fail(6);
    s.time = 6.0f;
    s.ProcessEvent(EV_SELECT);                                 // one list left: not popped
    if (g_previous.size() != 1 || g_manager.change != 0 || g_manager.removed != 1 || s.lastInput != 6.0f) return Fail(7);
    std::printf("00549440_TEST PASS\n");
    return 0;
}