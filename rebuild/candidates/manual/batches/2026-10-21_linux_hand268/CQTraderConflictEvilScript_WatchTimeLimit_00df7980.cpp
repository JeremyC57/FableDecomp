// CQ_TraderConflictEvilScript::WatchTimeLimit() -- start the escort time limit once the quest data says so, show it as a quest-info timer until the mission succeeds, and flag the data if time remained
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF7980, length 412 bytes (padded to 416).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; CCharString(const char*, long) @ 0x0099EBF0 and
//   ~CCharString @ 0x0099EAE0; CTimer::~CTimer @ 0x00CD4470 (the one out-of-line destructor site); through the global
//   game interface 0x0143E8F8: RegisterTimer/DeregisterTimer/SetTimer/GetTimer (slots 87-90); through the script's own
//   interface (+0x40): NewScriptFrame (slot 7), DisplayQuestInfo (321), AddQuestInfoTimer (326),
//   RemoveQuestInfoElement (338).
// Identity: spawned (function pointer stored at 0x00DF6615) from the Evil script's Main; its twin 0x00DFAEB0 is the
//   Good script's copy (data flag +0x132 instead of +0x131).
// Layout (retail = Ego_r PDB -0x18 after the 12-byte retail AllCreatures vector): MissionSucceeded +0x68. The quest
//   data (+0x44) flags +0x129 / +0x131 and the script-def time limit (+0xF80 of the def at 0x0143E90C) are unnamed.
// Shape: a CTimer local whose inline destructor deregisters the timer at every return; the first wait loop also ends
//   when the 5-second timer runs out (then a terminate check whose two arms both just return). The "HUD_CLOCK_ICON"
//   literal is passed straight to the `const CCharString&` parameter: the implicit conversion temporary is built before
//   the interface pointer is loaded (an explicit CCharString(...) temporary interleaves with the pushes).
struct CCharString {
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CTimer;
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void SetTimer(long timer, long seconds);   // slot 89 (+0x164)
    virtual long GetTimer(long timer);   // slot 90 (+0x168)
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
    virtual void DisplayQuestInfo(bool display);   // slot 321 (+0x504)
    virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325();
    virtual long AddQuestInfoTimer(const CTimer& timer, const CCharString& icon, float scale);   // slot 326 (+0x518)
    virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337();
    virtual void RemoveQuestInfoElement(long element);   // slot 338 (+0x548)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }   // 0x00CD4470 where not inlined
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};
struct CScriptDef {
    unsigned char _pad[0xf80];
    long TraderConflictTimeLimit;                       // +0xF80 (unnamed in the PDB view)
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CQ_TraderConflictData {
    unsigned char _pad[0x129];
    bool TimeLimitBeaten;                               // +0x129
    unsigned char _pad12a[7];
    bool StartTimeLimit;                                // +0x131
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQ_TraderConflictEvilScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    CQ_TraderConflictData* Data;                        // +0x44
    unsigned char _pad48[0x20];                         // AllCreatures +0x48 .. NextTimeToSpawnGuards +0x64
    bool MissionSucceeded;                              // +0x68
    void WatchTimeLimit();
};

void CQ_TraderConflictEvilScript::WatchTimeLimit()
{
    CTimer timer;
    timer.SetTime(5);
    while (!Data->StartTimeLimit) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            return;
        }
    }
    timer.SetTime(g_pScriptDef->TraderConflictTimeLimit);
    long element = Game->AddQuestInfoTimer(timer, "HUD_CLOCK_ICON", 1.0f);
    Game->DisplayQuestInfo(true);
    while (!MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(element);
    if (timer.GetTime() > 0) {
        if (IsActiveThreadTerminating())
            return;
        Data->TimeLimitBeaten = true;
    }
}
