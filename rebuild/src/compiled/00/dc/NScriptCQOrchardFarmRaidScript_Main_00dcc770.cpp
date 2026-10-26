// CQ_OrchardFarmRaidScript::Main() -- the Orchard Farm raid quest's main: bind the team-spawn / artefact / Whisper / team-member / rear-entrance / intro-marker entity scripts, make sure the barricade quest is active, wait for OrchardFarm, fetch both teams' crate drop points, then spawn the game-rules thread for the hero's team (evil or good), the cutscene thread and the external-deactivation watcher
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCC770, length 1981 bytes. Catalog label "CQ_GuildTrainingScript::Main" is
//   propagated: the body uses the CQ_OrchardFarmRaidScript strings and spawns the landed
//   CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation 0x00DCCF30.
// Callees: operator new 0x00BFEA1A, operator delete 0x00BFE9BC, CCharString(const char*, long) 0x0099EBF0 /
//   CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities
//   0x00CB8930 / AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase ctor 0x00CDD450 (CSpawnedFunc vtable 0x012DB13C);
//   binding allocators (binding vtable 0x012DB2BC): CTeamSpawn 0x00DCD0E0 (GuardTeamSpawn, BanditTeamSpawn), CArtefact
//   0x00DCDB80, COrchardFarmWhisper 0x00DCEF50, CCrateTeamMember 0x00DCDE20 (GuardTeamMember, BanditTeamMember),
//   CFarmRearEntrance 0x00DCF3D0, CWhisperFarmRaidIntro 0x00DCF540, CWhisperMarker 0x00DCF610; spawned functions
//   ProcessGameRulesEvil 0x00DD03D0, ProcessGameRulesGood 0x00DD0F60, DoCutsceneIfRequired 0x00DCFA60,
//   WatchForExternalScriptDeactivation 0x00DCCF30; the game script interface (+0x40) slots 7, 12, 64, 72, 276, 284.
// Layout (retail = Ego_r.pdb - 0x14, and - 0x18 past the 12-byte retail STLport CrateList vector): HeroTeam +0x78,
//   Teams[2] (CCrateTeamManager, 0x40 bytes each) +0x8c; CCrateTeamManager::CrateDropPos +0x24.
// Shape: bindings / spawned functions as the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20; the crate
//   drop assignment is CScriptThing::operator= over the counted pointer (as the landed CCrateTeamMember::Init 0x00DCDF60)
//   with the returned temporary's ~CScriptThing out of line.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
struct CScriptThing {                                   // 12 bytes, vtable 0x01238C8C
    virtual void t000();
    CCountedPointer<void> PImp;                         // +0x04 Data, +0x08 Info
    ~CScriptThing();                                    // 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
struct CSpawnedFuncBase {                               // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
struct CScriptDataBase;
struct CEntityScriptBase;
struct __declspec(novtable) CEntityScriptBindingBase {
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30) IsRegionLoaded
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;   // slot 64 (+0x100) PostAddScriptedEntities
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152();
    virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168();
    virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176();
    virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
    virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192();
    virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200();
    virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208();
    virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216();
    virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224();
    virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232();
    virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240();
    virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248();
    virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256();
    virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264();
    virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272();
    virtual void v273(); virtual void v274(); virtual void v275();
    virtual void ActivateQuest(const CCharString& quest) const;   // slot 276 (+0x450) ActivateQuest
    virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283();
    virtual bool IsQuestActive(const CCharString& quest) const;   // slot 284 (+0x470) IsQuestActive
};
struct CCrateTeamManager {                              // 0x40 bytes
    long  StateCounter[6];                              // +0x00
    long  MemberCount;                                  // +0x18
    long  TeamReinforcementsTimer;                      // +0x1c (CTimer)
    long  MaxTeamMembers;                               // +0x20
    CScriptThing CrateDropPos;                          // +0x24
    CScriptThing TeamCrateCarrier;                      // +0x30
    CCrateTeamManager* EnemyTeam;                       // +0x3c
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
class CQ_OrchardFarmRaidScript : public CScriptBase {
public:
#define ALLOC(Name, Addr) struct Name { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& thing); };
    ALLOC(CTeamSpawn, 0x00DCD0E0)
    ALLOC(CArtefact, 0x00DCDB80)
    ALLOC(COrchardFarmWhisper, 0x00DCEF50)
    ALLOC(CCrateTeamMember, 0x00DCDE20)
    ALLOC(CFarmRearEntrance, 0x00DCF3D0)
    ALLOC(CWhisperFarmRaidIntro, 0x00DCF540)
    ALLOC(CWhisperMarker, 0x00DCF610)
#undef ALLOC
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x78 - 0x48];          // CrateCount +0x48 .. MissionFailed +0x68, FailReasons +0x6c
    long                  HeroTeam;                     // +0x78
    unsigned char         _pad7c[0x8c - 0x7c];          // BanditWavesSpawned, RemindHeroOfObjectivesTimer, TextSystemScriptCode, flags
    CCrateTeamManager     Teams[2];                     // +0x8c
    void Main();
    void ProcessGameRulesEvil();                        // 0x00DD03D0
    void ProcessGameRulesGood();                        // 0x00DD0F60
    void DoCutsceneIfRequired();                        // 0x00DCFA60
    void WatchForExternalScriptDeactivation();          // 0x00DCCF30
};
void CQ_OrchardFarmRaidScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("GuardTeamSpawn", this, &CTeamSpawn::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("BanditTeamSpawn", this, &CTeamSpawn::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("Artefact", this, &CArtefact::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("OrchardFarmWhisper", this, &COrchardFarmWhisper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("GuardTeamMember", this, &CCrateTeamMember::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("BanditTeamMember", this, &CCrateTeamMember::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("FarmRearEntrance", this, &CFarmRearEntrance::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("M_WhisperFarmRaidIntro", this, &CWhisperFarmRaidIntro::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OrchardFarmRaidScript>("MK_OFI_GWLL_WHIS2", this, &CWhisperMarker::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!Game->IsQuestActive("Q_OrchardFarm_Barricade"))
        Game->ActivateQuest("Q_OrchardFarm_Barricade");
    while (!Game->IsRegionLoaded("OrchardFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Teams[1].CrateDropPos = Game->GetThingWithScriptName("BanditTeamCrateDrop");
    Teams[0].CrateDropPos = Game->GetThingWithScriptName("GuardTeamCrateDrop");
    if (HeroTeam == 1)
        SpawnFunction(new CSpawnedFunc<CQ_OrchardFarmRaidScript>("ProcessGameRulesEvil", 0, this, &CQ_OrchardFarmRaidScript::ProcessGameRulesEvil));
    else
        SpawnFunction(new CSpawnedFunc<CQ_OrchardFarmRaidScript>("ProcessGameRulesGood", 0, this, &CQ_OrchardFarmRaidScript::ProcessGameRulesGood));
    SpawnFunction(new CSpawnedFunc<CQ_OrchardFarmRaidScript>("DoCutsceneIfRequired", 0, this, &CQ_OrchardFarmRaidScript::DoCutsceneIfRequired));
    SpawnFunction(new CSpawnedFunc<CQ_OrchardFarmRaidScript>("WatchForExternalScriptDeactivation", 0, this, &CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation));
}
}