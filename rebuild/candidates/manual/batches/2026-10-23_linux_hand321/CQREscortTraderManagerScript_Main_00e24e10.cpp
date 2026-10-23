// CQR_EscortTrader_ManagerScript::Main() -- the repeatable trader-escort manager: on first run reset the counter and phase; then each frame run the phase machine: activate an escort (phases 0/2/4/6), or wait for the running escort to finish and for the gating quest (Q_TraderEscort / Q_BanditCamp / Q_Arena (+ activate the Orchard Farm barricade quest) / Q_OpeningGraveyardSecretPassage) to be completed before moving on; in the last phase keep activating an escort whenever the previous one has ended, the hero is away from LookoutPoint, the day is a quest day and the start time has passed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E24E10, length 1972 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, CQR_EscortTrader_ManagerScript::Activate 0x00E255D0 (landed); g_pScriptDef 0x0143E90C; the game script
//   interface (+0x40) slots 7, 12, 276, 284, 286, 647, 651.
// Layout (retail = Ego_r.pdb - 0x14): OrchardFarmTraderEscortCounter +0x48, DebugCounter +0x4c, Activated +0x50,
//   OrchardFarmTraderEscortPhase +0x54; CScriptDef (retail = PDB - 0xa0 here): EscortTraderQuestDayInterval +0x9d4,
//   EscortTraderQuestStartTime +0x9d8.
// Shape: the day/time tests go through inline helpers that take the CScriptDef pointer, so g_pScriptDef is loaded into
//   callee-saved edi before each interface call; that makes edi a saved register, which VC then also uses as the zero
//   register (xor edi,edi) for the flag word, the first-run stores and the phase-0 test.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
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
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156();
    virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164();
    virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172();
    virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180();
    virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188();
    virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196();
    virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204();
    virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212();
    virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220();
    virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228();
    virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236();
    virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244();
    virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252();
    virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260();
    virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268();
    virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275();
    virtual void ActivateQuest(const CCharString& quest) const;   // slot 276 (+0x450) ActivateQuest
    virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283();
    virtual bool IsQuestActive(const CCharString& quest) const;   // slot 284 (+0x470) IsQuestActive
    virtual void v285();
    virtual bool IsQuestCompleted(const CCharString& quest) const;   // slot 286 (+0x478) IsQuestCompleted
    virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302();
    virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310();
    virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318();
    virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326();
    virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342();
    virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350();
    virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358();
    virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366();
    virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374();
    virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382();
    virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390();
    virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398();
    virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406();
    virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414();
    virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422();
    virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430();
    virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438();
    virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446();
    virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453(); virtual void v454();
    virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461(); virtual void v462();
    virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470();
    virtual void v471(); virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477(); virtual void v478();
    virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486();
    virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493(); virtual void v494();
    virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501(); virtual void v502();
    virtual void v503(); virtual void v504(); virtual void v505(); virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510();
    virtual void v511(); virtual void v512(); virtual void v513(); virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517(); virtual void v518();
    virtual void v519(); virtual void v520(); virtual void v521(); virtual void v522(); virtual void v523(); virtual void v524(); virtual void v525(); virtual void v526();
    virtual void v527(); virtual void v528(); virtual void v529(); virtual void v530(); virtual void v531(); virtual void v532(); virtual void v533(); virtual void v534();
    virtual void v535(); virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541(); virtual void v542();
    virtual void v543(); virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547(); virtual void v548(); virtual void v549(); virtual void v550();
    virtual void v551(); virtual void v552(); virtual void v553(); virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557(); virtual void v558();
    virtual void v559(); virtual void v560(); virtual void v561(); virtual void v562(); virtual void v563(); virtual void v564(); virtual void v565(); virtual void v566();
    virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573(); virtual void v574();
    virtual void v575(); virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581(); virtual void v582();
    virtual void v583(); virtual void v584(); virtual void v585(); virtual void v586(); virtual void v587(); virtual void v588(); virtual void v589(); virtual void v590();
    virtual void v591(); virtual void v592(); virtual void v593(); virtual void v594(); virtual void v595(); virtual void v596(); virtual void v597(); virtual void v598();
    virtual void v599(); virtual void v600(); virtual void v601(); virtual void v602(); virtual void v603(); virtual void v604(); virtual void v605(); virtual void v606();
    virtual void v607(); virtual void v608(); virtual void v609(); virtual void v610(); virtual void v611(); virtual void v612(); virtual void v613(); virtual void v614();
    virtual void v615(); virtual void v616(); virtual void v617(); virtual void v618(); virtual void v619(); virtual void v620(); virtual void v621(); virtual void v622();
    virtual void v623(); virtual void v624(); virtual void v625(); virtual void v626(); virtual void v627(); virtual void v628(); virtual void v629(); virtual void v630();
    virtual void v631(); virtual void v632(); virtual void v633(); virtual void v634(); virtual void v635(); virtual void v636(); virtual void v637(); virtual void v638();
    virtual void v639(); virtual void v640(); virtual void v641(); virtual void v642(); virtual void v643(); virtual void v644(); virtual void v645(); virtual void v646();
    virtual long GetTimeOfDay() const;   // slot 647 (+0xa1c) GetTimeOfDay
    virtual void v648(); virtual void v649(); virtual void v650();
    virtual long GetDayOfWeek() const;   // slot 651 (+0xa2c) GetDayOfWeek
};
struct CScriptDef {
    unsigned char _pad0[0x9d4];
    long EscortTraderQuestDayInterval;                  // +0x9d4
    long EscortTraderQuestStartTime;                    // +0x9d8
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
enum EscortTraderManagerPhase {
    PHASE_ACTIVATE_FIRST, PHASE_WAIT_TRADER_ESCORT, PHASE_ACTIVATE_SECOND, PHASE_WAIT_BANDIT_CAMP, PHASE_ACTIVATE_THIRD,
    PHASE_WAIT_ARENA, PHASE_ACTIVATE_FOURTH, PHASE_WAIT_GRAVEYARD, PHASE_REPEAT
};
static __forceinline long DayModInterval(long day, const CScriptDef* def) { return day % def->EscortTraderQuestDayInterval; }
static __forceinline bool NotPastStartTime(long time, const CScriptDef* def) { return time <= def->EscortTraderQuestStartTime; }
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQR_EscortTrader_ManagerScript : CScriptBase {
    CGameScriptInterface*    Game;                      // +0x40
    void*                    Data;                      // +0x44
    long                     OrchardFarmTraderEscortCounter;   // +0x48
    long                     DebugCounter;              // +0x4c
    bool                     Activated;                 // +0x50
    EscortTraderManagerPhase OrchardFarmTraderEscortPhase;     // +0x54
    void Main();
    void Activate();                                    // 0x00E255D0
};
void CQR_EscortTrader_ManagerScript::Main()
{
    if (!Activated) {
        if (IsActiveThreadTerminating())
            return;
        OrchardFarmTraderEscortCounter = 0;
        OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_FIRST;
        Activated = true;
    }
    while (!IsActiveThreadTerminating()) {
        if (OrchardFarmTraderEscortPhase == PHASE_ACTIVATE_FIRST) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            OrchardFarmTraderEscortPhase = PHASE_WAIT_TRADER_ESCORT;
        } else if (OrchardFarmTraderEscortPhase == PHASE_WAIT_TRADER_ESCORT) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsQuestActive("QR_EscortTrader") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsQuestCompleted("Q_TraderEscort")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_SECOND;
        } else if (OrchardFarmTraderEscortPhase == PHASE_ACTIVATE_SECOND) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            OrchardFarmTraderEscortPhase = PHASE_WAIT_BANDIT_CAMP;
        } else if (OrchardFarmTraderEscortPhase == PHASE_WAIT_BANDIT_CAMP) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsQuestActive("QR_EscortTrader") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsQuestCompleted("Q_BanditCamp")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_THIRD;
        } else if (OrchardFarmTraderEscortPhase == PHASE_ACTIVATE_THIRD) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            OrchardFarmTraderEscortPhase = PHASE_WAIT_ARENA;
        } else if (OrchardFarmTraderEscortPhase == PHASE_WAIT_ARENA) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsQuestActive("QR_EscortTrader") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsQuestCompleted("Q_Arena")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->ActivateQuest("Q_OrchardFarm_Barricade");
            OrchardFarmTraderEscortPhase = PHASE_ACTIVATE_FOURTH;
        } else if (OrchardFarmTraderEscortPhase == PHASE_ACTIVATE_FOURTH) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            OrchardFarmTraderEscortPhase = PHASE_WAIT_GRAVEYARD;
        } else if (OrchardFarmTraderEscortPhase == PHASE_WAIT_GRAVEYARD) {
            if (IsActiveThreadTerminating())
                return;
            while (Game->IsQuestActive("QR_EscortTrader") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsQuestCompleted("Q_OpeningGraveyardSecretPassage")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            OrchardFarmTraderEscortPhase = PHASE_REPEAT;
        } else if (OrchardFarmTraderEscortPhase == PHASE_REPEAT) {
            if (IsActiveThreadTerminating())
                return;
            Activate();
            if (IsActiveThreadTerminating())
                return;
            for (;;) {
                while (Game->IsQuestActive("QR_EscortTrader") != false) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                while (Game->IsRegionLoaded("LookoutPoint")
                       || DayModInterval(Game->GetDayOfWeek(), g_pScriptDef) != 0
                       || NotPastStartTime(Game->GetTimeOfDay(), g_pScriptDef)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                Activate();
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        Game->NewScriptFrame();
    }
}
}
