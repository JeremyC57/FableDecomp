// Behaviour test for 0x00da2860 (RST2): model plus the body verbatim from final.cpp, callees stubbed.
// Checks that a hit which costs health clears the no-damage flag and ends the watch, a hit without damage keeps
// watching, termination before and after the hit, and the release of the by-value thing on every exit.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0, g_iatt = 0, g_termAt = 1 << 30, g_frames = 0, g_hitFrame = 1 << 30, g_damageFrame = 1 << 30, g_stopFrame = 1 << 30;
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
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
    bool MsgIsHitBy(const CCharString& a) const { if (strcmp(a.Text, "") != 0) g_badArg = true; return g_frames >= g_hitFrame; }
};
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    float GetHealth(const CScriptThing& t) const { if (t.Id != 7) g_badArg = true; return g_frames >= g_damageFrame ? 40.5f : 100.5f; }
};
namespace NScript {
class CQ_SunnyvaleMasterData { public: bool MinionCampBriarNoDamage; };
struct CScriptBase {
    bool IsActiveThreadTerminating() { return Term(); }
};
class CQ_MinionCampScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;
    CQ_SunnyvaleMasterData* PMasterData;
    void CheckBriarHit(CScriptThing briar);
};

void CQ_MinionCampScript::CheckBriarHit(CScriptThing briar)
{
    long health = (long)GSI->GetHealth(briar);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        bool hit = briar.MsgIsHitBy("") && GSI->GetHealth(briar) < health;
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            PMasterData->MinionCampBriarNoDamage = false;
            return;
        }
        GSI->NewScriptFrame();
    }
}
}
using namespace NScript;
static CQ_SunnyvaleMasterData g_md;
static void Run(CQ_MinionCampScript& s, int termAt, int stopFrame, int hitFrame, int damageFrame)
{
    static CGameScriptInterface gsi;
    g_live = 0; g_iatt = 0; g_frames = 0; g_badArg = false;
    g_termAt = termAt; g_stopFrame = stopFrame; g_hitFrame = hitFrame; g_damageFrame = damageFrame;
    g_md.MinionCampBriarNoDamage = true; s.GSI = &gsi; s.PMasterData = &g_md;
    s.CheckBriarHit(CScriptThing(7));
}
int main()
{
    CQ_MinionCampScript s;
    Run(s, 1, 1 << 30, 0, 0);          CHECK(g_frames == 0 && g_md.MinionCampBriarNoDamage && g_live == 0);           // terminating at once
    Run(s, 1 << 30, 1 << 30, 3, 3);    CHECK(g_frames == 3 && !g_md.MinionCampBriarNoDamage && g_live == 0 && !g_badArg); // hit with damage
    Run(s, 1 << 30, 8, 2, 1 << 30);    CHECK(g_frames == 8 && g_md.MinionCampBriarNoDamage && g_live == 0);           // hit, no damage: keep watching
    Run(s, 1 << 30, 8, 1 << 30, 2);    CHECK(g_frames == 8 && g_md.MinionCampBriarNoDamage && g_live == 0);           // damage, no hit
    Run(s, 3, 1 << 30, 1, 1);          CHECK(g_frames == 1 && g_md.MinionCampBriarNoDamage && g_iatt == 3 && g_live == 0); // terminating after the hit
    char line[64];
    _snprintf(line, sizeof(line), "00da2860_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}