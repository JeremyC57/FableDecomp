#include <cstdio>
// Behaviour test for 00ed4830: with the statue at position 3 only the abort test runs; otherwise the chest is removed
// through the quest (thing, false, true) unless the script was aborted.
struct CScriptThing { int m_id; };
struct SRemove { const CScriptThing* thing; bool a; bool b; };
static SRemove g_removes[4]; static int g_nRemoves = 0;
struct CGameScriptInterface {
    void RemoveThing(const CScriptThing& thing, bool a, bool b) const { SRemove r; r.thing = &thing; r.a = a; r.b = b; g_removes[g_nRemoves++] = r; }
};
struct CV_StatueMasterScript {
    long m_position;
    long GetStatuePointingPosition() { return m_position; }
};
static int g_abortTests = 0;
struct CStatueMasterChest {
    CGameScriptInterface*   PQuest;
    CScriptThing            Thing;
    CV_StatueMasterScript*  ParentClass;
    bool                    m_aborted;
    bool IsAborted() { ++g_abortTests; return m_aborted; }
    void Main();
};
void CStatueMasterChest::Main()
{
    if (ParentClass->GetStatuePointingPosition() == 3) {
        if (IsAborted())
            return;
    } else {
        if (IsAborted())
            return;
        PQuest->RemoveThing(Thing, false, true);
    }
}
int main()
{
    CGameScriptInterface quest; CV_StatueMasterScript script;
    CStatueMasterChest chest; chest.PQuest = &quest; chest.Thing.m_id = 9; chest.ParentClass = &script; chest.m_aborted = false;
    script.m_position = 3; chest.Main();
    if (g_nRemoves != 0 || g_abortTests != 1) { std::printf("00ed4830_TEST FAIL\n"); return 1; }
    script.m_position = 1; chest.Main();
    if (g_nRemoves != 1 || g_removes[0].thing != &chest.Thing || g_removes[0].a || !g_removes[0].b || g_abortTests != 2) { std::printf("00ed4830_TEST FAIL\n"); return 1; }
    chest.m_aborted = true; chest.Main();
    if (g_nRemoves != 1 || g_abortTests != 3) { std::printf("00ed4830_TEST FAIL\n"); return 1; }
    std::printf("00ed4830_TEST PASS\n");
    return 0;
}