// Behaviour test for 0x00A0E0A0 CJoystickPositionTracker::CJoystickPositionTracker() (history resized to 30 zero positions).
#include <cstdio>
#include <cstring>

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
class C2DVector {
public:
    float X, Y;
    C2DVector() : X(-1.0f), Y(-1.0f) {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CPositionHistory {             // stands in for deque<C2DVector>: starts empty, resize appends copies of the fill value
public:
    C2DVector Items[64];
    unsigned int Count;
    CPositionHistory() : Count(0) {}
    void resize(unsigned int n, const C2DVector& x)
    {
        while (Count < n) Items[Count++] = x;
        Count = n;
    }
};
class CJoystickPositionTracker {
public:
    CPositionHistory RecentPositions;
    CJoystickPositionTracker() { RecentPositions.resize(30, C2DVector(0.0f, 0.0f)); }
};

int main()
{
    CJoystickPositionTracker t;
    bool ok = true;
    for (unsigned int i = 0; i < t.RecentPositions.Count; ++i)
        if (!Near(t.RecentPositions.Items[i].X, 0.0f) || !Near(t.RecentPositions.Items[i].Y, 0.0f)) ok = false;
    if (!(ok && t.RecentPositions.Count == 30 && Near(t.RecentPositions.Items[30].X, -1.0f))) {
        std::printf("00a0e0a0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a0e0a0_TEST PASS\n");
    return 0;
}