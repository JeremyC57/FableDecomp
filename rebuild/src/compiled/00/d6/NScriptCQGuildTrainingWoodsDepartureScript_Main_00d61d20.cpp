// CQ_GuildTrainingWoodsDepartureScript::Main() -- the woods departure quest's main: reset the mission flags, wait for GuildWoods, bind the ArtifactThief / FinalMaze / ScorpionHome entity scripts, finish adding scripted entities, spawn the WatchForTermination and DoMission threads, set the quest-card objective and create the maze tutorial rival hero at the maze creation marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D61D20, length 1047 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30
//   / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptBase::AddEntityScriptBinding
//   0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450
//   (CSpawnedFunc vtable 0x012D1610), CScriptBase::AddSpawnedFunction 0x00CB7E50, ~CScriptThing 0x004AA840; binding
//   allocators 0x00D62360 (ArtifactThief), 0x00D62270 (FinalMaze), 0x00D63CC0 (ScorpionHome) (binding vtable 0x012D16A8);
//   WatchForTermination 0x00D62140, DoMission 0x00D63DC0 (landed); the game script interface (+0x40) slots 7, 11, 64, 72,
//   91, 296; CScriptThing::GetPos (slot 6).
// Layout (retail = Ego_r.pdb - 0x14): MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4a, DepartureMissionPoint +0x4c.
// Shape: the landed CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620 model (one flag word guarding the name temporaries).
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
struct CScriptThing {                                   // 12 bytes, vtable 0x01238C8C
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    void* PImpData;
    void* PImpInfo;
    ~CScriptThing();                                    // 0x004AA840
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
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
struct CScriptDataBase;
struct CEntityScriptBase;
template <class T> struct CCountedPointer { T* Data; void* Info; };
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
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual bool IsLevelLoaded(const CCharString& level) const;   // slot 11 (+0x2c) IsLevelLoaded
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;   // slot 64 (+0x100) PostAddScriptedEntities
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& pos, const CCharString& scriptName, bool a) const;   // slot 91 (+0x16c) CreateCreature
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163();
    virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171();
    virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187();
    virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195();
    virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203();
    virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211();
    virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227();
    virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235();
    virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243();
    virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251();
    virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259();
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267();
    virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275();
    virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283();
    virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291();
    virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295();
    virtual void SetQuestCardObjective(const CCharString& quest, const CCharString& text, const CCharString& region, const CCharString& level) const;   // slot 296 (+0x4a0) SetQuestCardObjective
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
class CQ_GuildTrainingWoodsDepartureScript : public CScriptBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAlloc)(CQ_GuildTrainingWoodsDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing);
    struct CArtifactThief { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingWoodsDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D62360
    struct CFinalMaze { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingWoodsDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing); };       // 0x00D62270
    struct CScorpionHome { static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingWoodsDepartureScript& parent, CScriptDataBase* data, const CScriptThing& thing); };    // 0x00D63CC0
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    bool                  MissionSucceeded;             // +0x48
    bool                  MissionFailed;                // +0x49
    bool                  MissionOver;                  // +0x4a
    long                  DepartureMissionPoint;        // +0x4c
    void InitialiseVariables()
    {
        MissionSucceeded = false;
        MissionFailed = false;
        MissionOver = false;
    }
    void Main();
    void WatchForTermination();                         // 0x00D62140
    void DoMission();                                   // 0x00D63DC0
};
void CQ_GuildTrainingWoodsDepartureScript::Main()
{
    InitialiseVariables();
    while (!GSI->IsLevelLoaded("GuildWoods")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsDepartureScript>("ArtifactThief", this, &CArtifactThief::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsDepartureScript>("FinalMaze", this, &CFinalMaze::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsDepartureScript>("ScorpionHome", this, &CScorpionHome::Alloc, 1));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsDepartureScript>("WatchForTermination", 0, this, &CQ_GuildTrainingWoodsDepartureScript::WatchForTermination));
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsDepartureScript>("DoMission", 0, this, &CQ_GuildTrainingWoodsDepartureScript::DoMission));
    DepartureMissionPoint = 0;
    GSI->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "");
    GSI->CreateCreature("CREATURE_RIVAL_HERO_MAZE_TUTORIAL", GSI->GetThingWithScriptName("MazeCreationMarker").GetPos(), "FinalMaze", false);
}
}