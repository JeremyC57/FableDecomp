// Behaviour test for 0x009F1650 CGameEventPackage::AddEvent.
#include <cstdio>
#include <cstring>
struct CGameEvent { int Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement; };
struct CGameEventPackage {
    int TimeStamp; unsigned int NoEvents; CGameEvent Events[40];
    void AddEvent(const CGameEvent& event)
    {
        if (event.Replacement) {
            for (unsigned int i = 0; i < NoEvents; ++i) {
                if (Events[i].Type == event.Type && Events[i].Player == event.Player) { Events[i] = event; return; }
            }
        }
        Events[NoEvents] = event;
        ++NoEvents;
    }
};
static int Fail() { std::printf("009f1650_TEST FAIL\n"); return 1; }
int main()
{
    static CGameEventPackage p; p.NoEvents = 0;
    CGameEvent e; std::memset(&e, 0, sizeof(e)); e.Type = 5; e.Player = 1; e.EndPos = 1;
    p.AddEvent(e);
    e.Replacement = true; e.EndPos = 2; p.AddEvent(e);
    if (p.NoEvents != 1 || p.Events[0].EndPos != 2) return Fail();
    e.Player = 2; e.EndPos = 3; p.AddEvent(e);
    if (p.NoEvents != 2 || p.Events[1].EndPos != 3 || p.Events[1].Player != 2) return Fail();
    e.Replacement = false; e.Player = 1; p.AddEvent(e);
    if (p.NoEvents != 3 || p.Events[0].EndPos != 2) return Fail();
    std::printf("009f1650_TEST PASS\n");
    return 0;
}