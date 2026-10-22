// Behaviour test for CAIStateGroup_HoverMovementBase::AIAction_MoveToPosProximityAndFacePos (0x0091D510): model copy.
#include <cstdio>

struct V { float X, Y, Z; float LenSq() const { return Y * Y + Z * Z + X * X; } };
struct NavOut { long Status; V Offset; };
static NavOut g_script[8];
static int g_idx, g_yields, g_outputs, g_stopAt;
static bool g_busy;
static V g_lastMove, g_lastLook;
static long g_lastFlags;
static const float EPS = 0.0001f;

static bool Stopping() { return g_stopAt >= 0 && g_yields >= g_stopAt; }
static int Run(int repeat, const V& pos, const V& face, bool& moving)
{
    if (Stopping()) return 1;
    int result = 4;
    do {
        NavOut out = g_script[g_idx < 7 ? g_idx++ : 7];
        moving = out.Status == 0 || out.Offset.LenSq() > EPS * EPS;
        if (!g_busy) {
            switch (out.Status) {
            case 0: result = 2; break;
            case 1: result = 3; break;
            case 2:
                if (out.Offset.LenSq() > EPS * EPS) {
                    V m; m.X = out.Offset.X + pos.X; m.Y = out.Offset.Y + pos.Y; m.Z = out.Offset.Z + pos.Z;
                    g_lastMove = m; g_lastLook = face; g_lastFlags = 0x11; ++g_outputs;
                }
                result = 4;
                break;
            }
        }
        ++g_yields;
        if (Stopping()) return 1;
    } while (repeat == 1 && result == 4);
    return result;
}

static int fail(int n) { std::printf("0091d510_TEST FAIL %d\n", n); return 1; }
static void reset() { g_idx = g_yields = g_outputs = 0; g_stopAt = -1; g_busy = false; for (int i = 0; i < 8; ++i) { g_script[i].Status = 2; g_script[i].Offset.X = 1.0f; g_script[i].Offset.Y = 0.0f; g_script[i].Offset.Z = 0.0f; } }

int main()
{
    V pos; pos.X = 10.0f; pos.Y = 20.0f; pos.Z = 0.0f;
    V face; face.X = 5.0f; face.Y = 6.0f; face.Z = 7.0f;
    bool moving = false;
    reset(); g_script[2].Status = 0;
    if (Run(1, pos, face, moving) != 2 || g_yields != 3 || g_outputs != 2 || !moving) return fail(1);
    if (g_lastMove.X != 11.0f || g_lastLook.Z != 7.0f || g_lastFlags != 0x11) return fail(2);
    reset(); g_script[0].Status = 1; g_script[0].Offset.X = 0.0f;
    if (Run(1, pos, face, moving) != 3 || moving) return fail(3);
    reset();
    if (Run(0, pos, face, moving) != 4 || g_yields != 1) return fail(4);
    reset(); g_stopAt = 0;
    if (Run(1, pos, face, moving) != 1 || g_yields != 0) return fail(5);
    reset(); g_stopAt = 2;
    if (Run(1, pos, face, moving) != 1 || g_yields != 2) return fail(6);
    reset(); g_busy = true; g_script[0].Status = 0;
    if (Run(0, pos, face, moving) != 4 || g_outputs != 0) return fail(7);
    reset(); g_script[0].Offset.X = 0.0f; g_script[1].Status = 0;
    if (Run(1, pos, face, moving) != 2 || g_outputs != 0) return fail(8);
    std::printf("0091d510_TEST PASS\n");
    return 0;
}