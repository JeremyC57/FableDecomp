// CQ_HangingTreeEvilScript::DoMission() -- start the mission-end and region watchers, wait for BowerstoneJail, then run the stage machine: stage 1 spawns the party at the spawn point and plays the introduction, then starts the mission timer and moves to stage 2, which waits for the HeroDualArea level (stage 6); finally play the conclusion and, unless failed, flag success
// Leaf: ?DoMission@CQ_HangingTreeEvilScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6BFA0, length 1023 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012D1FE8), CScriptBase::AddSpawnedFunction
//   0x00CB7E50, CScriptBase::IsActiveThreadTerminating 0x00CB7940, WatchForMissionEnd 0x00D6C3A0, MonitorRegionPresence
//   0x00D6CDC0, SpawnParty 0x00D6C4A0, Introduction 0x00D6D080, Conclusion 0x00D6D670 (by retail order against the
//   Ego_r.pdb method order), operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0 (inline ~CScriptThing); the timer service
//   g_pGameScriptInterface 0x0143E8F8 SetTimer (89); through the interface (+0x40): NewScriptFrame (7), IsLevelLoaded (11),
//   GetThingWithScriptName (72), DisplayQuestInfo (321), AddQuestInfoTimer (326).
// Data: pGlobalScriptDef 0x0143E90C HangingTreeEvilMissionTimer (+0xF58).
// Strings: "WatchForMissionEnd" 0x012D2328, "MonitorRegionPresence" 0x012D2310, "" 0x0122D70E, "BowerstoneJail" 0x012C7788,
//   "HTE_PartySpawnPoint" 0x012D2274, "HUD_CLOCK_ICON" 0x012CA7C8, "HeroDualArea" 0x012D2300.
// Layout (retail = Ego_r.pdb - 0x14): BanditsStarted +0x49, MissionSucceeded +0x4c, MissionFailed +0x4d, SpawnHere +0x4f,
//   SwitchingRegions +0x51, HTE_State +0x60, CurrentBandits +0x64, TimerGUIIndex +0x6c, MissionTimer +0xa4.
// Shape: spawn model of CQ_MinionCampScript::PhaseOne 0x00DA05D0; CScriptThing model of the landed
//   CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80 (inline destructor).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CBaseClass { public:                                     // polymorphic root of the script handles
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                  // out of line 0x004AA840
};
class CRGBColour { public:                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CSpawnedFuncBase { public:                               // retail 0x34 bytes
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

class CTimer;
class CGameScriptInterface { public:                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
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
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c) RegisterTimer
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160) DeregisterTimer
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164) SetTimer
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168) GetTimer
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146();
    virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
    virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170();
    virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
    virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186();
    virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194();
    virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202();
    virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210();
    virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218();
    virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226();
    virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234();
    virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242();
    virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250();
    virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258();
    virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266();
    virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274();
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282();
    virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290();
    virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298();
    virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306();
    virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
    virtual void DisplayQuestInfo(bool show) const;   // slot 321 (+0x504) DisplayQuestInfo
    virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325();
    virtual long AddQuestInfoTimer(const CTimer& timer, const CCharString& icon, float scale) const;   // slot 326 (+0x518) AddQuestInfoTimer
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
class CScriptDef { public:
    unsigned char _pad[0xf58];
    long HangingTreeEvilMissionTimer;                   // +0xF58
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool IntroCutscenePlayed;                           // +0x48
    bool BanditsStarted;                                // +0x49
    bool BanditKilled;                                  // +0x4a
    bool PartySuccessfullySetUp;                        // +0x4b
    bool MissionSucceeded;                              // +0x4c
    bool MissionFailed;                                 // +0x4d
    bool BanditsLeftRegion;                             // +0x4e
    bool SpawnHere;                                     // +0x4f
    bool AtWayPoint;                                    // +0x50
    bool SwitchingRegions;                              // +0x51
    unsigned char _pad52[0x5c - 0x52];
    CCharString ReasonForFailing;                       // +0x5c
    long HTE_State;                                     // +0x60
    long CurrentBandits;                                // +0x64
    long BanditsAtNextRegion;                           // +0x68
    long TimerGUIIndex;                                 // +0x6c
    unsigned char _pad70[0xa4 - 0x70];
    CTimer MissionTimer;                                // +0xa4
    void DoMission();
    void WatchForMissionEnd();                          // 0x00D6C3A0
    void SpawnParty(long count, CScriptThing& spawnPoint);   // 0x00D6C4A0
    void MonitorRegionPresence();                       // 0x00D6CDC0
    void Introduction();                                // 0x00D6D080
    void Conclusion();                                  // 0x00D6D670
};
void CQ_HangingTreeEvilScript::DoMission()
{
    SpawnFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("WatchForMissionEnd", 0, this, &CQ_HangingTreeEvilScript::WatchForMissionEnd));
    SpawnFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("MonitorRegionPresence", 0, this, &CQ_HangingTreeEvilScript::MonitorRegionPresence));
    while (!Game->IsLevelLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    HTE_State = 1;
    do {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (HTE_State == 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            while (SwitchingRegions) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (SpawnHere) {
                CScriptThing spawnPoint = Game->GetThingWithScriptName("HTE_PartySpawnPoint");
                SpawnParty(CurrentBandits, spawnPoint);
                Introduction();
            }
            while (!BanditsStarted) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            MissionTimer.SetTime(pGlobalScriptDef->HangingTreeEvilMissionTimer);
            TimerGUIIndex = Game->AddQuestInfoTimer(MissionTimer, "HUD_CLOCK_ICON", 1.0f);
            Game->DisplayQuestInfo(true);
            HTE_State = 2;
        }
        if (IsActiveThreadTerminating())
            return;
        while (HTE_State == 2) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsLevelLoaded("HeroDualArea")) {
                if (IsActiveThreadTerminating())
                    return;
                HTE_State = 6;
            }
        }
        if (IsActiveThreadTerminating())
            return;
    } while (HTE_State != 6);
    if (IsActiveThreadTerminating())
        return;
    Conclusion();
    if (MissionFailed)
        return;
    if (IsActiveThreadTerminating())
        return;
    MissionSucceeded = true;
}
}