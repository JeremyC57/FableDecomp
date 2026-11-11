#ifndef FABLE_ENGINE_CTCVILLAGE_H
#define FABLE_ENGINE_CTCVILLAGE_H
#include "fable_compiler.h"
#ifdef FABLE_RETAIL_CTCVILLAGE
// Retail opt-in (2026-11-03, chunk 719 row 0x004FF750 CreateRelevantPersonAndUpdateStats): the real base. Retail
// CTCVillage derives CTCBase (Fable.exe RTTI; the flattened default spells the base's vptr/ParentThing/flags as
// __vftable/_pad_0x04/_pad_0x08), so code that reads village->ParentThing and calls CTCVillage methods needs the
// class with its base. The members after the base are the generated ones, byte for byte at the same offsets.
#include "engine/CTCBase.h"
class CThing;
// Retail (2026-11-03, chunk 720 rows 0x0065B854 AddThingToVillage / 0x00659800 CalculateSecuritySweepTasks; was FABLE_RETAIL_CTCVILLAGE_MEMBERS,
// folded into this class branch at landing): the
// container and def members typed as Ego_r.pdb types them (struct_layouts_egor.tsv; STLport 4.6 list = one node pointer, so the
// retail offsets are the ones below, as the landed siblings read them: VillageTorches +0x30 0x006592E2, CrateTasks +0x60
// 0x00659C7E), and FableWin.pdb's inline ?GetPDef@CTCVillage@@QBE?AV?$CDefPointer@$$CBVCVillageDef@@@@XZ (by value: retail's
// two add-ref copies of +0x94 at 0x0065BD59). Same offsets and sizes as the opaque members of the branch below.
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
class CThingBuilding; class CThingAICreature; class CThingObject; class CThingTrackNode; class CThingMarker; class CVillageDef;
#ifdef FABLE_RETAIL_CTCVILLAGE_REMOVE_TASK
class CVillageTaskBase;
#endif
class CVillageTaskTorch; class CVillageTaskHorn; class CVillageTaskPatrol; class CVillageTaskPickCrops;
class CVillageTaskSecuritySweep; class CVillageTaskCarryCrate;
#pragma pack(push, 1)
class CTCVillage : public CTCBase {
#ifdef FABLE_RETAIL_HEARING
public:long GetFrameBribeEnds()const{return FrameBribeEnds;}
#endif
#ifdef FABLE_RETAIL_HORN_TASKS
private:void UpdateHornTasks();void RemoveThingFromAllTasks(const CThing&);
#endif
public:
#ifdef FABLE_RETAIL_CTCVILLAGE_TCI
    // Original FableWin01901440; native00488684 type34.
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_VILLAGE;}
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_ENABLE_DEF_TYPES
    void EnableVillagerDefTypes(bool,const CCharString&);
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_TORCHTASK
    bool SetTorchTaskAsAcceptedByThing(CVillageTaskTorch&,const CThing&);
    void SetTorchTaskAsCompleted(CIntelligentPointer<CVillageTaskTorch>);
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_HORNTASK
    // Retail opt-in (chunk Q11, row 0x00940960 CAIStateGroup_VillageTaskHorn::Update): Ego_r.pdb
    // ?SetHornTaskAsAcceptedByThing@CTCVillage@@QAE_NAAVCVillageTaskHorn@@ABVCThing@@@Z (retail 0x006584F9) and
    // ?SetHornTaskAsCompleted@CTCVillage@@QAEXV?$CIntelligentPointer@VCVillageTaskHorn@@@@@Z (retail 0x0065A00F), out of line.
    bool SetHornTaskAsAcceptedByThing(CVillageTaskHorn&,const CThing&);
    void SetHornTaskAsCompleted(CIntelligentPointer<CVillageTaskHorn>);
#endif
#ifdef FABLE_RETAIL_CTCWIFE_MARRY
    void DetachFromDummyVillager(CThing*);
#endif
    void AddThingToVillage(CThing& thing);                   // 0x0065B854 ?AddThingToVillage@CTCVillage@@QAEXAAVCThing@@@Z
    bool IsHeroCriminal();                                   // 0x0065BFCE ?IsHeroCriminal@CTCVillage@@QAE_NXZ (Ego_r; chunk 755 row 0x0091EA70)
#ifdef FABLE_RETAIL_CTCVILLAGE_GUARDFOLLOW
    // Nested opt-in (chunk 755, row 0x0090C540): Ego_r.pdb ?SetGuardFollowingPlayer@CTCVillage@@QAEXPAVCThingAICreature@@@Z (retail
    // 0x0090C780, out of line) and FableWin.pdb ?GetGuardFollowingPlayer@CTCVillage@@QAEPAVCThingAICreature@@XZ (inline in
    // retail: one GetPItem on the packed pointer at +0xBD, 0x0090C5CD). GuardFollowingPlayer stays opaque bytes here.
    void SetGuardFollowingPlayer(CThingAICreature* guard);   // 0x0090C780
    CThingAICreature* GetGuardFollowingPlayer() { return reinterpret_cast<CIntelligentPointer<CThingAICreature>*>(GuardFollowingPlayer)->Get(); }
#endif
    FABLE_FORCEINLINE CDefPointer<const CVillageDef> GetPDef() const { return PDef; }   // FableWin ?GetPDef@CTCVillage@@QBE?AV?$CDefPointer@$$CBVCVillageDef@@@@XZ (inline)
#ifdef FABLE_RETAIL_CTCVILLAGE_BRIBE
    // Retail opt-in (chunk 795, row 0x0092FAB0): Ego_r.pdb ?GetCostToBribe@CTCVillage@@QAEJXZ, out of line in retail (0x006588CF:
    // `mov eax,[ecx+94h] ; mov eax,[eax+0F8h] ; ret`, PDef's CVillageDef::BribeCost; landed as _global::GetMember_dword0x94_deref0xf8).
    long GetCostToBribe();                                               // 0x006588CF
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_LIMBO
    // Retail opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): FableWin.pdb ?GetVillageLimbo@CTCVillage@@QBE_NXZ, inline in retail
    // (`mov cl,[eax+0B3h]` straight after GetPVillage 0x007EE540). +0xB3 is LaggyLimbo in the retail layout below (Ego_r.pdb +0x13F;
    // Limbo is +0xB2) -- which member the accessor returns is read from the retail byte offset only. Body only: no layout change.
    bool GetVillageLimbo() const { return LaggyLimbo; }
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_CRATETASK
    // Chunk 754 (row 0x00943C20 CAIStateGroup_WorkInWarehouse::IsRunnable): Ego_r.pdb / FableWin.pdb
    // ?GetBestCrateTaskForThing@CTCVillage@@QAEPAVCVillageTaskCarryCrate@@ABVCThing@@@Z (retail 0x00658EA0, out of line).
    CVillageTaskCarryCrate* GetBestCrateTaskForThing(const CThing& thing);   // 0x00658EA0
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_CRIMES
    // Retail opt-in (agentNF14, row 0x0065C1FC): Ego_r.pdb ?RemoveCrime@CTCVillage@@QAEXJ@Z (retail 0x0065C146, out of line) and
    // ?RemoveCrimeIfApologisable@CTCVillage@@QAEXJ@Z (0x0065C1FC), both public. Declarations only: no layout change.
    void RemoveCrime(long deedHandle);                       // 0x0065C146
    void RemoveCrimeIfApologisable(long deedHandle);         // 0x0065C1FC
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_APOLOGISE
    // Retail opt-in (agentNF14, rows 0x006585EE / 0x00658604): Ego_r.pdb ?NotifyHasApologised@CTCVillage@@QAEXXZ and
    // ?GetIsHeroEnemyOfGuards@CTCVillage@@QAE_NXZ. The Ego_r donor order NotifyHasApologised, CreateDummyVillagersIfNecessary (an
    // empty body), GetIsHeroEnemyOfGuards is retail's packed run 0x006585EE (stores the world frame to LastFrameApologised +0xd8),
    // the lone `ret` 0x00658603, 0x00658604 (frame < HeroEnemyOfGuardsUntil +0xd4). Declarations only: no layout change.
    void NotifyHasApologised();                              // 0x006585EE
    void CreateDummyVillagersIfNecessary();                  // 0x00658603
    bool GetIsHeroEnemyOfGuards();                           // 0x00658604
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_CURFEW
    // Retail opt-in (chunk Q15, row 0x0090C900): Ego_r.pdb ?IsCurfewTime@CTCVillage@@QBE_NXZ (out of line, 0x00658D5D). Declaration only.
public:
    bool IsCurfewTime() const;                               // 0x00658D5D
#endif
private:
#ifdef FABLE_RETAIL_CTCVILLAGE_REMOVE_TASK
    // Ego_r/FableWin original private const member template; six retail instances 00660ae2..00660d49.
    template<class T> void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, std::list<CCountedPointer<T> >& task_list) const;
#endif
    void CalculateSecuritySweepTasks();                      // 0x00659800 ?CalculateSecuritySweepTasks@CTCVillage@@AAEXXZ
public:
    long             TorchTasksUpdateFrame;                  // +0x0c
    long             FrameStartedSecuritySweep;              // +0x10
    C3DVector        PositionPlayerWasInWhenSecuritySweepCalculated;  // +0x14
    long             LastAweCrowdEventFrame;                 // +0x20
    long             LastLynchCrowdEventFrame;               // +0x24
    std::list<CIntelligentPointer<CThingBuilding> > VillageBuildings; // +0x28
    std::list<CIntelligentPointer<CThingAICreature> > VillagePeople; // +0x2c
    std::list<CIntelligentPointer<CThing> > VillageTorches; // +0x30
    std::list<CIntelligentPointer<CThing> > VillageHorns; // +0x34
    std::list<CIntelligentPointer<CThing> > VillageGates; // +0x38
    std::list<CIntelligentPointer<CThingObject> > VillageCrops; // +0x3c
    std::list<CIntelligentPointer<CThing> > VillageCages; // +0x40
    std::list<CIntelligentPointer<CThing> > VillageCampfires; // +0x44
    std::list<CIntelligentPointer<CThingTrackNode> > VillagePatrols; // +0x48
    std::list<CCountedPointer<CVillageTaskTorch> > TorchTasks; // +0x4c
    std::list<CCountedPointer<CVillageTaskHorn> > HornTasks; // +0x50
    std::list<CCountedPointer<CVillageTaskPatrol> > PatrolTasks; // +0x54
    std::list<CCountedPointer<CVillageTaskPickCrops> > CropTasks; // +0x58
    std::list<CCountedPointer<CVillageTaskSecuritySweep> > SecuritySweepTasks; // +0x5c
    std::list<CCountedPointer<CVillageTaskCarryCrate> > CrateTasks; // +0x60
    unsigned char    PlayersCrimes[0xc];                     // +0x64 set<long,std::less<long>,std::allocator<long>_>
    std::list<CIntelligentPointer<CThingMarker> > VillageMarkers;    // +0x70
    std::list<CIntelligentPointer<CThing> > VillageDummies;          // +0x74
    bool             Initialised;                            // +0x78
    bool             InitiallyPopulated;                     // +0x79
    bool             HasBeenInitiallyPopulated;              // +0x7a
    bool             HornTasksAdded;                         // +0x7b
    bool             IsEnemyBecauseOfCrime;                  // +0x7c
    unsigned char    _pad_0x7d[0x3];                         // +0x7d
    long             FramePlayerLastSeenByGuard;             // +0x80
    bool             InCurfew;                               // +0x84
    unsigned char    _pad_0x85[0x3];                         // +0x85
    long             NumGuards;                              // +0x88
    long             HouseCurrentlyChecking;                 // +0x8c
    bool             TimeIsPaused;                           // +0x90
    unsigned char    _pad_0x91[0x3];                         // +0x91
    CDefPointer<const CVillageDef> PDef;                     // +0x94
    unsigned char    StanceToHero[0x4];                      // +0x98 EVillageStanceToHero
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
    unsigned char    _pad_0xb5[0x3];                         // +0xb5
    float            InitialSecuritySweepRadius;             // +0xb8
    bool             TeacherTeaching;                        // +0xbc
    unsigned char    GuardFollowingPlayer[0xb];              // +0xbd CIntelligentPointer<CThingAICreature>
    long             NextFrameToFollowPlayer;                // +0xc8
    bool             LightActiveStateToChange;               // +0xcc
    unsigned char    _pad_0xcd[0x3];                         // +0xcd
    long             MaxFrameToAssignTask;                   // +0xd0
    long             HeroEnemyOfGuardsUntil;                 // +0xd4
    long             LastFrameApologised;                    // +0xd8
    unsigned char    GuardWarningHero[0x8];                  // +0xdc CIntelligentPointer<CThingAICreature>
    bool             CurrentIsHeroCriminal;                  // +0xe4
    unsigned char    _pad_0xe5[0x3];                         // +0xe5
#ifdef FABLE_RETAIL_CTCVILLAGE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x00661245
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x00661248
public:
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual overrides this declaration lacks, declaration only (no layout change; the vtable exists). Slots from the TLC
// vtable (agentFAM03 proof, re-verified by agentFAM05); spellings from the Ego_r / FableWin name sets. SetVillageLimbo: Ego_r.pdb
// ?SetVillageLimbo@CTCVillage@@QAEX_N@Z, out of line at 0x0065C36E (the landed binding); RemoveThingAndUpdateCameraPoints: Ego_r.pdb
// ?RemoveThingAndUpdateCameraPoints@CTCVillage@@QAEXPAVCThing@@@Z, retail 0x0065B6DD (landed).
public:
    virtual ETCInterfaceType GetInterfaceType() const;  // slot 21, 0x0066122A
public:
    virtual void OnInitialActivate();  // slot 6, 0x0065CE88
    void SetVillageLimbo(bool limbo);  // 0x0065C36E
    void RemoveThingAndUpdateCameraPoints(CThing* thing);  // 0x0065B6DD
public:
#endif // FABLE_RETAIL_CTCVILLAGE_VSLOTS3
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CTCVillage) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TorchTasksUpdateFrame) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameStartedSecuritySweep) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastAweCrowdEventFrame) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastLynchCrowdEventFrame) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageBuildings) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillagePeople) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageTorches) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageHorns) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageGates) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCrops) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCages) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCampfires) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillagePatrols) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TorchTasks) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HornTasks) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PatrolTasks) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CropTasks) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, SecuritySweepTasks) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CrateTasks) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PlayersCrimes) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageMarkers) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageDummies) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, Initialised) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InitiallyPopulated) == 0x79);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HasBeenInitiallyPopulated) == 0x7a);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HornTasksAdded) == 0x7b);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, IsEnemyBecauseOfCrime) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FramePlayerLastSeenByGuard) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InCurfew) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NumGuards) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HouseCurrentlyChecking) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TimeIsPaused) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PDef) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, StanceToHero) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TimeStopIndex) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, AmountOfFineAlreadyPaid) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NextGuardRespawn) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameBribeEnds) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameForNextGuardGreeting) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, WorkTimeAccordingToCrier) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CurfewTimeAccordingToCrier) == 0xb1);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, Limbo) == 0xb2);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LaggyLimbo) == 0xb3);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, SecuritySweepUrgent) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InitialSecuritySweepRadius) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TeacherTeaching) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, GuardFollowingPlayer) == 0xbd);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NextFrameToFollowPlayer) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LightActiveStateToChange) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, MaxFrameToAssignTask) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HeroEnemyOfGuardsUntil) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastFrameApologised) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, GuardWarningHero) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CurrentIsHeroCriminal) == 0xe4);
#else // flattened Ego_r layout (the landed default)

// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCVillage = 0x174.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCVillage = 0xe8.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CDefPointeeBase;

#pragma pack(push, 1)
class CTCVillage {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    long             TorchTasksUpdateFrame;                  // +0x0c
    long             FrameStartedSecuritySweep;              // +0x10
    float            PositionPlayerWasInWhenSecuritySweepCalculated_X; // +0x14 C3DVector::X
    float            PositionPlayerWasInWhenSecuritySweepCalculated_Y; // +0x18 C3DVector::Y
    float            PositionPlayerWasInWhenSecuritySweepCalculated_Z; // +0x1c C3DVector::Z
    long             LastAweCrowdEventFrame;                 // +0x20
    long             LastLynchCrowdEventFrame;               // +0x24
    unsigned char    VillageBuildings[0x4];                  // +0x28 list<CIntelligentPointer<CThingBuilding>,std::allocator<CIntelligentPointer<CThingBuilding>_>_>
    unsigned char    VillagePeople[0x4];                     // +0x2c list<CIntelligentPointer<CThingAICreature>,std::allocator<CIntelligentPointer<CThingAICreature>_>_>
    unsigned char    VillageTorches[0x4];                    // +0x30 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageHorns[0x4];                      // +0x34 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageGates[0x4];                      // +0x38 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageCrops[0x4];                      // +0x3c list<CIntelligentPointer<CThingObject>,std::allocator<CIntelligentPointer<CThingObject>_>_>
    unsigned char    VillageCages[0x4];                      // +0x40 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillageCampfires[0x4];                  // +0x44 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    unsigned char    VillagePatrols[0x4];                    // +0x48 list<CIntelligentPointer<CThingTrackNode>,std::allocator<CIntelligentPointer<CThingTrackNode>_>_>
    unsigned char    TorchTasks[0x4];                        // +0x4c list<CCountedPointer<CVillageTaskTorch>,std::allocator<CCountedPointer<CVillageTaskTorch>_>_>
    unsigned char    HornTasks[0x4];                         // +0x50 list<CCountedPointer<CVillageTaskHorn>,std::allocator<CCountedPointer<CVillageTaskHorn>_>_>
    unsigned char    PatrolTasks[0x4];                       // +0x54 list<CCountedPointer<CVillageTaskPatrol>,std::allocator<CCountedPointer<CVillageTaskPatrol>_>_>
    unsigned char    CropTasks[0x4];                         // +0x58 list<CCountedPointer<CVillageTaskPickCrops>,std::allocator<CCountedPointer<CVillageTaskPickCrops>_>_>
    unsigned char    SecuritySweepTasks[0x4];                // +0x5c list<CCountedPointer<CVillageTaskSecuritySweep>,std::allocator<CCountedPointer<CVillageTaskSecuritySweep>_>_>
    unsigned char    CrateTasks[0x4];                        // +0x60 list<CCountedPointer<CVillageTaskCarryCrate>,std::allocator<CCountedPointer<CVillageTaskCarryCrate>_>_>
    unsigned char    PlayersCrimes[0xc];                     // +0x64 set<long,std::less<long>,std::allocator<long>_>
    unsigned char    VillageMarkers[0x4];                    // +0x70 list<CIntelligentPointer<CThingMarker>,std::allocator<CIntelligentPointer<CThingMarker>_>_>
    unsigned char    VillageDummies[0x4];                    // +0x74 list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
    bool             Initialised;                            // +0x78
    bool             InitiallyPopulated;                     // +0x79
    bool             HasBeenInitiallyPopulated;              // +0x7a
    bool             HornTasksAdded;                         // +0x7b
    bool             IsEnemyBecauseOfCrime;                  // +0x7c
    unsigned char    _pad_0x7d[0x3];                         // +0x7d
    long             FramePlayerLastSeenByGuard;             // +0x80
    bool             InCurfew;                               // +0x84
    unsigned char    _pad_0x85[0x3];                         // +0x85
    long             NumGuards;                              // +0x88
    long             HouseCurrentlyChecking;                 // +0x8c
    bool             TimeIsPaused;                           // +0x90
    unsigned char    _pad_0x91[0x3];                         // +0x91
    CDefPointeeBase* PDef_Object;                            // +0x94 CDefPointer<CVillageDef_const_>::Object
    unsigned char    StanceToHero[0x4];                      // +0x98 EVillageStanceToHero
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
    unsigned char    _pad_0xb5[0x3];                         // +0xb5
    float            InitialSecuritySweepRadius;             // +0xb8
    bool             TeacherTeaching;                        // +0xbc
    unsigned char    GuardFollowingPlayer[0xb];              // +0xbd CIntelligentPointer<CThingAICreature>
    long             NextFrameToFollowPlayer;                // +0xc8
    bool             LightActiveStateToChange;               // +0xcc
    unsigned char    _pad_0xcd[0x3];                         // +0xcd
    long             MaxFrameToAssignTask;                   // +0xd0
    long             HeroEnemyOfGuardsUntil;                 // +0xd4
    long             LastFrameApologised;                    // +0xd8
    unsigned char    GuardWarningHero[0x8];                  // +0xdc CIntelligentPointer<CThingAICreature>
    bool             CurrentIsHeroCriminal;                  // +0xe4
    unsigned char    _pad_0xe5[0x3];                         // +0xe5
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCVillage) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TorchTasksUpdateFrame) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameStartedSecuritySweep) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_X) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_Y) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_Z) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastAweCrowdEventFrame) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastLynchCrowdEventFrame) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageBuildings) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillagePeople) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageTorches) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageHorns) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageGates) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCrops) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCages) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageCampfires) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillagePatrols) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TorchTasks) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HornTasks) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PatrolTasks) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CropTasks) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, SecuritySweepTasks) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CrateTasks) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PlayersCrimes) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageMarkers) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, VillageDummies) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, Initialised) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InitiallyPopulated) == 0x79);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HasBeenInitiallyPopulated) == 0x7a);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HornTasksAdded) == 0x7b);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, IsEnemyBecauseOfCrime) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FramePlayerLastSeenByGuard) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InCurfew) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NumGuards) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HouseCurrentlyChecking) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TimeIsPaused) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, PDef_Object) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, StanceToHero) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TimeStopIndex) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, AmountOfFineAlreadyPaid) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NextGuardRespawn) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameBribeEnds) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, FrameForNextGuardGreeting) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, WorkTimeAccordingToCrier) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CurfewTimeAccordingToCrier) == 0xb1);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, Limbo) == 0xb2);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LaggyLimbo) == 0xb3);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, SecuritySweepUrgent) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, InitialSecuritySweepRadius) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, TeacherTeaching) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, GuardFollowingPlayer) == 0xbd);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, NextFrameToFollowPlayer) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LightActiveStateToChange) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, MaxFrameToAssignTask) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, HeroEnemyOfGuardsUntil) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, LastFrameApologised) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, GuardWarningHero) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CTCVillage, CurrentIsHeroCriminal) == 0xe4);

#endif // FABLE_RETAIL_CTCVILLAGE

#endif // FABLE_ENGINE_CTCVILLAGE_H
