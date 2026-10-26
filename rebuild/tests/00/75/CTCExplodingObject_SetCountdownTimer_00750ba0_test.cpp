#include <cstdio>
class CThing { public:
    bool m_forceFrameUpdate; int m_calls;
    void SetForceFrameUpdate(bool update) { m_forceFrameUpdate = update; ++m_calls; }
};
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
class CTCExplodingObject { public:
    CThing* ParentThing; long CountdownTimer;
    void SetCountdownTimer(long frames)
    {
        ParentThing->SetForceFrameUpdate(true);
        if (CountdownTimer > 0) CountdownTimer = Min(frames, CountdownTimer);
        else CountdownTimer = frames;
    }
};
int main()
{
    CThing t; t.m_forceFrameUpdate = false; t.m_calls = 0;
    CTCExplodingObject o; o.ParentThing = &t; o.CountdownTimer = 0;
    o.SetCountdownTimer(50); long a = o.CountdownTimer;       // not running: take it
    o.SetCountdownTimer(80); long b = o.CountdownTimer;       // running: keep the shorter
    o.SetCountdownTimer(20); long c = o.CountdownTimer;
    o.CountdownTimer = -1; o.SetCountdownTimer(90); long d = o.CountdownTimer;
    if (a != 50 || b != 50 || c != 20 || d != 90 || t.m_calls != 4 || !t.m_forceFrameUpdate) { std::printf("00750ba0_TEST FAIL\n"); return 1; }
    std::printf("00750ba0_TEST PASS\n");
    return 0;
}