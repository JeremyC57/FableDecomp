#include <cstdio>
// Behaviour model of CTCBoastUI::BoastTaken: erase the taken boast's group, re-select the first type-8 child of
// every group sharing its group bits, notify the crowd, and close the UI when no accepted boast is left.
struct Group { int id; int bits; int types[4]; int count; int selected; bool alive; };
struct State { Group groups[3]; int crowdBoasts; bool hasCrowd; int acceptedLeft; bool requested; bool active; bool quitting; bool closed; int anim; };
static void BoastTaken(State& s, int id, int bits)
{
    for (int g = 0; g < 3; ++g) if (s.groups[g].alive && s.groups[g].id == id) { s.groups[g].alive = false; break; }
    for (int g = 0; g < 3; ++g) {
        Group& gr = s.groups[g];
        if (!gr.alive || !(gr.bits & bits)) continue;
        for (int i = 0; i != gr.count; ++i) if (gr.types[i] == 8) { gr.selected = i; break; }
    }
    if (s.hasCrowd) ++s.crowdBoasts;
    if (s.acceptedLeft == 0 && s.requested) {
        if (s.active) { s.quitting = true; s.closed = true; } else s.requested = false;
    }
    ++s.anim;
}
int main()
{
    State s = { { { 1, 1, { 3, 8, 8, 0 }, 3, -1, true }, { 2, 3, { 8, 1, 0, 0 }, 2, -1, true }, { 3, 4, { 8, 0, 0, 0 }, 1, -1, true } },
                0, true, 0, true, true, false, false, 0 };
    BoastTaken(s, 1, 1);
    int ok = !s.groups[0].alive && s.groups[1].selected == 0 && s.groups[2].selected == -1 && s.crowdBoasts == 1
          && s.quitting && s.closed && s.anim == 1;
    s.requested = true; s.active = false; s.quitting = false; s.acceptedLeft = 0;
    BoastTaken(s, 9, 4);
    ok = ok && s.groups[2].selected == 0 && !s.requested && !s.quitting;
    std::printf(ok ? "0056a980_TEST PASS\n" : "0056a980_TEST FAIL\n");
    return ok ? 0 : 1;
}