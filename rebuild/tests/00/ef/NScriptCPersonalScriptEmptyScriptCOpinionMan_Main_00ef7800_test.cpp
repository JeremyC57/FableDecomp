// Behaviour test for 0x00EF7800 COpinionMan::Main (RST2): model plus the body verbatim from final.cpp, the engine stubbed.
// Checks: the guide is looked up by script name and a CPredicate_And(CIsThing(Me), CIsThingAlive(guide)) is installed;
// then, until the thread terminates, each time the hero talks to Me the hero-guide quest-card flag is set to the
// current toggle (false, true, ...) with the expected script frames; every exit releases every script thing
// (reference counts back to the start) and every CCharString temporary.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_strings = 0, g_deleted = 0, g_destroyed = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { (void)length; ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CBaseClass { public:
    virtual ~CBaseClass() { }
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
static void __fastcall DestroyData(void* data) { (void)data; ++g_destroyed; }
static void ReleaseInfo(CCPPointerInfo* info) { if (info && --info->RefCount == 0) { info->DeleteFunc(info->Data); ++g_deleted; } }
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer() { ReleaseInfo(Info); Data = 0; Info = 0; }
};
static int g_talkCalls = 0;
class CScriptThing : public CBaseClass { public:
    CCountedPointer<CScriptThing> PImp;
    explicit CScriptThing(CCPPointerInfo* info) { PImp.Data = 0; PImp.Info = info; ++info->RefCount; }
    CScriptThing(const CScriptThing& other) { PImp.Data = other.PImp.Data; PImp.Info = other.PImp.Info; if (PImp.Info) ++PImp.Info->RefCount; }
    virtual bool MsgIsTalkedToBy(const CCharString& name) const { CHECK(strcmp(name.PStringData, "SCRIPT_NAME_HERO") == 0); return (++g_talkCalls % 2) == 0; }
    __forceinline ~CScriptThing() {}
};
static CCPPointerInfo g_meInfo, g_guideInfo;
static int g_frames = 0, g_sets = 0; static bool g_setArgs[16];
struct GameCalls {
    virtual void NewScriptFrame() const { ++g_frames; }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const { CHECK(strcmp(name.PStringData, "HeroGuideDude") == 0); return CScriptThing(&g_guideInfo); }
    virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool show) { if (g_sets < 16) g_setArgs[g_sets] = show; ++g_sets; }
};
namespace NScript {
class CPredicateBase { public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() {}
};
#pragma inline_depth(0)
struct CHeldThing {
    CScriptThing Thing;
    __forceinline CHeldThing(const CScriptThing& t) : Thing(t) {}
    ~CHeldThing() {}
};
#pragma inline_depth()
struct CIsThing : CPredicateBase {
    CHeldThing Thing;
    __forceinline CIsThing(const CScriptThing& t) : Thing(t) {}
    virtual bool operator()() const { return true; }
    virtual CPredicateBase* Clone() const { return 0; }
    ~CIsThing() {}
};
struct CIsThingStored : CPredicateBase {
    CScriptThing Thing;
    CIsThingStored(const CIsThing& a) : Thing(a.Thing.Thing) {}
    virtual bool operator()() const { return true; }
    virtual CPredicateBase* Clone() const { return 0; }
    ~CIsThingStored() {}
};
class CIsThingAlive : public CPredicateBase { public:
    CScriptThing Thing;
    __forceinline CIsThingAlive(const CScriptThing& t) : Thing(t) {}
    virtual bool operator()() const { return true; }
    virtual CPredicateBase* Clone() const { return 0; }
    ~CIsThingAlive() {}
};
struct CPredicate_And : CPredicateBase {
    CIsThingStored A;
    CIsThingAlive B;
    CPredicate_And(const CIsThing& a, const CIsThingAlive& b) : A(a), B(b.Thing) {}
    CPredicate_And(const CPredicate_And& o) : CPredicateBase(), A(o.A), B(o.B.Thing) {}
    virtual bool operator()() const { return true; }
    virtual CPredicateBase* Clone() const { return 0; }
    ~CPredicate_And() {}
};
}
inline NScript::CPredicate_And And(const NScript::CIsThing& a, const NScript::CIsThingAlive& b) { return NScript::CPredicate_And(a, b); }
static int g_predicates = 0; static CCPPointerInfo* g_predA = 0; static CCPPointerInfo* g_predB = 0;
static int g_termCalls = 0, g_termAt = 0;
struct COpinionMan {
    void* VTable; GameCalls* Game; CScriptThing Me;
    void* ParentClass;
    void* PMasterData;
    COpinionMan(GameCalls* game) : VTable(0), Game(game), Me(&g_meInfo), ParentClass(0), PMasterData(0) { }
    void SetPredicate(const NScript::CPredicateBase& p)
    {
        const NScript::CPredicate_And* a = static_cast<const NScript::CPredicate_And*>(&p);   // Main installs an And
        ++g_predicates;
        if (a) { g_predA = a->A.Thing.PImp.Info; g_predB = a->B.Thing.PImp.Info; }
    }
    bool IsActiveThreadTerminating() { return ++g_termCalls >= g_termAt; }
    void Main();
};

void COpinionMan::Main()
{
    CScriptThing guide = Game->GetThingWithScriptName("HeroGuideDude");
    SetPredicate(And(NScript::CIsThing(Me), NScript::CIsThingAlive(guide)));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    bool show = false;
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
        Game->SetHeroGuideToShowQuestCardsWhenSpokenTo(show);
        show = !show;
        Game->NewScriptFrame();
#pragma inline_depth(0)
        if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
        Game->NewScriptFrame();
    }
}

static void Run(int termAt, int frames, int sets, const bool* expect)
{
    g_meInfo.RefCount = 1; g_meInfo.DeleteFunc = DestroyData; g_meInfo.Data = 0;
    g_guideInfo.RefCount = 0; g_guideInfo.DeleteFunc = DestroyData; g_guideInfo.Data = 0;
    g_frames = g_sets = g_termCalls = g_talkCalls = g_predicates = g_deleted = g_destroyed = 0; g_predA = g_predB = 0;
    g_termAt = termAt;
    GameCalls game;
    {
        COpinionMan man(&game);
        man.Main();
        CHECK(g_meInfo.RefCount == 2);               // Me (held by man) plus the test's own reference
    }
    CHECK(g_predicates == 1 && g_predA == &g_meInfo && g_predB == &g_guideInfo);
    CHECK(g_frames == frames);
    CHECK(g_sets == sets);
    for (int i = 0; i < sets && i < 16; ++i) CHECK(g_setArgs[i] == expect[i]);
    CHECK(g_guideInfo.RefCount == 0 && g_strings == 0);
    CHECK(g_termCalls == termAt);
}

int main()
{
    static const bool tf[] = { false, true, false, true };
    Run(1, 1, 0, tf);    // terminating straight after the first frame
    Run(2, 1, 0, tf);    // terminating at the top of the talk loop
    Run(3, 2, 0, tf);    // inside the wait-for-talk loop
    Run(4, 2, 0, tf);    // the check after the hero talks
    Run(5, 3, 1, tf);    // after the first toggle
    Run(7, 5, 1, tf);    // waiting for the second talk
    Run(8, 5, 1, tf);
    Run(10, 7, 2, tf);   // second round complete: false then true
    Run(14, 10, 3, tf);  // third toggle (false again), terminating at the loop top
    char line[64];
    _snprintf(line, sizeof(line), "00ef7800_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}