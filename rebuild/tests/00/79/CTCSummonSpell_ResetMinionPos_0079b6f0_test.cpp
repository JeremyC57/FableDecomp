// Behaviour test for 0x0079B6F0 CTCSummonSpell::ResetMinionPos: target = hero facing + hero position; a live minion with physics is
// moved to the target, then to the valid teleport position, and faces away from the hero (forward = -normalised(pos - hero));
// the appearing effect is spawned at the final position only while the minion lives; a dead minion is left untouched.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V3 operator+(const V3& o) const { return V3(X + o.X, Y + o.Y, Z + o.Z); }
    V3 operator-(const V3& o) const { return V3(X - o.X, Y - o.Y, Z - o.Z); }
    V3 operator-() const { return V3(-X, -Y, -Z); }
    void Normalise() { float f = 1.0f / std::sqrt(X * X + Z * Z + Y * Y); X *= f; Y *= f; Z *= f; } };
struct Physics { V3 Pos; V3 Up, Forward; int SetCount; void SetPosition(const V3& p) { Pos = p; ++SetCount; } void SetFacing(const V3& up, const V3& f) { Up = up; Forward = f; } };
struct Minion { bool Dead; Physics* P; };
struct Spell {
    V3 HeroPos, HeroFacing; Minion* M; int Sounds; int Effects; V3 EffectPos; V3 Valid;
    V3 GetValidTeleportPos(Minion*) { return Valid; }
    void ResetMinionPos()
    {
        ++Sounds;
        V3 offset = HeroFacing;
        V3 target = offset + HeroPos;
        V3 pos = target;
        if (M && !M->Dead) {
            Physics* physics = M->P;
            if (physics) {
                physics->SetPosition(target);
                pos = GetValidTeleportPos(M);
                physics->SetPosition(pos);
                offset = pos - HeroPos;
                offset.Normalise();
                physics->SetFacing(V3(0, 0, 1), -offset);
            }
            ++Effects; EffectPos = pos;
        }
    }
};
int main()
{
    Physics p; p.SetCount = 0;   // VC7.1 predates C++03 value-initialisation: `Physics()` would leave SetCount unset
    Minion m = { false, &p };
    Spell s; s.HeroPos = V3(10, 0, 0); s.HeroFacing = V3(0, 1, 0); s.M = &m; s.Sounds = 0; s.Effects = 0; s.Valid = V3(10, 3, 0);
    s.ResetMinionPos();
    bool ok = p.SetCount == 2 && p.Pos.X == 10 && p.Pos.Y == 3 && std::fabs(p.Forward.Y + 1.0f) < 1e-5f && p.Up.Z == 1 &&
              s.Effects == 1 && s.EffectPos.Y == 3 && s.Sounds == 1;
    Minion dead = { true, &p }; s.M = &dead; s.ResetMinionPos();
    ok = ok && s.Effects == 1 && p.SetCount == 2 && s.Sounds == 2;
    Minion nophys = { false, 0 }; s.M = &nophys; s.ResetMinionPos();
    ok = ok && s.Effects == 2 && s.EffectPos.X == 10 && s.EffectPos.Y == 1;
    if (!ok) { std::printf("0079b6f0_TEST FAIL\n"); return 1; }
    std::printf("0079b6f0_TEST PASS\n");
    return 0;
}