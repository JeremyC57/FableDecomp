// Behaviour test for CAIStateGroup_GoToPub::SitIdle (0x00947CA0).
#include <cstdio>
#include <cstring>

// Stand-ins: the action records the animation name it was built with; PerformAction returns a scripted
// EAIActionReturn; destructors are counted so both temporaries are seen to die.
static char g_lastAnim[64];
static int g_actionDtors = 0, g_stringDtors = 0, g_performed = 0;
static long g_performResult = 2, g_doNothingResult = 4;

class CThingCreatureBase { public: int dummy; };
class CCharString { public:
    const char* m_pData;
    CCharString(const char* text, long) : m_pData(text) {}
    ~CCharString() { ++g_stringDtors; }
};
class CSoundPair { public:
    long m_first, m_second;
    CSoundPair() : m_first(0), m_second(0) {}
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase() { ++g_actionDtors; }
};
static CThingCreatureBase* g_actionCreature = 0;
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool, bool b, bool, long, long)
    {
        g_actionCreature = creature;
        std::strcpy(g_lastAnim, animation.m_pData);
        if (sound.m_first != 0 || sound.m_second != 0 || !b) g_lastAnim[0] = '?';
    }
};
// The retail source passes &TemporaryAction (VC extension); Tmp() gives the same lifetime under g++.
template<class T> T* Tmp(const T& t) { return const_cast<T*>(&t); }
class CAIBrain { public: CThingCreatureBase* PCreature; };
class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_DoNothing() { return g_doNothingResult; }
    long AIAction_PerformAction(CCreatureActionBase*) { ++g_performed; return g_performResult; }
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    bool HandsOffMode;
    bool SitIdle()
    {
        if (HandsOffMode)
            return AIAction_DoNothing() == 4;
        CCharString animation("SIT_AT_TABLE_LOOP", -1);
        return AIAction_PerformAction(Tmp(CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
            false, true, false, 0, 0))) == 2;
    }
};

int main()
{
    CThingCreatureBase creature; CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_GoToPub g; g.PBrain = &brain; g.HandsOffMode = true;
    bool ok = g.SitIdle() == true && g_performed == 0;
    g_doNothingResult = 3; ok = ok && g.SitIdle() == false;
    g.HandsOffMode = false;
    ok = ok && g.SitIdle() == true && std::strcmp(g_lastAnim, "SIT_AT_TABLE_LOOP") == 0 && g_actionCreature == &creature
        && g_actionDtors == 1 && g_stringDtors == 1;
    g_performResult = 1; ok = ok && g.SitIdle() == false && g_actionDtors == 2 && g_stringDtors == 2;
    if (!ok) { std::printf("00947ca0_TEST FAIL\n"); return 1; }
    std::printf("00947ca0_TEST PASS\n");
    return 0;
}