// Behaviour test for CTauntingBandit::Init (0x00E48EB0): clears the anger state, sets Me's faction to
// FACTION_BANDITS_FRIENDLY, centres Me's wander on the M_BanditCampCentrePos marker's position, sets min 0 and max
// = the script def's toll-bandit wander distance; the by-value CScriptThing copies balance their ref counts.
#include <cstdio>
#include <cstring>

static int g_live = 0;                       // outstanding CScriptThing copies of Me / the marker
struct C3DVector { float X, Y, Z; };
struct CCharString {
    const char* text;
    CCharString(const char* t, long = -1) : text(t) {}
};
struct CScriptThing {
    int id;
    C3DVector pos;
    CScriptThing(int i, float x, float y, float z) : id(i) { pos.X = x; pos.Y = y; pos.Z = z; ++g_live; }
    CScriptThing(const CScriptThing& o) : id(o.id), pos(o.pos) { ++g_live; }
    ~CScriptThing() { --g_live; }
    const C3DVector& GetPos() const { return pos; }
};

static bool g_faction = false, g_centre = false, g_min = false;
static float g_max = -1.0f;
struct CGameScriptInterface {
    CScriptThing GetThingWithScriptName(const CCharString& n) const
    {
        return std::strcmp(n.text, "M_BanditCampCentrePos") == 0 ? CScriptThing(9, 1.5f, 2.5f, -3.0f)
                                                                  : CScriptThing(0, 0, 0, 0);
    }
    void EntitySetInFaction(const CScriptThing& t, const CCharString& f) const
    { g_faction = t.id == 3 && std::strcmp(f.text, "FACTION_BANDITS_FRIENDLY") == 0; }
    void SetWanderCentrePoint(CScriptThing t, C3DVector p)
    { g_centre = t.id == 3 && p.X == 1.5f && p.Y == 2.5f && p.Z == -3.0f; }
    void SetWanderMinDistance(CScriptThing t, float d) { g_min = t.id == 3 && d == 0.0f; }
    void SetWanderMaxDistance(CScriptThing t, float d) { if (t.id == 3) g_max = d; }
};
struct CScriptDef { long BanditTollBanditWanderDistance; };
static CScriptDef g_def = { 25 };
static CScriptDef* g_pScriptDef = &g_def;

class CTauntingBandit {
public:
    CGameScriptInterface* Game;
    CScriptThing          Me;
    long                  AngerCount;
    bool                  AngryWithHero;
    CTauntingBandit() : Me(3, 0, 0, 0) {}
    void Init();
};

void CTauntingBandit::Init()
{
    AngerCount = 0;
    AngryWithHero = false;
    Game->EntitySetInFaction(Me, "FACTION_BANDITS_FRIENDLY");
    Game->SetWanderCentrePoint(Me, Game->GetThingWithScriptName("M_BanditCampCentrePos").GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, (float)g_pScriptDef->BanditTollBanditWanderDistance);
}

int main()
{
    int ok = 1;
    {
        CGameScriptInterface game;
        CTauntingBandit s; s.Game = &game; s.AngerCount = 4; s.AngryWithHero = true;
        s.Init();
        ok &= s.AngerCount == 0 && !s.AngryWithHero;
        ok &= g_faction && g_centre && g_min && g_max == 25.0f;
        ok &= g_live == 1;                   // only Me remains
    }
    ok &= g_live == 0;
    if (ok) std::printf("00e48eb0_TEST PASS\n"); else std::printf("00e48eb0_TEST FAIL\n");
    return ok ? 0 : 1;
}