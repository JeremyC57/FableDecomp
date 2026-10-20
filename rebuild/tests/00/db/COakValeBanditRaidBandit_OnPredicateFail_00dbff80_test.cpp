// Behaviour test for 0x00DBFF80 COakValeBanditRaidBandit::OnPredicateFail: a killed bandit sets BanditKilled; unless the
// raid is over, Bandit becomes the thing nearest the hero named "OakValeBanditRaidBandit" (counted reference swapped)
// and BanditInRange is set when it is under 8 m from this bandit.
#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; };
struct CImp { long refs; C3DVector pos; };
struct CScriptThing {
    CImp* PImp;
    bool killed;
    CScriptThing() : PImp(0), killed(false) {}
    CScriptThing(CImp* p) : PImp(p), killed(false) { if (p) ++p->refs; }
    CScriptThing(const CScriptThing& o) : PImp(o.PImp), killed(o.killed) { if (PImp) ++PImp->refs; }
    ~CScriptThing() { if (PImp) --PImp->refs; }
    CScriptThing& operator=(const CScriptThing& o) { if (PImp != o.PImp) { if (PImp) --PImp->refs; PImp = o.PImp; if (PImp) ++PImp->refs; } return *this; }
    bool MsgIsKilledBy(const char* who) const { return killed && std::strcmp(who, "") == 0; }
    const C3DVector& GetPos() const { return PImp->pos; }
};
static bool IsDistanceFromThingToPositionUnder(const CScriptThing& t, const C3DVector& p, float d)
{
    float dx = t.PImp->pos.X - p.X, dy = t.PImp->pos.Y - p.Y, dz = t.PImp->pos.Z - p.Z;
    return dx * dx + dy * dy + dz * dz < d * d;
}
static CImp* g_nearest; static const char* g_lastName;
struct CGameScriptInterface {
    CScriptThing hero;
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const char* name) const { g_lastName = name; return CScriptThing(g_nearest); }
};
struct CRaid { bool RaidOver, BanditKilled, BanditInRange; CScriptThing Bandit; };
struct COakValeBanditRaidBandit {
    CGameScriptInterface* Game; CScriptThing Me; CRaid* ParentClass;
    void OnPredicateFail()
    {
        if (Me.MsgIsKilledBy("")) {
            ParentClass->BanditKilled = true;
            if (!ParentClass->RaidOver) {
                ParentClass->Bandit = Game->GetNearestWithScriptName(Game->GetHero(), "OakValeBanditRaidBandit");
                if (IsDistanceFromThingToPositionUnder(ParentClass->Bandit, Me.GetPos(), 8.0f))
                    ParentClass->BanditInRange = true;
            }
        }
    }
};
static int Fail(int n) { std::printf("00dbff80_TEST FAIL %d\n", n); return 1; }
int main()
{
    CImp meImp = { 0, { 0, 0, 0 } }, nearImp = { 0, { 3, 4, 0 } }, oldImp = { 0, { 50, 0, 0 } };
    CGameScriptInterface gsi;
    CRaid raid; raid.RaidOver = false; raid.BanditKilled = false; raid.BanditInRange = false; raid.Bandit = CScriptThing(&oldImp);
    COakValeBanditRaidBandit b; b.Game = &gsi; b.Me = CScriptThing(&meImp); b.ParentClass = &raid;
    g_nearest = &nearImp;
    b.OnPredicateFail();
    if (raid.BanditKilled || raid.Bandit.PImp != &oldImp) return Fail(1);      // not killed: nothing
    b.Me.killed = true; b.OnPredicateFail();
    if (!raid.BanditKilled || raid.Bandit.PImp != &nearImp || !raid.BanditInRange || std::strcmp(g_lastName, "OakValeBanditRaidBandit")) return Fail(2);
    if (oldImp.refs != 0 || nearImp.refs != 1) return Fail(3);
    raid.BanditInRange = false; nearImp.pos.X = 30; raid.Bandit = CScriptThing(&oldImp); b.OnPredicateFail();
    if (raid.Bandit.PImp != &nearImp || raid.BanditInRange) return Fail(4);        // too far
    raid.RaidOver = true; raid.Bandit = CScriptThing(&oldImp); b.OnPredicateFail();
    if (raid.Bandit.PImp != &oldImp) return Fail(5);                             // raid over: no re-target
    std::printf("00dbff80_TEST PASS\n");
    return 0;
}