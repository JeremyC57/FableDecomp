// Behaviour test for CTCVillage::CTCVillage (0x0065B400): runs the reconstructed ctor over a 0xCD-filled buffer with
// recording stubs for the out-of-line base/list/set/pointer ctors and InitialiseSecuritySweeps.
#include <cstdio>
#include <cstring>
#include <new>
static int g_calls = 0;
static int g_listCalls = 0;
static int g_ipCalls = 0;
static int g_sweepOrder = -1;
static void* g_parentSeen = 0;
class CThing;
class CTCBase { public:
    virtual void v00() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* parent) : ParentThing(parent), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { g_parentSeen = parent; ++g_calls; }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCalls; ++g_calls; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    virtual void v01() {}
    CIntelligentPointerCreature() {}
};
struct CEmptyAllocator { CEmptyAllocator() {} };
#define LISTTYPE(N) struct N { void* _M_node; N(const CEmptyAllocator& a = CEmptyAllocator()) : _M_node(this) { ++g_listCalls; ++g_calls; } };
LISTTYPE(CBuildingList) LISTTYPE(CCreatureList) LISTTYPE(CThingList) LISTTYPE(CObjectList) LISTTYPE(CTrackNodeList)
LISTTYPE(CTorchTaskList) LISTTYPE(CHornTaskList) LISTTYPE(CPatrolTaskList) LISTTYPE(CCropTaskList) LISTTYPE(CSweepTaskList)
LISTTYPE(CCrateTaskList) LISTTYPE(CMarkerList)
struct CLongSet { void* _M_header; long _M_node_count; long _M_key_compare; CLongSet() : _M_header(this), _M_node_count(0), _M_key_compare(0) { ++g_calls; } };
class CVillageDef;
struct CVillageDefPointer { const CVillageDef* Object; CVillageDefPointer() : Object(0) {} };
class C3DVector { public: float X, Y, Z; };
class CTCVillage : public CTCBase { public:
    long TorchTasksUpdateFrame; long FrameStartedSecuritySweep; C3DVector PositionPlayerWasInWhenSecuritySweepCalculated;
    long LastAweCrowdEventFrame; long LastLynchCrowdEventFrame;
    CBuildingList VillageBuildings; CCreatureList VillagePeople; CThingList VillageTorches; CThingList VillageHorns;
    CThingList VillageGates; CObjectList VillageCrops; CThingList VillageCages; CThingList VillageCampfires;
    CTrackNodeList VillagePatrols; CTorchTaskList TorchTasks; CHornTaskList HornTasks; CPatrolTaskList PatrolTasks;
    CCropTaskList CropTasks; CSweepTaskList SecuritySweepTasks; CCrateTaskList CrateTasks; CLongSet PlayersCrimes;
    CMarkerList VillageMarkers; CThingList VillageDummies;
    bool Initialised; bool InitiallyPopulated; bool HasBeenInitiallyPopulated; bool HornTasksAdded; bool IsEnemyBecauseOfCrime;
    long FramePlayerLastSeenByGuard; bool InCurfew; long NumGuards; long HouseCurrentlyChecking; bool TimeIsPaused;
    CVillageDefPointer PDef; long StanceToHero; long TimeStopIndex; long AmountOfFineAlreadyPaid; long NextGuardRespawn;
    long FrameBribeEnds; long FrameForNextGuardGreeting; bool WorkTimeAccordingToCrier; bool CurfewTimeAccordingToCrier;
    bool Limbo; bool LaggyLimbo; bool SecuritySweepUrgent; float InitialSecuritySweepRadius; bool TeacherTeaching;
    CIntelligentPointerCreature GuardFollowingPlayer; long NextFrameToFollowPlayer; bool LightActiveStateToChange;
    long MaxFrameToAssignTask; long HeroEnemyOfGuardsUntil; long LastFrameApologised;
    CIntelligentPointerCreature GuardWarningHero; bool CurrentIsHeroCriminal;
    virtual void v01() {}
    CTCVillage(CThing* parent);
    void InitialiseSecuritySweeps() { g_sweepOrder = g_calls; FrameStartedSecuritySweep = 0; InitialSecuritySweepRadius = 20.0f; }
};
CTCVillage::CTCVillage(CThing* parent)
    : CTCBase(parent), TorchTasksUpdateFrame(0), LastAweCrowdEventFrame(0), LastLynchCrowdEventFrame(0),
      Initialised(false), InitiallyPopulated(false), HasBeenInitiallyPopulated(false), IsEnemyBecauseOfCrime(false),
      FramePlayerLastSeenByGuard(0), NumGuards(0), HouseCurrentlyChecking(0), TimeIsPaused(false), TimeStopIndex(0),
      AmountOfFineAlreadyPaid(0), FrameBribeEnds(0), FrameForNextGuardGreeting(0), Limbo(false), LaggyLimbo(false),
      TeacherTeaching(false), NextFrameToFollowPlayer(0), HeroEnemyOfGuardsUntil(0), LastFrameApologised(0),
      CurrentIsHeroCriminal(false)
{
    InitialiseSecuritySweeps();
}
static double g_storage[256];
int main()
{
    std::memset(g_storage, 0xCD, sizeof(g_storage));
    int dummy;
    CThing* parent = (CThing*)&dummy;
    CTCVillage* v = new (g_storage) CTCVillage(parent);
    bool ok = g_parentSeen == (void*)parent && v->ParentThing == parent
        && g_listCalls == 17 && g_ipCalls == 2 && g_calls == 1 + 17 + 1 + 2 && g_sweepOrder == g_calls
        && v->TorchTasksUpdateFrame == 0 && v->LastAweCrowdEventFrame == 0 && v->LastLynchCrowdEventFrame == 0
        && !v->Initialised && !v->InitiallyPopulated && !v->HasBeenInitiallyPopulated && !v->IsEnemyBecauseOfCrime
        && v->FramePlayerLastSeenByGuard == 0 && v->NumGuards == 0 && v->HouseCurrentlyChecking == 0 && !v->TimeIsPaused
        && v->PDef.Object == 0 && v->TimeStopIndex == 0 && v->AmountOfFineAlreadyPaid == 0 && v->FrameBribeEnds == 0
        && v->FrameForNextGuardGreeting == 0 && !v->Limbo && !v->LaggyLimbo && !v->TeacherTeaching
        && v->NextFrameToFollowPlayer == 0 && v->HeroEnemyOfGuardsUntil == 0 && v->LastFrameApologised == 0
        && !v->CurrentIsHeroCriminal && v->GuardFollowingPlayer.m_pItem == 0 && v->GuardWarningHero.m_pItem == 0
        && v->FrameStartedSecuritySweep == 0 && v->InitialSecuritySweepRadius == 20.0f
        && v->VillageDummies._M_node == &v->VillageDummies;
    // members the ctor leaves alone keep the fill pattern
    unsigned char raw; std::memcpy(&raw, &v->HornTasksAdded, 1);
    if (raw != 0xCD) ok = false;
    std::memcpy(&raw, &v->NextGuardRespawn, 1);
    if (raw != 0xCD) ok = false;
    if (!ok) { std::printf("0065b400_TEST FAIL\n"); return 1; }
    std::printf("0065b400_TEST PASS\n");
    return 0;
}