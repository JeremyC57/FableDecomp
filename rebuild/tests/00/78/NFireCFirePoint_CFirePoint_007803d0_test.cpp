// Behaviour test for 0x007803D0 (implicit NFire::CFirePoint::~CFirePoint): destroys PSingeEffect, PEffect, then HelperName.
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
namespace NFire {
class CFirePoint {
public:
    CCharString HelperName;
    long HelperIndex, EffectIndex, Resistance;
    bool Burning;
    long FireDamageAfterFrame;
    CThing* PParentThing;
    CIntelligentPointer<CThing> PEffect;
    CIntelligentPointer<CThing> PSingeEffect;
};
}

int main()
{
    {
        NFire::CFirePoint point;
        point.HelperName.Id = 1; point.PEffect.Id = 2; point.PSingeEffect.Id = 3;
    }
    if (g_count != 3 || g_log[0] != 3 || g_log[1] != 2 || g_log[2] != 1) { std::printf("007803d0_TEST FAIL\n"); return 1; }
    std::printf("007803d0_TEST PASS\n");
    return 0;
}