// Behaviour test for 0x00EB6BD0 CV_MayorsInvitationScript::Init: flags cleared, old HSP block released and deleted,
// new block referenced once more after the returned temporary dies.
#include <cstdio>
#include <cstring>
class CCharString { public: char text[32]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
class CCPPointerInfo { public: long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static int g_deleted = 0;
static void DeleteThing(void*) { ++g_deleted; }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info; T* data = other.Data;
        if (Info != info) {
            if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
            Data = data; Info = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CScriptThing : public CBaseClass { public: CCountedPointer<CScriptThing> PImp; ~CScriptThing() { } };
static CCPPointerInfo* g_newBlock = 0;
static char g_lastName[32];
class CGameScriptInterface { public:
    virtual CScriptThing GetThingWithScriptName(const CCharString& name)
    {
        std::strcpy(g_lastName, name.text);
        CScriptThing t; t.PImp.Data = (CScriptThing*)g_newBlock; t.PImp.Info = g_newBlock; ++g_newBlock->RefCount;
        return t;
    }
};
struct CV_MayorsInvitationScript {
    CGameScriptInterface* Game;
    bool MissionSucceeded, MissionFailed, MissionOver, HeroAbortedQuest, HeroLearnedSecret, HeroReturnedFromParanoidQuest,
         HeroHasDefeatedThunder, HeroHasLostToThunder, QuestCardGiven;
    long SecretsLearned, WhichQuest, MonitorThunderHeroDuelRunning;
    CScriptThing destinationHSP;
    void Init()
    {
        MissionSucceeded = false; MissionFailed = false; MissionOver = false; HeroAbortedQuest = false;
        SecretsLearned = 0; MonitorThunderHeroDuelRunning = 0; HeroLearnedSecret = false; WhichQuest = 0;
        HeroHasDefeatedThunder = false; HeroHasLostToThunder = false; HeroReturnedFromParanoidQuest = false;
        QuestCardGiven = false;
        destinationHSP = Game->GetThingWithScriptName("OakvaleHSP");
    }
};
int main()
{
    CGameScriptInterface g;
    CCPPointerInfo* oldBlock = new CCPPointerInfo; oldBlock->RefCount = 1; oldBlock->DeleteFunc = DeleteThing; oldBlock->Data = 0;
    g_newBlock = new CCPPointerInfo; g_newBlock->RefCount = 1; g_newBlock->DeleteFunc = DeleteThing; g_newBlock->Data = 0;
    CV_MayorsInvitationScript s; std::memset(&s.MissionSucceeded, 1, 9);
    s.Game = &g; s.SecretsLearned = s.WhichQuest = s.MonitorThunderHeroDuelRunning = 7;
    s.destinationHSP.PImp.Info = oldBlock; s.destinationHSP.PImp.Data = 0;
    s.Init();
    bool ok = g_deleted == 1 && s.destinationHSP.PImp.Info == g_newBlock && g_newBlock->RefCount == 2
        && !s.MissionSucceeded && !s.QuestCardGiven && !s.HeroReturnedFromParanoidQuest && s.SecretsLearned == 0
        && s.WhichQuest == 0 && s.MonitorThunderHeroDuelRunning == 0 && std::strcmp(g_lastName, "OakvaleHSP") == 0;
    if (!ok) { std::printf("00eb6bd0_TEST FAIL\n"); return 1; }
    std::printf("00eb6bd0_TEST PASS\n");
    return 0;
}