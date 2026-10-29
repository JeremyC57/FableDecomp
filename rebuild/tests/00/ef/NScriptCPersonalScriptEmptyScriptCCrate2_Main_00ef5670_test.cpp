// Behaviour test for 0x00ef5670 NScript::CPersonalScript_EmptyScript::CCrate2::Main: the marker is looked up by script name, the
// thread predicate is "Me alive && marker alive", the crate waits (one frame, then each frame) until it is within 2.0 of
// the marker, then a THING_EXPLOSION_BLASTBOX goes off at its position and the crate fades out and dies (FadeOutAndKillEntity(Me, 0.5f, true, true)); a terminated thread stops early.
#include <cstdio>
#include <cstring>

class C3DVector { public: float X, Y, Z; };
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
class CScriptThing { public: int Id; C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
static int g_frame, g_nearFrame, g_endFrame, g_explosions, g_removed, g_faded;
static float g_fadeTime; static bool g_fadeA, g_fadeB, g_removeA, g_removeB;
static const C3DVector* g_explodePos; static char g_lookup[32]; static char g_explDef[32]; static char g_explName[8];
static const void* g_dirArg = (const void*)1;
static int g_pred1, g_pred2;
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d)
{ return a.Id == 1 && b.Id == 2 && d == 2.0f && g_frame >= g_nearFrame; }
static void FadeOutAndKillEntity(const CScriptThing& t, float time, bool a, bool b)
{ ++g_faded; g_fadeTime = time; g_fadeA = a; g_fadeB = b; (void)t; }
class CGame { public:
    void NewScriptFrame() const { ++g_frame; }
    CScriptThing GetThingWithScriptName(const CCharString& n) const
    { std::strcpy(g_lookup, n.Text); CScriptThing t; t.Id = 2; t.Pos.X = t.Pos.Y = t.Pos.Z = 0; return t; }
    CScriptThing CreateExplosion(const CCharString& def, const C3DVector& pos, const C3DVector* dir, CCharString name) const
    { ++g_explosions; std::strcpy(g_explDef, def.Text); g_explodePos = &pos; g_dirArg = dir; std::strcpy(g_explName, name.Text); return CScriptThing(); }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { ++g_removed; g_removeA = a; g_removeB = b; (void)t; }
};
struct CIsThingAlive { int Id; CIsThingAlive(const CScriptThing& t) : Id(t.Id) { } };
struct CPredicate_And { int A, B; };
static CPredicate_And operator&&(const CIsThingAlive& a, const CIsThingAlive& b) { CPredicate_And p; p.A = a.Id; p.B = b.Id; return p; }
struct CCrate {
    CGame* Game; CScriptThing Me;
    void SetPredicate(const CPredicate_And& p) { g_pred1 = p.A; g_pred2 = p.B; }
    bool IsActiveThreadTerminating() const { return g_frame >= g_endFrame; }
    void Main()
    {
        CScriptThing marker = Game->GetThingWithScriptName("CrateDeathMarker");
        SetPredicate(CIsThingAlive(Me) && CIsThingAlive(marker));
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!IsDistanceBetweenThingsUnder(Me, marker, 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->CreateExplosion("THING_EXPLOSION_BLASTBOX", Me.GetPos(), 0, "");
        FadeOutAndKillEntity(Me, 0.5f, true, true);
    }
};

int main()
{
    CGame g; CCrate c; c.Game = &g; c.Me.Id = 1; c.Me.Pos.X = 3; c.Me.Pos.Y = 4; c.Me.Pos.Z = 5;
    g_frame = 0; g_nearFrame = 4; g_endFrame = 100;
    c.Main();
    int ok = !std::strcmp(g_lookup, "CrateDeathMarker") && g_pred1 == 1 && g_pred2 == 2 && g_frame == 4
        && g_explosions == 1 && !std::strcmp(g_explDef, "THING_EXPLOSION_BLASTBOX") && g_explodePos == &c.Me.Pos
        && g_dirArg == 0 && !std::strcmp(g_explName, "") && g_faded == 1 && g_fadeTime == 0.5f && g_fadeA && g_fadeB && g_removed == 0;
    g_frame = 0; g_nearFrame = 50; g_endFrame = 3; g_explosions = 0;
    c.Main();
    ok = ok && g_explosions == 0 && g_frame == 3;
    g_frame = 0; g_endFrame = 1;
    c.Main();
    ok = ok && g_explosions == 0 && g_frame == 1;
    if (!ok) { std::printf("00ef5670_TEST FAIL\n"); return 1; }
    std::printf("00ef5670_TEST PASS\n");
    return 0;
}