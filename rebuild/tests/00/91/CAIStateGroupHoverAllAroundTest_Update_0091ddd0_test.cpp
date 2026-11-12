// Behaviour test for CAIStateGroup_HoverAllAroundTest::Update (0x0091DDD0): a copy of the model's logic over test doubles.
#include <stdio.h>
#include <math.h>
struct V3 { float X, Y, Z; V3(float x, float y, float z) : X(x), Y(y), Z(z) {} V3() {} };
static unsigned long Seed = 12345;
static long ROR13(unsigned long v) { return (long)((v >> 13) | (v << 19)); }
static float Fraction(unsigned long* s) { *s = *s * 0x24A1 + 0x24DF; *s = ROR13(*s); return (float)(fmod((double)*s, 65536.0) / 65536.0); }
static float FloatRandom(float range, unsigned long* s) {
    double scaled = range; *s = *s * 0x24A1 + 0x24DF; scaled *= 65536.0; *s = ROR13(*s);
    if (scaled > 0.0001) return (float)(fmod((double)*s, scaled) * (1.0 / 65536.0)); return 0.0f; }
static float Ground; static int MovesLeft; static int Terminated; static int Moves; static bool PlayerAlive;
static V3 Me(0, 0, 5), Them(3, 4, 0), LastTarget;
static int MoveTo(const V3& t) { LastTarget = t; ++Moves; if (--MovesLeft == 1) PlayerAlive = PlayerAlive; return MovesLeft > 0 ? 2 : 0; }
static void Update()
{
    V3 start = Me;
    while (PlayerAlive) {
        float dx = Them.X - Me.X, dy = Them.Y - Me.Y, len = (float)sqrt(dx * dx + dy * dy);
        V3 dir(dx / len, dy / len, 0.0f);
        V3 perp(dir.Y, -dir.X, 0.0f);
        float frac = Fraction(&Seed);
        float dist = FloatRandom(10.0f, &Seed);
        double a = frac * (double)6.2831855f;
        float s = (float)sin(a), c = (float)cos(a);
        V3 target(start.X + 0.0f * s * dist + perp.X * c * dist, start.Y + 0.0f * s * dist + perp.Y * c * dist,
                  start.Z + 1.0f * s * dist + perp.Z * c * dist);
        if (target.Z < Ground) target.Z = Ground;
        if (MoveTo(target) == 2) continue;
        ++Terminated;
        return;
    }
}
int main()
{
    int ok = 1;
    PlayerAlive = true; MovesLeft = 3; Ground = -1000.0f; Update();
    ok &= (Moves == 3 && Terminated == 1);
    float r = (float)sqrt((LastTarget.X - Me.X) * (LastTarget.X - Me.X) + (LastTarget.Y - Me.Y) * (LastTarget.Y - Me.Y) + (LastTarget.Z - Me.Z) * (LastTarget.Z - Me.Z));
    ok &= (r >= 0.0f && r < 10.01f);
    ok &= fabs((LastTarget.X - Me.X) * 0.6f + (LastTarget.Y - Me.Y) * 0.8f) < 1e-3f;   // perpendicular to creature->player
    Moves = 0; Terminated = 0; PlayerAlive = false; Update();
    ok &= (Moves == 0 && Terminated == 0);
    PlayerAlive = true; MovesLeft = 1; Ground = 100.0f; Update();
    ok &= (Moves == 1 && Terminated == 1 && LastTarget.Z == 100.0f);
    if (!ok) { printf("0091ddd0_TEST FAIL\n"); return 1; }
    printf("0091ddd0_TEST PASS\n"); return 0;
}