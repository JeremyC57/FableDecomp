// Behaviour test for 0x0093D030 CAIStateGroup_ButterflyWander::Update (model copy over stubs).
#include <cstdio>
#include <string.h>
class CThingCreatureBase;
class C3DVector { public: float X, Y, Z; };
class CCharString { public: const char* m_pData; CCharString(const char* t, long = -1) : m_pData(t) {} ~CCharString() {} };
class CSoundPair { public: long m_first; long m_second; CSoundPair() : m_first(0), m_second(0) {} };
class CCreatureActionBase { public: int dummy; };
static int g_actions = 0; static const char* g_lastAnim = 0;
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayAnimation(CThingCreatureBase*, const CCharString& animation, const CSoundPair&, bool, bool, bool, long, long)
    { g_lastAnim = animation.m_pData; }
};
class CAIBrain { public: CThingCreatureBase* PCreature; };
unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static int g_terminated = 0; static bool g_posOk = true; static long g_flight = 2; static int g_failAt = 1000;
static const C3DVector* g_centre = 0; static float g_radius = 0;
class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    unsigned long Seed;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase*, bool) { ++g_actions; return g_actions >= g_failAt ? 1 : 2; }
    unsigned long* GetPWorldSeed() { return &Seed; }
    void Terminate() { ++g_terminated; }
};
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    C3DVector StartPos;
    bool GetRandomFlightPos(const C3DVector* centre, float radius, C3DVector* out, bool) { g_centre = centre; g_radius = radius; out->X = 1; return g_posOk; }
    long ExecuteFlyingPath(const C3DVector*) { return g_flight; }
};
// g++ rejects `&T(...)` on a temporary (VC7.1 accepts it); the test takes the address through a helper.
template<class T> T* TempAddress(const T& t) { return const_cast<T*>(&t); }
struct CAIStateGroup_ButterflyWander : CAIStateGroup_FlyingThingBase {
    void Update();
};
void CAIStateGroup_ButterflyWander::Update()
{
    C3DVector target;
    if (!GetRandomFlightPos(&StartPos, 15.0f, &target, true)) {
        Terminate();
        return;
    }
    if (ExecuteFlyingPath(&target) == 2) {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        long count = *seed % 6;
        for (long i = 0; i <= count; ++i) {
            bool failed;
            {
                CCharString animation("ST_IDLE");
                failed = AIAction_PerformActionAndWait(TempAddress(CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
                    false, true, false, 0, 0)), false) != 2;
            }
            if (failed)
                break;
        }
    }
    Terminate();
}

static int fail(int n) { std::printf("0093d030_TEST FAIL (%d)\n", n); return 1; }
int main()
{
    CAIBrain brain; brain.PCreature = 0;
    CAIStateGroup_ButterflyWander w; w.PBrain = &brain; w.Seed = 12345;
    unsigned long s = 12345u * 0x24a1 + 0x24df; s = (s >> 13) | (s << 19);
    long expected = (long)(s % 6) + 1;
    w.Update();
    if (g_actions != expected || g_terminated != 1 || w.Seed != s || g_centre != &w.StartPos || g_radius != 15.0f) return fail(1);
    if (strcmp(g_lastAnim, "ST_IDLE") != 0) return fail(2);
    g_actions = 0; g_failAt = 1; w.Update();
    if (g_actions != 1 || g_terminated != 2) return fail(3);      // a failed idle stops the loop
    g_actions = 0; g_posOk = false; w.Update();
    if (g_actions != 0 || g_terminated != 3) return fail(4);
    g_posOk = true; g_flight = 4; w.Update();
    if (g_actions != 0 || g_terminated != 4) return fail(5);
    std::printf("0093d030_TEST PASS\n");
    return 0;
}