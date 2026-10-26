// Behaviour test for CAIStateGroup_BalverineWander::Update @ 0x0091BD10: wander/return target, animation pick, loop exit.
#include <cstdio>
#include <cstring>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
};
class CCharString { public:
    const char* text;
    CCharString(const char* t, long = -1) : text(t) { }
};
class CSoundPair { public:
    long m_first, m_second;
    CSoundPair() : m_first(0), m_second(0) { }
    CSoundPair(long a, long b) : m_first(a), m_second(b) { }
};
class CTCCreatureNavigation { public:
    bool ok; C3DVector wander;
    bool GetNearbyWanderPosition(const C3DVector&, float, C3DVector* out, bool) { if (ok) *out = wander; return ok; }
};
class CThingCreatureBase { public:
    C3DVector pos; bool hasNav; CTCCreatureNavigation nav;
    const C3DVector& GetPosition() const { return pos; }
    CSoundPair GetSoundPair(const CCharString& name) { return CSoundPair(std::strcmp(name.text, "HOWL") == 0 ? 7 : 1, 0); }
    template<class T> void GetInterfaceR(T** ppOut, long, bool& has) { has = hasNav; if (has) *ppOut = &nav; }
};
class CCreatureActionBase { public: const char* anim; long sound; };
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayAnimation(CThingCreatureBase*, const CCharString& name, const CSoundPair& s, bool, bool, bool, long, long)
    { anim = name.text; sound = s.m_first; }
};
template <class T> T* TempAddress(const T& t) { return const_cast<T*>(&t); }   // g++ has no &temporary extension
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_WALK = 3 };
class CThing;
struct CAIStateGroup_BalverineWander {
    CThingCreatureBase* creature; unsigned long rolls[8]; int roll; int loopsLeft; bool moveOk; bool terminated;
    C3DVector moves[8]; int moveCount; const char* anims[8]; long sounds[8]; int animCount;
    CThingCreatureBase* GetCreature() const { return creature; }
    void Terminate() { terminated = true; }
    unsigned long Random(unsigned long range) { return rolls[roll++] % range; }
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat, C3DVector pos, float, bool& moving, EMovementType, bool, bool, bool,
                                                const CThing*, bool, float)
    { moving = true; moves[moveCount++] = pos; creature->pos = pos; return moveOk ? AI_ACTION_DONE : AI_ACTION_ABORTED; }
    EAIActionReturn AIAction_PerformAction(CCreatureActionBase* a) { anims[animCount] = a->anim; sounds[animCount++] = a->sound; return AI_ACTION_DONE; }
    EAIActionReturn AIAction_DoNothing() { return --loopsLeft > 0 ? AI_ACTION_RUNNING : AI_ACTION_DONE; }
    void Update();
};

void CAIStateGroup_BalverineWander::Update()
{
    C3DVector startPos = GetCreature()->GetPosition();
    CTCCreatureNavigation* nav;
    do {
        C3DVector dest;
        C3DVector offset = GetCreature()->GetPosition() - startPos;
        float distanceSquared = offset.GetLengthSquared();
        if (!(distanceSquared < 100.0f))
            dest = startPos;
        else {
            CThingCreatureBase* creature = GetCreature();
            bool found;
            creature->GetInterfaceR(&nav, 7, found);
            if (!nav->GetNearbyWanderPosition(creature->GetPosition(), 4.0f, &dest, true))
                dest = startPos;
        }
        bool moving;
        if (AIAction_MoveToPosProximity(AI_ACTION_REPEAT_UNTIL_DONE, dest, 1.5f, moving, MOVEMENT_WALK, true, false, true, 0,
                                        true, 0.0f) != AI_ACTION_DONE) {
            Terminate();
            return;
        }
        if (Random(100) < 20) {
            AIAction_PerformAction(TempAddress(CCreatureAction_PlayAnimation(GetCreature(), "HOWL",
                                   GetCreature()->GetSoundPair("HOWL"), false, true, false, 0, 0)));
        } else
            AIAction_PerformAction(TempAddress(CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE", CSoundPair(), false,
                                   true, false, 0, 0)));
    } while (AIAction_DoNothing() == AI_ACTION_RUNNING);
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++failures; } }

int main()
{
    CThingCreatureBase c; c.pos = C3DVector(1, 2, 0); c.hasNav = true; c.nav.ok = true; c.nav.wander = C3DVector(4, 2, 0);
    CAIStateGroup_BalverineWander g;
    g.creature = &c; g.roll = 0; g.loopsLeft = 3; g.moveOk = true; g.terminated = false; g.moveCount = 0; g.animCount = 0;
    g.rolls[0] = 5; g.rolls[1] = 250; g.rolls[2] = 119;
    g.Update();
    Check(g.moveCount == 3 && g.animCount == 3 && !g.terminated, "three loops");
    Check(g.moves[0].X == 4 && g.moves[0].Y == 2, "wander target");
    Check(std::strcmp(g.anims[0], "HOWL") == 0 && g.sounds[0] == 7, "5 rolls HOWL with sound");
    Check(std::strcmp(g.anims[1], "ST_IDLE") == 0 && g.sounds[1] == 0, "50 rolls idle");
    Check(std::strcmp(g.anims[2], "HOWL") == 0, "19 rolls HOWL");

    // Far from start: straight back to start.
    c.pos = C3DVector(1, 2, 0); c.nav.wander = C3DVector(20, 2, 0);
    g.roll = 0; g.loopsLeft = 2; g.moveCount = 0; g.animCount = 0;
    g.Update();
    Check(g.moves[0].X == 20 && g.moves[1].X == 1 && g.moves[1].Y == 2, "returns to start when 10+ away");

    // Wander fails: start position.
    c.pos = C3DVector(5, 5, 0); c.nav.ok = false;
    g.roll = 0; g.loopsLeft = 1; g.moveCount = 0; g.animCount = 0;
    g.Update();
    Check(g.moveCount == 1 && g.moves[0].X == 5 && g.moves[0].Y == 5, "wander failure falls back to start");

    // Move failure terminates without animation.
    g.moveOk = false; g.roll = 0; g.loopsLeft = 5; g.moveCount = 0; g.animCount = 0;
    g.Update();
    Check(g.terminated && g.animCount == 0 && g.moveCount == 1, "move failure terminates");

    if (failures == 0)
        std::printf("0091bd10_TEST PASS\n");
    return failures;
}