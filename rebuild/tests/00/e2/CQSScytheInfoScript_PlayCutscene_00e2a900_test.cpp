// Behaviour test for 00e2a900 (CQS_ScytheInfoScript::PlayCutscene): hero scripted at priority 4 and entered in the cast,
// camera fixed, the cut-scene runs (wrapped in a paused movie sequence when inMovieSequence), camera released, both maps cleared;
// the counted-pointer handle shares and releases its implementation exactly once.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
static std::string g_log;
static void L(const std::string& s) { g_log += s; g_log += ' '; }
class CCharString { public:
    std::string s;
    CCharString() {}
    CCharString(const char* text, long = -1) : s(text) {}
    bool operator<(const CCharString& o) const { return s < o.s; }
};
class CCPPointerInfo { public: long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int s_deleted = 0;
static void DeleteImp(void*) { ++s_deleted; }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    void Release() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) { Release(); Data = data; Info = info; if (info) ++info->RefCount; }
        return *this;
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
};
struct Imp { int id; };
struct Handle { CCountedPointer<Imp> PImp; };
struct Movie { int unused; };
struct Hero { int id; };
struct Game {
    Hero hero; int priority;
    Hero& GetHero() { return hero; }
    bool StartScriptingEntity(const Hero&, Handle& h, int prio)
    {
        priority = prio;
        h.PImp.Info = new CCPPointerInfo; h.PImp.Info->RefCount = 1; h.PImp.Info->DeleteFunc = DeleteImp; h.PImp.Info->Data = 0;
        L("SSE");
        return true;
    }
    void StartMovieSequence(const CCharString& n, Movie&) { if (n.s.empty()) L("MOV"); }
    void FixMovieSequenceCamera(bool f) { L(f ? "FIX1" : "FIX0"); }
    void PauseAllNonScriptedEntities(bool p) { L(p ? "P1" : "P0"); }
};
typedef std::map<CCharString, CCharString> ArgMap;
typedef std::map<CCharString, Handle> CastMap;
static long s_refsDuringRun = 0;
static void RunScriptedCutscene(const CCharString& name, CastMap& cast, void*, ArgMap& args, void*, bool)
{
    s_refsDuringRun = cast[CCharString("HERO")].PImp.Info ? cast[CCharString("HERO")].PImp.Info->RefCount : -1;
    L("RUN:" + name.s + (args.count(CCharString("a")) ? "+a" : ""));
}
struct Script {
    Game* G; ArgMap cs_args; CastMap cs_chars;
    void PlayCutscene(CCharString name, bool inMovieSequence)
    {
        Handle hero;
        G->StartScriptingEntity(G->GetHero(), hero, 4);
        cs_chars["HERO"] = hero;
        G->FixMovieSequenceCamera(true);
        if (inMovieSequence) {
            Movie movie;
            {
                CCharString sequence("");
                G->StartMovieSequence(sequence, movie);
            }
            Game* game = G;
            game->PauseAllNonScriptedEntities(true);
            RunScriptedCutscene(name, cs_chars, 0, cs_args, 0, true);
            game->PauseAllNonScriptedEntities(false);
        } else
            RunScriptedCutscene(name, cs_chars, 0, cs_args, 0, true);
        G->FixMovieSequenceCamera(false);
        cs_args.clear();
        cs_chars.clear();
    }
};
static int fails = 0;
static void check(bool c, int n) { if (!c) { std::printf("fail %d: %s\n", n, g_log.c_str()); ++fails; } }
int main()
{
    Game g; g.priority = 0; g.hero.id = 1;
    Script s; s.G = &g;
    s.cs_args[CCharString("a")] = CCharString("b");
    s.PlayCutscene(CCharString("CS_ONE"), false);
    check(g_log == "SSE FIX1 RUN:CS_ONE+a FIX0 ", 1);
    check(g.priority == 4 && s_refsDuringRun == 2 && s_deleted == 1, 2);
    check(s.cs_args.empty() && s.cs_chars.empty(), 3);
    g_log.erase(); s_deleted = 0;
    s.PlayCutscene(CCharString("CS_TWO"), true);
    check(g_log == "SSE FIX1 MOV P1 RUN:CS_TWO P0 FIX0 ", 4);
    check(s_deleted == 1, 5);
    if (fails == 0) std::printf("00e2a900_TEST PASS\n");
    return fails;
}