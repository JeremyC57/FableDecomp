// Behaviour test for 0x006299C0 CWorld::EAExpressionWait: mock interfaces log every call; the body is the source's, verbatim.
#include <cstdio>
#include <cstring>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int Check(const char* expect) { int ok = std::strcmp(g_log, expect) == 0; if (!ok) std::printf("got  %s\nwant %s\n", g_log, expect); g_log[0] = 0; return ok; }

class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
class CThingPlayerCreature;
class CCreatureAction_ActivateQuest {
public:
    char Desc[96];
    CCreatureAction_ActivateQuest(CThingPlayerCreature& c, void* obj, const CCharString& s, bool load);
};
class CThingPlayerCreature {
public:
    int Id; bool Dead;
    bool IsDead() const { return Dead; }
    void SetCurrentAction(const CCreatureAction_ActivateQuest& a) { char b[128]; std::sprintf(b, "SetAction(%d,%s)", Id, a.Desc); Log(b); }
};
CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(CThingPlayerCreature& c, void* obj, const CCharString& s, bool load)
{
    std::sprintf(Desc, "%d:%s:%d:%d", c.Id, s.Text, obj == 0, (int)load);
}
class CGameEvent { public: long Type; signed char Player; };
class CPlayer { public: CThingPlayerCreature* Controlled; CThingPlayerCreature* GetPControlledCreature() const { return Controlled; } };
class CWorld {
public:
    CPlayer Players[4];
    void EAExpressionWait(const CGameEvent* event);
    CPlayer* GetPlayer(unsigned long number) { char b[32]; std::sprintf(b, "GetPlayer(%lu)", number); Log(b); return &Players[number]; }
};
void CWorld::EAExpressionWait(const CGameEvent* event)
{
    CThingPlayerCreature* creature = GetPlayer(event->Player)->GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CCharString scriptName("Expression_Wait");
        creature->SetCurrentAction(CCreatureAction_ActivateQuest(*creature, 0, scriptName, false));
    }
}
int main()
{
    CWorld w; CThingPlayerCreature hero; hero.Id = 7; hero.Dead = false;
    for (int i = 0; i < 4; ++i) w.Players[i].Controlled = 0;
    w.Players[2].Controlled = &hero;
    CGameEvent e; e.Type = 0; e.Player = 2;
    int ok = 1;
    w.EAExpressionWait(&e); ok &= Check("GetPlayer(2);SetAction(7,7:Expression_Wait:1:0);");
    hero.Dead = true; w.EAExpressionWait(&e); ok &= Check("GetPlayer(2);");
    e.Player = 1; w.EAExpressionWait(&e); ok &= Check("GetPlayer(1);");
    if (ok) std::printf("006299c0_TEST PASS\n");
    return ok ? 0 : 1;
}