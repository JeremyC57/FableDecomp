// Behaviour test for CombatDialogueManager::AttemptToSpeak (0x008AF500), g++ model of the retail logic.
#include <cstdio>
#include <map>
struct Talker { bool dead; bool hasTalk; int result; unsigned long lastGroup; int plays; };
struct Info { long NextAllowedFrame; };
struct Manager { std::map<unsigned long, Info> groups; float repeat; Talker* talker; unsigned long pending; };
static long g_frame = 1000, g_fps = 30;
static void AttemptToSpeak(Manager& m)
{
    if (m.talker == 0 || m.talker->dead) return;
    if (m.pending == 0) return;
    if (!m.talker->hasTalk) return;
    ++m.talker->plays; m.talker->lastGroup = m.pending;
    int result = m.talker->result;
    if (result == 0) {
        std::map<unsigned long, Info>::iterator it = m.groups.find(m.pending);
        it->second.NextAllowedFrame = (long)(g_frame + g_fps * m.repeat);
        m.pending = 0;
    } else if (result == 1)
        m.pending = 0;
}
int main()
{
    Talker t = { false, true, 0, 0, 0 };
    Manager m; m.repeat = 2.5f; m.talker = &t; m.pending = 7; m.groups[7].NextAllowedFrame = 0;
    AttemptToSpeak(m);
    int ok = t.plays == 1 && t.lastGroup == 7 && m.pending == 0 && m.groups[7].NextAllowedFrame == 1075;
    m.pending = 7; t.result = 2; AttemptToSpeak(m); ok &= m.pending == 7 && t.plays == 2;
    t.result = 1; AttemptToSpeak(m); ok &= m.pending == 0 && t.plays == 3;
    m.pending = 7; t.dead = true; AttemptToSpeak(m); ok &= t.plays == 3 && m.pending == 7;
    t.dead = false; t.hasTalk = false; AttemptToSpeak(m); ok &= t.plays == 3;
    std::printf(ok ? "008af500_TEST PASS\n" : "008af500_TEST FAIL\n");
    return ok ? 0 : 1;
}