// Behaviour test for 0x00E32080 (RST2): model plus the body verbatim from final.cpp, callees stubbed.
// Checks that EXPRESSION_FOLLOW at the buddy flags ExpressionDetectedFollow and BuddyHelped, EXPRESSION_WAIT flags
// ExpressionDetectedWait, other expressions flag nothing, termination at the loop test and at both re-tests, and the
// release of the by-value buddy on every exit.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0, g_iatt = 0, g_termAt = 1 << 30, g_frames = 0, g_stopFrame = 1 << 30, g_queries = 0;
static const char* g_script[16];
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_stopFrame; }
struct CCharString {
    const char* Text;
    CCharString() : Text("") { }
    ~CCharString() { }
    bool operator==(const char* text) const { return strcmp(Text, text) == 0; }
};
struct CScriptThing {
    int Id;
    explicit CScriptThing(int id) : Id(id) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { ++g_live; }
    __forceinline ~CScriptThing() { --g_live; }
    bool MsgExpressionPerformedTo(CCharString& expression) const
    {
        ++g_queries;
        const char* e = g_frames < 16 ? g_script[g_frames] : 0;
        if (!e) return false;
        expression.Text = e;
        return true;
    }
};
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
};
struct CScriptBase {
    bool IsActiveThreadTerminating() { return Term(); }
};
struct CV_AmbushScamScript : CScriptBase {
    CGameScriptInterface* GSI;
    bool ExpressionDetectedFollow;
    bool ExpressionDetectedWait;
    bool BuddyHelped;
    void WatchForExpressions(CScriptThing buddy);
};

void CV_AmbushScamScript::WatchForExpressions(CScriptThing buddy)
{
    CCharString expression;
    while (!IsActiveThreadTerminating()) {
        if (buddy.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_FOLLOW") {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            ExpressionDetectedFollow = true;
            BuddyHelped = true;
        } else if (buddy.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_WAIT") {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            ExpressionDetectedWait = true;
        }
        GSI->NewScriptFrame();
    }
}

static void Run(CV_AmbushScamScript& s, int termAt, int stopFrame, const char* f0, const char* f1, const char* f2)
{
    static CGameScriptInterface gsi;
    g_live = 0; g_iatt = 0; g_frames = 0; g_queries = 0; g_termAt = termAt; g_stopFrame = stopFrame;
    memset(g_script, 0, sizeof(g_script)); g_script[0] = f0; g_script[1] = f1; g_script[2] = f2;
    s.GSI = &gsi; s.ExpressionDetectedFollow = s.ExpressionDetectedWait = s.BuddyHelped = false;
    s.WatchForExpressions(CScriptThing(3));
}
int main()
{
    CV_AmbushScamScript s;
    Run(s, 1, 1 << 30, 0, 0, 0); CHECK(g_frames == 0 && g_live == 0 && !s.ExpressionDetectedFollow);
    Run(s, 1 << 30, 3, 0, "EXPRESSION_FOLLOW", 0); CHECK(g_frames == 3 && s.ExpressionDetectedFollow && s.BuddyHelped && !s.ExpressionDetectedWait && g_live == 0);
    Run(s, 1 << 30, 3, "EXPRESSION_WAIT", 0, 0); CHECK(g_frames == 3 && !s.ExpressionDetectedFollow && !s.BuddyHelped && s.ExpressionDetectedWait && g_live == 0 && g_queries == 6);
    Run(s, 1 << 30, 3, "EXPRESSION_POINT", 0, 0); CHECK(g_frames == 3 && !s.ExpressionDetectedFollow && !s.ExpressionDetectedWait && g_live == 0);
    Run(s, 2, 1 << 30, "EXPRESSION_FOLLOW", 0, 0); CHECK(g_frames == 0 && !s.ExpressionDetectedFollow && !s.BuddyHelped && g_iatt == 2 && g_live == 0);
    Run(s, 2, 1 << 30, "EXPRESSION_WAIT", 0, 0); CHECK(g_frames == 0 && !s.ExpressionDetectedWait && g_iatt == 2 && g_live == 0);
    char line[64];
    _snprintf(line, sizeof(line), "00e32080_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}