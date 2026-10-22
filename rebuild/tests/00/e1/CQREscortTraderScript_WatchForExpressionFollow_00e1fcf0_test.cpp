// Behaviour test for 0x00E1FCF0 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks EXPRESSION_FOLLOW detection (timer restart + flag, watch continues), the other expression and an empty one
// (ignored), termination inside the hit block, immediate termination and a dead trader; the trader is released on every exit.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0;                  // live CScriptThing copies (every exit must release the watched thing)
static int g_iatt = 0;                  // IsActiveThreadTerminating call count
static int g_termAt = 1 << 30;          // ... it returns true from this call number on
static int g_termFrame = 1 << 30;       // ... or once this many script frames have run
static int g_frames = 0;                // NewScriptFrame count
static bool g_badName = false;          // a callee saw an unexpected name / argument
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_termFrame; }
static void Reset() { g_live = 0; g_iatt = 0; g_termAt = 1 << 30; g_termFrame = 1 << 30; g_frames = 0; g_badName = false; }

struct CCharString {
    const char* PStringData;
    CCharString() : PStringData(0) { }
    ~CCharString() { }
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return strcmp(PStringData, text) == 0;
    }
};
static int g_dieAt = 1 << 30;
static int g_exprAt = -1; static const char* g_exprName = 0;
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
    bool IsAlive() const { return g_frames < g_dieAt; }
    bool MsgExpressionPerformedTo(CCharString& expression) const
    {
        if (g_frames != g_exprAt) return false;
        expression.PStringData = g_exprName;
        return true;
    }
};
struct CGameScriptInterface { void NewScriptFrame() const { ++g_frames; } };
static int g_restarts = 0;
struct CQR_EscortTraderScript {
    CGameScriptInterface* Game;
    bool ExpressionDetectedFollow;
    bool ExpressionDetectedWait;
    bool IsActiveThreadTerminating() { return Term(); }
    void RestartRandomSpeechTimer() { ++g_restarts; }
    void WatchForExpressionFollow(CScriptThing trader);
    void WatchForExpressionWait(CScriptThing trader);
};

void CQR_EscortTraderScript::WatchForExpressionFollow(CScriptThing trader)
{
    CCharString expression;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (trader.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (trader.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_FOLLOW") {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                RestartRandomSpeechTimer();
                ExpressionDetectedFollow = true;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        Game->NewScriptFrame();
    }
}

static void Run(int termAt, int dieAt, int exprAt, const char* name, CQR_EscortTraderScript& s)
{
    Reset(); g_termAt = termAt; g_dieAt = dieAt; g_exprAt = exprAt; g_exprName = name; g_restarts = 0;
    static CGameScriptInterface game; s.Game = &game; s.ExpressionDetectedFollow = false; s.ExpressionDetectedWait = false;
    s.WatchForExpressionFollow(CScriptThing(7));
}
int main()
{
    CQR_EscortTraderScript s;
    Run(1, 1000, -1, 0, s);  CHECK(!s.ExpressionDetectedFollow && g_frames == 0 && g_live == 0);
    Run(1 << 30, 4, 2, "EXPRESSION_FOLLOW", s); CHECK(s.ExpressionDetectedFollow && g_restarts == 1 && g_frames == 4 && g_live == 0);
    Run(1 << 30, 4, 2, "EXPRESSION_WAIT", s); CHECK(!s.ExpressionDetectedFollow && !s.ExpressionDetectedWait && g_restarts == 0 && g_frames == 4 && g_live == 0);
    Run(1 << 30, 4, 1, 0, s); CHECK(!s.ExpressionDetectedFollow && g_restarts == 0 && g_live == 0);   // performed, no name
    Run(5, 1000, 1, "EXPRESSION_FOLLOW", s); CHECK(!s.ExpressionDetectedFollow && g_restarts == 0 && g_frames == 1 && g_live == 0);   // terminating in the hit block
    Run(1 << 30, 0, -1, 0, s); CHECK(g_frames == 0 && g_live == 0 && g_iatt == 2);   // dead at once: top, dead-branch check, break to the end
    char line[64];
    _snprintf(line, sizeof(line), "00e1fcf0_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}