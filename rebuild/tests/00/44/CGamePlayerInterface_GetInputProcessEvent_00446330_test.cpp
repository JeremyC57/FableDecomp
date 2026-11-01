// Behaviour test for CGamePlayerInterface::GetInputProcessEvent 0x00446330 (model copy of the source logic: event scan,
// process scan with the safe scan-info registration, best-priority selection, consumed events flagged as processed).
#include <cstdio>
struct Event { int type, deviceType, key, id; };
struct Result { int type, priority; };
struct Process { bool accepts; int resultType, priority; bool whilePaused; };
struct ScanInfo { int current; bool started; void* list; ScanInfo* nextScan; ScanInfo* prevScan; };
struct Model {
    Event events[4]; int noEvents; bool processed[16]; int flagged[16]; int noFlagged;
    Process procs[4]; int noProcs; bool paused; bool editor; ScanInfo* firstScan;
    bool AltQueued() const { for (int i = 0; i < noEvents; ++i) if (events[i].type == 2 && events[i].key == 0x38) return true; return false; }
    void Flag(const Event& e) { processed[e.id] = true; flagged[noFlagged++] = e.id; }
    bool Get(Result& out)
    {
        Result best = { 0, 0 }; Event bestEvent = { 0, 0, 0, 0 };
        bool altDown = !editor && AltQueued();
        long bestPriority = 0; bool found = false;
        for (int ei = 0; ei < noEvents; ++ei) {
            const Event& e = events[ei];
            if (altDown && e.deviceType == 2 && e.key == 0xf) continue;
            if (e.type == 0) continue;
            if (processed[e.id]) continue;
            ScanInfo s = { 0, true, this, firstScan, 0 }; firstScan = &s;
            for (s.current = 0; s.current < noProcs; ++s.current) {
                const Process& p = procs[s.current];
                Result r = { 0, 0 };
                if (p.accepts && (p.whilePaused || !paused)) { r.type = p.resultType; r.priority = p.priority; }
                if (r.type == 1) { found = true; if (r.priority > bestPriority) { bestPriority = r.priority; best = r; bestEvent = e; } }
                else if (r.type == 2) { Flag(e); break; }
            }
            firstScan = s.nextScan; s.started = false; s.list = 0;
        }
        if (found) { out = best; if (bestEvent.type) Flag(bestEvent); return true; }
        return false;
    }
};
int main()
{
    Model m = {};
    m.noEvents = 3;
    m.events[0].type = 1; m.events[0].id = 1;
    m.events[1].type = 0; m.events[1].id = 2;
    m.events[2].type = 3; m.events[2].id = 3;
    m.noProcs = 2;
    m.procs[0].accepts = true; m.procs[0].resultType = 1; m.procs[0].priority = 5;
    m.procs[1].accepts = true; m.procs[1].resultType = 1; m.procs[1].priority = 9; m.procs[1].whilePaused = true;
    Result r;
    if (!m.Get(r) || r.priority != 9 || m.noFlagged != 1 || m.flagged[0] != 1 || m.firstScan != 0) { std::printf("00446330_TEST FAIL\n"); return 1; }
    Model c = {};
    c.noEvents = 1; c.events[0].type = 1; c.events[0].id = 4;
    c.noProcs = 2; c.procs[0].accepts = true; c.procs[0].resultType = 2; c.procs[1].accepts = true; c.procs[1].resultType = 1; c.procs[1].priority = 3;
    if (c.Get(r) || c.noFlagged != 1 || c.flagged[0] != 4) { std::printf("00446330_TEST FAIL\n"); return 1; }
    Model p = {};
    p.noEvents = 1; p.events[0].type = 1; p.events[0].id = 5; p.paused = true;
    p.noProcs = 1; p.procs[0].accepts = true; p.procs[0].resultType = 1; p.procs[0].priority = 1;
    if (p.Get(r)) { std::printf("00446330_TEST FAIL\n"); return 1; }
    std::printf("00446330_TEST PASS\n");
    return 0;
}