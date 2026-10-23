// Behaviour test for CTCLook::FrameUpdate (0x00753C70): stop-looking countdown, override paths and the head state machine.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static long g_frame = 100;

struct Look {
    long LookTimer, HeadTurnState, BodyTurnState, TimeToStopLooking;
    bool OneFrameDelay, force8, forceAA, overridden, ableToTurn, viaAnim, timeOver;
    void SetHeadTurnState(long s) { TimeToStopLooking = -1; if (HeadTurnState != s) { HeadTurnState = s; OneFrameDelay = true; } }
    void FrameUpdate()
    {
        Log("pos");
        if (TimeToStopLooking == 0) {
            if (BodyTurnState != 1) BodyTurnState = 1;
            if (HeadTurnState != 1) { HeadTurnState = 1; OneFrameDelay = true; }
            TimeToStopLooking = -2;
        } else if (TimeToStopLooking > 0)
            --TimeToStopLooking;
        if (force8) {
            if (HeadTurnState != 4) { HeadTurnState = 4; OneFrameDelay = true; }
            TimeToStopLooking = -1;
            LookTimer = g_frame + 90;
        }
        if (forceAA) { Log("centre"); Log("impl"); return; }
        if (overridden) { Log("ovr"); Log("impl"); return; }
        if (BodyTurnState & 2) {
            if (ableToTurn) {
                if (!(BodyTurnState & 8)) { Log(viaAnim ? "bodyAnim" : "body"); Log("timer"); }
                else if (BodyTurnState != 1) BodyTurnState = 1;
            }
        }
        if (OneFrameDelay) { OneFrameDelay = false; Log("impl"); return; }
        if (HeadTurnState & 1) {
            if (!(HeadTurnState & 8)) { Log("centre"); Log("impl"); return; }
        } else if (HeadTurnState & 4) {
            if (timeOver) { SetHeadTurnState(1); Log("impl"); return; }
            Log("interested"); Log("impl"); return;
        } else if (HeadTurnState & 2) {
            if (timeOver) { SetHeadTurnState(1); Log("impl"); return; }
            if (!(HeadTurnState & 8)) { Log("lookat"); Log("timer"); }
        }
        Log("impl");
    }
};

int main()
{
    Look l; std::memset(&l, 0, sizeof(l));
    l.TimeToStopLooking = 0; l.HeadTurnState = 2; l.BodyTurnState = 2;
    g_log[0] = 0; l.FrameUpdate();
    if (std::strcmp(g_log, "pos;impl;") != 0 || l.HeadTurnState != 1 || l.BodyTurnState != 1 || l.TimeToStopLooking != -2 || l.OneFrameDelay) { std::printf("00753c70_TEST FAIL\n"); return 1; }
    g_log[0] = 0; l.FrameUpdate();
    if (std::strcmp(g_log, "pos;centre;impl;") != 0) { std::printf("00753c70_TEST FAIL\n"); return 1; }
    l.HeadTurnState = 2; l.TimeToStopLooking = 5; g_log[0] = 0; l.FrameUpdate();
    if (std::strcmp(g_log, "pos;lookat;timer;impl;") != 0 || l.TimeToStopLooking != 4) { std::printf("00753c70_TEST FAIL\n"); return 1; }
    l.force8 = true; g_log[0] = 0; l.FrameUpdate();
    if (std::strcmp(g_log, "pos;impl;") != 0 || l.LookTimer != 190 || l.HeadTurnState != 4) { std::printf("00753c70_TEST FAIL\n"); return 1; }
    l.force8 = false; l.timeOver = true; g_log[0] = 0; l.FrameUpdate();
    if (std::strcmp(g_log, "pos;impl;") != 0 || l.HeadTurnState != 1) { std::printf("00753c70_TEST FAIL\n"); return 1; }
    l.forceAA = true; g_log[0] = 0; l.FrameUpdate();
    if (std::strcmp(g_log, "pos;centre;impl;") != 0) { std::printf("00753c70_TEST FAIL\n"); return 1; }
    std::printf("00753c70_TEST PASS\n");
    return 0;
}