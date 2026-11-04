// Behaviour test for NUISystem::CSlider::ProcessEvent (0x0054AC90): a standalone model of the slider input -- LEFT / RIGHT step
// Value.X by Step.X clamped to Min.X / Max.X; when the value moved, every action's float parameter gets the new value and is run,
// Move is set, the accept button starts observing mouse movement at full alpha, and the back (LEFT) / forward (RIGHT) sound
// plays; a clamped no-op does nothing. BACK / SELECT pop the previous-selected stack when it holds more than one list, play the
// back / forward sound, post REJECT / ACCEPT and run the on-back / on-select action list.
#include <cstdio>
#include <vector>
#include <list>
enum { SOUND_NONE, SOUND_BACK, SOUND_FORWARD };
enum { CHANGE_NONE, CHANGE_ACCEPT, CHANGE_REJECT };
struct Slider {
    float value, oldValue, step, minV, maxV; bool move; bool acceptObserving; unsigned char acceptAlpha;
    std::vector<float> actionParams; int actionsRun; int sound; int change; std::list<int> previous; int listRuns;
};
enum { EV_SELECT = 4, EV_BACK = 5, EV_LEFT = 20, EV_RIGHT = 21 };
static void ProcessEvent(Slider& s, int ev, int listCount)
{
    if (ev == EV_LEFT || ev == EV_RIGHT) {
        s.oldValue = s.value;
        float v = ev == EV_LEFT ? s.value - s.step : s.value + s.step;
        if (ev == EV_LEFT) s.value = v >= s.minV ? v : s.minV; else s.value = v <= s.maxV ? v : s.maxV;
        if (s.oldValue != s.value) {
            if (!s.acceptObserving) { s.acceptObserving = true; s.acceptAlpha = 255; }
            for (size_t i = 0; i < s.actionParams.size(); ++i) { s.actionParams[i] = s.value; ++s.actionsRun; }
            s.move = true;
            s.sound = ev == EV_LEFT ? SOUND_BACK : SOUND_FORWARD;
        }
    } else if (ev == EV_BACK || ev == EV_SELECT) {
        if (s.previous.size() > 1) s.previous.pop_front();
        s.sound = ev == EV_BACK ? SOUND_BACK : SOUND_FORWARD;
        s.change = ev == EV_BACK ? CHANGE_REJECT : CHANGE_ACCEPT;
        s.listRuns += listCount;
    }
}
int main()
{
    int fail = 0;
    Slider s = { 0.5f, 0, 0.25f, 0.0f, 1.0f, false, false, 0, std::vector<float>(2, 0.0f), 0, SOUND_NONE, CHANGE_NONE, std::list<int>(), 0 };
    ProcessEvent(s, EV_LEFT, 0);
    if (s.value != 0.25f || s.actionParams[1] != 0.25f || s.actionsRun != 2 || !s.move || s.sound != SOUND_BACK || s.acceptAlpha != 255) fail = 1;
    ProcessEvent(s, EV_LEFT, 0); ProcessEvent(s, EV_LEFT, 0);
    if (s.value != 0.0f || s.actionsRun != 4) fail = 2;                 // clamped at Min, the second step is a no-op
    s.sound = SOUND_NONE;
    ProcessEvent(s, EV_RIGHT, 0);
    if (s.value != 0.25f || s.sound != SOUND_FORWARD || s.actionsRun != 6) fail = 3;
    s.value = 1.0f; s.sound = SOUND_NONE;
    ProcessEvent(s, EV_RIGHT, 0);
    if (s.value != 1.0f || s.sound != SOUND_NONE) fail = 4;             // at Max: nothing
    s.previous.push_back(1); s.previous.push_back(2);
    ProcessEvent(s, EV_BACK, 3);
    if (s.previous.size() != 1 || s.change != CHANGE_REJECT || s.sound != SOUND_BACK || s.listRuns != 3) fail = 5;
    ProcessEvent(s, EV_SELECT, 2);
    if (s.previous.size() != 1 || s.change != CHANGE_ACCEPT || s.sound != SOUND_FORWARD || s.listRuns != 5) fail = 6;
    if (fail) { std::printf("0054ac90_TEST FAIL %d\n", fail); return 1; }
    std::printf("0054ac90_TEST PASS\n");
    return 0;
}
