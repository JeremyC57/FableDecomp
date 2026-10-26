// CQ_GuildTrainingWoodsMeleeScript::DoMission() -- give the hero the first objective, wait until GuildWoods is loaded, spawn the WatchForLeaving and TeleportOutHero threads, take a frame and run EndMission unless the mission failed
// Leaf: DoMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D66CA0, 487 bytes (496 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("first objective" 0x012D19FC,
//   "GuildWoods" 0x012CD7DC, "WatchForLeaving" 0x012D19EC, "TeleportOutHero" 0x012D19DC, "" 0x0122D70E),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A, CSpawnedFuncBase(const CCharString&,
//   unsigned long) 0x00CDD450 (CSpawnedFunc<CQ_GuildTrainingWoodsMeleeScript> vtable 0x012D1D30, functions
//   WatchForLeaving 0x00D66E90 and TeleportOutHero 0x00D66F50 -- manifest labels for both are propagated),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50, EndMission 0x00D66EE0 (manifest "CQ_CinemaTestScript::EndMission" is
//   propagated); CGameScriptInterface (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded (+0x2c), 268
//   GiveHeroNewQuestObjective (+0x430).
// Shape: as the landed CV_ChapelOfEvilScript::DoMission 0x00E6A7F0 (inline `const char*` SpawnFunction, `__forceinline`
//   CSpawnedFunc ctor); each thread name temporary is guarded by a construction flag (built only when new succeeds).
// Layout (retail = Ego_r.pdb - 0x14): MissionFailed +0x49.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual bool IsLevelLoaded(const CCharString& level) const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
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
    virtual void GiveHeroNewQuestObjective(const CCharString& text, unsigned long id) const;   // slot 268 (+0x430)
};
struct CSpawnedFuncBase {                                // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                           // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                   // +0x34
    T* PClass;                                           // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
struct CScriptBase {                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CQ_GuildTrainingWoodsMeleeScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    void*                 Data;                          // +0x44
    bool                  MissionSucceeded;              // +0x48
    bool                  MissionFailed;                 // +0x49
    void WatchForLeaving();                              // 0x00D66E90
    void TeleportOutHero();                              // 0x00D66F50
    void EndMission();                                   // 0x00D66EE0
    void DoMission();
};
} // namespace NScript
using NScript::CQ_GuildTrainingWoodsMeleeScript;

void NScript::CQ_GuildTrainingWoodsMeleeScript::DoMission()
{
    Game->GiveHeroNewQuestObjective("first objective", 1);
    while (!Game->IsLevelLoaded("GuildWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsMeleeScript>("WatchForLeaving", 0, this, &NScript::CQ_GuildTrainingWoodsMeleeScript::WatchForLeaving));
    SpawnFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsMeleeScript>("TeleportOutHero", 0, this, &NScript::CQ_GuildTrainingWoodsMeleeScript::TeleportOutHero));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!MissionFailed)
        EndMission();
}