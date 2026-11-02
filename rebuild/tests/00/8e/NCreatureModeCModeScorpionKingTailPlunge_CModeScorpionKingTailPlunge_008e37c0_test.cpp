// Behaviour test for 0x008e37c0 NCreatureMode::CModeScorpionKingTailPlunge::~CModeScorpionKingTailPlunge() (implicit destructor):
// PDustTrailEmitter, PTailEmitter, PTarget, the Filter's Group string, the inline ~CAnimFilterBase, then ~CModeStandBase.
#include <cstdio>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString { public: ~CCharString() { Log('S'); } };
class CBaseIntelligentPointer {
public:
    explicit CBaseIntelligentPointer(char id) : Id(id), PData(0) { }
    virtual ~CBaseIntelligentPointer() { Log(Id); }
    char Id;
    void* PData;
};
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { Log('F'); } };
class CAnimFilterOfGroup : public CAnimFilterBase { public: CCharString Group; unsigned int GroupCRC; };
class CModeStandBase { public: virtual ~CModeStandBase() { Log('B'); } };

class CModeScorpionKingTailPlunge : public CModeStandBase {
public:
    CModeScorpionKingTailPlunge() : RecoveryCount(0), DustEffectCounter(0), SpikeTime(0), SpikeDecisionTime(0), PCreature(0), bRecovering(false), PTarget('T'), PTailEmitter('A'), PDustTrailEmitter('D') { }
    long                    RecoveryCount;
    long                    DustEffectCounter;
    long                    SpikeTime;
    long                    SpikeDecisionTime;
    CAnimFilterOfGroup      Filter;
    void*                   PCreature;
    bool                    bRecovering;
    float                   TargetPos[3];
    float                   TailPos[3];
    CBaseIntelligentPointer PTarget;
    CBaseIntelligentPointer PTailEmitter;
    CBaseIntelligentPointer PDustTrailEmitter;
};

int main()
{
    CModeStandBase* mode = new CModeScorpionKingTailPlunge;
    delete mode;
    if (std::strcmp(g_log, "DATSFB") != 0) {
        std::printf("008e37c0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("008e37c0_TEST PASS\n");
    return 0;
}