// Behaviour test for 0x008F06F0 CCard::FrameUpdate(const C3DVector& position, const CRightHandedSet& orientation)
// The model mirrors the source: dt = 1/fps; a moving card advances MoveFraction by dt*MoveSpeed and either arrives
// (LocalPos = NewLocalPos, move cleared) or sits at the lerp; a turning card advances TurnFraction and either finishes
// (InitialRotation = FinalRotation, turn cleared) or lerps the rotation; the tilt is (1 - cos(2*pi*turn)) / 4 with the
// turn's sign and the card lifts by max(sin, 0) * Width / 2 along Up. A card that is not turning keeps its final rotation
// as the tilt and sits at its position. The mesh is placed with (position, orientation, CurrentPos, tilt, 0.5).
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
struct RHSet { Vec Up; Vec Forward; };
struct Card {
    float InitialRotation, FinalRotation, TurnSpeed, TurnFraction, MoveSpeed, MoveFraction;
    bool Turning, Moving;
    Vec LocalPos, NewLocalPos, CurrentPos;
    float Width;
};

static long g_fps = 4;
static int g_placed = 0;
static Vec g_placedOffset;
static float g_placedTilt, g_placedScale;
static const Vec* g_placedPos;
static const RHSet* g_placedRH;

static void PositionMesh(const Vec& pos, const RHSet& rh, const Vec& offset, float tilt, float scale)
{
    ++g_placed; g_placedPos = &pos; g_placedRH = &rh; g_placedOffset = offset; g_placedTilt = tilt; g_placedScale = scale;
}

static void FrameUpdate(Card& c, const Vec& position, const RHSet& orientation)
{
    float dt = 1.0f / g_fps;
    Vec pos;
    if (c.Moving) {
        c.MoveFraction += dt * c.MoveSpeed;
        if (c.MoveFraction >= 1.0f) {
            c.LocalPos = c.NewLocalPos;
            pos = c.NewLocalPos;
            c.MoveFraction = 0.0f;
            c.Moving = false;
        } else {
            pos.X = (c.NewLocalPos.X - c.LocalPos.X) * c.MoveFraction + c.LocalPos.X;
            pos.Y = (c.NewLocalPos.Y - c.LocalPos.Y) * c.MoveFraction + c.LocalPos.Y;
            pos.Z = (c.NewLocalPos.Z - c.LocalPos.Z) * c.MoveFraction + c.LocalPos.Z;
        }
    } else
        pos = c.LocalPos;
    float tilt;
    if (c.Turning) {
        c.TurnFraction += dt * c.TurnSpeed;
        float turn;
        if (c.TurnFraction >= 1.0f) {
            turn = c.FinalRotation;
            c.InitialRotation = c.FinalRotation;
            c.Turning = false;
            c.TurnFraction = 0.0f;
        } else
            turn = c.InitialRotation + (c.FinalRotation - c.InitialRotation) * c.TurnFraction;
        double angle = turn * (double)6.2831855f;
        tilt = (float)((1.0 - std::cos(angle)) * 0.25);
        if (turn < 0.0f)
            tilt = -tilt;
        float lift = (float)std::sin(angle);
        if (lift < 0.0f)
            lift = 0.0f;
        c.CurrentPos.X = pos.X + orientation.Up.X * lift * c.Width * 0.5f;
        c.CurrentPos.Y = pos.Y + orientation.Up.Y * lift * c.Width * 0.5f;
        c.CurrentPos.Z = pos.Z + orientation.Up.Z * lift * c.Width * 0.5f;
    } else {
        tilt = c.FinalRotation;
        c.CurrentPos = pos;
    }
    PositionMesh(position, orientation, c.CurrentPos, tilt, 0.5f);
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    int fail = 0;
    Vec table = { 1.0f, 2.0f, 3.0f };
    RHSet rh = { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } };
    Card c = { 0.0f, 0.25f, 1.0f, 0.0f, 1.0f, 0.0f, true, true,
               { 0.0f, 0.0f, 0.0f }, { 4.0f, 8.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 2.0f };
    FrameUpdate(c, table, rh);                                   // dt 0.25: quarter move, quarter turn of a quarter turn
    if (!Near(c.MoveFraction, 0.25f) || !c.Moving) fail |= 1;
    if (!Near(c.TurnFraction, 0.25f) || !c.Turning) fail |= 2;
    {
        double angle = 0.0625 * (double)6.2831855f;
        float lift = (float)std::sin(angle);
        if (!Near(c.CurrentPos.X, 1.0f) || !Near(c.CurrentPos.Y, 2.0f) || !Near(c.CurrentPos.Z, lift)) fail |= 4;
        if (!Near(g_placedTilt, (float)((1.0 - std::cos(angle)) * 0.25)) || g_placedScale != 0.5f) fail |= 8;
        if (g_placed != 1 || g_placedPos != &table || g_placedRH != &rh) fail |= 16;
    }
    c.MoveFraction = 0.9f; c.TurnFraction = 0.9f;
    FrameUpdate(c, table, rh);                                   // both finish
    if (c.Moving || c.MoveFraction != 0.0f || c.LocalPos.X != 4.0f || c.LocalPos.Y != 8.0f) fail |= 32;
    if (c.Turning || c.TurnFraction != 0.0f || c.InitialRotation != 0.25f) fail |= 64;
    if (!Near(g_placedTilt, 0.25f) || !Near(c.CurrentPos.X, 4.0f) || !Near(c.CurrentPos.Z, 1.0f)) fail |= 128;   // sin(pi/2) * 2 * 0.5
    FrameUpdate(c, table, rh);                                   // at rest: tilt is the final rotation, no lift
    if (g_placedTilt != 0.25f || c.CurrentPos.X != 4.0f || c.CurrentPos.Y != 8.0f || c.CurrentPos.Z != 0.0f) fail |= 256;
    c.FinalRotation = -0.25f; c.Turning = true; c.TurnFraction = 0.75f;
    FrameUpdate(c, table, rh);                                   // a negative turn tilts the other way; sin < 0 is clamped
    if (!(g_placedTilt < 0.0f) || !Near(g_placedTilt, -0.25f) || c.CurrentPos.Z != 0.0f || g_placed != 4) fail |= 512;
    if (fail) { std::printf("008f06f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("008f06f0_TEST PASS\n");
    return 0;
}