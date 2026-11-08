// Behaviour test for 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed: crimes count only in a region that uses or permits crimes
// and a village that has not suspended them; a bribed guard ignores a crime that is not against himself; the deed joins the pending
// list ahead of the first deed with a lower reaction priority (stale handles are dropped on the way); the transient effects are
// applied only when the per-type minimum gap since the last application has passed.
#include <cstdio>
#include <list>
#include <map>
struct Model {
    std::list<long> PendingDeeds; long LastFrameApplied[4]; std::map<long, long> Priority; int applied;
    Model() : applied(0) { for (int i = 0; i < 4; ++i) LastFrameApplied[i] = 0; }
    static bool CrimesCount(bool has_region, bool use, bool permit, bool suspended) { return has_region && (use || permit) && !suspended; }
    static bool GuardIgnores(long frame, long bribe_ends, bool against_me) { return frame < bribe_ends && !against_me; }
    void Inform(int type, long handle, long frame, long min_seconds, long fps)
    {
        bool apply_effects = true;
        if (min_seconds > 0) {
            long gap = min_seconds * fps;
            long since = gap < frame ? frame - gap : 0;
            apply_effects = LastFrameApplied[type] <= since;
        }
        long priority = Priority[handle];
        std::list<long>::iterator it = PendingDeeds.begin();
        while (it != PendingDeeds.end()) {
            if (Priority.find(*it) == Priority.end()) { it = PendingDeeds.erase(it); continue; }
            if (Priority[*it] < priority) { PendingDeeds.insert(it, handle); goto inserted; }
            ++it;
        }
        PendingDeeds.push_back(handle);
    inserted:
        if (apply_effects) { LastFrameApplied[type] = frame; ++applied; }
    }
};
int main()
{
    if (!Model::CrimesCount(true, false, true, false) || Model::CrimesCount(true, true, true, true) || Model::CrimesCount(false, true, true, false))
    { std::printf("00682810_TEST FAIL\n"); return 1; }
    if (!Model::GuardIgnores(10, 20, false) || Model::GuardIgnores(10, 20, true) || Model::GuardIgnores(30, 20, false))
    { std::printf("00682810_TEST FAIL\n"); return 1; }
    Model m; m.Priority[1] = 5; m.Priority[2] = 3; m.Priority[3] = 4;
    m.PendingDeeds.push_back(99);                       // stale handle: dropped
    m.Inform(1, 1, 100, 2, 30);                         // frame 100 > gap 60: since 40, last 0 -> applied
    m.Inform(1, 2, 110, 2, 30);                         // since 50 < last 100 -> not applied
    m.Inform(2, 3, 120, 0, 30);                         // no gap -> applied; priority 4 goes before 3
    long expect[3] = { 1, 3, 2 }; int i = 0;
    for (std::list<long>::iterator it = m.PendingDeeds.begin(); it != m.PendingDeeds.end(); ++it, ++i)
        if (i >= 3 || *it != expect[i]) { std::printf("00682810_TEST FAIL\n"); return 1; }
    if (i != 3 || m.applied != 2 || m.LastFrameApplied[1] != 100 || m.LastFrameApplied[2] != 120)
    { std::printf("00682810_TEST FAIL\n"); return 1; }
    std::printf("00682810_TEST PASS\n");
    return 0;
}
