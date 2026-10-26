// Behaviour test for CAIStateGroup_GoToPub::Laugh (0x00947D30).
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
    bool Laugh()
    {
        CCharString animation("SEATED_TAVERN_LAUGH", -1);
        bool failed = AIAction_PerformAction(Tmp(CCreatureAction_PlayAnimation(GetCreature(), animation,
            CSoundPair(), false, true, false, 0, 0))) != 2;
        return !failed;
    }
};

int main()
{
    CThingCreatureBase creature; CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_GoToPub g; g.PBrain = &brain;
    bool ok = g.Laugh() == true && std::strcmp(g_lastAnim, "SEATED_TAVERN_LAUGH") == 0 && g_actionCreature == &creature
        && g_actionDtors == 1 && g_stringDtors == 1;
    g_performResult = 0; ok = ok && g.Laugh() == false && g_performed == 2;
    if (!ok) { std::printf("00947d30_TEST FAIL\n"); return 1; }
    std::printf("00947d30_TEST PASS\n");
    return 0;
}