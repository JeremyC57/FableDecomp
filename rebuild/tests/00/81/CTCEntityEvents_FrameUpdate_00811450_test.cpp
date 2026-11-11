// Behaviour test for 0x00811450 CTCEntityEvents::FrameUpdate: Events.remove_if(CPred_EventExpired(GetWorld().GetFrame())) -- the
// GetWorld() call is made, the predicate carries the current frame, and only expired events (ready to die and past
// creation frame + lifetime) leave the list.
#include <cstdio>
class CWorld { public: static long GetFrame(); };
static long g_frame = 0;
static int g_getWorldCalls = 0;
long CWorld::GetFrame() { return g_frame; }
static CWorld g_world;
struct CEvent { long WorldFrame; long LifeTime; bool ReadyToDie; };
struct CPred_EventExpired {
    explicit CPred_EventExpired(long worldFrame) : WorldFrame(worldFrame) { }
    bool operator()(const CEvent& e) const { return e.ReadyToDie && WorldFrame > e.WorldFrame + e.LifeTime; }
    long WorldFrame;
};
struct CEventList {                                        // the list's remove_if over a small fixed store
    CEvent Items[8]; int Count;
    CEventList() : Count(0) { }
    void push_back(const CEvent& e) { Items[Count++] = e; }
    template <class P> void remove_if(P pred)
    {
        int kept = 0;
        for (int i = 0; i < Count; ++i)
            if (!pred(Items[i]))
                Items[kept++] = Items[i];
        Count = kept;
    }
    int size() const { return Count; }
    const CEvent& front() const { return Items[0]; }
};
struct CTCEntityEvents {
    CEventList Events;
    CWorld& GetWorld() const { ++g_getWorldCalls; return g_world; }
    void FrameUpdate() { Events.remove_if(CPred_EventExpired(GetWorld().GetFrame())); }
};
int main()
{
    CTCEntityEvents events;
    CEvent a = { 0, 10, true }, b = { 5, 10, true }, c = { 0, 1, false };
    events.Events.push_back(a); events.Events.push_back(b); events.Events.push_back(c);
    g_frame = 11;
    events.FrameUpdate();                                  // a expires (11 > 10); b (15) stays; c not ready to die
    bool ok = events.Events.size() == 2 && events.Events.front().WorldFrame == 5 && g_getWorldCalls == 1;
    g_frame = 16;
    events.FrameUpdate();                                  // b expires
    ok = ok && events.Events.size() == 1 && !events.Events.front().ReadyToDie && g_getWorldCalls == 2;
    if (ok) printf("00811450_TEST PASS\n");
    else printf("00811450_TEST FAIL\n");
    return ok ? 0 : 1;
}
