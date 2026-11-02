// Behaviour test for 0x008e3720 NCreatureMode::CModeJackOfBladesFlash::~CModeJackOfBladesFlash() (implicit destructor):
// PSpotLight, PEmitter, PTarget, the Filter's Group string, the inline ~CAnimFilterBase, then ~CModeStandBase.
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

class CModeJackOfBladesFlash : public CModeStandBase {
public:
    CModeJackOfBladesFlash() : Timer(0), BurnCounter(0), PCreature(0), PTarget('T'), PEmitter('E'), PSpotLight('L') { }
    long                    Timer;
    long                    BurnCounter;
    CAnimFilterOfGroup      Filter;
    void*                   PCreature;
    CBaseIntelligentPointer PTarget;
    CBaseIntelligentPointer PEmitter;
    CBaseIntelligentPointer PSpotLight;
};

int main()
{
    CModeStandBase* mode = new CModeJackOfBladesFlash;
    delete mode;
    if (std::strcmp(g_log, "LETSFB") != 0) {
        std::printf("008e3720_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("008e3720_TEST PASS\n");
    return 0;
}