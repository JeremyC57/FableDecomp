// Behaviour test for 0x007BE0F9 NThingShot::CTCShotDefinitionForcePushBall::OnCollision (see src/007be0f9.cpp): with a
// shooter and a game-object thing the ball spawns its impact effect, adds a force-pushed component (with a hit event when
// the thing has entity events) pushing along the normalised shooter->thing direction, marks the thing attacked and posts
// the attack message; it then fades the shield and kills itself. Without a shooter it falls back to the base collision.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct Thing { bool GameObject, HasPushed, HasEvents; V Pos; int Events; V Push; float Force; bool Attacked; int Messages; };
struct Ball {
    Thing* Shooter; float PushForce; int Effects, Kills, BaseCollisions, Fades;
    static void Normalise(V& v)
    {
        float l2 = v.X * v.X + v.Y * v.Y + v.Z * v.Z;
        float r = 1.0f / std::sqrt(l2);
        v.X *= r; v.Y *= r; v.Z *= r;
    }
    void OnCollision(Thing* thing)
    {
        if (!Shooter) { ++BaseCollisions; return; }
        if (thing) {
            ++Effects;
            if (thing->GameObject) {
                V away = { Shooter->Pos.X - thing->Pos.X, Shooter->Pos.Y - thing->Pos.Y, Shooter->Pos.Z - thing->Pos.Z };
                Normalise(away);
                if (!thing->HasPushed) {
                    thing->HasPushed = true;
                    if (thing->HasEvents) ++thing->Events;
                    V push = { -away.X, -away.Y, -away.Z };
                    thing->Push = push; thing->Force = PushForce;
                }
                thing->Attacked = true;
                ++thing->Messages;
            }
        }
        ++Fades; ++Kills;
    }
};
int main()
{
    Thing shooter = { true, false, false, { 0, 0, 0 }, 0, { 0, 0, 0 }, 0, false, 0 };
    Thing target = { true, false, true, { 3, 4, 0 }, 0, { 0, 0, 0 }, 0, false, 0 };
    Ball ball = { &shooter, 2.5f, 0, 0, 0, 0 };
    ball.OnCollision(&target);
    bool ok = ball.Effects == 1 && target.HasPushed && target.Events == 1 && target.Attacked && target.Messages == 1 &&
              std::fabs(target.Push.X - 0.6f) < 1e-4f && std::fabs(target.Push.Y - 0.8f) < 1e-4f && target.Force == 2.5f &&
              ball.Kills == 1 && ball.Fades == 1;
    Ball lone = { 0, 1.0f, 0, 0, 0, 0 };
    lone.OnCollision(&target);
    ok = ok && lone.BaseCollisions == 1 && lone.Kills == 0;
    std::printf(ok ? "007be0f9_TEST PASS\n" : "007be0f9_TEST FAIL\n");
    return ok ? 0 : 1;
}