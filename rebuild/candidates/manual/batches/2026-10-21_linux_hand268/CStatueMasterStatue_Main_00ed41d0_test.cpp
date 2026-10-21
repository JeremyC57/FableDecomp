// Behaviour test for CStatueMasterStatue::Main (0x00ED41D0): cutscene behaviour set once, the facing angle is the
// parent's angle + 0.5 wrapped into [0,1), and the readable tag changes only when the pointing position changes.
#include <cstdio>
#include <cstring>
#include <cmath>
struct CCharString {
    char Text[48];
    CCharString() { Text[0] = 0; }
    CCharString(const char* text) { std::strncpy(Text, text, 47); Text[47] = 0; }
};
static int g_frames = 0;
struct CScriptThing { int id; };
struct CGameScriptInterface {
    long behaviour; float angles[16]; int nAngles; char tags[256]; int altarLookups;
    void NewScriptFrame() { ++g_frames; }
    CScriptThing GetThingWithScriptName(const CCharString& name) { if (std::strcmp(name.Text, "StatueAltar") == 0) ++altarLookups; CScriptThing t; t.id = 5; return t; }
    void EntitySetFacingAngle(const CScriptThing&, float a, bool immediate) { if (immediate && nAngles < 16) angles[nAngles++] = a; }
    void EntitySetCutsceneBehaviour(const CScriptThing&, long b) { behaviour = b; }
    void SetReadableObjectTextTag(const CScriptThing&, const CCharString& tag) { std::strcat(tags, tag.Text); std::strcat(tags, ";"); }
};
struct CV_StatueMasterScript {
    float GetStatueAngle() { return 0.25f * (float)g_frames; }
    long GetStatuePointingPosition() { static const long p[] = { 0, 1, 1, 3, 2, 2 }; return p[g_frames % 6]; }
};
struct CStatueMasterStatue {
    CGameScriptInterface* Game; CScriptThing Me; CV_StatueMasterScript* ParentClass; int doneAt;
    bool IsDone() { return g_frames >= doneAt; }
    void Main();
};
void CStatueMasterStatue::Main()
{
    Game->EntitySetCutsceneBehaviour(Me, 2);
    CScriptThing altar = Game->GetThingWithScriptName("StatueAltar");
    CCharString text;
    long lastPosition = 0;
    for (;;) {
        if (IsDone())
            break;
        float angle = ParentClass->GetStatueAngle() + 0.5f;
        Game->EntitySetFacingAngle(Me, (float)fmod((double)angle, 1.0), true);
        long previous = lastPosition;
        long position = ParentClass->GetStatuePointingPosition();
        lastPosition = position;
        if (position != previous) {
            if (IsDone())
                return;
            switch (position) {
            case 1:
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_061_STATUE_BOWERSTONE");
                break;
            case 3:
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_061_STATUE_GREATWOOD");
                break;
            default:
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_061_STATUE_NOWHERE");
                break;
            }
        }
        Game->NewScriptFrame();
    }
}
int main()
{
    CGameScriptInterface g; std::memset(&g, 0, sizeof(g));
    CV_StatueMasterScript p;
    CStatueMasterStatue s; s.Game = &g; s.Me.id = 1; s.ParentClass = &p; s.doneAt = 6;
    s.Main();
    bool ok = g.behaviour == 2 && g.altarLookups == 1 && g.nAngles == 6 && g_frames == 6
        && g.angles[0] == 0.5f && g.angles[1] == 0.75f && g.angles[2] == 0.0f && g.angles[3] == 0.25f
        && std::strcmp(g.tags, "TEXT_QST_061_STATUE_BOWERSTONE;TEXT_QST_061_STATUE_GREATWOOD;TEXT_QST_061_STATUE_NOWHERE;") == 0;
    if (!ok) { std::printf("00ed41d0_TEST FAIL\n"); return 1; }
    std::printf("00ed41d0_TEST PASS\n");
    return 0;
}
