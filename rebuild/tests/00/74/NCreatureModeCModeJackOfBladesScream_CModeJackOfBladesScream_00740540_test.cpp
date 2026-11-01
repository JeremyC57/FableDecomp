// Test for 0x00740540 NCreatureMode::CModeJackOfBladesScream::~CModeJackOfBladesScream (implicit): PEmitter, then PSword, then the Filter's Group string and the
// CAnimFilterBase part, then ~CModeStandBase -- reverse declaration order.
#include <cstdio>
static char g_log[16]; static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; }
class CCharString { public: void* PStringData; ~CCharString() { Log('S'); } };
class CBaseIntelligentPointer { public: char Tag; virtual ~CBaseIntelligentPointer() { Log(Tag); } void* PData; };
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { Log('F'); } };
class CAnimFilterOfGroup : public CAnimFilterBase { public: CCharString Group; unsigned long GroupCRC; };
class CModeStandBase { public: virtual ~CModeStandBase() { Log('B'); } unsigned char _base[0x64]; };
namespace NCreatureMode {
class CModeJackOfBladesScream : public CModeStandBase { public:
    CAnimFilterOfGroup Filter; long Timer;
    CBaseIntelligentPointer Second;   // PSword
    CBaseIntelligentPointer First;    // PEmitter
};
}
int main()
{
    NCreatureMode::CModeJackOfBladesScream* m = new NCreatureMode::CModeJackOfBladesScream;
    m->First.Tag = '1'; m->Second.Tag = '2';
    CModeStandBase* b = m; delete b;
    g_log[g_n] = 0;
    const char* want = "12SFB";
    for (int i = 0; i < 6; ++i) if (g_log[i] != want[i]) { std::printf("00740540_TEST FAIL\n"); return 1; }
    std::printf("00740540_TEST PASS\n");
    return 0;
}