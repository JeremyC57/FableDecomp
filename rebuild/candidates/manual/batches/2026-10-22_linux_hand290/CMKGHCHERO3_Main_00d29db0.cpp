// CMK_GHC_HERO3::Main() (0x00D29DB0) -- Gateway to Hook Coast, hero marker 3: fade in; once, pass the guildmaster's
//   "find the abbey" message; unless the abbey cut-scene already played, set the Hook Coast objective, wait until the hero
//   is within 15 of Me, take the hero under scripted control and play CS_GATEWAY_ABBEY in a paused movie sequence; then add
//   log-book story entry 250, mark the cut-scene played, remove Me's mini-map marker and set the next objective
// Leaf: ?Main@CMK_GHC_HERO3@
// Retail: Fable.exe @ 0x00D29DB0, 963 bytes (+ int3 padding), plain `ret`.
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0, IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall), scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / IsScripting 0x00CD23B9 / Release
//   0x00CD2770, CBaseIntelligentPointer ctor 0x0099A380 / operator= 0x0099A3B0 / dtor 0x0099A430, the handle's
//   CCountedPointer operator= 0x008AB1E0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie PImp
//   dtor 0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall), AddLogbookStoryEntry 0x00CBE87F (__fastcall); GameCalls
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 274 HeroReceiveMessageFromGuildMaster, 296
//   SetQuestCardObjective, 352 MiniMapRemoveMarker, 370 StartMovieSequence, 374 FadeScreenIn, 379
//   PauseAllNonScriptedEntities, 655 GetActiveQuestName.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14 (flags +0x50 abbey cut-scene played, +0x53 message
//   given), PMasterData +0x18 (flag +0x60). Handle / map / movie classes as the landed CBowerstoneIntroGuard::Main
//   0x00D246C0, except that this TU calls the handle's ctor and dtor out of line.
// Shape: the movie handle is a destructor-less view destroyed by hand (Bowerstone idiom): retail expands its destructor
//   without the 0x01260EF4 vptr store (PImp dtor 0x006E7AB0, then the base vptr 0x0126008C and 0x0099A430), and VC7.1
//   stores that vptr in a user-written destructor and calls the implicit one out of line here.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
struct CCPPointerInfo {                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
struct CScriptThingImpl;
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
struct CScriptThing : CBase {                           // vtable 0x01238C8C, 12 bytes
    
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    __forceinline ~CScriptThing() {}                    // inline here; out-of-line copy 0x004AA840
};
struct CBaseObject {                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
    CBaseObject();                                      // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);   // 0x0099A3B0
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CHandlePImp {
    void*           Data;
    CCPPointerInfo* Info;
    CHandlePImp& operator=(const CHandlePImp& other);   // 0x008AB1E0
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CHandlePImp PImp;                                   // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
struct CScriptGameResourceObjectMovieBase : CBaseObject {   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
void __fastcall AddLogbookStoryEntry(long entry);       // 0x00CBE87F
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CGameScriptInterface {                           // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026();
    virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172();
    virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
    virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
    virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196();
    virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202();
    virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208();
    virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220();
    virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226();
    virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232();
    virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238();
    virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244();
    virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250();
    virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256();
    virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262();
    virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268();
    virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273();
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString& speaker, bool a, bool b) const;   // slot 274 (+0x448)
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280();
    virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286();
    virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292();
    virtual void v293(); virtual void v294(); virtual void v295();
    virtual void SetQuestCardObjective(const CCharString& quest, const CCharString& text, const CCharString& region1, const CCharString& region2) const;   // slot 296 (+0x4a0)
    virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302();
    virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308();
    virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
    virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326();
    virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332();
    virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338();
    virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344();
    virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350();
    virtual void v351();
    virtual void MiniMapRemoveMarker(const CScriptThing& thing) const;   // slot 352 (+0x580)
    virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358();
    virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364();
    virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369();
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void v371(); virtual void v372(); virtual void v373();
    virtual void FadeScreenIn();   // slot 374 (+0x5d8)
    virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378();
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec)
    virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385();
    virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391();
    virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397();
    virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403();
    virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409();
    virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415();
    virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421();
    virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427();
    virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433();
    virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439();
    virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445();
    virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451();
    virtual void v452(); virtual void v453(); virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457();
    virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461(); virtual void v462(); virtual void v463();
    virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469();
    virtual void v470(); virtual void v471(); virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475();
    virtual void v476(); virtual void v477(); virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481();
    virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486(); virtual void v487();
    virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493();
    virtual void v494(); virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499();
    virtual void v500(); virtual void v501(); virtual void v502(); virtual void v503(); virtual void v504(); virtual void v505();
    virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510(); virtual void v511();
    virtual void v512(); virtual void v513(); virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517();
    virtual void v518(); virtual void v519(); virtual void v520(); virtual void v521(); virtual void v522(); virtual void v523();
    virtual void v524(); virtual void v525(); virtual void v526(); virtual void v527(); virtual void v528(); virtual void v529();
    virtual void v530(); virtual void v531(); virtual void v532(); virtual void v533(); virtual void v534(); virtual void v535();
    virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541();
    virtual void v542(); virtual void v543(); virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547();
    virtual void v548(); virtual void v549(); virtual void v550(); virtual void v551(); virtual void v552(); virtual void v553();
    virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557(); virtual void v558(); virtual void v559();
    virtual void v560(); virtual void v561(); virtual void v562(); virtual void v563(); virtual void v564(); virtual void v565();
    virtual void v566(); virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571();
    virtual void v572(); virtual void v573(); virtual void v574(); virtual void v575(); virtual void v576(); virtual void v577();
    virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581(); virtual void v582(); virtual void v583();
    virtual void v584(); virtual void v585(); virtual void v586(); virtual void v587(); virtual void v588(); virtual void v589();
    virtual void v590(); virtual void v591(); virtual void v592(); virtual void v593(); virtual void v594(); virtual void v595();
    virtual void v596(); virtual void v597(); virtual void v598(); virtual void v599(); virtual void v600(); virtual void v601();
    virtual void v602(); virtual void v603(); virtual void v604(); virtual void v605(); virtual void v606(); virtual void v607();
    virtual void v608(); virtual void v609(); virtual void v610(); virtual void v611(); virtual void v612(); virtual void v613();
    virtual void v614(); virtual void v615(); virtual void v616(); virtual void v617(); virtual void v618(); virtual void v619();
    virtual void v620(); virtual void v621(); virtual void v622(); virtual void v623(); virtual void v624(); virtual void v625();
    virtual void v626(); virtual void v627(); virtual void v628(); virtual void v629(); virtual void v630(); virtual void v631();
    virtual void v632(); virtual void v633(); virtual void v634(); virtual void v635(); virtual void v636(); virtual void v637();
    virtual void v638(); virtual void v639(); virtual void v640(); virtual void v641(); virtual void v642(); virtual void v643();
    virtual void v644(); virtual void v645(); virtual void v646(); virtual void v647(); virtual void v648(); virtual void v649();
    virtual void v650(); virtual void v651(); virtual void v652(); virtual void v653(); virtual void v654();
    virtual CCharString GetActiveQuestName() const;   // slot 655 (+0xa3c)
};
struct CEntityScriptBase {                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CQ_GatewayToHookCoastScript {
    unsigned char _pad[0x50];
    bool          AbbeyCutscenePlayed;                  // +0x50
    unsigned char _pad51[2];
    bool          MotherMessageGiven;                   // +0x53
};
struct CGatewayMasterData {
    unsigned char _pad[0x60];
    bool          AbbeyCutscenePlayed;                  // +0x60
};
struct CMK_GHC_HERO3 : CEntityScriptBase {
    CQ_GatewayToHookCoastScript* ParentClass;           // +0x14
    CGatewayMasterData*          PMasterData;           // +0x18
    void Main();
};

void CMK_GHC_HERO3::Main()
{
    Game->FadeScreenIn();
    if (!ParentClass->MotherMessageGiven) {
        if (IsActiveThreadTerminating())
            return;
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_FIND_ABBEY_10", "", true, true);
        ParentClass->MotherMessageGiven = true;
    }
    if (ParentClass->AbbeyCutscenePlayed)
        return;
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GATEWAY_TO_HOOK_COAST_OBJECTIVE_02", "HookCoast", "HookCoast");
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    {
        CScriptGameResourceObjectScriptedThingBase hero;
        if (hero.IsScripting())
            hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CCutsceneCastMap cast;
        cast["HERO"] = hero;
        {
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro("CS_GATEWAY_ABBEY", cast, 0, 0, false, true);
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyInline();
        }
    }
    AddLogbookStoryEntry(250);
    ParentClass->AbbeyCutscenePlayed = true;
    PMasterData->AbbeyCutscenePlayed = true;
    Game->MiniMapRemoveMarker(Me);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GATEWAY_TO_HOOK_COAST_OBJECTIVE_03", "HeroGuildComplexInside", "HeroGuildComplexInside");
}

