// Behaviour test for 0x00768BF0 (implicit AttackParticleAndEndPoints::~AttackParticleAndEndPoints): destroys
// ParticleEffect, then the base BasicParticleAndEndPoints' EndPoint and StartPoint.
#include <cstdio>
#include <new>

static int g_log[16];
static int g_count = 0;
static void Log(int id) { if (g_count < 16) g_log[g_count++] = id; }

class CCharString {                                      // model: records its id when destroyed
public:
    CCharString() : Id(0) {}
    ~CCharString() { Log(Id); }
    int Id;
};

class CThing;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : Id(0), PData(0) {}
    virtual ~CBaseIntelligentPointer() { Log(Id); }
    int Id;
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };
struct BasicParticleAndEndPoints {
    CCharString StartPoint;
    CCharString EndPoint;
    long ParticleEffectID;
    long ParticleEffectIDToBlendTo;
};
struct AttackParticleAndEndPoints : public BasicParticleAndEndPoints {
    CIntelligentPointer<CThing> ParticleEffect;
};

int main()
{
    {
        AttackParticleAndEndPoints points;
        points.StartPoint.Id = 1; points.EndPoint.Id = 2; points.ParticleEffect.Id = 3;
    }
    if (g_count != 3 || g_log[0] != 3 || g_log[1] != 2 || g_log[2] != 1) { std::printf("00768bf0_TEST FAIL\n"); return 1; }
    std::printf("00768bf0_TEST PASS\n");
    return 0;
}