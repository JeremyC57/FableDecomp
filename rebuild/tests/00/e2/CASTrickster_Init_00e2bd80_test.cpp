#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct SCountInfo { long RefCount; };
struct CScriptThing {
    int id; SCountInfo* Info; C3DVector pos;
    CScriptThing(int i, SCountInfo* c) : id(i), Info(c) { pos.X = 1; pos.Y = 2; pos.Z = 3; }
    CScriptThing(const CScriptThing& o) : id(o.id), Info(o.Info), pos(o.pos) { if (Info) ++Info->RefCount; }
    ~CScriptThing() { if (Info) --Info->RefCount; }
    const C3DVector& GetPos() const { return pos; }
};
struct CGameScriptInterface {
    CScriptThing hero; int allies; C3DVector centre; float minD, maxD; int group; bool info;
    CGameScriptInterface() : hero(1, 0), allies(0), minD(-1), maxD(-1), group(0), info(false) { }
    CScriptThing& GetHero() { return hero; }
    void EntitySetThingAsAllyOfThing(const CScriptThing& a, const CScriptThing& b) const { const_cast<CGameScriptInterface*>(this)->allies += a.id * 10 + b.id; }
    void SetWanderCentrePoint(CScriptThing, C3DVector c) { centre = c; }
    void SetWanderMinDistance(CScriptThing, float d) { minD = d; }
    void SetWanderMaxDistance(CScriptThing, float d) { maxD = d; }
    void SetScriptingStateGroup(CScriptThing, int g) { group = g; }
    void SetThingHasInformation(const CScriptThing&, bool i, bool, bool) const { const_cast<CGameScriptInterface*>(this)->info = i; }
};
struct CScriptDef { float AmbushScamTricksterWanderMax; };
static CScriptDef g_def = { 12.5f };
static CScriptDef* g_pScriptDef = &g_def;
struct CAS_Trickster {
    CGameScriptInterface* Game; CScriptThing Me; long lastRandomSpeechIdx1, lastRandomSpeechIdx2;
    CAS_Trickster(CGameScriptInterface* g, SCountInfo* c) : Game(g), Me(2, c), lastRandomSpeechIdx1(0), lastRandomSpeechIdx2(0) { }
    void Init()
    {
        Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
        Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
        Game->SetWanderCentrePoint(Me, Me.GetPos());
        Game->SetWanderMinDistance(Me, 0.0f);
        Game->SetWanderMaxDistance(Me, g_pScriptDef->AmbushScamTricksterWanderMax);
        Game->SetScriptingStateGroup(Me, 4);
        lastRandomSpeechIdx1 = 5;
        lastRandomSpeechIdx2 = 5;
        Game->SetThingHasInformation(Me, true, false, false);
    }
};
int main()
{
    SCountInfo c = { 1 };
    CGameScriptInterface g;
    CAS_Trickster t(&g, &c);
    t.Init();
    if (g.allies != 21 + 12 || g.centre.Y != 2 || g.minD != 0.0f || g.maxD != 12.5f || g.group != 4 || !g.info
        || t.lastRandomSpeechIdx1 != 5 || t.lastRandomSpeechIdx2 != 5 || c.RefCount != 1) { std::printf("00e2bd80_TEST FAIL\n"); return 1; }
    std::printf("00e2bd80_TEST PASS\n");
    return 0;
}