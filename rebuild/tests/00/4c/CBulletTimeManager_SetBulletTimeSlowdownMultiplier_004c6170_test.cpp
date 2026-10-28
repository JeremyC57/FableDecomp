// Behaviour test for 0x004C6170 CBulletTimeManager::SetBulletTimeSlowdownMultiplier: with no override and an empty map the strike tick is reset; the default multiplier is switched off, the requester's entry set to {multiplier as float, sound pitch}, then the sound pitch recalculated
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
struct Info { float Multiplier; float SoundPitch; Info() : Multiplier(0), SoundPitch(0) { } Info(float m, float p) : Multiplier(m), SoundPitch(p) { } };
struct Map { void* keys[4]; Info vals[4]; unsigned count;
    Info& operator[](void* k) { for (unsigned i = 0; i < count; ++i) if (keys[i] == k) return vals[i]; keys[count] = k; vals[count] = Info(); return vals[count++]; }
    bool empty() const { return count == 0; } };
static int g_recalc = 0;
class CBulletTimeManager { public:
    Map BulletTimeInfoMap; bool Override; bool UseDefaultMultiplier; int LastStrikeActionTick;
    void RecalculateSoundPitch() { ++g_recalc; }
    void Set(int multiplier, void* requester, float soundPitch)
    {
        if (!Override && BulletTimeInfoMap.empty()) LastStrikeActionTick = 0;
        UseDefaultMultiplier = false;
        BulletTimeInfoMap[requester] = Info((float)multiplier, soundPitch);
        RecalculateSoundPitch();
    }
};
int main()
{
    int a, b; CBulletTimeManager m; m.BulletTimeInfoMap.count = 0; m.Override = false; m.UseDefaultMultiplier = true; m.LastStrikeActionTick = 99;
    m.Set(3, &a, 0.5f);
    CHECK(m.LastStrikeActionTick == 0 && !m.UseDefaultMultiplier && g_recalc == 1);
    CHECK(m.BulletTimeInfoMap.count == 1 && m.BulletTimeInfoMap.vals[0].Multiplier == 3.0f && m.BulletTimeInfoMap.vals[0].SoundPitch == 0.5f);
    m.LastStrikeActionTick = 42; m.Set(5, &b, 1.0f); CHECK(m.LastStrikeActionTick == 42 && m.BulletTimeInfoMap.count == 2);
    m.Set(7, &a, 2.0f); CHECK(m.BulletTimeInfoMap.count == 2 && m.BulletTimeInfoMap.vals[0].Multiplier == 7.0f && g_recalc == 3);
    m.BulletTimeInfoMap.count = 0; m.Override = true; m.Set(1, &a, 1.0f); CHECK(m.LastStrikeActionTick == 42);
    if (fails == 0) printf("004c6170_TEST PASS\n"); else printf("004c6170_TEST FAIL\n");
    return fails;
}