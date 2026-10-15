#include <cstdio>
// IsAnotherGuardAdmonishingPlayer: a type-0x1b message not originated by this creature -> adopt id/source, true
struct Event { unsigned long long originatorUID; void* source; long id; };
static Event* g_event = 0;
struct Group {
    unsigned long long creatureUID; void* warningSource; long lastWarningId;
    bool Run()
    {
        Event* e = g_event;
        if (e) {
            if (e->originatorUID != creatureUID) { lastWarningId = e->id; warningSource = e->source; return true; }
        }
        return false;
    }
};
int main()
{
    int src; Event e; e.originatorUID = 0x100000002ULL; e.source = &src; e.id = 42; g_event = &e;
    Group g; g.creatureUID = 0x100000003ULL; g.warningSource = 0; g.lastWarningId = 0;
    if (!g.Run() || g.lastWarningId != 42 || g.warningSource != &src) { std::printf("00939d00_TEST FAIL\n"); return 1; }
    g.creatureUID = 0x100000002ULL; g.lastWarningId = 0;
    if (g.Run() || g.lastWarningId != 0) { std::printf("00939d00_TEST FAIL\n"); return 1; }
    g_event = 0; if (g.Run()) { std::printf("00939d00_TEST FAIL\n"); return 1; }
    std::printf("00939d00_TEST PASS\n"); return 0;
}