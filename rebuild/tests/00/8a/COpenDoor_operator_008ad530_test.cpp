// behaviour test for 0x008AD530 COpenDoor::operator()
#include <cstdio>
struct CTCDoor {
    bool CurrentOpen, OldOpen, Opening, Closing; int triggered; bool lastArg;
    void ScriptTriggerOpen(bool open) { ++triggered; lastArg = open; }
    bool IsOpen() const { return !Opening && !Closing && CurrentOpen; }
};
struct CThing { CTCDoor* door; unsigned int HasTC(long) const { return door != 0; } CTCDoor* GetTC(long) { return door; } };
struct COpenDoor {
    bool m_open, m_force;
    void operator()(CThing* thing) const
    {
        if (thing && thing->HasTC(0x26)) {
            CTCDoor* door = thing->GetTC(0x26);
            if (m_force) door->ScriptTriggerOpen(m_open);
            else door->ScriptTriggerOpen(!door->IsOpen());
        }
    }
};
int main()
{
    CTCDoor d = { true, false, false, false, 0, false };
    CThing t = { &d }, none = { 0 };
    COpenDoor forceClose = { false, true }, toggle = { true, false };
    forceClose(&t);
    int bad = d.triggered != 1 || d.lastArg != false;
    toggle(&t);
    bad |= d.triggered != 2 || d.lastArg != false;
    d.CurrentOpen = false; toggle(&t);
    bad |= d.triggered != 3 || d.lastArg != true;
    d.CurrentOpen = true; d.Opening = true; toggle(&t);
    bad |= d.triggered != 4 || d.lastArg != true;
    toggle(&none); toggle(0);
    bad |= d.triggered != 4;
    if (bad) { std::printf("008ad530_TEST FAIL\n"); return 1; }
    std::printf("008ad530_TEST PASS\n");
    return 0;
}