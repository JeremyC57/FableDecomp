#include <cstdio>
// Behaviour model of the CVideoSys DirectShow wrapper with fake control/position/event objects.
struct Control { int runFailures; int runs; int pauses; long Run() { ++runs; return runs <= runFailures ? 1 : 0; } long Pause() { ++pauses; return 0; } };
struct Position { int putFailures; int puts; double cur, dur; long put_CurrentPosition(double t) { ++puts; if (puts <= putFailures) return 1; cur = t; return 0; }
    long get_CurrentPosition(double* t) { *t = cur; return 0; } long get_Duration(double* d) { *d = dur; return 0; } };
struct Event { long code; long GetEvent(long* c, long* p1, long* p2, long) { *c = code; *p1 = 0; *p2 = 0; return 0; } };
struct VideoSys {
    Control* MControl; Position* MPosition; Event* MEvent; long PlayMode; long State; int lastWait; int stops;
    void WaitForState(int s) { lastWait = s; }
    unsigned char Stop() { ++stops; State = 3; return 1; }
    float GetCurTime() { if (!MPosition) return 0.0f; double t = 0.0; MPosition->get_CurrentPosition(&t); return (float)t; }
    float GetLength() { if (!MPosition) return 0.0f; double l = 0.0; MPosition->get_Duration(&l); return (float)l; }
    bool Play()
    {
        if (State != 0 && MControl != 0 && MPosition != 0) {
            while (MPosition->put_CurrentPosition(0.0) != 0) ;
            long tries = 0;
            while (tries < 50 && (++tries, MControl->Run() != 0)) ;
            if (tries != 50) { WaitForState(2); State = 1; return true; }
        }
        return false;
    }
    float GetProgress() { float t = GetCurTime(); float l = GetLength(); if (l == 0.0f) return 0.0f; return t / l; }
    bool Process()
    {
        long code = 0, p1 = 0, p2 = 0;
        if (MEvent != 0) { MEvent->GetEvent(&code, &p1, &p2, 0); if (code == 1) { if (PlayMode == 0) Play(); else Stop(); } }
        return true;
    }
    void TogglePause(bool pause)
    {
        if (pause) { if (State != 0) { MControl->Pause(); WaitForState(1); State = 2; } }
        else if (State != 0 && MControl != 0 && MPosition != 0) { while (MControl->Run() != 0) ; WaitForState(2); State = 1; }
    }
};
static void Reset(VideoSys& v, Control& c, Position& p, Event& e)
{
    c.runFailures = 0; c.runs = 0; c.pauses = 0; p.putFailures = 0; p.puts = 0; p.cur = 5.0; p.dur = 20.0; e.code = 0;
    v.MControl = &c; v.MPosition = &p; v.MEvent = &e; v.PlayMode = 0; v.State = 3; v.lastWait = -1; v.stops = 0;
}
int main()
{
    Control c; Position p; Event e; VideoSys v; Reset(v, c, p, e);
    v.State = 1; v.TogglePause(true);
    if (v.State != 2 || c.pauses != 1 || v.lastWait != 1) { std::printf("00a3b4a0_TEST FAIL\n"); return 1; }
    c.runFailures = 2; v.TogglePause(false);
    if (v.State != 1 || c.runs != 3 || v.lastWait != 2) { std::printf("00a3b4a0_TEST FAIL\n"); return 1; }
    v.State = 0; v.TogglePause(true); if (c.pauses != 1) { std::printf("00a3b4a0_TEST FAIL\n"); return 1; }
    std::printf("00a3b4a0_TEST PASS\n");
    return 0;
}