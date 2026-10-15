#include <cstdio>
#include <cstring>

struct CCharString {
    char text[64];
    CCharString(const char* t, long) { std::strcpy(text, t); }
    ~CCharString() {}
};

struct CSoundPair { long a, b; CSoundPair() : a(0), b(0) {} };

struct CCreatureActionBase { char anim[64]; bool flags[3]; virtual ~CCreatureActionBase() {} };

struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    CCreatureAction_PlayAnimation(void*, const CCharString& animation, const CSoundPair& s, bool a, bool b, bool c, long, long)
    { std::strcpy(anim, animation.text); flags[0] = a; flags[1] = b; flags[2] = c; if (s.a || s.b) anim[0] = 0; }
};

struct CStateGroup {
    long nextResult; char lastAnim[64]; bool lastWait; int calls;
    void* GetCreature() const { return 0; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait)
    {
        std::strcpy(lastAnim, action->anim); lastWait = wait; ++calls;
        if (action->flags[0] || !action->flags[1] || action->flags[2]) return -1;
        return nextResult;
    }
};

struct CSitForMeal : CStateGroup {
    bool StandAroundAndEat()
    {
        long result;
        {
            CCharString animation("ST_IDLE", -1);
            CCreatureAction_PlayAnimation action(GetCreature(), animation, CSoundPair(), false, true, false, 0, 0);
            result = AIAction_PerformActionAndWait(&action, false);
        }
        return result == 2 ? true : false;
    }
};

static int fail() { std::printf("009454c0_TEST FAIL\n"); return 1; }

int main()
{
    CSitForMeal g; g.calls = 0; g.nextResult = 2; g.lastWait = true;
    if (!g.StandAroundAndEat() || std::strcmp(g.lastAnim, "ST_IDLE") != 0 || g.lastWait) return fail();
    g.nextResult = 1;
    if (g.StandAroundAndEat() || g.calls != 2) return fail();
    std::printf("009454c0_TEST PASS\n");
    return 0;
}
