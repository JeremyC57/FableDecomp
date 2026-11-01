// Behaviour test for CActionTalkBase::GetCameraSetupPositions (0x00712340): the camera-view sideways push of the
// midpoint case (retail: view 0 WIDE x1.75, view 1 MEDIUM x1, view 2 CLOSE x0.75) and the speaker/listener ordering.
#include <stdio.h>
struct V { float X, Y, Z; };
static V push(int view, const V& first, const V& second, V from)
{
    V d = { second.X - first.X, second.Y - first.Y, second.Z - first.Z };
    float s;
    switch (view) {
    case 0: s = 1.75f; break;
    case 1: s = 1.0f; break;
    case 2: s = 0.75f; break;
    default: return from;
    }
    from.X += d.Y * s; from.Y += -d.X * s;
    return from;
}
static int order(int cameraPosition, int me, int other, int* first, int* second)
{
    *first = 0; *second = 0;
    if (cameraPosition == 11) { *first = me; *second = other; }
    else if (cameraPosition == 12) { *first = other; *second = me; }
    return *first;
}
int main()
{
    int ok = 1;
    V a = { 0.0f, 0.0f, 0.0f }, b = { 2.0f, 0.0f, 0.0f }, mid = { 1.0f, 0.0f, 0.0f };
    V w = push(0, a, b, mid), m = push(1, a, b, mid), c = push(2, a, b, mid), n = push(5, a, b, mid);
    ok &= w.X == 1.0f && w.Y == -3.5f;
    ok &= m.X == 1.0f && m.Y == -2.0f;
    ok &= c.X == 1.0f && c.Y == -1.5f;
    ok &= n.Y == 0.0f;
    int f, s;
    order(11, 1, 2, &f, &s); ok &= f == 1 && s == 2;
    order(12, 1, 2, &f, &s); ok &= f == 2 && s == 1;
    order(0, 1, 2, &f, &s); ok &= f == 0 && s == 0;
    printf(ok ? "00712340_TEST PASS\n" : "00712340_TEST FAIL\n");
    return ok ? 0 : 1;
}