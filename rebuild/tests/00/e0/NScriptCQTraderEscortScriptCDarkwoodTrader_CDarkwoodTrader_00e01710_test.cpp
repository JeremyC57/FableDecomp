// Behaviour test for 0x00E01710 CQ_TraderEscortScript::CDarkwoodTrader::~CDarkwoodTrader (compiler-generated): the three script
// timers hand their indices back to the game-script interface in reverse member order, then the entity-script base part.
#include <cstdio>
static long g_released[8];
static int g_n = 0;
static int g_baseDone = 0;

class CGameScriptInterfaceBase { public: virtual void DeregisterTimer(long index) { if (g_n < 8) g_released[g_n++] = index; } };
static CGameScriptInterfaceBase g_game;
static CGameScriptInterfaceBase* pDSTGame = &g_game;
class CTimer { public: long TimerIndex; ~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); } };
class CEntityScriptBase { public: virtual ~CEntityScriptBase() { g_baseDone = g_n; } void* Game; long Me[3]; };
struct CDarkwoodTrader : public CEntityScriptBase {
    void* ParentClass; void* PMasterData;
    long Infected, BrainState, CurrentAIState, PreviousAIState;
    CTimer IncubationTime;
    long TraderHealthID;
    bool GreetedBuddy, LeadToCamp, InSafeZone;
    CTimer RegulateFollowStateComment;
    CTimer RegulateBanterComment;
};

int main()
{
    CDarkwoodTrader* t = new CDarkwoodTrader;
    t->IncubationTime.TimerIndex = 11; t->RegulateFollowStateComment.TimerIndex = 22; t->RegulateBanterComment.TimerIndex = 33;
    delete (CEntityScriptBase*)t;
    if (g_n != 3 || g_released[0] != 33 || g_released[1] != 22 || g_released[2] != 11 || g_baseDone != 3) {
        std::printf("00e01710_TEST FAIL\n"); return 1;
    }
    std::printf("00e01710_TEST PASS\n");
    return 0;
}