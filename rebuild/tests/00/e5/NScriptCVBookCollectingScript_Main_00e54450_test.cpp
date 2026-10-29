// Behaviour test for 0x00E54450 NScript::CV_BookCollectingScript::Main: wait (frame by frame) for BowerstoneSlums to load,
// then bind BS_Teacher (this script, the teacher factory, flags 0) and post the scripted entities to the script and the game;
// a terminating thread binds nothing.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
struct CV_BookCollectingScript;
typedef int (*TAllocFunc)(CV_BookCollectingScript&);
static int TeacherAlloc(CV_BookCollectingScript&) { return 42; }
struct CEntityScriptBinding {
    CCharString EntityScriptName; CV_BookCollectingScript* PParentScript; TAllocFunc ClassAllocFunc; bool ClassAllocValid; unsigned long Flags;
    CEntityScriptBinding(const CCharString& n, CV_BookCollectingScript* p, TAllocFunc a, unsigned long f)
        : EntityScriptName(n), PParentScript(p), ClassAllocFunc(a), ClassAllocValid(true), Flags(f) { }
};
class CGameScriptInterface { public:
    int Frames; int LoadedAt;
    void NewScriptFrame() { ++Frames; }
    bool IsRegionLoaded(const CCharString& n) const { return !std::strcmp(n.Text, "BowerstoneSlums") && Frames >= LoadedAt; }
    void PostAddScriptedEntities() const { Log("game-post"); }
};
struct CV_BookCollectingScript {
    CGameScriptInterface* Game; int TermAt, TermCalls; CEntityScriptBinding* Bound;
    bool IsActiveThreadTerminating() { return TermAt && ++TermCalls >= TermAt; }
    void AddEntityScriptBinding(CEntityScriptBinding* b) { Bound = b; Log("bind"); }
    void PostAddScriptedEntities() { Log("post"); }
    void Main()
    {
        while (!Game->IsRegionLoaded("BowerstoneSlums")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        AddEntityScriptBinding(new CEntityScriptBinding("BS_Teacher", this, &TeacherAlloc, 0));
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
    }
};

int main()
{
    CGameScriptInterface game; game.Frames = 0; game.LoadedAt = 4;
    CV_BookCollectingScript s; s.Game = &game; s.TermAt = 0; s.TermCalls = 0; s.Bound = 0;
    s.Main();
    int ok = !std::strcmp(g_log, "bind;post;game-post;") && game.Frames == 4 && s.Bound
             && !std::strcmp(s.Bound->EntityScriptName.Text, "BS_Teacher") && s.Bound->PParentScript == &s
             && s.Bound->ClassAllocFunc(s) == 42 && s.Bound->ClassAllocValid && s.Bound->Flags == 0;
    delete s.Bound;
    g_log[0] = 0; game.Frames = 0; s.Bound = 0; s.TermAt = 2; s.TermCalls = 0;
    s.Main();
    ok = ok && g_log[0] == 0 && !s.Bound && game.Frames == 2;
    if (!ok) { std::printf("00e54450_TEST FAIL\n"); return 1; }
    std::printf("00e54450_TEST PASS\n");
    return 0;
}