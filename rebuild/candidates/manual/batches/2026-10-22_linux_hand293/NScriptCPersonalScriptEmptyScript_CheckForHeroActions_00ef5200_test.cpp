// Behaviour test for NScript::CPersonalScript_EmptyScript::CheckForHeroActions (0x00EF5200): while the thread runs,
// an alive hero with kills produces "You killed <n> things, of types: a, b." once per frame, a dead hero produces
// nothing, every exit (including the terminate-after-kills one) releases the hero's counted reference and frees the
// kill vector, and NewScriptFrame runs once per completed iteration.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

struct CCharString {
    std::string s;
    CCharString() { }
    CCharString(const char* text) : s(text) { }
    CCharString& operator+=(const CCharString& other) { s += other.s; return *this; }
    CCharString& operator+=(const char* text) { s += text; return *this; }
};
CCharString operator+(const char* left, const CCharString& right) { CCharString r(left); r.s += right.s; return r; }
CCharString operator+(const CCharString& left, const char* right) { CCharString r = left; r.s += right; return r; }
CCharString GFIntToCharString(long value) { char b[32]; sprintf(b, "%ld", value); return CCharString(b); }
CCharString GFUIntToCharString(unsigned long value) { char b[32]; sprintf(b, "%lu", value); return CCharString(b); }

static int g_frees = 0;
namespace stl {
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) { free(_M_start); ++g_frees; } }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int i) { return _M_start[i]; }
};
}
struct CCPPointerInfo {
    long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer(T* data, CCPPointerInfo* info) : Data(data), Info(info) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseClass {
    CBaseClass() { }
    virtual ~CBaseClass() { }
};
struct CScriptThing : CBaseClass {
    CCountedPointer<CScriptThing> PImp;
    CScriptThing(CScriptThing* data, CCPPointerInfo* info) : PImp(data, info) { }
    CScriptThing(const CScriptThing& other) : CBaseClass(), PImp(other.PImp) { }
    virtual ~CScriptThing() { }
    virtual bool MsgGetThingsKilled(stl::vector<unsigned long>& killed) const
    {
        if (PImp.Data)
            return PImp.Data->MsgGetThingsKilled(killed);
        return false;
    }
    virtual bool IsAlive() const
    {
        return PImp.Data && PImp.Data->IsAlive();
    }
};
struct CHeroImpl : CScriptThing {                       // the counted implementation the handle forwards to
    bool alive;
    int  kills;
    CHeroImpl() : CScriptThing(0, 0), alive(true), kills(2) { }
    virtual bool MsgGetThingsKilled(stl::vector<unsigned long>& killed) const
    {
        if (kills == 0)
            return false;
        killed._M_start = (unsigned long*)malloc(kills * sizeof(unsigned long));
        for (int i = 0; i < kills; ++i)
            killed._M_start[i] = 3 + 4 * i;
        killed._M_finish = killed._M_end_of_storage = killed._M_start + kills;
        return true;
    }
    virtual bool IsAlive() const { return alive; }
};
static void NoDelete(void*) { }

typedef long ETextGroupSelectionMethod;
struct CGameScriptInterface {
    CScriptThing* hero;
    int frames;
    int messages;
    std::string last;
    void NewScriptFrame() { ++frames; }
    CScriptThing& GetHero() { return *hero; }
    void AddScreenMessage(const CCharString& text, ETextGroupSelectionMethod) { ++messages; last = text.s; }
};
struct CScriptBase {
    CGameScriptInterface* GSI;
    int calls;
    int stopAt;                                          // IsActiveThreadTerminating returns true from this call on
    bool IsActiveThreadTerminating() { return ++calls >= stopAt; }
};
namespace NScript {
struct CPersonalScript_EmptyScript : CScriptBase {
    void CheckForHeroActions();
};

void CPersonalScript_EmptyScript::CheckForHeroActions()
{
    if (IsActiveThreadTerminating())
        return;
    do {
        {
            CScriptThing hero = GSI->GetHero();
            if (hero.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                {
                    stl::vector<unsigned long> killed;
                    if (hero.MsgGetThingsKilled(killed)) {
                        if (IsActiveThreadTerminating())
                            goto terminating;
                        CCharString text = "You killed " + GFIntToCharString(killed.size()) + " things, of types: ";
                        for (unsigned int i = 0; i < killed.size(); ++i) {
                            text += GFUIntToCharString(killed[i]);
                            if (i == killed.size() - 1)
                                text += ".";
                            else
                                text += ", ";
                        }
                        GSI->AddScreenMessage(text, 0);
                    }
                }
            }
            if (false) {
terminating:
                return;
            }
        }
        GSI->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}

static int Run(CHeroImpl& impl, int stopAt, CGameScriptInterface& game, long& refs)
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 1;
    info->DeleteFunc = NoDelete;
    info->Data = &impl;
    CScriptThing heroHandle(&impl, info);
    game.hero = &heroHandle;
    game.frames = 0;
    game.messages = 0;
    game.last = "";
    NScript::CPersonalScript_EmptyScript script;
    script.GSI = &game;
    script.calls = 0;
    script.stopAt = stopAt;
    script.CheckForHeroActions();
    refs = info->RefCount;
    return script.calls;
}

int main()
{
    int ok = 1;
    CHeroImpl impl;
    CGameScriptInterface game;
    long refs = 0;

    // Immediately terminating: nothing happens.
    g_frees = 0;
    Run(impl, 1, game, refs);
    ok &= game.frames == 0 && game.messages == 0 && refs == 1;

    // Alive hero with two kills, two full frames, then the loop test stops it.
    // calls: 1 entry, 2 alive check, 3 kills check, 4 loop test, 5, 6, 7 loop test -> stop
    g_frees = 0;
    Run(impl, 7, game, refs);
    ok &= game.messages == 2 && game.frames == 2 && refs == 1 && g_frees == 2;
    ok &= game.last == "You killed 2 things, of types: 3, 7.";

    // Terminate right after the kills were fetched: vector freed, hero released, no message, no frame.
    g_frees = 0;
    Run(impl, 3, game, refs);
    ok &= game.messages == 0 && game.frames == 0 && refs == 1 && g_frees == 1;

    // Terminate at the alive check.
    g_frees = 0;
    Run(impl, 2, game, refs);
    ok &= game.messages == 0 && game.frames == 0 && refs == 1 && g_frees == 0;

    // Dead hero: no message, a frame per iteration.
    impl.alive = false;
    Run(impl, 4, game, refs);
    ok &= game.messages == 0 && game.frames == 3 && refs == 1;

    // Alive hero without kills: no message, no free.
    impl.alive = true;
    impl.kills = 0;
    g_frees = 0;
    Run(impl, 4, game, refs);
    ok &= game.messages == 0 && game.frames == 1 && refs == 1 && g_frees == 0;

    // One kill: "." straight after the first type.
    impl.kills = 1;
    Run(impl, 4, game, refs);
    ok &= game.messages == 1 && game.last == "You killed 1 things, of types: 3." && refs == 1;

    printf("00ef5200_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
