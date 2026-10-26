// Behaviour test for 0x0049DFB0 CWorld::Update: non-type-1 events dispatched first in order, type-1 events only when
// asked and afterwards, null handlers skipped, timing members written.
#include <cstdio>
#include <cstring>
class CWorld;
class CGameEvent { public: long Type; int id; };
class CGameEventPackage { public:
    CGameEvent* events; unsigned long count;
    unsigned long GetNoEvents() const { return count; }
    const CGameEvent* GetEvent(unsigned long i) const { return &events[i]; }
};
struct SGameEventHandler { void (CWorld::*ActionFunction)(const CGameEvent* event); };
static SGameEventHandler g_GameEventHandlers[4];
static double g_time = 100.0;
static double GFGetTime() { g_time += 2.5; return g_time; }
class CThingManager { public: int id; };
static CThingManager* g_current = 0;
static void SetCurrentThingManager(CThingManager* m) { g_current = m; }
struct CWorldPrimaryBase { virtual ~CWorldPrimaryBase() { } virtual CThingManager& GetThingManager() = 0; };
struct CWorldSecondaryBase { virtual ~CWorldSecondaryBase() { } };
class CWorld : public CWorldPrimaryBase, public CWorldSecondaryBase { public:
    CThingManager things; char log[64]; double LastUpdateTimeLength, LastUpdateTime;
    CThingManager& GetThingManager() { return things; }
    void Handle(const CGameEvent* e) { char b[8]; std::sprintf(b, "%d,", e->id); std::strcat(log, b); }
    const SGameEventHandler* GetEventDispatchTableEntry(long type) const { return &g_GameEventHandlers[type]; }
    void ProcessEvent(const CGameEvent* event)
    {
        const SGameEventHandler* entry = GetEventDispatchTableEntry(event->Type);
        if (entry->ActionFunction) (this->*entry->ActionFunction)(event);
    }
    void Update(const CGameEventPackage& events, bool processType1Events)
    {
        double start = GFGetTime();
        LastUpdateTime = start;
        SetCurrentThingManager(&GetThingManager());
        for (unsigned long i = 0; i < events.GetNoEvents(); ++i) {
            const CGameEvent* event = events.GetEvent(i);
            if (event->Type != 1) ProcessEvent(event);
        }
        if (processType1Events) {
            for (unsigned long i = 0; i < events.GetNoEvents(); ++i) {
                const CGameEvent* event = events.GetEvent(i);
                if (event->Type == 1) ProcessEvent(event);
            }
        }
        LastUpdateTimeLength = GFGetTime() - start;
    }
};
int main()
{
    g_GameEventHandlers[0].ActionFunction = &CWorld::Handle;
    g_GameEventHandlers[1].ActionFunction = &CWorld::Handle;
    g_GameEventHandlers[2].ActionFunction = 0;
    g_GameEventHandlers[3].ActionFunction = &CWorld::Handle;
    CGameEvent ev[5] = { { 1, 1 }, { 0, 2 }, { 2, 3 }, { 3, 4 }, { 1, 5 } };
    CGameEventPackage p; p.events = ev; p.count = 5;
    CWorld w; w.log[0] = 0;
    w.Update(p, false);
    bool ok = std::strcmp(w.log, "2,4,") == 0 && g_current == &w.things && w.LastUpdateTime == 102.5 && w.LastUpdateTimeLength == 2.5;
    w.log[0] = 0;
    w.Update(p, true);
    ok = ok && std::strcmp(w.log, "2,4,1,5,") == 0;
    if (!ok) { std::printf("0049dfb0_TEST FAIL\n"); return 1; }
    std::printf("0049dfb0_TEST PASS\n");
    return 0;
}