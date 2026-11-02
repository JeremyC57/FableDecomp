// Behaviour test for 0x008e31a0 NCreatureMode::CModeDragonHover::~CModeDragonHover() (implicit destructor, called by the
// CModeDragonFireBeam deleting destructor): FireBeam's PBeamWeaponEffect first, then PTarget, the Filter's Group string, the
// inline ~CAnimFilterBase, and ~CModeStandBase last.
#include <cstdio>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString { public: ~CCharString() { Log('S'); } };
class CBaseIntelligentPointer {
public:
    explicit CBaseIntelligentPointer(char id) : Id(id) { }
    virtual ~CBaseIntelligentPointer() { Log(Id); }
    char Id;
};
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { Log('F'); } };
class CAnimFilterOfGroup : public CAnimFilterBase { public: CCharString Group; unsigned int GroupCRC; };
class CModeStandBase { public: virtual ~CModeStandBase() { Log('B'); } };

class CModeDragonHover : public CModeStandBase {
public:
    CModeDragonHover() : PTarget('T'), UseHeadTurning(false) { }
    CAnimFilterOfGroup      Filter;
    CBaseIntelligentPointer PTarget;
    bool                    UseHeadTurning;
};
class CModeDragonFireBeam : public CModeDragonHover {
public:
    CModeDragonFireBeam() : Timer(0), PBeamWeaponEffect('E'), LastExplodeFrame(0), BeamActive(false) { }
    long                    Timer;
    CBaseIntelligentPointer PBeamWeaponEffect;
    long                    LastExplodeFrame;
    bool                    BeamActive;
};

int main()
{
    CModeStandBase* mode = new CModeDragonFireBeam;
    delete mode;
    if (std::strcmp(g_log, "ETSFB") != 0) {
        std::printf("008e31a0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    g_n = 0; g_log[0] = 0;
    CModeStandBase* hover = new CModeDragonHover;
    delete hover;
    if (std::strcmp(g_log, "TSFB") != 0) {
        std::printf("008e31a0_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("008e31a0_TEST PASS\n");
    return 0;
}