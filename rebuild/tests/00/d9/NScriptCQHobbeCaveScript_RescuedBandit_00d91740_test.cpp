// Behaviour test for 0x00D91740 (RST2): model plus the body verbatim from final.cpp, callees stubbed.
// Checks the wait for the HobbeCaveSideRoom level, the per-frame HobbeRoaming lookup, KilledSideRoomHobbes once they
// are all dead (only when neither re-test is terminating), termination at every test, and that the hobbe vector's
// elements and buffer are released on every exit.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#define __fastcall
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0, g_bufs = 0, g_iatt = 0, g_termAt = 1 << 30, g_frames = 0, g_stopFrame = 1 << 30, g_loadFrame = 0, g_deadFrame = 0, g_lookups = 0;
static bool g_badArg = false;
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_stopFrame; }
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; }
    ~CCharString() { }
};
class CScriptThing { public:
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    virtual ~CScriptThing() { --g_live; }
};
static void FreeBuffer(void* p) { --g_bufs; free(p); }
namespace model {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            FreeBuffer(_M_start);
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}
namespace std { using model::vector; }
static bool __fastcall AreAllThingsInVectorDead(const std::vector<CScriptThing>& things)
{
    if (things.size() != 4) g_badArg = true;
    return g_frames >= g_deadFrame;
}
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    bool IsLevelLoaded(const CCharString& level) const
    {
        if (strcmp(level.Text, "HobbeCaveSideRoom") != 0) g_badArg = true;
        return g_frames >= g_loadFrame;
    }
    long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const
    {
        if (strcmp(name.Text, "HobbeRoaming") != 0) g_badArg = true;
        ++g_lookups;
        model::_Destroy(things._M_start, things._M_finish);   // refill: drop the previous frame's handles
        if (things._M_start) FreeBuffer(things._M_start);
        ++g_bufs;
        things._M_start = (CScriptThing*)malloc(sizeof(CScriptThing) * 4);
        for (int k = 0; k < 4; ++k)
            new (&things._M_start[k]) CScriptThing(k);
        things._M_finish = things._M_end_of_storage = things._M_start + 4;
        return 4;
    }
};
class CScriptBase { public:
    bool IsActiveThreadTerminating() { return Term(); }
};
struct CQ_HobbeCaveScript : CScriptBase {
    CGameScriptInterface* Game;
    bool                  KilledSideRoomHobbes;
    void RescuedBandit();
};

void CQ_HobbeCaveScript::RescuedBandit()
{
    std::vector<CScriptThing> hobbes;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("HobbeCaveSideRoom")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GetAllThingsWithScriptName("HobbeRoaming", hobbes);
        if (AreAllThingsInVectorDead(hobbes)) {
            if (!IsActiveThreadTerminating()) {
                if (!IsActiveThreadTerminating())
                    KilledSideRoomHobbes = true;
            }
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}

static void Run(CQ_HobbeCaveScript& s, int termAt, int stopFrame, int loadFrame, int deadFrame)
{
    static CGameScriptInterface gsi;
    g_live = 0; g_bufs = 0; g_iatt = 0; g_frames = 0; g_lookups = 0; g_badArg = false;
    g_termAt = termAt; g_stopFrame = stopFrame; g_loadFrame = loadFrame; g_deadFrame = deadFrame;
    s.Game = &gsi; s.KilledSideRoomHobbes = false;
    s.RescuedBandit();
}
int main()
{
    CQ_HobbeCaveScript s;
    Run(s, 1 << 30, 1 << 30, 2, 5); CHECK(g_frames == 5 && s.KilledSideRoomHobbes && g_lookups == 4 && g_live == 0 && g_bufs == 0 && !g_badArg);
    Run(s, 1 << 30, 1 << 30, 0, 0); CHECK(g_frames == 0 && s.KilledSideRoomHobbes && g_iatt == 4 && g_lookups == 1 && g_live == 0 && g_bufs == 0);
    Run(s, 1, 1 << 30, 0, 0);       CHECK(g_iatt == 1 && !s.KilledSideRoomHobbes && g_lookups == 0 && g_live == 0 && g_bufs == 0);   // loop top
    Run(s, 1 << 30, 2, 5, 0);       CHECK(g_frames == 2 && !s.KilledSideRoomHobbes && g_lookups == 0 && g_bufs == 0);               // in the level wait
    Run(s, 2, 1 << 30, 0, 0);       CHECK(g_iatt == 2 && !s.KilledSideRoomHobbes && g_lookups == 0 && g_bufs == 0);                 // after the wait
    Run(s, 3, 1 << 30, 0, 0);       CHECK(g_iatt == 3 && !s.KilledSideRoomHobbes && g_lookups == 1 && g_live == 0 && g_bufs == 0);  // first re-test
    Run(s, 4, 1 << 30, 0, 0);       CHECK(g_iatt == 4 && !s.KilledSideRoomHobbes && g_lookups == 1 && g_live == 0 && g_bufs == 0);  // second re-test
    Run(s, 1 << 30, 3, 0, 9);       CHECK(g_frames == 3 && !s.KilledSideRoomHobbes && g_lookups == 3 && g_live == 0 && g_bufs == 0); // alive hobbes, stopped
    char line[64];
    _snprintf(line, sizeof(line), "00d91740_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}