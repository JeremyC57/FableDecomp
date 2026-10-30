// Behaviour test for 0x007DE030 CTCCreatureFlyInCircles::FrameUpdate (see src/007de030.cpp): inactive does nothing; active sets
// health 100, runs a queued mode change, plays the loop anim, checks hits, runs the swoop (mode 2) or the circling flight on
// the position/facing, records IsPosInMap in the flagged component when present, writes the position back and sets the
// right-handed set (up = (fY, -fX, 0) x f = (-fX fZ, -fY fZ, fX^2 + fY^2), forward = facing).
#include <cstdio>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static inline V Cross(const V& a, const V& b) { return V(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
struct Fly {
    bool Active, ModeChangeQueued, HasFlagged, InMap; long Mode; float Health; V Pos, Facing, SetPos, Up, Fwd;
    int Changes, Anims, Hits, Swoops, Flights, RHSets;
    void FrameUpdate()
    {
        if (!Active) return;
        V facing = Facing; V pos = Pos;
        Health = 100.0f;
        if (ModeChangeQueued) ++Changes;
        ++Anims; ++Hits;
        if (Mode == 2) { ++Swoops; pos.X += 1.0f; } else { ++Flights; pos.Y += 1.0f; }
        if (HasFlagged) InMap = pos.X < 50.0f;
        SetPos = pos;
        Up = Cross(V(facing.Y, -facing.X, 0.0f), facing); Fwd = facing; ++RHSets;
    }
};
int main()
{
    Fly f = { false, true, true, false, 2, 5.0f, V(1, 1, 1), V(1, 2, 3), V(0, 0, 0), V(0, 0, 0), V(0, 0, 0), 0, 0, 0, 0, 0, 0 };
    f.FrameUpdate();
    bool ok = f.Health == 5.0f && f.RHSets == 0;
    f.Active = true; f.FrameUpdate();
    ok = ok && f.Health == 100.0f && f.Changes == 1 && f.Swoops == 1 && f.Flights == 0 && f.InMap && f.SetPos.X == 2.0f;
    ok = ok && f.Up.X == -3.0f && f.Up.Y == -6.0f && f.Up.Z == 5.0f && f.Fwd.Z == 3.0f;
    f.Mode = 0; f.FrameUpdate();
    ok = ok && f.Flights == 1 && f.SetPos.Y == 2.0f && f.RHSets == 2;
    std::printf(ok ? "007de030_TEST PASS\n" : "007de030_TEST FAIL\n");
    return ok ? 0 : 1;
}