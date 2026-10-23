// Grade: DIFFER(1097v1103) -- Knothole guard bool set as mov0/jne/mov1 v retail jne/mov1/jmp/mov0; IsCameraPosOnScreen loop exits laid out in the body (timer-dtor tails moved to the end); one mov edi hoisted
// CM_AssassinTrigger::Main() -- an assassin-attack trigger marker: unless the hero is in Knothole Glade on the ransom quest,
//   wait for the hero to come within the script-def trigger distance of Me; then repeatedly wait for the marker (named by
//   Me's data string) to leave the screen and for the off-screen timer to run out; when the hero's region still has no
//   assassin and is not the current region, spawn an AttackingAssassin at the marker and give it the next carried item
// Leaf: ?Main@CM_AssassinTrigger@
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_AssassinAttacksScript: next item +0x4c, current
//   region +0x54, RegionInfo[5] +0x58 as the landed GetRegionIdForRegion 0x00E38290), PMasterData +0x18, region name
//   +0x1c, region id +0x20; CTimer as the landed CHS_Screamer::Main 0x00D83FA0; g_pScriptDef 0x0143E90C.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    bool operator!=(const CCharString& other) const;   // 0x0099E900
};
struct CScriptThingImpl {
    virtual void i00(); virtual void i01(); virtual void i02(); virtual void i03(); virtual void i04(); virtual void i05();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
};
extern const C3DVector g_ZeroPos;                       // 0x0143E8E0
struct CCPPointerInfo { long RefCount; };
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
struct CScriptThing : CBase {                           // vtable 0x01238C8C, 12 bytes
    CScriptThingImpl* Data;                             // +0x04 PImp
    CCPPointerInfo*   Info;                             // +0x08
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
    virtual void s01(); virtual void s02();
    virtual CCharString GetDataString() const;          // slot 3 (+0x0c) 0x004AA900
    __forceinline const C3DVector& PosOrZero() const { return Data == 0 ? g_ZeroPos : Data->GetPos(); }
};
static inline CCharString DataStringOf(const CScriptThing& thing) { return thing.GetDataString(); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006e7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString&) const;   // slot 12 (+0x30) 0x0088e300
    virtual void v013(); virtual void v014();
    virtual const CCharString& GetRegionName() const;   // slot 15 (+0x3c) 0x0088e340
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing & GetHero();   // slot 70 (+0x118) 0x00891ca0
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString&) const;   // slot 72 (+0x120) 0x008a7d60
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c) 0x006e79e0
    virtual void DeregisterTimer(long);   // slot 88 (+0x160) 0x006e7690
    virtual void SetTimer(long, long);   // slot 89 (+0x164) 0x006e76e0
    virtual long GetTimer(long) const;   // slot 90 (+0x168) 0x006e7710
    virtual CScriptThing CreateCreature(const CCharString&, C3DVector const &, const CCharString&, bool) const;   // slot 91 (+0x16c) 0x008a9100
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
    virtual bool IsQuestActive(const CCharString&) const;   // slot 284 (+0x470) 0x00892f40
    virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292();
    virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300();
    virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308();
    virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316();
    virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324();
    virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332();
    virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340();
    virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348();
    virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356();
    virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364();
    virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372();
    virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380();
    virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388();
    virtual void v389(); virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396();
    virtual void v397(); virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404();
    virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412();
    virtual void v413(); virtual void v414(); virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420();
    virtual bool IsCameraPosOnScreen(C3DVector const &) const;   // slot 421 (+0x694) 0x006e7300
    virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429();
    virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437();
    virtual void v438(); virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445();
    virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453();
    virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461();
    virtual void v462(); virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469();
    virtual void v470(); virtual void v471(); virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477();
    virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485();
    virtual void v486(); virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493();
    virtual void v494(); virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501();
    virtual void v502(); virtual void v503(); virtual void v504(); virtual void v505(); virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509();
    virtual void v510(); virtual void v511(); virtual void v512(); virtual void v513(); virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517();
    virtual void v518(); virtual void v519(); virtual void v520(); virtual void v521(); virtual void v522(); virtual void v523(); virtual void v524(); virtual void v525();
    virtual void v526(); virtual void v527(); virtual void v528(); virtual void v529(); virtual void v530(); virtual void v531(); virtual void v532(); virtual void v533();
    virtual void v534(); virtual void v535(); virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541();
    virtual void v542(); virtual void v543(); virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547(); virtual void v548(); virtual void v549();
    virtual void v550(); virtual void v551(); virtual void v552(); virtual void v553(); virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557();
    virtual void v558(); virtual void v559(); virtual void v560(); virtual void v561(); virtual void v562(); virtual void v563(); virtual void v564(); virtual void v565();
    virtual void v566(); virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573();
    virtual void v574(); virtual void v575(); virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581();
    virtual void v582(); virtual void v583(); virtual void v584();
    virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const;   // slot 585 (+0x924) 0x0089e780
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CScriptDef {
    unsigned char _pad[0x39c];
    long          AssassinTriggerDistance;              // +0x39C (descriptive)
    unsigned char _pad3a0[8];
    long          AssassinOffScreenTime;                // +0x3A8
    unsigned char _pad3ac[0x14];
    CCharString*  AssassinItemsFirst;                   // +0x3C0 (vector<CCharString>)
    CCharString*  AssassinItemsLast;                    // +0x3C4
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CTimer {                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer();                                           // 0x00CD4450
    ~CTimer();                                          // 0x00CD4470
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
struct CIsThingAlive {                                  // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                   // 0x00E241B0
};
struct RegionData {
    CCharString name;                                   // +0x00
    long        count;                                  // +0x04
    long        max;                                    // +0x08
    bool        assassinActive;                         // +0x0c
};
struct CV_AssassinAttacksScript {
    unsigned char _pad[0x4c];
    long          NextItem;                             // +0x4c
    unsigned char _pad50[4];
    CCharString   CurrentRegion;                        // +0x54
    RegionData    RegionInfo[5];                        // +0x58
    long GetRegionIdForRegion(CCharString region);      // 0x00E38290
};
struct CEntityScriptBase {                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    void RegisterFilter(CIsThingAlive& filter);         // 0x00F35B10
};
struct CM_AssassinTrigger : CEntityScriptBase {
    CV_AssassinAttacksScript* ParentClass;              // +0x14
    void*                     PMasterData;              // +0x18
    CCharString               Region;                   // +0x1c
    long                      RegionId;                 // +0x20
    void Main();
};

void CM_AssassinTrigger::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!Game->IsRegionLoaded("KnotholeGlade") || !Game->IsQuestActive("Q_RansomVictim")) {
    if (IsActiveThreadTerminating())
        return;
    CCharString name = DataStringOf(Me);
    CScriptThing marker = Game->GetThingWithScriptName(name);
    C3DVector pos = marker.PosOrZero();
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, (float)g_pScriptDef->AssassinTriggerDistance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    while (!IsActiveThreadTerminating()) {
        while (Game->IsCameraPosOnScreen(pos)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        timer.SetTime(g_pScriptDef->AssassinOffScreenTime);
        while (!Game->IsCameraPosOnScreen(pos) && timer.GetTime() != 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!Game->IsCameraPosOnScreen(pos))
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    Region = Game->GetRegionName();
    RegionId = ParentClass->GetRegionIdForRegion(Region);
    if (ParentClass->RegionInfo[RegionId].assassinActive)
        return;
    if (Region != ParentClass->CurrentRegion) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing assassin = Game->CreateCreature("CREATURE_ASSASSIN", pos, "AttackingAssassin", false);
        ParentClass->RegionInfo[RegionId].assassinActive = true;
        {
        CCharString item = g_pScriptDef->AssassinItemsFirst[ParentClass->NextItem];
        Game->AddItemToContainer(assassin, item);
        ParentClass->NextItem++;
        if ((unsigned long)ParentClass->NextItem >= (unsigned long)(g_pScriptDef->AssassinItemsLast - g_pScriptDef->AssassinItemsFirst))
            ParentClass->NextItem = 0;
        }
    }
    }
}
