// CScytheNearOracle::Main() -- Scythe waiting by the Oracle: take Me under scripted control, add the quest-core minimap marker and face the MK_OW_SCYTHE3 marker; then every frame: when the hero comes within 20 units and the 2-second timer has run out, face the hero; within 12 units, play CS_ORACLE_AWAKENS inside a paused movie sequence (Me cast as "SCYTHE"), flag the parent and remove Me; when the hero hits Me, speak the on-hit line and make Me and the hero allies
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2A320, 1503 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Literals "HUD_ORB_QUEST_CORE" 0x01258AB0, "MK_OW_SCYTHE3" 0x012C7D28, "" 0x0122D70E, "SCYTHE" 0x012CC544,
//   "CS_ORACLE_AWAKENS" 0x012E2414, "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_B03_SCYTHE_ON_HIT_RETURNED" 0x012E23F0.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, ~CIsThingAlive 0x00E241B0, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer 0x0099A380 / 0x0099A430, handle IsScripting 0x00CD23B9 /
//   Release 0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / operator=
//   0x008ABD10 / dtor 0x007E74D0, movie handle dtor 0x006E7B80 (counted-pointer member dtor 0x006E7AB0), CTimer dtor 0x00CD4470,
//   cast map operator[] 0x00CD3D2E, CQS_ScytheInfoScript::PlayCutscene 0x00E2A900, IsDistanceBetweenThingsUnder 0x00CBE2FF;
//   GameCalls slots 7, 8, 70, 72, 87-90 timers (through g_pGameScriptInterface 0x0143E8F8), 108, 264, 350, 352, 370, 379, 475, 599.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQS_ScytheInfoScript: oracle-awakened flag +0x48,
//   cutscene cast map +0x58).
// Shape: the CEndScythe (0x00D79D50) handle model; __forceinline CTimer ctor/dtor/accessors keep the timer index in ebp (home
//   slot) across the loop, inline_depth(0) in the hit block keeps the movie and CTimer destructors out of line there as retail does,
//   and the main loop is `for (;;) { if (IsActiveThreadTerminating()) return; ... }` so the loop exit keeps its own ~CTimer copy.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class C3DVector;
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const;
    virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const;
    virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    void* PImpData;                                     // +0x04
    void* PImpInfo;                                     // +0x08
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};
}
using NScript::CIsThingAlive;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
class CBaseObject { public:                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
struct CResourcePImp {                                  // the handles' counted implementation pointer
    void* Data;
    void* Info;
    CResourcePImp() : Data(0), Info(0) { }
    ~CResourcePImp();                                   // 0x006E7AB0
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    CScriptGameResourceObjectScriptedThingBase() { PImpData = 0; PImpInfo = 0; }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7390
public:
    virtual CScriptThing GetScriptThing() const;   // 0x007E7490
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4, 0x10 bytes
    CResourcePImp PImp;                                 // +0x08
    CScriptGameResourceObjectMovieBase() { }
    // implicit destructor; out-of-line copy 0x006E7B80
};
class CGameScriptInterface { public:
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const; virtual void v017() const; virtual void v018() const;
    virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const;
    virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const;
    virtual void v039() const; virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const;
    virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const;
    virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const;
    virtual void v069() const;
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071() const;
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const;
    virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const;
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168)
    virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const; virtual void v100() const;
    virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const; virtual void v107() const;
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    virtual void v109() const; virtual void v110() const; virtual void v111() const; virtual void v112() const; virtual void v113() const; virtual void v114() const; virtual void v115() const; virtual void v116() const; virtual void v117() const; virtual void v118() const;
    virtual void v119() const; virtual void v120() const; virtual void v121() const; virtual void v122() const; virtual void v123() const; virtual void v124() const; virtual void v125() const; virtual void v126() const; virtual void v127() const; virtual void v128() const;
    virtual void v129() const; virtual void v130() const; virtual void v131() const; virtual void v132() const; virtual void v133() const; virtual void v134() const; virtual void v135() const; virtual void v136() const; virtual void v137() const; virtual void v138() const;
    virtual void v139() const; virtual void v140() const; virtual void v141() const; virtual void v142() const; virtual void v143() const; virtual void v144() const; virtual void v145() const; virtual void v146() const; virtual void v147() const; virtual void v148() const;
    virtual void v149() const; virtual void v150() const; virtual void v151() const; virtual void v152() const; virtual void v153() const; virtual void v154() const; virtual void v155() const; virtual void v156() const; virtual void v157() const; virtual void v158() const;
    virtual void v159() const; virtual void v160() const; virtual void v161() const; virtual void v162() const; virtual void v163() const; virtual void v164() const; virtual void v165() const; virtual void v166() const; virtual void v167() const; virtual void v168() const;
    virtual void v169() const; virtual void v170() const; virtual void v171() const; virtual void v172() const; virtual void v173() const; virtual void v174() const; virtual void v175() const; virtual void v176() const; virtual void v177() const; virtual void v178() const;
    virtual void v179() const; virtual void v180() const; virtual void v181() const; virtual void v182() const; virtual void v183() const; virtual void v184() const; virtual void v185() const; virtual void v186() const; virtual void v187() const; virtual void v188() const;
    virtual void v189() const; virtual void v190() const; virtual void v191() const; virtual void v192() const; virtual void v193() const; virtual void v194() const; virtual void v195() const; virtual void v196() const; virtual void v197() const; virtual void v198() const;
    virtual void v199() const; virtual void v200() const; virtual void v201() const; virtual void v202() const; virtual void v203() const; virtual void v204() const; virtual void v205() const; virtual void v206() const; virtual void v207() const; virtual void v208() const;
    virtual void v209() const; virtual void v210() const; virtual void v211() const; virtual void v212() const; virtual void v213() const; virtual void v214() const; virtual void v215() const; virtual void v216() const; virtual void v217() const; virtual void v218() const;
    virtual void v219() const; virtual void v220() const; virtual void v221() const; virtual void v222() const; virtual void v223() const; virtual void v224() const; virtual void v225() const; virtual void v226() const; virtual void v227() const; virtual void v228() const;
    virtual void v229() const; virtual void v230() const; virtual void v231() const; virtual void v232() const; virtual void v233() const; virtual void v234() const; virtual void v235() const; virtual void v236() const; virtual void v237() const; virtual void v238() const;
    virtual void v239() const; virtual void v240() const; virtual void v241() const; virtual void v242() const; virtual void v243() const; virtual void v244() const; virtual void v245() const; virtual void v246() const; virtual void v247() const; virtual void v248() const;
    virtual void v249() const; virtual void v250() const; virtual void v251() const; virtual void v252() const; virtual void v253() const; virtual void v254() const; virtual void v255() const; virtual void v256() const; virtual void v257() const; virtual void v258() const;
    virtual void v259() const; virtual void v260() const; virtual void v261() const; virtual void v262() const; virtual void v263() const;
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
    virtual void v265() const; virtual void v266() const; virtual void v267() const; virtual void v268() const; virtual void v269() const; virtual void v270() const; virtual void v271() const; virtual void v272() const; virtual void v273() const; virtual void v274() const;
    virtual void v275() const; virtual void v276() const; virtual void v277() const; virtual void v278() const; virtual void v279() const; virtual void v280() const; virtual void v281() const; virtual void v282() const; virtual void v283() const; virtual void v284() const;
    virtual void v285() const; virtual void v286() const; virtual void v287() const; virtual void v288() const; virtual void v289() const; virtual void v290() const; virtual void v291() const; virtual void v292() const; virtual void v293() const; virtual void v294() const;
    virtual void v295() const; virtual void v296() const; virtual void v297() const; virtual void v298() const; virtual void v299() const; virtual void v300() const; virtual void v301() const; virtual void v302() const; virtual void v303() const; virtual void v304() const;
    virtual void v305() const; virtual void v306() const; virtual void v307() const; virtual void v308() const; virtual void v309() const; virtual void v310() const; virtual void v311() const; virtual void v312() const; virtual void v313() const; virtual void v314() const;
    virtual void v315() const; virtual void v316() const; virtual void v317() const; virtual void v318() const; virtual void v319() const; virtual void v320() const; virtual void v321() const; virtual void v322() const; virtual void v323() const; virtual void v324() const;
    virtual void v325() const; virtual void v326() const; virtual void v327() const; virtual void v328() const; virtual void v329() const; virtual void v330() const; virtual void v331() const; virtual void v332() const; virtual void v333() const; virtual void v334() const;
    virtual void v335() const; virtual void v336() const; virtual void v337() const; virtual void v338() const; virtual void v339() const; virtual void v340() const; virtual void v341() const; virtual void v342() const; virtual void v343() const; virtual void v344() const;
    virtual void v345() const; virtual void v346() const; virtual void v347() const; virtual void v348() const; virtual void v349() const;
    virtual void MiniMapAddMarker(const CScriptThing& thing, const CCharString& marker) const;   // slot 350 (+0x578)
    virtual void v351() const;
    virtual void MiniMapRemoveMarker(const CScriptThing& thing) const;   // slot 352 (+0x580)
    virtual void v353() const; virtual void v354() const; virtual void v355() const; virtual void v356() const; virtual void v357() const; virtual void v358() const; virtual void v359() const; virtual void v360() const; virtual void v361() const; virtual void v362() const;
    virtual void v363() const; virtual void v364() const; virtual void v365() const; virtual void v366() const; virtual void v367() const; virtual void v368() const; virtual void v369() const;
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void v371() const; virtual void v372() const; virtual void v373() const; virtual void v374() const; virtual void v375() const; virtual void v376() const; virtual void v377() const; virtual void v378() const;
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec)
    virtual void v380() const; virtual void v381() const; virtual void v382() const; virtual void v383() const; virtual void v384() const; virtual void v385() const; virtual void v386() const; virtual void v387() const; virtual void v388() const; virtual void v389() const;
    virtual void v390() const; virtual void v391() const; virtual void v392() const; virtual void v393() const; virtual void v394() const; virtual void v395() const; virtual void v396() const; virtual void v397() const; virtual void v398() const; virtual void v399() const;
    virtual void v400() const; virtual void v401() const; virtual void v402() const; virtual void v403() const; virtual void v404() const; virtual void v405() const; virtual void v406() const; virtual void v407() const; virtual void v408() const; virtual void v409() const;
    virtual void v410() const; virtual void v411() const; virtual void v412() const; virtual void v413() const; virtual void v414() const; virtual void v415() const; virtual void v416() const; virtual void v417() const; virtual void v418() const; virtual void v419() const;
    virtual void v420() const; virtual void v421() const; virtual void v422() const; virtual void v423() const; virtual void v424() const; virtual void v425() const; virtual void v426() const; virtual void v427() const; virtual void v428() const; virtual void v429() const;
    virtual void v430() const; virtual void v431() const; virtual void v432() const; virtual void v433() const; virtual void v434() const; virtual void v435() const; virtual void v436() const; virtual void v437() const; virtual void v438() const; virtual void v439() const;
    virtual void v440() const; virtual void v441() const; virtual void v442() const; virtual void v443() const; virtual void v444() const; virtual void v445() const; virtual void v446() const; virtual void v447() const; virtual void v448() const; virtual void v449() const;
    virtual void v450() const; virtual void v451() const; virtual void v452() const; virtual void v453() const; virtual void v454() const; virtual void v455() const; virtual void v456() const; virtual void v457() const; virtual void v458() const; virtual void v459() const;
    virtual void v460() const; virtual void v461() const; virtual void v462() const; virtual void v463() const; virtual void v464() const; virtual void v465() const; virtual void v466() const; virtual void v467() const; virtual void v468() const; virtual void v469() const;
    virtual void v470() const; virtual void v471() const; virtual void v472() const; virtual void v473() const; virtual void v474() const;
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing& thing, const CScriptThing& target, bool a) const;   // slot 475 (+0x76c)
    virtual void v476() const; virtual void v477() const; virtual void v478() const; virtual void v479() const; virtual void v480() const; virtual void v481() const; virtual void v482() const; virtual void v483() const; virtual void v484() const; virtual void v485() const;
    virtual void v486() const; virtual void v487() const; virtual void v488() const; virtual void v489() const; virtual void v490() const; virtual void v491() const; virtual void v492() const; virtual void v493() const; virtual void v494() const; virtual void v495() const;
    virtual void v496() const; virtual void v497() const; virtual void v498() const; virtual void v499() const; virtual void v500() const; virtual void v501() const; virtual void v502() const; virtual void v503() const; virtual void v504() const; virtual void v505() const;
    virtual void v506() const; virtual void v507() const; virtual void v508() const; virtual void v509() const; virtual void v510() const; virtual void v511() const; virtual void v512() const; virtual void v513() const; virtual void v514() const; virtual void v515() const;
    virtual void v516() const; virtual void v517() const; virtual void v518() const; virtual void v519() const; virtual void v520() const; virtual void v521() const; virtual void v522() const; virtual void v523() const; virtual void v524() const; virtual void v525() const;
    virtual void v526() const; virtual void v527() const; virtual void v528() const; virtual void v529() const; virtual void v530() const; virtual void v531() const; virtual void v532() const; virtual void v533() const; virtual void v534() const; virtual void v535() const;
    virtual void v536() const; virtual void v537() const; virtual void v538() const; virtual void v539() const; virtual void v540() const; virtual void v541() const; virtual void v542() const; virtual void v543() const; virtual void v544() const; virtual void v545() const;
    virtual void v546() const; virtual void v547() const; virtual void v548() const; virtual void v549() const; virtual void v550() const; virtual void v551() const; virtual void v552() const; virtual void v553() const; virtual void v554() const; virtual void v555() const;
    virtual void v556() const; virtual void v557() const; virtual void v558() const; virtual void v559() const; virtual void v560() const; virtual void v561() const; virtual void v562() const; virtual void v563() const; virtual void v564() const; virtual void v565() const;
    virtual void v566() const; virtual void v567() const; virtual void v568() const; virtual void v569() const; virtual void v570() const; virtual void v571() const; virtual void v572() const; virtual void v573() const; virtual void v574() const; virtual void v575() const;
    virtual void v576() const; virtual void v577() const; virtual void v578() const; virtual void v579() const; virtual void v580() const; virtual void v581() const; virtual void v582() const; virtual void v583() const; virtual void v584() const; virtual void v585() const;
    virtual void v586() const; virtual void v587() const; virtual void v588() const; virtual void v589() const; virtual void v590() const; virtual void v591() const; virtual void v592() const; virtual void v593() const; virtual void v594() const; virtual void v595() const;
    virtual void v596() const; virtual void v597() const; virtual void v598() const;
    virtual void EntitySetThingAsAllyOfThing(const CScriptThing& thing, const CScriptThing& other) const;   // slot 599 (+0x95c)
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    __forceinline CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
    __forceinline ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }   // out-of-line copy 0x00CD4470
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern const float g_fZero;                            // 0x0122DEDC (0.0f)
struct CCutsceneCastMap {
    unsigned char _rep[12];
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CQS_ScytheInfoScript {
    unsigned char _pad00[0x48];
    bool OracleAwakened;                                // +0x48
    unsigned char _pad49[0x58 - 0x49];
    CCutsceneCastMap Cast;                              // +0x58
    void PlayCutscene(CCharString name, bool a);        // 0x00E2A900
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CScytheNearOracle : CEntityScriptBase {
    CQS_ScytheInfoScript* ParentClass;                  // +0x14
    void*                 PMasterData;                  // +0x18
    void Main();
};

void CScytheNearOracle::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase face;
    if (face.IsScripting())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("MK_OW_SCYTHE3"), false);
    CTimer timer;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f) && timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            timer.SetTime(2);
        }
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 12.0f)) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Game->MiniMapRemoveMarker(Me);
            ParentClass->Cast["SCYTHE"] = face;
            ParentClass->PlayCutscene("CS_ORACLE_AWAKENS", false);
            ParentClass->OracleAwakened = true;
            Game->RemoveThing(Me, false, true);
            game->PauseAllNonScriptedEntities(false);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                face.Speak(Game->GetHero(), "TEXT_QST_B03_SCYTHE_ON_HIT_RETURNED", (ETextGroupSelectionMethod)2, false, true, false);
                while (face.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            }
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
        }
#pragma inline_depth()
        Game->NewScriptFrame();
    }
}