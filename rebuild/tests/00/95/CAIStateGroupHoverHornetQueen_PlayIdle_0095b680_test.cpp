// Behaviour test for CAIStateGroup_HoverHornetQueen::PlayIdle (0x0095B680): the SPECIAL_BOAST combat
// animation is performed and waited on (wait flag false), both temporaries die before the runnable check,
// and the group terminates only when it is no longer runnable.
#include <cstdio>
#include <cstring>

static int g_stringDtors = 0, g_actionDtors = 0, g_terminated = 0, g_waits = 0;
static bool g_runnable = true, g_okArgs = false, g_orderOk = true;

struct CThingCreatureBase { int dummy; };
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long) : m_pData(text) {}
    ~CCharString() { ++g_stringDtors; }
};
struct CCreatureActionBase { virtual ~CCreatureActionBase() {} };
static CThingCreatureBase* g_creature = 0;
struct CCreatureAction_PlayCombatAnimation : CCreatureActionBase {
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop)
    {
        g_okArgs = creature == g_creature && std::strcmp(animation.m_pData, "SPECIAL_BOAST") == 0 && loop;
    }
    ~CCreatureAction_PlayCombatAnimation() { ++g_actionDtors; }
};
template<class T> T* Tmp(const T& t) { return const_cast<T*>(&t); }
struct CAIBrain { CThingCreatureBase* PCreature; };
struct CAIStateGroupBase {
    CAIBrain* PBrain;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase*, bool wait) { if (!wait) ++g_waits; return 2; }
    void Terminate() { ++g_terminated; }
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    bool IsStillRunnable() { if (g_stringDtors == 0 || g_actionDtors == 0) g_orderOk = false; return g_runnable; }
    void PlayIdle()
    {
        {
            CCharString animation("SPECIAL_BOAST", -1);
            AIAction_PerformActionAndWait(Tmp(CCreatureAction_PlayCombatAnimation(GetCreature(), animation, true)), false);
        }
        if (!IsStillRunnable())
            Terminate();
    }
};

int main()
{
    CThingCreatureBase c; g_creature = &c;
    CAIBrain brain; brain.PCreature = &c;
    CAIStateGroup_HoverHornetQueen g; g.PBrain = &brain;
    g.PlayIdle();
    bool ok = g_okArgs && g_waits == 1 && g_terminated == 0 && g_orderOk;
    g_runnable = false; g.PlayIdle();
    ok = ok && g_terminated == 1 && g_waits == 2 && g_stringDtors == 2 && g_actionDtors == 2;
    if (!ok) { std::printf("0095b680_TEST FAIL\n"); return 1; }
    std::printf("0095b680_TEST PASS\n");
    return 0;
}