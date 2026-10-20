// Behaviour test for CTCLook::TrackNearestInterestingThing @ 0x00753840 (self-contained model of the candidate logic).
#include <stdio.h>
#include <string.h>
struct CTCTargetingBase { void* nearest; void* GetPNearestInterestingThing() { return nearest; } };
struct CThing { CTCTargetingBase* targeting; bool GetInterface(CTCTargetingBase** out) { if (targeting) { *out = targeting; return true; } return false; } };
static char g_log[64];
struct Model {
    CThing* ParentThing; unsigned long HeadTurnState; long TimeToStopLooking; bool OneFrameDelay;
    void UpdateLookTimer() { strcat(g_log, "U"); }
    void SetHeadTurnState(long s) { HeadTurnState = (unsigned long)s; strcat(g_log, s == 1 ? "1" : "4"); }
    long TrackNearestInterestingThing(bool track)
    {
        unsigned long state = HeadTurnState;
        if (!(state & 1)) return (state >> 2) & 1;
        if (track) {
            CTCTargetingBase* t;
            if (ParentThing->GetInterface(&t)) {
                if (state & 4) { if (t->GetPNearestInterestingThing()) UpdateLookTimer(); else SetHeadTurnState(1); }
                else if (t->GetPNearestInterestingThing()) { UpdateLookTimer(); SetHeadTurnState(4); }
            }
        } else {
            if (state != 1) { HeadTurnState = 1; OneFrameDelay = true; }
            TimeToStopLooking = -1;
        }
        return (HeadTurnState >> 2) & 1;
    }
};
int main()
{
    bool ok = true; int dummy = 0;
    CTCTargetingBase targeting = { 0 }; CThing thing = { &targeting }; CThing bare = { 0 };
    Model m = { &thing, 0, 5, false };
    ok = ok && m.TrackNearestInterestingThing(true) == 0 && g_log[0] == 0;            // disabled: nothing happens, not tracking
    m.HeadTurnState = 4; ok = ok && m.TrackNearestInterestingThing(true) == 1;          // disabled (bit 0 clear) but bit 2 reported
    m.HeadTurnState = 1; targeting.nearest = &dummy;
    ok = ok && m.TrackNearestInterestingThing(true) == 1 && strcmp(g_log, "U4") == 0;  // starts tracking
    m.HeadTurnState = 5;                                                               // enabled and tracking
    ok = ok && m.TrackNearestInterestingThing(true) == 1 && strcmp(g_log, "U4U") == 0; // keeps tracking
    targeting.nearest = 0;
    ok = ok && m.TrackNearestInterestingThing(true) == 0 && strcmp(g_log, "U4U1") == 0; // loses the target: idle turn
    m.HeadTurnState = 5; ok = ok && m.TrackNearestInterestingThing(false) == 0 && m.HeadTurnState == 1 && m.OneFrameDelay && m.TimeToStopLooking == -1;
    m.OneFrameDelay = false; m.TimeToStopLooking = 9; ok = ok && m.TrackNearestInterestingThing(false) == 0 && !m.OneFrameDelay && m.TimeToStopLooking == -1;
    Model n = { &bare, 5, 0, false }; g_log[0] = 0; ok = ok && n.TrackNearestInterestingThing(true) == 1 && g_log[0] == 0;   // no targeting component
    if (!ok) { printf("00753840_TEST FAIL\n"); return 1; }
    printf("00753840_TEST PASS\n"); return 0;
}
