#include <cstdio>
// Behaviour model of CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos @ 0x0091D7F0: forwards with proximity 0.
struct C3DVector { float X, Y, Z; };
static long g_repeat; static C3DVector* g_pos; static float g_prox = -1; static C3DVector* g_face; static bool* g_arrived; static long g_move;
struct CAIStateGroup_HoverMovementBase { virtual void v00() {}
    long AIAction_MoveToPosProximityAndFacePos(long r, C3DVector* p, float prox, C3DVector* f, bool* a, long m) { g_repeat = r; g_pos = p; g_prox = prox; g_face = f; g_arrived = a; g_move = m; return 4; }
    long AIAction_MoveToPosAndFacePos(long r, C3DVector* p, C3DVector* f, bool* a, long m) { return AIAction_MoveToPosProximityAndFacePos(r, p, 0.0f, f, a, m); } };
int main()
{
    C3DVector pos, face; bool arrived = false; CAIStateGroup_HoverMovementBase h;
    long r = h.AIAction_MoveToPosAndFacePos(2, &pos, &face, &arrived, 7);
    bool ok = r == 4 && g_repeat == 2 && g_pos == &pos && g_prox == 0.0f && g_face == &face && g_arrived == &arrived && g_move == 7;
    if (!ok) { std::printf("0091d7f0_TEST FAIL\n"); return 1; }
    std::printf("0091d7f0_TEST PASS\n");
    return 0;
}