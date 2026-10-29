// NScript::CQ_MinionCampScript::RunChantTimer() -- run Briar Rose's 72-second chant timer: while time is left (or the timer is paused) advance a frame, hold the timer at PausedTime while ChantTimerPaused (and once more when the pause ends), and show the elapsed time on the chant timer bar
// Leaf: ?RunChantTimer@CQ_MinionCampScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA24F0, 276 bytes, thiscall; the trailing termination test is a tail `jmp`.
// Identity (proven): the row is referenced once, from 0x00DA1AD1 inside the landed CQ_MinionCampScript code, as the
//   CSpawnedFunc member pointer (+0x34) of a spawned thread (vtable 0x012D6CAC). Ego_r.pdb
//   ?RunChantTimer@CQ_MinionCampScript@NScript@@QAEXXZ (0x00DC95D0) follows ?PauseChantTimer@CQ_MinionCampScript@NScript@@QAEXXZ
//   (0x00DC9570, 0x60 bytes) exactly as retail follows the landed PauseChantTimer 0x00DA2490 (0x60 bytes); PauseChantTimer
//   records PausedTime and raises ChantTimerPaused, the two members this row consumes. Catalog label sub_00da24f0.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (Ego_r); the global script interface 0x0143E8F8 slots 89 SetTimer /
//   90 GetTimer (the inline CTimer operator= / operator>= / GetTime of Ego_r ??4CTimer@@QAEAAV0@J@Z, ??PCTimer@@QBE_NJ@Z,
//   ?GetTime@CTimer@@QBEJXZ); Game slots 7 NewScriptFrame and 332 UpdateQuestInfoBar.
// Layout: the landed PauseChantTimer 0x00DA2490 (retail = Ego_r - 0x14): ChantTimer +0x60, ChantTimerBar +0x64,
//   ChantTimerPaused +0x68, PausedTime +0x6c.
// Shape: the script WAIT/IF macro forms -- every arm opens with a termination test; `bool wasPaused` lives in bl; the
//   `||` loop test reads the timer first; -1.0f immediates for the two unused bar arguments. The arm order is
//   `if (!ChantTimerPaused) {..} else {..}` (retail lays the not-paused arm first; the positive test came out swapped, 272v276).
class CGameScriptInterfaceBase { public:                // the script interface (slots: ghidra_out/cgsi_slots_aligned.tsv, CGameScriptInterface vtable 0x01260F0C)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088();
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168)
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
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322();
    virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330();
    virtual void v331();
    virtual void UpdateQuestInfoBar(long id, float value, float a, float b) const;   // slot 332 (+0x530)
};
class CGameScriptInterface : public CGameScriptInterfaceBase { };
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface; no PDB data symbol)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    bool operator>=(long time) const { return GetTime() >= time; }
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
template <class TData> class CLevelScriptBase : public CScriptBase {   // Ego_r 0x5c: the quest's first member is at 0x5c
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_MinionCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestStartScreenShown;                         // +0x48
    bool IntroDone;                                     // +0x49
    bool BriarRoseAtAltar;                              // +0x4a
    bool MinionsSummoned;                               // +0x4b
    bool PortalSealed;                                  // +0x4c
    bool MissionFailed;                                 // +0x4d
    long NumberOfMinions;                               // +0x50
    long MaxNumberOfMinions;                            // +0x54
    float MinMinionSpawnPeriod;                         // +0x58
    float MaxMinionSpawnPeriod;                         // +0x5c
    CTimer ChantTimer;                                  // +0x60
    long ChantTimerBar;                                 // +0x64
    bool ChantTimerPaused;                              // +0x68
    long PausedTime;                                    // +0x6c
    void RunChantTimer();
};

void CQ_MinionCampScript::RunChantTimer()
{
    ChantTimer = 72;
    bool wasPaused = false;
    while (ChantTimer >= 0 || ChantTimerPaused) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!ChantTimerPaused) {
            if (IsActiveThreadTerminating())
                return;
            if (wasPaused) {
                if (IsActiveThreadTerminating())
                    return;
                ChantTimer = PausedTime;
                wasPaused = false;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            ChantTimer = PausedTime;
            wasPaused = true;
        }
        Game->UpdateQuestInfoBar(ChantTimerBar, (float)(72 - ChantTimer.GetTime()), -1.0f, -1.0f);
    }
    if (IsActiveThreadTerminating())
        return;
}
}   // namespace NScript