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
#ifdef FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES
#include "stlport/fable_map.h"
#endif
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
class CThingBuilding; class CThingAICreature; class CThingObject; class CThingTrackNode; class CThingMarker; class CVillageDef;
#ifdef FABLE_RETAIL_CTCVILLAGE_REMOVE_TASK
class CVillageTaskBase;
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_TAVERN
enum ESimBuildingProperty;                                   // MS forward enum (the row casts the value it uses)
#endif
class CVillageTaskTorch; class CVillageTaskHorn; class CVillageTaskPatrol; class CVillageTaskPickCrops;
class CVillageTaskSecuritySweep; class CVillageTaskCarryCrate;
#pragma pack(push, 1)
class CTCVillage : public CTCBase {
#ifdef FABLE_RETAIL_CTCVILLAGE_PEEKMARKERS_PASS27
public:
    // 00922860, FableWin: the original const list reference accessor, inline at native +70h.
    const std::list<CIntelligentPointer<CThingMarker> >& PeekVillageMarkers()const { return VillageMarkers; }
private:
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_UPDATE_TORCH_PASS27
    void UpdateTorchTasks(); // Both PDBs, private.
    template<class T> bool IsThingInTaskList(const CThing&,std::list<CCountedPointer<T> >&) const;
    template<class T> bool IsTaskTaken(const CThing&,std::list<CCountedPointer<T> >&) const;
    template<class T> void RemoveThingFromTaskList(const CThing&,std::list<CCountedPointer<T> >&) const;
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_PEEKCAMPFIRES_PASS26
public:const std::list<CIntelligentPointer<CThing> >& PeekVillageCampfires()const{return VillageCampfires;}
private:
#endif
#ifdef FABLE_RETAIL_HEARING
public:long GetFrameBribeEnds()const{return FrameBribeEnds;}
#endif
#if defined(FABLE_RETAIL_CTCVILLAGE_GETFRAMEBRIBEENDS) && !defined(FABLE_RETAIL_HEARING)
    // Retail opt-in (wfA1, row 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed): Ego_r.pdb ?GetFrameBribeEnds@CTCVillage@@QBEJXZ,
    // inline in retail: the row reads +0xA8 into a register between the GetWorld and GetFrame calls (0x00682BE5), which the plain
    // member read does not give (`cmp eax,[esi+0A8h]` after the call). Same text as the FABLE_RETAIL_HEARING accessor above.
public:long GetFrameBribeEnds()const{return FrameBribeEnds;}
#endif
#ifdef FABLE_RETAIL_HORN_TASKS
private:void UpdateHornTasks();void RemoveThingFromAllTasks(const CThing&);
#endif
public:
#ifdef FABLE_RETAIL_SECURITY_SWEEP_LIFETIMES_PASS23
    // Both original CTCVillage TPI records: public CThing& ctor and virtual dtor.
    CTCVillage(CThing&);
    virtual ~CTCVillage();
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_TAVERNTIMES
    // Retail opt-in (row 0x0065AA6F CTCVillage::IsTimeToGoToTavern): Ego_r.pdb / FableWin.pdb ?IsTavernTime@CTCVillage@@QBE_NXZ
    // (retail 0x00658C0A), ?GetTavernTimes@CTCVillage@@QBE_NAAM0@Z (retail 0x00658C5B) and
    // ?IsTimeToGoToTavern@CTCVillage@@QAE_NPAVCThingAICreature@@ABVCWorld@@_NM@Z (retail 0x0065AA6F), all out of line. Declarations only.
    bool IsTavernTime() const;                                           // 0x00658C0A
    bool GetTavernTimes(float& start, float& end) const;                 // 0x00658C5B
    bool IsTimeToGoToTavern(CThingAICreature* creature, const CWorld& world, bool run, float speed);   // 0x0065AA6F
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_TCI
    // Original FableWin01901440; native00488684 type34.
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_VILLAGE;}
#ifdef FABLE_RETAIL_CTCVILLAGE_ISNIGHTTIME
    // Nested opt-in (NF435, row 0x00896210 CGameScriptInterface::IsSleepingTime): Ego_r.pdb / FableWin.pdb ?IsNightTime@CTCVillage@@QBE_NXZ,
    // out of line in retail at 0x00658D07 (the row's call target after the inline PeekTC<const CTCVillage>). Declaration only: no layout change.
    bool IsNightTime() const;                                            // 0x00658D07
#endif
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_ENABLE_DEF_TYPES
    void EnableVillagerDefTypes(bool,const CCharString&);
#endif
#ifdef FABLE_RETAIL_VILLAGETASK_SECURITYSWEEP_UPDATE_PASS24
    bool SetSecuritySweepTaskAsAcceptedByThing(CVillageTaskSecuritySweep&, const CThing&);
#endif
#ifdef FABLE_RETAIL_CROP_TASK_UPDATE_PASS24
    bool SetCropTaskAsAcceptedByThing(class CVillageTaskPickCrops&,const CThing&);
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
#ifdef FABLE_RETAIL_CTCVILLAGE_TRYTORESPAWNDEFNAMED
    CThingAICreature* TryToRespawnDefNamed(const CCharString&, const C3DVector&);   // 0x0065B349 (NF196 opt-in: Ego_r.pdb ?TryToRespawnDefNamed@CTCVillage@@QAEPAVCThingAICreature@@ABVCCharString@@ABVC3DVector@@@Z)
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_ENABLEGUARDS
    void EnableGuards(bool);   // 0x0065C45A (NF196 opt-in: Ego_r.pdb 0x0069DFE7 + FableWin.pdb 0x0216A4EB ?EnableGuards@CTCVillage@@QAEX_N@Z)
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_ISREPORTEDORUNREPORTEDCRIMEKNOWN
    bool IsReportedOrUnreportedCrimeKnown();   // 0x0065BFFA (NF196 opt-in: Ego_r.pdb 0x0069CECC + FableWin.pdb 0x0216B218 ?IsReportedOrUnreportedCrimeKnown@CTCVillage@@QAE_NXZ)
#endif
#if defined(FABLE_RETAIL_CTCWIFE_MARRY) || defined(FABLE_RETAIL_CTCVILLAGE_DETACHFROMDUMMYVILLAGER)
    void DetachFromDummyVillager(CThing*);   // 0x0065B2C4 (NF196 member-specific opt-in FABLE_RETAIL_CTCVILLAGE_DETACHFROMDUMMYVILLAGER: the declaration alone)
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_TAVERN
    // Opt-in (chunk 2020, row 0x00949AB0 CAIStateGroup_GoToPub::IsRunnable): Ego_r.pdb / FableWin.pdb ?IsTavernTime@CTCVillage@@QBE_NXZ
    // (retail 0x00658C0A, landed; called at 0x00949D1A) and FableWin.pdb
    // ?GetNearestBuildingWithProperty@CTCVillage@@QAEPAVCThingBuilding@@ABVC3DVector@@W4ESimBuildingProperty@@@Z (retail 0x00658E38, landed;
    // called at 0x00949D37). Declarations only: no layout change.
    bool IsTavernTime() const;                               // 0x00658C0A
    CThingBuilding* GetNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property);   // 0x00658E38
#endif
#ifdef FABLE_RETAIL_CREATE_VILLAGER_PASS27
    long GetNextGuardTypeAndIncrement();
    long GetNumberOfGayVillagers() const;
#endif
    void AddThingToVillage(CThing& thing);                   // 0x0065B854 ?AddThingToVillage@CTCVillage@@QAEXAAVCThing@@@Z
#ifdef FABLE_RETAIL_CTCVILLAGE_ONHEROSLEPT
    // Retail opt-in (wfA1, row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb ?OnHeroSlept@CTCVillage@@QAEXXZ (0x0069B1A6); retail call
    // 0x006D09A9 -> 0x0065935E with ecx = the village TC (GetTC on each CThingVillage). Declaration only.
    void OnHeroSlept();                                      // 0x0065935E
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE
    // Retail opt-in (wf17, row 0x007E2A60 CTCFineDialogue::GlobalUpdate): Ego_r.pdb ?IsHeroMurderer@CTCVillage@@QAE_NXZ (0x0065A1FD),
    // ?ClearCrimes@CTCVillage@@QAEXXZ (0x0065C2D0) and ?EndSecuritySweep@CTCVillage@@QAEXXZ (0x006590BF, body `jmp 0x006590A0`: the
    // tail call to the private ?InvalidateAndClearAllSecuritySweepTasks@CTCVillage@@AAEXXZ). Ego_r.pdb names the same 5-byte jmp
    // 0x0069D8D1 EndSecuritySweep (strict signature f448c9f6d17c6e3f in both exes) and its target 0x0069D86E InvalidateAndClear...;
    // FableWin.pdb has both (0x02161BAA / 0x021619D9). Declarations only.
    bool IsHeroMurderer();                                   // 0x0065A1FD
    void ClearCrimes();                                      // 0x0065C2D0
    void EndSecuritySweep();                                 // 0x006590BF
private:
    void InvalidateAndClearAllSecuritySweepTasks();          // 0x006590A0
public:
#endif
    bool IsHeroCriminal();                                   // 0x0065BFCE ?IsHeroCriminal@CTCVillage@@QAE_NXZ (Ego_r; chunk 755 row 0x0091EA70)
#ifdef FABLE_RETAIL_CTCVILLAGE_INCREASEHEROENEMYOFGUARDS
    // Retail opt-in (acctC wfA1, row 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed): Ego_r.pdb
    // ?IncreaseHeroEnemyOfGuardsUntil@CTCVillage@@QAEXJ@Z, out of line in retail at 0x006588DC (call 0x00682B90). Declaration only.
    void IncreaseHeroEnemyOfGuardsUntil(long frames);                    // 0x006588DC
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_GUARDFOLLOW
    // Nested opt-in (chunk 755, row 0x0090C540): Ego_r.pdb ?SetGuardFollowingPlayer@CTCVillage@@QAEXPAVCThingAICreature@@@Z (retail
    // 0x0090C780, out of line) and FableWin.pdb ?GetGuardFollowingPlayer@CTCVillage@@QAEPAVCThingAICreature@@XZ (inline in
    // retail: one GetPItem on the packed pointer at +0xBD, 0x0090C5CD). GuardFollowingPlayer stays opaque bytes here.
    void SetGuardFollowingPlayer(CThingAICreature* guard);   // 0x0090C780
    CThingAICreature* GetGuardFollowingPlayer() { return reinterpret_cast<CIntelligentPointer<CThingAICreature>*>(GuardFollowingPlayer)->Get(); }
#endif
    FABLE_FORCEINLINE CDefPointer<const CVillageDef> GetPDef() const { return PDef; }   // FableWin ?GetPDef@CTCVillage@@QBE?AV?$CDefPointer@$$CBVCVillageDef@@@@XZ (inline)
#ifdef FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE
    // Retail opt-in (chunk Q23, row 0x006C1250 CTCBuyableHouse::FrameUpdate, expanding IsHouseBeingUsed): FableWin.pdb
    // ?PeekVillagePeople@CTCVillage@@QBEABV?$list@V?$CIntelligentPointer@VCThingAICreature@@@@...@std@@XZ (inline in Ego_r and retail:
    // the caller reads the list's node word +0x2C in place, 0x006C1443). Its const_iterators are what retail passes to count_if (the
    // const iterator's copy is trivial, so the arguments are plain pushes; the non-const iterator's user copy constructor builds them
    // in their stack slots). No layout change.
    const std::list<CIntelligentPointer<CThingAICreature> >& PeekVillagePeople() const { return VillagePeople; }
#endif
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
#ifdef FABLE_RETAIL_CTCVILLAGE_PATROLTASK
    // Opt-in (p25M, row 0x0093F3C0 CAIStateGroup_VillageTaskPatrol::Update): Ego_r.pdb / FableWin.pdb
    // ?GetBestPatrolTaskForThing@CTCVillage@@QAEPAVCVillageTaskPatrol@@ABVCThing@@@Z (retail 0x00658E90, out of line: GetBestTaskForThing
    // <CVillageTaskPatrol> 0x0065F856 over PatrolTasks +0x54; GetBestCrateTaskForThing 0x00658EA0 follows it, as in Ego_r) and
    // ?SetPatrolTaskAsAcceptedByThing@CTCVillage@@QAE_NAAVCVillageTaskPatrol@@ABVCThing@@@Z (retail 0x0065852D, the catalog name). Declarations only.
    CVillageTaskPatrol* GetBestPatrolTaskForThing(const CThing& thing);                   // 0x00658E90
    bool SetPatrolTaskAsAcceptedByThing(CVillageTaskPatrol& task, const CThing& thing);  // 0x0065852D
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_CRATETASK
    // Chunk 754 (row 0x00943C20 CAIStateGroup_WorkInWarehouse::IsRunnable): Ego_r.pdb / FableWin.pdb
    // ?GetBestCrateTaskForThing@CTCVillage@@QAEPAVCVillageTaskCarryCrate@@ABVCThing@@@Z (retail 0x00658EA0, out of line).
    CVillageTaskCarryCrate* GetBestCrateTaskForThing(const CThing& thing);   // 0x00658EA0
#ifdef FABLE_RETAIL_CTCVILLAGE_WAREHOUSE_UPDATE_PASS27
    // Both PDBs, CTCVillage::SetCrateTaskAsAcceptedByThing and SetCrateTaskAsCompleted.
    bool SetCrateTaskAsAcceptedByThing(CVillageTaskCarryCrate& task, const CThing& thing); // 0x006585BA
    void SetCrateTaskAsCompleted(CIntelligentPointer<CVillageTaskCarryCrate> task);        // 0x0065A053
#endif
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES
    // Q19 opt-in (row 0x0065C5BB): Ego_r.pdb ?UpdateMirroredCrimeData@CTCVillage@@QAEXXZ (public). The same opt-in types
    //   PlayersCrimes below as the Ego_r.pdb set<long> (STLport three-word tree; needs FABLE_RETAIL_STLP_SET for begin()/end()).
    void UpdateMirroredCrimeData();                          // 0x0065C5BB
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_FINEAMOUNT
    // Retail opt-in (wf4, row 0x0065A41C): Ego_r.pdb + FableWin.pdb ?GetFineAmountForCurrentCrimes@CTCVillage@@QAEJXZ (public, non-const,
    // out of line at 0x0065A41C; bound by PayFines 0x0065C760, IsHeroCriminal 0x0065BFCE and others). Declaration only: no layout change.
public:
    long GetFineAmountForCurrentCrimes();                    // 0x0065A41C
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_WITNESSEDCRIME
    // e09 opt-in (row 0x007993F0 CTCShopKeeper::WitnessedCrime): Ego_r.pdb / FableWin.pdb ?HasCrimeBeenReported@CTCVillage@@QAE_NJ@Z
    // (retail 0x0065914B, landed), ?ReportCrime@CTCVillage@@QAEXJ@Z (0x0065CB6B) and ?StartSecuritySweep@CTCVillage@@QAEX_N@Z (0x00659C32,
    // landed), all public and out of line (Ego_d.pdb tc_village.obj records with parameters deed_handle / urgent). Declarations only.
    bool HasCrimeBeenReported(long deed_handle);              // 0x0065914B
    void ReportCrime(long deed_handle);                       // 0x0065CB6B
    void StartSecuritySweep(bool urgent);                     // 0x00659C32
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
    // Retail opt-in (chunk Q15 / w24A, row 0x0090C900): Ego_r.pdb ?IsCurfewTime@CTCVillage@@QBE_NXZ (0x00658DB3) and ?IsSociableTime@CTCVillage@@QBE_NXZ (0x00658D5D, landed; the row calls this one -- Q15 had bound IsCurfewTime to it). Declarations only.
public:
    bool IsCurfewTime() const; bool IsSociableTime() const;  // 0x00658DB3 / 0x00658D5D
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_SCHOOLSTORYTIME
    // Opt-in (agentNF26, 2026-11-12, rows 0x00658A35 / 0x00658A80 and their binders): Ego_r.pdb + FableWin.pdb
    // ?IsSchoolTime@CTCVillage@@QBE_NXZ and ?IsStoryTime@CTCVillage@@QBE_NXZ, out of line at 0x00658A35 (IsTimeBetween over
    // CVillageDef::SchoolStartTimes +0x54 / SchoolDuration +0x60) and 0x00658A80 (StoryStartTimes +0x44 / StoryDuration +0x50); Ego_r
    // order IsSchoolTime, IsStoryTime (0x0069A9A4 / 0x0069A9F1) = retail order. IsTimeBetween (0x00658620) is declared here only when
    // FABLE_RETAIL_CTCVILLAGE_ISWORKTIME does not already declare it. Declarations only.
public:
#ifndef FABLE_RETAIL_CTCVILLAGE_ISWORKTIME
    bool IsTimeBetween(const std::vector<long>& start_times, long duration) const;   // 0x00658620
#endif
    bool IsSchoolTime() const;                               // 0x00658A35
    bool IsStoryTime() const;                                // 0x00658A80
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_ISWORKTIME
    // Opt-in (agentNF25b, 2026-11-12, row 0x00658B20 and its binders): Ego_r.pdb + FableWin.pdb ?IsWorkTime@CTCVillage@@QBE_NXZ (out of line at
    // 0x00658B20: IsTimeBetween over CVillageDef::WorkStartTimes +0x64 / WorkDuration +0x70; Ego_r order IsWorkTime, IsCookTime = retail
    // 0x00658B20, 0x00658B6B) and ?IsTimeBetween@CTCVillage@@QBE_NABV?$vector@JV?$allocator@J@std@@@std@@J@Z (0x00658620, landed).
    // Declarations only.
public:
    bool IsTimeBetween(const std::vector<long>& start_times, long duration) const;   // 0x00658620
    bool IsWorkTime() const;                                 // 0x00658B20
#endif
#ifdef FABLE_RETAIL_CTCVILLAGE_YELLFORHELP
    // Retail opt-in (chunk 2015, row 0x0097FEA0 CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp): Ego_r.pdb
    // ?RemoveApologiseableCrimes@CTCVillage@@QAEXJ@Z (out of line, landed 0x0065CE56) and the FableWin.pdb inline accessors
    // ?IsHeroEnemyBecauseOfCrime@CTCVillage@@QBE_NXZ (`mov cl,[eax+7Ch]`, 0x00980036) and ?SetFrameBribeEnds@CTCVillage@@QAEXJ@Z
    // (`mov dword ptr [eax+0A8h],0`, 0x009803FD). No layout change.
    void RemoveApologiseableCrimes(long deedHandle);         // 0x0065CE56
    bool IsHeroEnemyBecauseOfCrime() const { return IsEnemyBecauseOfCrime; }
    void SetFrameBribeEnds(long frame) { FrameBribeEnds = frame; }
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
#ifdef FABLE_RETAIL_CTCVILLAGE_MIRROREDCRIMES
    std::set<long>   PlayersCrimes;                          // +0x64 (Q19 opt-in: the Ego_r.pdb type)
#else
    unsigned char    PlayersCrimes[0xc];                     // +0x64 set<long,std::less<long>,std::allocator<long>_>
#endif
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
FABLE_STATIC_ASSERT_AT(286, sizeof(CTCVillage) == 0xe8);
FABLE_STATIC_ASSERT_AT(287, offsetof(CTCVillage, TorchTasksUpdateFrame) == 0xc);
FABLE_STATIC_ASSERT_AT(288, offsetof(CTCVillage, FrameStartedSecuritySweep) == 0x10);
FABLE_STATIC_ASSERT_AT(289, offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated) == 0x14);
FABLE_STATIC_ASSERT_AT(290, offsetof(CTCVillage, LastAweCrowdEventFrame) == 0x20);
FABLE_STATIC_ASSERT_AT(291, offsetof(CTCVillage, LastLynchCrowdEventFrame) == 0x24);
FABLE_STATIC_ASSERT_AT(292, offsetof(CTCVillage, VillageBuildings) == 0x28);
FABLE_STATIC_ASSERT_AT(293, offsetof(CTCVillage, VillagePeople) == 0x2c);
FABLE_STATIC_ASSERT_AT(294, offsetof(CTCVillage, VillageTorches) == 0x30);
FABLE_STATIC_ASSERT_AT(295, offsetof(CTCVillage, VillageHorns) == 0x34);
FABLE_STATIC_ASSERT_AT(296, offsetof(CTCVillage, VillageGates) == 0x38);
FABLE_STATIC_ASSERT_AT(297, offsetof(CTCVillage, VillageCrops) == 0x3c);
FABLE_STATIC_ASSERT_AT(298, offsetof(CTCVillage, VillageCages) == 0x40);
FABLE_STATIC_ASSERT_AT(299, offsetof(CTCVillage, VillageCampfires) == 0x44);
FABLE_STATIC_ASSERT_AT(300, offsetof(CTCVillage, VillagePatrols) == 0x48);
FABLE_STATIC_ASSERT_AT(301, offsetof(CTCVillage, TorchTasks) == 0x4c);
FABLE_STATIC_ASSERT_AT(302, offsetof(CTCVillage, HornTasks) == 0x50);
FABLE_STATIC_ASSERT_AT(303, offsetof(CTCVillage, PatrolTasks) == 0x54);
FABLE_STATIC_ASSERT_AT(304, offsetof(CTCVillage, CropTasks) == 0x58);
FABLE_STATIC_ASSERT_AT(305, offsetof(CTCVillage, SecuritySweepTasks) == 0x5c);
FABLE_STATIC_ASSERT_AT(306, offsetof(CTCVillage, CrateTasks) == 0x60);
FABLE_STATIC_ASSERT_AT(307, offsetof(CTCVillage, PlayersCrimes) == 0x64);
FABLE_STATIC_ASSERT_AT(308, offsetof(CTCVillage, VillageMarkers) == 0x70);
FABLE_STATIC_ASSERT_AT(309, offsetof(CTCVillage, VillageDummies) == 0x74);
FABLE_STATIC_ASSERT_AT(310, offsetof(CTCVillage, Initialised) == 0x78);
FABLE_STATIC_ASSERT_AT(311, offsetof(CTCVillage, InitiallyPopulated) == 0x79);
FABLE_STATIC_ASSERT_AT(312, offsetof(CTCVillage, HasBeenInitiallyPopulated) == 0x7a);
FABLE_STATIC_ASSERT_AT(313, offsetof(CTCVillage, HornTasksAdded) == 0x7b);
FABLE_STATIC_ASSERT_AT(314, offsetof(CTCVillage, IsEnemyBecauseOfCrime) == 0x7c);
FABLE_STATIC_ASSERT_AT(315, offsetof(CTCVillage, FramePlayerLastSeenByGuard) == 0x80);
FABLE_STATIC_ASSERT_AT(316, offsetof(CTCVillage, InCurfew) == 0x84);
FABLE_STATIC_ASSERT_AT(317, offsetof(CTCVillage, NumGuards) == 0x88);
FABLE_STATIC_ASSERT_AT(318, offsetof(CTCVillage, HouseCurrentlyChecking) == 0x8c);
FABLE_STATIC_ASSERT_AT(319, offsetof(CTCVillage, TimeIsPaused) == 0x90);
FABLE_STATIC_ASSERT_AT(320, offsetof(CTCVillage, PDef) == 0x94);
FABLE_STATIC_ASSERT_AT(321, offsetof(CTCVillage, StanceToHero) == 0x98);
FABLE_STATIC_ASSERT_AT(322, offsetof(CTCVillage, TimeStopIndex) == 0x9c);
FABLE_STATIC_ASSERT_AT(323, offsetof(CTCVillage, AmountOfFineAlreadyPaid) == 0xa0);
FABLE_STATIC_ASSERT_AT(324, offsetof(CTCVillage, NextGuardRespawn) == 0xa4);
FABLE_STATIC_ASSERT_AT(325, offsetof(CTCVillage, FrameBribeEnds) == 0xa8);
FABLE_STATIC_ASSERT_AT(326, offsetof(CTCVillage, FrameForNextGuardGreeting) == 0xac);
FABLE_STATIC_ASSERT_AT(327, offsetof(CTCVillage, WorkTimeAccordingToCrier) == 0xb0);
FABLE_STATIC_ASSERT_AT(328, offsetof(CTCVillage, CurfewTimeAccordingToCrier) == 0xb1);
FABLE_STATIC_ASSERT_AT(329, offsetof(CTCVillage, Limbo) == 0xb2);
FABLE_STATIC_ASSERT_AT(330, offsetof(CTCVillage, LaggyLimbo) == 0xb3);
FABLE_STATIC_ASSERT_AT(331, offsetof(CTCVillage, SecuritySweepUrgent) == 0xb4);
FABLE_STATIC_ASSERT_AT(332, offsetof(CTCVillage, InitialSecuritySweepRadius) == 0xb8);
FABLE_STATIC_ASSERT_AT(333, offsetof(CTCVillage, TeacherTeaching) == 0xbc);
FABLE_STATIC_ASSERT_AT(334, offsetof(CTCVillage, GuardFollowingPlayer) == 0xbd);
FABLE_STATIC_ASSERT_AT(335, offsetof(CTCVillage, NextFrameToFollowPlayer) == 0xc8);
FABLE_STATIC_ASSERT_AT(336, offsetof(CTCVillage, LightActiveStateToChange) == 0xcc);
FABLE_STATIC_ASSERT_AT(337, offsetof(CTCVillage, MaxFrameToAssignTask) == 0xd0);
FABLE_STATIC_ASSERT_AT(338, offsetof(CTCVillage, HeroEnemyOfGuardsUntil) == 0xd4);
FABLE_STATIC_ASSERT_AT(339, offsetof(CTCVillage, LastFrameApologised) == 0xd8);
FABLE_STATIC_ASSERT_AT(340, offsetof(CTCVillage, GuardWarningHero) == 0xdc);
FABLE_STATIC_ASSERT_AT(341, offsetof(CTCVillage, CurrentIsHeroCriminal) == 0xe4);
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
#ifdef FABLE_RETAIL_CTCVILLAGE_SCHOOLSTORYTIME
    // Opt-in (agentNF26, 2026-11-12) for the flattened view, same declarations as the retail branch's block: Ego_r.pdb + FableWin.pdb
    // ?IsSchoolTime@CTCVillage@@QBE_NXZ (0x00658A35) and ?IsStoryTime@CTCVillage@@QBE_NXZ (0x00658A80). Declarations only; no layout change.
    bool IsSchoolTime() const;                               // 0x00658A35
    bool IsStoryTime() const;                                // 0x00658A80
#endif
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

FABLE_STATIC_ASSERT_AT(433, sizeof(CTCVillage) == 0xe8);
FABLE_STATIC_ASSERT_AT(434, offsetof(CTCVillage, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(435, offsetof(CTCVillage, TorchTasksUpdateFrame) == 0xc);
FABLE_STATIC_ASSERT_AT(436, offsetof(CTCVillage, FrameStartedSecuritySweep) == 0x10);
FABLE_STATIC_ASSERT_AT(437, offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_X) == 0x14);
FABLE_STATIC_ASSERT_AT(438, offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_Y) == 0x18);
FABLE_STATIC_ASSERT_AT(439, offsetof(CTCVillage, PositionPlayerWasInWhenSecuritySweepCalculated_Z) == 0x1c);
FABLE_STATIC_ASSERT_AT(440, offsetof(CTCVillage, LastAweCrowdEventFrame) == 0x20);
FABLE_STATIC_ASSERT_AT(441, offsetof(CTCVillage, LastLynchCrowdEventFrame) == 0x24);
FABLE_STATIC_ASSERT_AT(442, offsetof(CTCVillage, VillageBuildings) == 0x28);
FABLE_STATIC_ASSERT_AT(443, offsetof(CTCVillage, VillagePeople) == 0x2c);
FABLE_STATIC_ASSERT_AT(444, offsetof(CTCVillage, VillageTorches) == 0x30);
FABLE_STATIC_ASSERT_AT(445, offsetof(CTCVillage, VillageHorns) == 0x34);
FABLE_STATIC_ASSERT_AT(446, offsetof(CTCVillage, VillageGates) == 0x38);
FABLE_STATIC_ASSERT_AT(447, offsetof(CTCVillage, VillageCrops) == 0x3c);
FABLE_STATIC_ASSERT_AT(448, offsetof(CTCVillage, VillageCages) == 0x40);
FABLE_STATIC_ASSERT_AT(449, offsetof(CTCVillage, VillageCampfires) == 0x44);
FABLE_STATIC_ASSERT_AT(450, offsetof(CTCVillage, VillagePatrols) == 0x48);
FABLE_STATIC_ASSERT_AT(451, offsetof(CTCVillage, TorchTasks) == 0x4c);
FABLE_STATIC_ASSERT_AT(452, offsetof(CTCVillage, HornTasks) == 0x50);
FABLE_STATIC_ASSERT_AT(453, offsetof(CTCVillage, PatrolTasks) == 0x54);
FABLE_STATIC_ASSERT_AT(454, offsetof(CTCVillage, CropTasks) == 0x58);
FABLE_STATIC_ASSERT_AT(455, offsetof(CTCVillage, SecuritySweepTasks) == 0x5c);
FABLE_STATIC_ASSERT_AT(456, offsetof(CTCVillage, CrateTasks) == 0x60);
FABLE_STATIC_ASSERT_AT(457, offsetof(CTCVillage, PlayersCrimes) == 0x64);
FABLE_STATIC_ASSERT_AT(458, offsetof(CTCVillage, VillageMarkers) == 0x70);
FABLE_STATIC_ASSERT_AT(459, offsetof(CTCVillage, VillageDummies) == 0x74);
FABLE_STATIC_ASSERT_AT(460, offsetof(CTCVillage, Initialised) == 0x78);
FABLE_STATIC_ASSERT_AT(461, offsetof(CTCVillage, InitiallyPopulated) == 0x79);
FABLE_STATIC_ASSERT_AT(462, offsetof(CTCVillage, HasBeenInitiallyPopulated) == 0x7a);
FABLE_STATIC_ASSERT_AT(463, offsetof(CTCVillage, HornTasksAdded) == 0x7b);
FABLE_STATIC_ASSERT_AT(464, offsetof(CTCVillage, IsEnemyBecauseOfCrime) == 0x7c);
FABLE_STATIC_ASSERT_AT(465, offsetof(CTCVillage, FramePlayerLastSeenByGuard) == 0x80);
FABLE_STATIC_ASSERT_AT(466, offsetof(CTCVillage, InCurfew) == 0x84);
FABLE_STATIC_ASSERT_AT(467, offsetof(CTCVillage, NumGuards) == 0x88);
FABLE_STATIC_ASSERT_AT(468, offsetof(CTCVillage, HouseCurrentlyChecking) == 0x8c);
FABLE_STATIC_ASSERT_AT(469, offsetof(CTCVillage, TimeIsPaused) == 0x90);
FABLE_STATIC_ASSERT_AT(470, offsetof(CTCVillage, PDef_Object) == 0x94);
FABLE_STATIC_ASSERT_AT(471, offsetof(CTCVillage, StanceToHero) == 0x98);
FABLE_STATIC_ASSERT_AT(472, offsetof(CTCVillage, TimeStopIndex) == 0x9c);
FABLE_STATIC_ASSERT_AT(473, offsetof(CTCVillage, AmountOfFineAlreadyPaid) == 0xa0);
FABLE_STATIC_ASSERT_AT(474, offsetof(CTCVillage, NextGuardRespawn) == 0xa4);
FABLE_STATIC_ASSERT_AT(475, offsetof(CTCVillage, FrameBribeEnds) == 0xa8);
FABLE_STATIC_ASSERT_AT(476, offsetof(CTCVillage, FrameForNextGuardGreeting) == 0xac);
FABLE_STATIC_ASSERT_AT(477, offsetof(CTCVillage, WorkTimeAccordingToCrier) == 0xb0);
FABLE_STATIC_ASSERT_AT(478, offsetof(CTCVillage, CurfewTimeAccordingToCrier) == 0xb1);
FABLE_STATIC_ASSERT_AT(479, offsetof(CTCVillage, Limbo) == 0xb2);
FABLE_STATIC_ASSERT_AT(480, offsetof(CTCVillage, LaggyLimbo) == 0xb3);
FABLE_STATIC_ASSERT_AT(481, offsetof(CTCVillage, SecuritySweepUrgent) == 0xb4);
FABLE_STATIC_ASSERT_AT(482, offsetof(CTCVillage, InitialSecuritySweepRadius) == 0xb8);
FABLE_STATIC_ASSERT_AT(483, offsetof(CTCVillage, TeacherTeaching) == 0xbc);
FABLE_STATIC_ASSERT_AT(484, offsetof(CTCVillage, GuardFollowingPlayer) == 0xbd);
FABLE_STATIC_ASSERT_AT(485, offsetof(CTCVillage, NextFrameToFollowPlayer) == 0xc8);
FABLE_STATIC_ASSERT_AT(486, offsetof(CTCVillage, LightActiveStateToChange) == 0xcc);
FABLE_STATIC_ASSERT_AT(487, offsetof(CTCVillage, MaxFrameToAssignTask) == 0xd0);
FABLE_STATIC_ASSERT_AT(488, offsetof(CTCVillage, HeroEnemyOfGuardsUntil) == 0xd4);
FABLE_STATIC_ASSERT_AT(489, offsetof(CTCVillage, LastFrameApologised) == 0xd8);
FABLE_STATIC_ASSERT_AT(490, offsetof(CTCVillage, GuardWarningHero) == 0xdc);
FABLE_STATIC_ASSERT_AT(491, offsetof(CTCVillage, CurrentIsHeroCriminal) == 0xe4);

#endif // FABLE_RETAIL_CTCVILLAGE

#endif // FABLE_ENGINE_CTCVILLAGE_H
