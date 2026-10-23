// Behaviour test for CTCKrakenTentacle::PerformStrikeDamage (0x007F11D0): points along the facing at the target height,
// explosion handed to the kraken, obstruction replaced, strike counted, strike flags cleared.
#include <cstdio>
#include <cstring>

struct V { float X, Y, Z; };
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct Kraken { int strikes; bool targetAlive; V targetPos; };
struct Tentacle {
    bool krakenAlive, canStrike;
    int timer;
    V pos, facing, start, end, boom;
    float rMin, rMax, rBoom;
    void Perform(Kraken& k, bool explosionMade)
    {
        if (krakenAlive) {
            if (k.targetAlive) {
                V origin = { pos.X, pos.Y, 0.0f };
                origin.Z = k.targetPos.Z;
                start.X = facing.X * rMin + origin.X; start.Y = facing.Y * rMin + origin.Y; start.Z = facing.Z * rMin + origin.Z;
                end.X = facing.X * rMax + origin.X; end.Y = facing.Y * rMax + origin.Y; end.Z = facing.Z * rMax + origin.Z;
                boom.X = facing.X * rBoom + origin.X; boom.Y = facing.Y * rBoom + origin.Y; boom.Z = facing.Z * rBoom + origin.Z;
                if (explosionMade) Log("addexplosion");
                Log("removeobstruction");
                Log("newobstruction");
            }
            ++k.strikes;
        }
        canStrike = false;
        timer = 0;
    }
};

int main()
{
    Tentacle t;
    std::memset(&t, 0, sizeof(t));
    Kraken k;
    std::memset(&k, 0, sizeof(k));
    t.krakenAlive = true; t.canStrike = true; t.timer = 5;
    t.pos.X = 1.0f; t.pos.Y = 2.0f; t.pos.Z = 99.0f;
    t.facing.X = 1.0f; t.rMin = 2.0f; t.rMax = 4.0f; t.rBoom = 3.0f;
    k.targetAlive = true; k.targetPos.Z = 7.0f; g_log[0] = 0;
    t.Perform(k, true);
    if (t.start.X != 3.0f || t.end.X != 5.0f || t.boom.X != 4.0f || t.boom.Y != 2.0f || t.boom.Z != 7.0f ||
        k.strikes != 1 || t.canStrike || t.timer != 0 ||
        std::strcmp(g_log, "addexplosion;removeobstruction;newobstruction;") != 0) { std::printf("007f11d0_TEST FAIL\n"); return 1; }
    k.targetAlive = false; g_log[0] = 0;
    t.Perform(k, true);
    if (k.strikes != 2 || g_log[0] != 0) { std::printf("007f11d0_TEST FAIL\n"); return 1; }
    t.krakenAlive = false;
    t.Perform(k, true);
    if (k.strikes != 2) { std::printf("007f11d0_TEST FAIL\n"); return 1; }
    std::printf("007f11d0_TEST PASS\n");
    return 0;
}
