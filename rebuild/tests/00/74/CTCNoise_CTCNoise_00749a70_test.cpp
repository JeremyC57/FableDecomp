// Behaviour test for 0x00749a70 CTCNoise::~CTCNoise() (implicit destructor): Creator first, then the embedded Definition
// (CTCNoiseDef's def chain down to ~CDefPointeeBase and ~CResource), then ~CTCBase last.
#include <cstdio>
#include <cstring>

static char g_log[64];
static int g_n = 0;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }

class CResource { public: virtual ~CResource() { Log('R'); } };
class CDefPointeeBase : public CResource { public: virtual ~CDefPointeeBase() { Log('D'); } };
class CDefClassBase : public CDefPointeeBase { };
class CSubDefClassBase : public CDefClassBase { };
class CTCNoiseDef : public CSubDefClassBase { public: int Type; float LifeTime; float DistanceCanBeHeardFrom; };
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer() { Log('C'); } };
class CTCBase { public: virtual ~CTCBase() { Log('T'); } };

class CTCNoise : public CTCBase {
public:
    CTCNoise() : LifeTimer(0), DistanceCanBeHeardFrom(0.0f) { }
    CTCNoiseDef             Definition;
    long                    LifeTimer;
    float                   DistanceCanBeHeardFrom;
    CBaseIntelligentPointer Creator;
};

int main()
{
    CTCBase* component = new CTCNoise;
    delete component;
    if (std::strcmp(g_log, "CDRT") != 0) {
        std::printf("00749a70_TEST FAIL (%s)\n", g_log);
        return 1;
    }
    std::printf("00749a70_TEST PASS\n");
    return 0;
}