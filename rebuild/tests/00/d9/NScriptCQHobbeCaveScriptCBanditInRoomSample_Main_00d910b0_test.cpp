// Behaviour test for NScript::CQ_HobbeCaveScript::CBanditInRoomSample::Main (0x00D910B0): a timer is registered for the
// script's life; each frame, once the parent reports the side-room hobbes killed the sample removes itself (unless the
// thread is terminating) and stops; otherwise when the timer has run out and the hero is within range it restarts the timer
// with the def delay and starts a conversation line to the hero. The timer is deregistered on every exit.
#include <cstdio>

class CGame { public:
    int  registered, deregistered, timerValue, removed, conversations, frames;
    int  terminateAfterFrames;      // IsActiveThreadTerminating() turns true after this many frames
    int  killedAtFrame;             // the parent flag turns on at this frame
    int  heroNearFrom;              // the hero is within range from this frame on
    long RegisterTimer() { ++registered; return 7; }
    void DeregisterTimer(long) { ++deregistered; }
    long GetTimer(long) const { return timerValue; }
    void SetTimer(long, long v) { timerValue = v; }
    bool Terminating() const { return frames >= terminateAfterFrames; }
    void NewScriptFrame() { ++frames; if (timerValue > 0) --timerValue; }
};
static CGame* g_pGame;
class CTimer { public:
    long TimerIndex;
    CTimer() : TimerIndex(g_pGame->RegisterTimer()) {}
    ~CTimer() { g_pGame->DeregisterTimer(TimerIndex); }
    long GetTime() const { return g_pGame->GetTimer(TimerIndex); }
    CTimer& operator=(long t) { g_pGame->SetTimer(TimerIndex, t); return *this; }
};
const long kDelay = 3;

void Main()
{
    CTimer timer;
    while (!g_pGame->Terminating()) {
        if (!(g_pGame->frames >= g_pGame->killedAtFrame)) {
            if (timer.GetTime() == 0 && g_pGame->frames >= g_pGame->heroNearFrom) {
                if (g_pGame->Terminating())
                    return;
                timer = kDelay;
                ++g_pGame->conversations;
            }
            g_pGame->NewScriptFrame();
        } else {
            if (g_pGame->Terminating())
                return;
            ++g_pGame->removed;
            return;
        }
    }
}

int main()
{
    CGame a = { 0, 0, 0, 0, 0, 0, 100, 10, 2 };     // hero near from frame 2, killed at frame 10
    g_pGame = &a;
    Main();
    // conversations at frames 2, 5, 8 (timer 3 counts down per frame); removed once at frame 10; timer freed once.
    bool ok = a.registered == 1 && a.deregistered == 1 && a.conversations == 3 && a.removed == 1 && a.frames == 10;
    CGame b = { 0, 0, 0, 0, 0, 0, 4, 100, 100 };    // never near, terminates at frame 4
    g_pGame = &b;
    Main();
    ok = ok && b.registered == 1 && b.deregistered == 1 && b.conversations == 0 && b.removed == 0 && b.frames == 4;
    if (!ok) { std::printf("00d910b0_TEST FAIL\n"); return 1; }
    std::printf("00d910b0_TEST PASS\n");
    return 0;
}