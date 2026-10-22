// CTraderScript::HandleTrader(long state) -- an infected-village trader entity script's state hook (the catalog class
//   CCreatureAction_TrollWhackGroundBase is a propagated label): remember the state; on state 2 make the trader
//   follow the hero (and the hero's region), show the quest info and, by the trader's data string, add its health bar
//   (INFECTED: plain trader icon and start the infection timer, longer when the parent's flag +0x5a is set; SCARED:
//   hat 01 icon and set scared; FRIENDLY: hat 02 icon)
// Leaf: HandleTrader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0A510, length 784 bytes, `ret 4`.
// Callees: IsInterrupted 0x00F35B30 (CEntityScriptBase), the game script interface (+4) slots 70/229/321/325/480/496
//   (ghidra_out/cgsi_slots_aligned.tsv), CScriptThing slot 3 GetDataString, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   Compare 0x00411570 (__fastcall), the timer service g_pThingManager 0x0143E8F8 slot 89 SetTimer, quest globals
//   g_pScriptDef 0x0143E90C (+0xdf4 / +0xdf8).
// Layout (retail bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, +0x1c mode, +0x20
//   state, +0x2c timer index, +0x30 health bar id; parent flag +0x5a.
#include <string.h>
struct CCharStringData {
    char*         PData;
    unsigned long Length;
};
long __fastcall Compare(const char* a, const char* b);       // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return Compare(PStringData->PData, text) == 0;
    }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CScriptThing {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CCharString GetDataString() const;               // slot 3 (+0xc)
    void* PImp_Data;
    void* PImp_Info;
};
struct CGameScriptInterface_Calls {               // slots: ghidra_out/cgsi_slots_aligned.tsv
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182();
    virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198();
    virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206();
    virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222();
    virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228();
    virtual void EntityFollowThing(const CScriptThing& thing, const CScriptThing& target, float distance, bool a) const;   // slot 229 (+0x394)
    virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237();
    virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245();
    virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253();
    virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261();
    virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269();
    virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277();
    virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285();
    virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293();
    virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301();
    virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309();
    virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317();
    virtual void v318(); virtual void v319(); virtual void v320();
    virtual void DisplayQuestInfo(bool show) const;   // slot 321 (+0x504)
    virtual void v322(); virtual void v323(); virtual void v324();
    virtual long AddQuestInfoBarHealth(const CScriptThing& thing, const CRGBColour& colour, const CCharString& name, float scale) const;   // slot 325 (+0x514)
    virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333();
    virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341();
    virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349();
    virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357();
    virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365();
    virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373();
    virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381();
    virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389();
    virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397();
    virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405();
    virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413();
    virtual void v414(); virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421();
    virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429();
    virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437();
    virtual void v438(); virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445();
    virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453();
    virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461();
    virtual void v462(); virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469();
    virtual void v470(); virtual void v471(); virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477();
    virtual void v478(); virtual void v479();
    virtual void SetEntityAsRegionFollowing(const CScriptThing& thing, const CScriptThing& leader, bool following) const;   // slot 480 (+0x780)
    virtual void v481(); virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486(); virtual void v487(); virtual void v488();
    virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493(); virtual void v494(); virtual void v495();
    virtual void EntitySetAsScared(const CScriptThing& thing, bool scared) const;   // slot 496 (+0x7c0)
};
struct CTimerService {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual void SetTimer(long index, long time);            // slot 89 (+0x164)
};
extern CTimerService* g_pThingManager_0143e8f8;              // 0x0143E8F8
struct CScriptDef {
    unsigned char _pad000[0xdf4];
    long          TraderInfectionTime;                       // +0xdf4
    long          TraderInfectionTimeLong;                   // +0xdf8
};
extern CScriptDef* g_pScriptDef;                             // 0x0143E90C
struct CTraderParent {
    unsigned char _pad00[0x5a];
    bool          LongInfection;                             // +0x5a
};
struct CTraderScript {
    void*                       __vftable;                   // +0x00
    CGameScriptInterface_Calls* Game;                        // +0x04
    CScriptThing                Me;                          // +0x08
    CTraderParent*              ParentClass;                 // +0x14
    long                        _18;                         // +0x18
    long                        Mode;                        // +0x1c
    long                        State;                       // +0x20
    long                        _24, _28;                    // +0x24
    long                        Timer;                       // +0x2c
    long                        HealthBar;                   // +0x30
    bool IsInterrupted();                                    // 0x00F35B30
    void HandleTrader(long state);
};

void CTraderScript::HandleTrader(long state)
{
    State = state;
    if (state != 2)
        return;
    if (IsInterrupted())
        return;
    Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->DisplayQuestInfo(true);
    if (Me.GetDataString() == "INFECTED") {
        if (IsInterrupted())
            return;
        HealthBar = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER", 1.0f);
        Mode = 1;
        if (ParentClass->LongInfection) {
            if (IsInterrupted())
                return;
            g_pThingManager_0143e8f8->SetTimer(Timer, g_pScriptDef->TraderInfectionTimeLong);
        } else {
            if (IsInterrupted())
                return;
            g_pThingManager_0143e8f8->SetTimer(Timer, g_pScriptDef->TraderInfectionTime);
        }
    } else if (Me.GetDataString() == "SCARED") {
        if (IsInterrupted())
            return;
        HealthBar = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER_HAT_01", 1.0f);
        Game->EntitySetAsScared(Me, true);
    } else if (Me.GetDataString() == "FRIENDLY") {
        if (IsInterrupted())
            return;
        HealthBar = Game->AddQuestInfoBarHealth(Me, CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_TRADER_HAT_02", 1.0f);
    }
}