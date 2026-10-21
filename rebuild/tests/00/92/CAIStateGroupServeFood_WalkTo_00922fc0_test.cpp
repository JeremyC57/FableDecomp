// Behaviour test for 00922fc0: src/00922fc0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_ServeFood::WalkTo(C3DVector, C3DVector, C3DVector)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_ServeFood::WalkTo(C3DVector, C3DVector, C3DVector) -- walk to the position (repeating while the action returns 4); on arrival turn to look at position + look offset; report whether the look completed.
// Leaf: WalkTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00922FC0, length 175 bytes (+ int3 padding), `ret 24h` (three by-value
// C3DVectors; the middle one is unused here).
// Callees: CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (repeat 2, pos, &reached, movement 3, true, true, 0,
// true, 0.0), CAIStateGroupBase::AIAction_LookAtPos @ 0x0097D580 (repeat 2, by-value C3DVector, flags 0x11, speed
// 0.0027777f).
// Shape: the landed do/while action loops; the look-at point is three named float sums (x, y, z in source order) fed to
// the value constructor, so VC7.1 keeps X/Y in the 0xC frame and homes Z in the dead pos.X argument slot as retail (an
// operator+ through the value constructor evaluates z first; a field-by-field sum needs a 0x10 frame);
// `return result == 2;` (`sete cl`). No engine header for this class.
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x04..+0x13
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movementType, bool a, bool b,
                            CThing* leader, bool c, float d);   // 0x0097EC00
};

struct CAIStateGroup_ServeFood : CAIStateGroupBase {
    bool WalkTo(C3DVector pos, C3DVector facing, C3DVector lookOffset);
};

bool CAIStateGroup_ServeFood::WalkTo(C3DVector pos, C3DVector facing, C3DVector lookOffset)
{
    bool reached;
    long result;
    do {
        result = AIAction_MoveToPos(2, pos, &reached, 3, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    float x = pos.X + lookOffset.X;
    float y = pos.Y + lookOffset.Y;
    float z = pos.Z + lookOffset.Z;
    C3DVector lookAt(x, y, z);
    do {
        result = AIAction_LookAtPos(2, lookAt, 0x11, 0.0027777f);
    } while (result == 4);
    return result == 2;
}

// ---- stubs and driver ----
static long g_moveSeq[8], g_lookSeq[8]; static int g_nMove, g_nLook, g_iMove, g_iLook;
static C3DVector g_movePos, g_lookPos; static bool g_argsOk = true;
CAIStateGroupBase::~CAIStateGroupBase() { }
long CAIStateGroupBase::AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movementType, bool a, bool b,
                                           CThing* leader, bool c, float d)
{
    if (repeat != 2 || reached == 0 || movementType != 3 || !a || !b || leader != 0 || !c || d != 0.0f) g_argsOk = false;
    g_movePos = pos; ++g_nMove; return g_moveSeq[g_iMove++];
}
long CAIStateGroupBase::AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed)
{
    if (repeat != 2 || flags != 0x11 || speed != 0.0027777f) g_argsOk = false;
    g_lookPos = pos; ++g_nLook; return g_lookSeq[g_iLook++];
}
static C3DVector V(float x, float y, float z) { return C3DVector(x, y, z); }
static void Reset() { g_nMove = g_nLook = g_iMove = g_iLook = 0; }
int main()
{
    CAIStateGroup_ServeFood s;
    Reset(); g_moveSeq[0] = 4; g_moveSeq[1] = 4; g_moveSeq[2] = 2; g_lookSeq[0] = 4; g_lookSeq[1] = 2;
    CHECK(s.WalkTo(V(1, 2, 3), V(9, 9, 9), V(10, 20, 30)));
    CHECK(g_nMove == 3 && g_nLook == 2);
    CHECK(g_movePos.X == 1 && g_movePos.Y == 2 && g_movePos.Z == 3);
    CHECK(g_lookPos.X == 11 && g_lookPos.Y == 22 && g_lookPos.Z == 33);
    Reset(); g_moveSeq[0] = 3;
    CHECK(!s.WalkTo(V(1, 2, 3), V(0, 0, 0), V(1, 1, 1)));
    CHECK(g_nMove == 1 && g_nLook == 0);
    Reset(); g_moveSeq[0] = 2; g_lookSeq[0] = 4; g_lookSeq[1] = 4; g_lookSeq[2] = 3;
    CHECK(!s.WalkTo(V(-1, -2, -3), V(0, 0, 0), V(0.5f, 0.5f, 0.5f)));
    CHECK(g_nMove == 1 && g_nLook == 3 && g_lookPos.X == -0.5f && g_lookPos.Z == -2.5f);
    CHECK(g_argsOk);
    if (fails == 0) printf("00922fc0_TEST PASS\n");
    return fails;
}
