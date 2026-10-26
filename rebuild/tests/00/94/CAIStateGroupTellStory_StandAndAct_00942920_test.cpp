#include <cstdio>
#include <cstring>

class CCharString { public:
    char text[64];
    CCharString(const char* t, long) { std::strcpy(text, t); }
    ~CCharString() {}
};

class CSoundPair { public: long a, b; CSoundPair() : a(0), b(0) {} };

class CCreatureActionBase { public: char anim[64]; bool flags[3]; virtual ~CCreatureActionBase() {} };

class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
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

long g_storyGestureIndex = 0;
const char* g_storyGestures[] = { "GESTURE_A", "GESTURE_B", "GESTURE_C", 0 };

struct CTellStory : CStateGroup {
    void StandAndAct(long)
    {
        if (g_storyGestures[++g_storyGestureIndex] == 0)
            g_storyGestureIndex = 0;
        CCharString animation(g_storyGestures[g_storyGestureIndex], -1);
        CCreatureAction_PlayAnimation action(GetCreature(), animation, CSoundPair(), false, true, false, 0, 0);
        AIAction_PerformActionAndWait(&action, false);
    }
};

static int fail() { std::printf("00942920_TEST FAIL\n"); return 1; }

int main()
{
    CTellStory g; g.calls = 0; g.nextResult = 2;
    g.StandAndAct(0); if (std::strcmp(g.lastAnim, "GESTURE_B") != 0) return fail();
    g.StandAndAct(0); if (std::strcmp(g.lastAnim, "GESTURE_C") != 0) return fail();
    g.StandAndAct(0); if (std::strcmp(g.lastAnim, "GESTURE_A") != 0 || g_storyGestureIndex != 0) return fail();
    g.StandAndAct(0); if (std::strcmp(g.lastAnim, "GESTURE_B") != 0 || g.calls != 4) return fail();
    std::printf("00942920_TEST PASS\n");
    return 0;
}