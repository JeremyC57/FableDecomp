// Behaviour test for 0x00740420 CModeJackOfBladesFloat::~CModeJackOfBladesFloat() (implicit destructor): members go in reverse declaration
// order -- PEmitter2, PEmitter, the Filter's Group string, the inline ~CAnimFilterBase -- and the base ~CModeStandBase runs last.
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    explicit CIntelligentPointer(char id) : CBaseIntelligentPointer(id) { }
};
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { Log('F'); } };
class CAnimFilterOfGroup : public CAnimFilterBase { public: CCharString Group; unsigned int GroupCRC; };
class CThing { };
class CModeStandBase { public: virtual ~CModeStandBase() { Log('B'); } };

class CModeJackOfBladesFloat : public CModeStandBase {
public:
    CModeJackOfBladesFloat() : PEmitter('1'), PEmitter2('2') { }
    CAnimFilterOfGroup Filter;
    CIntelligentPointer<CThing> PEmitter;
    CIntelligentPointer<CThing> PEmitter2;
};

int main()
{
    CModeStandBase* mode = new CModeJackOfBladesFloat;
    delete mode;
    if (std::strcmp(g_log, "21SFB") != 0) {
        std::printf("00740420_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("00740420_TEST PASS\n");
    return 0;
}