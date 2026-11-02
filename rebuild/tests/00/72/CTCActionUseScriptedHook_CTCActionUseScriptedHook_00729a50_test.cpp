// Behaviour test for 0x00729A50 CTCActionUseScriptedHook::~CTCActionUseScriptedHook: the body removes the component from
// the global TC list first, then the members die in reverse declaration order, then the CTCBase part.
#include <cstdio>

static char g_log[16];
static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }

class CTCBase {
public:
    CTCBase() : ParentThing(0), ToBeRemoved(false), IsInGlobalUpdateTCs(true) {}
    virtual ~CTCBase() { Log('B'); }
    void RemoveFromGlobalTCs() { Log('R'); IsInGlobalUpdateTCs = false; }
    void* ParentThing;
    bool  ToBeRemoved;
    bool  IsInGlobalUpdateTCs;
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer(char tag) : Tag(tag) {}
    virtual ~CBaseIntelligentPointer() { Log(Tag); }
    char Tag;
};
class CCharString {
public:
    CCharString(char tag) : Tag(tag) {}
    ~CCharString() { Log(Tag); }
    char Tag;
};
class CTCActionUseScriptedHook : public CTCBase {
public:
    CTCActionUseScriptedHook() : PEntranceConnectedTo('E'), PCameraTrack('C'), SoundName('S'), AnimationName('A') {}
    virtual ~CTCActionUseScriptedHook();
    CBaseIntelligentPointer PEntranceConnectedTo;
    CBaseIntelligentPointer PCameraTrack;
    CCharString SoundName;
    CCharString AnimationName;
};
CTCActionUseScriptedHook::~CTCActionUseScriptedHook()
{
    RemoveFromGlobalTCs();
}

int main()
{
    CTCBase* p = new CTCActionUseScriptedHook;
    delete p;
    const char* want = "RASCEB";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("00729a50_TEST FAIL\n"); return 1; }
    std::printf("00729a50_TEST PASS\n");
    return 0;
}