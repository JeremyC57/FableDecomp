// Behaviour test for CAIStateGroup_FireflyWander::Update @ 0x00911F80: day hides, night picks home/hero target and toggles.
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Get2DLengthSquared() const { return Y * Y + X * X; }
};
class CTCLight { public:
    bool active; int calls;
    CTCLight() : active(false), calls(0) { }
    void SetActive(bool a) { active = a; ++calls; }
};
class CThing { public:
    C3DVector pos; unsigned char m_flags91; bool hasLight; CTCLight light;
    CThing() : m_flags91(0), hasLight(true) { }
    const C3DVector& GetPosition() const { return pos; }
    bool HasTC(long) const { return hasLight; }
    CTCLight* GetTC(long) { return &light; }
    template <class T> bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) { *ppOut = (T*)&light; return true; }
        return false;
    }
};
class CThingAICreature : public CThing { public:
    bool shown; C3DVector InitialPos;
    CThingAICreature() : shown(false) { }
    bool IsShown() { return shown; }
    void SetShown(bool s) { shown = s; }
};
class CPlayer { public: CThing* hero; CThing* GetHeroCreature() { return hero; } };
class CPlayerManager { public: CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
class CEnvironment { public: bool day; bool IsDaytime() { return day; } };
class CWorld { public: CEnvironment* PEnvironment; };
struct CAIStateGroup_FireflyWander {
    CThingAICreature* creature; CPlayerManager* players; CWorld* world; unsigned long seed;
    bool terminated; bool flightOk; C3DVector flightCentre; C3DVector executed; int executions;
    C3DVector StartPos;
    unsigned long randomValue;
    CThingAICreature* GetCreature() const { return creature; }
    CPlayerManager* GetPPlayerManager() const { return players; }
    CWorld* GetPWorld() const { return world; }
    void Terminate() { terminated = true; }
    unsigned long Random(unsigned long range) const { return randomValue % range; }
    bool GetRandomFlightPos(const C3DVector& centre, float range, C3DVector& out, bool) const
    {
        ((CAIStateGroup_FireflyWander*)this)->flightCentre = centre;
        out = C3DVector(centre.X + range, centre.Y, centre.Z);
        return flightOk;
    }
    void ExecuteFlyingPath(const C3DVector& d) { executed = d; ++executions; }
    void Update();
};

void CAIStateGroup_FireflyWander::Update()
{
    if (!GetPWorld()->PEnvironment->IsDaytime()) {
        C3DVector target = StartPos;
        CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetHeroCreature();
        if (hero && !(hero->m_flags91 & 1)) {
            CThingAICreature* me = GetCreature();
            C3DVector toHero = me->InitialPos - hero->GetPosition();
            CThingAICreature* self = GetCreature();
            C3DVector toHome = self->InitialPos - self->GetPosition();
            if (toHome.Get2DLengthSquared() > 196.0f)
                target = StartPos;
            else if (toHero.Get2DLengthSquared() < 100.0f)
                target = hero->GetPosition();
        }
        C3DVector destination;
        if (!GetRandomFlightPos(target, 6.0f, destination, true)) {
            Terminate();
            return;
        }
        if (!GetCreature()->IsShown() && Random(100) <= 45) {
            GetCreature()->SetShown(!GetCreature()->IsShown());
            CThingAICreature* lit = GetCreature();
            if (lit->HasTC(0x27))
                ((CTCLight*)lit->GetTC(0x27))->SetActive(lit->IsShown());
        }
        ExecuteFlyingPath(destination);
    } else if (GetCreature()->IsShown()) {
        GetCreature()->SetShown(false);
        CThingAICreature* creature = GetCreature();
        CTCLight* light;
        if (creature->GetInterface(&light, 0x27))
            light->SetActive(false);
    }
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++failures; } }

int main()
{
    CThingAICreature fly; fly.InitialPos = C3DVector(0, 0, 0); fly.pos = C3DVector(1, 1, 0);
    CThing hero; hero.pos = C3DVector(3, 4, 1);
    CPlayerManager players; players.player.hero = &hero;
    CEnvironment env; env.day = true;
    CWorld world; world.PEnvironment = &env;
    CAIStateGroup_FireflyWander g;
    g.creature = &fly; g.players = &players; g.world = &world; g.terminated = false; g.flightOk = true;
    g.executions = 0; g.StartPos = C3DVector(50, 60, 2); g.randomValue = 10;

    // Day, shown: hidden and light off.
    fly.shown = true; fly.light.active = true;
    g.Update();
    Check(!fly.shown && !fly.light.active && fly.light.calls == 1, "day hides");
    g.Update();
    Check(fly.light.calls == 1 && g.executions == 0, "day hidden does nothing");

    // Night, near home, hero within 10 of home: target hero, random 10 <= 45 shows and lights.
    env.day = false;
    g.Update();
    Check(g.flightCentre.X == 3 && g.flightCentre.Y == 4, "hero target");
    Check(g.executions == 1 && g.executed.X == 9, "executed flight");
    Check(fly.shown && fly.light.active, "shown and lit");

    // Hero far from home: target start pos.
    hero.pos = C3DVector(30, 0, 0);
    g.Update();
    Check(g.flightCentre.X == 50 && g.flightCentre.Y == 60, "start target when hero far");

    // Firefly far from home: start pos even if hero near.
    hero.pos = C3DVector(1, 1, 0); fly.pos = C3DVector(20, 0, 0);
    g.Update();
    Check(g.flightCentre.X == 50, "start target when far from home");

    // Dead hero: start pos.
    fly.pos = C3DVector(1, 1, 0); hero.m_flags91 = 1;
    g.Update();
    Check(g.flightCentre.X == 50, "dead hero ignored");
    hero.m_flags91 = 0;

    // Hidden and random 90 > 45: stays hidden.
    fly.shown = false; g.randomValue = 90;
    g.Update();
    Check(!fly.shown, "no toggle on high roll");

    // Flight failure terminates.
    g.flightOk = false;
    int before = g.executions;
    g.Update();
    Check(g.terminated && g.executions == before, "terminate on no flight pos");

    if (failures == 0)
        std::printf("00911f80_TEST PASS\n");
    return failures;
}