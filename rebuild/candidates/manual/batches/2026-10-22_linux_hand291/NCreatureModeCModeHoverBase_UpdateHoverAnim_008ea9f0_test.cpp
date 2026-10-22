// Behaviour test for CModeHoverBase::UpdateHoverAnim (0x008EA9F0): queued-anim playback vs. state-driven re-queue.
#include <cstdio>
#include <cstring>
#include <list>
#include <string>

struct Anim { int index; bool looping; bool finished; bool empty; int startedIndex; int startedMode; bool startedLooping; };
struct Hover { int entry; std::string name; bool looping; };
struct Mode {
    std::list<Hover> queue;
    std::string currentName;
    std::string stateName;
};
static int Lookup(const std::string& name) { return name == "DEFAULT" || name == "HOVER_UNKNOWN" ? 1 : (int)name.size() + 10; }
static void Update(Mode& m, Anim& a)
{
    if (!m.queue.empty()) {
        Hover h = m.queue.front();
        int mode = (h.looping && a.looping) ? 6 : 2;
        a.startedIndex = h.entry;
        a.startedLooping = h.looping;
        a.startedMode = mode;
        m.queue.pop_front();
    } else {
        std::string name = m.stateName;
        if ((m.currentName != name && a.looping) || a.empty || a.finished) {
            int def = Lookup("DEFAULT");
            int entry = Lookup(name);
            if (entry != def) {
                Hover h = { entry, name, true };
                m.queue.push_back(h);
            }
            m.currentName = name;
        }
    }
}
int main()
{
    Mode m;
    Anim a;
    std::memset(&a, 0, sizeof(a));
    a.looping = true;
    m.currentName = "HOVER_SLOW";
    m.stateName = "HOVER_FAST";
    Update(m, a);                     // state changed while looping: queue HOVER_FAST
    int ok = m.queue.size() == 1 && m.currentName == "HOVER_FAST" && m.queue.front().entry == 20;
    Update(m, a);                     // play it: both loop -> mode 6
    ok = ok && m.queue.empty() && a.startedIndex == 20 && a.startedMode == 6 && a.startedLooping;
    Update(m, a);                     // same state, still running: nothing
    ok = ok && m.queue.empty();
    a.looping = false;
    a.finished = true;
    m.stateName = "HOVER_UNKNOWN";    // resolves to DEFAULT: not queued, but becomes current
    Update(m, a);
    ok = ok && m.queue.empty() && m.currentName == "HOVER_UNKNOWN";
    m.stateName = "HOVER_MED";
    Update(m, a);
    Update(m, a);                     // played with a non-looping current anim -> mode 2
    ok = ok && a.startedMode == 2 && a.startedIndex == 19;
    std::printf(ok ? "008ea9f0_TEST PASS\n" : "008ea9f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
