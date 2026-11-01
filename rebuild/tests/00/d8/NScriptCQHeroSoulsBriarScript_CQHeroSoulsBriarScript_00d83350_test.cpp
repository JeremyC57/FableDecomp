// Behaviour test for 0x00D83350 NScript::CQ_HeroSoulsBriarScript::~CQ_HeroSoulsBriarScript (implicit): both timers, the csargs
// tree clear + header free, the seh_BriarRose counted-pointer release (count block freed and the data deleted on the last
// reference, Data/Info zeroed), the CScriptGameResourceObjectBase/CBaseObject chain, then CScriptBase.
#include <cstdio>
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
class CTimer { public: long TimerIndex; ~CTimer() { Log(1000 + (int)TimerIndex); } };
struct Node { bool Color; Node* Parent; Node* Left; Node* Right; };
struct ArgTree {
    Node* Header; unsigned Count;
    void Erase(Node* x) { while (x) { Erase(x->Right); Node* y = x->Left; Log(500); delete x; x = y; } }
    ~ArgTree() { if (Count != 0) { Erase(Header->Parent); Header->Left = Header; Header->Parent = 0; Header->Right = Header; Count = 0; } Log(600); delete Header; }
};
static void DeleteData(void*) { Log(800); }
class CCPPointerInfo { public: unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
struct CCountedPointer {
    void* Data; CCPPointerInfo* Info;
    ~CCountedPointer() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); Log(801); delete Info; } } Data = 0; Info = 0; }
};
class CBaseObject { public: void* IntelligentPointer; virtual ~CBaseObject() { Log(900); } };
class CScriptGameResourceObjectBase : public CBaseObject { public: virtual ~CScriptGameResourceObjectBase() { Log(901); } };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public: CCountedPointer PImp; };
class CScriptBase { public: virtual ~CScriptBase() { Log(9999); } };
struct CQ_HeroSoulsBriarScript : public CScriptBase {
    void* Game; void* MasterScriptData; bool PlayedBriarCS;
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose; ArgTree csargs;
    CTimer BriarRoseImposterTauntTimer; CTimer BriarRoseImposterHereTimer; bool KickOffQuestStartScreen;
};
int main()
{
    {
        CQ_HeroSoulsBriarScript s;
        CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DeleteData; info->Data = &s;
        s.seh_BriarRose.PImp.Data = &s; s.seh_BriarRose.PImp.Info = info;
        Node* h = new Node; Node* r = new Node; h->Parent = r; h->Left = r; h->Right = r; r->Parent = h; r->Left = 0; r->Right = 0;
        s.csargs.Header = h; s.csargs.Count = 1;
        s.BriarRoseImposterTauntTimer.TimerIndex = 5; s.BriarRoseImposterHereTimer.TimerIndex = 6;
    }
    static const int expect[] = { 1006, 1005, 500, 600, 800, 801, 901, 900, 9999 };
    if (g_n != 9) { std::printf("00d83350_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (g_log[i] != expect[i]) { std::printf("00d83350_TEST FAIL\n"); return 1; }
    std::printf("00d83350_TEST PASS\n");
    return 0;
}