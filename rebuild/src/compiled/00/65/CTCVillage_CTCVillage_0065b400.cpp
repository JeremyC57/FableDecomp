// CTCVillage::CTCVillage(CThing* parent) -- village component ctor: CTCBase(parent), cleared frame counters, seventeen empty retail 4-byte lists and the crime set, cleared state flags/frames, the two guard intelligent pointers, then InitialiseSecuritySweeps()
// Leaf: ??0CTCVillage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065B400, length 408 bytes (frame-pointer, size-optimised module: the
// neighbouring InitialiseSecuritySweeps 0x00659085 sits at an odd address).
// Vtables: CTCVillage 0x0125A9AC (rtti-vtables.tsv), CIntelligentPointer<CThingAICreature> 0x0123E7E0.
// Callees: CTCBase::CTCBase(CThing*) 0x00686800; per-element-type list<...>::list(const allocator&) 0x0065F1AD,
// 0x00512870, 0x004B8920 (list<CIntelligentPointer<CThing> >, x6), 0x0065F1DF, 0x0065F211, 0x0065F243, 0x0065F25E,
// 0x0065F279, 0x0065F294, 0x0065F2AF, 0x0065F2CA, 0x0065F2E5; set<long>::set() 0x005157F0;
// CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10 (x2); CTCVillage::InitialiseSecuritySweeps() 0x00659085.
// Layout: engine/CTCVillage.h carries Ego_r.pdb offsets with 12-byte lists and a 16-byte set; retail STLport lists are
// one sentinel pointer (4 bytes) and the set 12 bytes, so every member after VillageBuildings sits lower in retail:
// lists +0x28..+0x60, PlayersCrimes +0x64, VillageMarkers +0x70, VillageDummies +0x74, Initialised +0x78 (PDB +0x104,
// delta 0x8C from here on), GuardFollowingPlayer +0xBD (odd: pack(1) on the pointer classes, as in the PDB),
// GuardWarningHero +0xDC, CurrentIsHeroCriminal +0xE4 (retail sizeof 0xE8). Local model.
// Shape: `optimize("s")` with frame pointers (the CTCKraken ctor 0x004DEFA7 model): each list's default `allocator()`
// argument is an empty temporary homed in the top byte of the dead parent slot (`lea eax,[ebp+0Bh]`); init-list
// members in declaration order, ebx zero register; the intelligent pointer's inline derived ctor stores its vtable
// after the out-of-line base ctor.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing;
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* parent);                    // 0x00686800
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // vtable 0x0123E7E0
    virtual void v01();
    CIntelligentPointerCreature() {}
};
#pragma pack(pop)
struct CEmptyAllocator {                        // std::allocator<T> (empty)
    CEmptyAllocator() {}
};
struct CBuildingList {      void* _M_node; CBuildingList(const CEmptyAllocator& a = CEmptyAllocator());      };  // 0x0065F1AD
struct CCreatureList {      void* _M_node; CCreatureList(const CEmptyAllocator& a = CEmptyAllocator());      };  // 0x00512870
struct CThingList {         void* _M_node; CThingList(const CEmptyAllocator& a = CEmptyAllocator());         };  // 0x004B8920
struct CObjectList {        void* _M_node; CObjectList(const CEmptyAllocator& a = CEmptyAllocator());        };  // 0x0065F1DF
struct CTrackNodeList {     void* _M_node; CTrackNodeList(const CEmptyAllocator& a = CEmptyAllocator());     };  // 0x0065F211
struct CTorchTaskList {     void* _M_node; CTorchTaskList(const CEmptyAllocator& a = CEmptyAllocator());     };  // 0x0065F243
struct CHornTaskList {      void* _M_node; CHornTaskList(const CEmptyAllocator& a = CEmptyAllocator());      };  // 0x0065F25E
struct CPatrolTaskList {    void* _M_node; CPatrolTaskList(const CEmptyAllocator& a = CEmptyAllocator());    };  // 0x0065F279
struct CCropTaskList {      void* _M_node; CCropTaskList(const CEmptyAllocator& a = CEmptyAllocator());      };  // 0x0065F294
struct CSweepTaskList {     void* _M_node; CSweepTaskList(const CEmptyAllocator& a = CEmptyAllocator());     };  // 0x0065F2AF
struct CCrateTaskList {     void* _M_node; CCrateTaskList(const CEmptyAllocator& a = CEmptyAllocator());     };  // 0x0065F2CA
struct CMarkerList {        void* _M_node; CMarkerList(const CEmptyAllocator& a = CEmptyAllocator());        };  // 0x0065F2E5
struct CLongSet {                               // set<long> (retail 12 bytes)
    void* _M_header; long _M_node_count; long _M_key_compare;
    CLongSet();                                 // 0x005157F0
};
struct CVillageDef;
struct CVillageDefPointer {
    const CVillageDef* Object;
    CVillageDefPointer() : Object(0) {}
};
struct C3DVector { float X, Y, Z; };

struct CTCVillage : CTCBase {
    long             TorchTasksUpdateFrame;                  // +0x0c
    long             FrameStartedSecuritySweep;              // +0x10
    C3DVector        PositionPlayerWasInWhenSecuritySweepCalculated; // +0x14
    long             LastAweCrowdEventFrame;                 // +0x20
    long             LastLynchCrowdEventFrame;               // +0x24
    CBuildingList    VillageBuildings;                       // +0x28
    CCreatureList    VillagePeople;                          // +0x2c
    CThingList       VillageTorches;                         // +0x30
    CThingList       VillageHorns;                           // +0x34
    CThingList       VillageGates;                           // +0x38
    CObjectList      VillageCrops;                           // +0x3c
    CThingList       VillageCages;                           // +0x40
    CThingList       VillageCampfires;                       // +0x44
    CTrackNodeList   VillagePatrols;                         // +0x48
    CTorchTaskList   TorchTasks;                             // +0x4c
    CHornTaskList    HornTasks;                              // +0x50
    CPatrolTaskList  PatrolTasks;                            // +0x54
    CCropTaskList    CropTasks;                              // +0x58
    CSweepTaskList   SecuritySweepTasks;                     // +0x5c
    CCrateTaskList   CrateTasks;                             // +0x60
    CLongSet         PlayersCrimes;                          // +0x64
    CMarkerList      VillageMarkers;                         // +0x70
    CThingList       VillageDummies;                         // +0x74
    bool             Initialised;                            // +0x78
    bool             InitiallyPopulated;                     // +0x79
    bool             HasBeenInitiallyPopulated;              // +0x7a
    bool             HornTasksAdded;                         // +0x7b
    bool             IsEnemyBecauseOfCrime;                  // +0x7c
    long             FramePlayerLastSeenByGuard;             // +0x80
    bool             InCurfew;                               // +0x84
    long             NumGuards;                              // +0x88
    long             HouseCurrentlyChecking;                 // +0x8c
    bool             TimeIsPaused;                           // +0x90
    CVillageDefPointer PDef;                                 // +0x94
    long             StanceToHero;                           // +0x98 EVillageStanceToHero
    long             TimeStopIndex;                          // +0x9c
    long             AmountOfFineAlreadyPaid;                // +0xa0
    long             NextGuardRespawn;                       // +0xa4
    long             FrameBribeEnds;                         // +0xa8
    long             FrameForNextGuardGreeting;              // +0xac
    bool             WorkTimeAccordingToCrier;               // +0xb0
    bool             CurfewTimeAccordingToCrier;             // +0xb1
    bool             Limbo;                                  // +0xb2
    bool             LaggyLimbo;                             // +0xb3
    bool             SecuritySweepUrgent;                    // +0xb4
    float            InitialSecuritySweepRadius;             // +0xb8
    bool             TeacherTeaching;                        // +0xbc
    CIntelligentPointerCreature GuardFollowingPlayer;        // +0xbd
    long             NextFrameToFollowPlayer;                // +0xc8
    bool             LightActiveStateToChange;               // +0xcc
    long             MaxFrameToAssignTask;                   // +0xd0
    long             HeroEnemyOfGuardsUntil;                 // +0xd4
    long             LastFrameApologised;                    // +0xd8
    CIntelligentPointerCreature GuardWarningHero;            // +0xdc
    bool             CurrentIsHeroCriminal;                  // +0xe4

    virtual void v01();
    CTCVillage(CThing* parent);
    void InitialiseSecuritySweeps();                         // 0x00659085
};

CTCVillage::CTCVillage(CThing* parent)
    : CTCBase(parent)
    , TorchTasksUpdateFrame(0)
    , LastAweCrowdEventFrame(0)
    , LastLynchCrowdEventFrame(0)
    , Initialised(false)
    , InitiallyPopulated(false)
    , HasBeenInitiallyPopulated(false)
    , IsEnemyBecauseOfCrime(false)
    , FramePlayerLastSeenByGuard(0)
    , NumGuards(0)
    , HouseCurrentlyChecking(0)
    , TimeIsPaused(false)
    , TimeStopIndex(0)
    , AmountOfFineAlreadyPaid(0)
    , FrameBribeEnds(0)
    , FrameForNextGuardGreeting(0)
    , Limbo(false)
    , LaggyLimbo(false)
    , TeacherTeaching(false)
    , NextFrameToFollowPlayer(0)
    , HeroEnemyOfGuardsUntil(0)
    , LastFrameApologised(0)
    , CurrentIsHeroCriminal(false)
{
    InitialiseSecuritySweeps();
}