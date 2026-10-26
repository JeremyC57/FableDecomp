// CStopDrawing::Main() -- snow-troll rock pile: shut the nearest region entrance/exit; unless the troll intro already ran, take the rock pile under scripted control, loop its idle animation and wait until the hero comes within 15 units, hits it, or hits it with a special ability other than ability 14; then play CS_SNOWTROLL_INTRO, unhide Me on the minimap, flag the intro done and release the pile; finally show the ice-troll health bar and idle
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE36A0, 1151 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Literals "REGION_ENTRANCE_POINT" 0x012CA3DC, "REGION_EXIT_POINT" 0x012CA510, "SCRIPT_ROCK_PILE" 0x012F461C,
//   "SCRIPT_NAME_HERO" 0x0125D1C8, "CS_SNOWTROLL_INTRO" 0x012F4608, "HUD_QUEST_ICON_ICE_TROLL" 0x012F45EC.
// Callees: CScriptThing copy 0x004ABE90 (filter member), ~CIsThingAlive 0x00E241B0 (out of line), ~CBase 0x0099A2E0,
//   RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC, CCharString 0x0099EBF0 /
//   0x0099EAE0, handle IsScripting 0x00CD23B9 / Release 0x00CD2770, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall),
//   parent PlayCutscene(CCharString) 0x00EE2DB0; GameCalls slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero,
//   81 GetNearestWithDefName, 321 DisplayQuestInfo, 325 AddQuestInfoBarHealth, 356 EntitySetAsHiddenOnMiniMap,
//   477 SetThingPersistent, 567 SetRegionExitAsActive, 568 SetRegionEntranceAsActive; CScriptThing slots 21 MsgIsHitBy,
//   41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom; handle slot 20 PlayLoopingAnimation.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (the troll quest script: rock-pile handle +0x48,
//   intro-played flag +0x7d), PMasterData +0x18, EntrancePoint +0x1C, ExitPoint +0x28 (CScriptThing members).
// Shape: the parent's IsScripting/Release pair is an inline parent helper (a local handle alias computes the handle with
//   `add` instead of `lea`); CRGBColour is the D3DCOLOR B,G,R,A byte view set R,G,B,A in its body (store order 0x16,0x17,0x15,0x14);
//   the GetNearestWithDefName results are assigned through inline operator= and destroyed by the inline ~CScriptThing;
//   the hero-name temporaries of the wait test are one `||`/`&&` chain (construction flags in ebx).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    __forceinline ~CCountedPointer()
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
struct CBase {                                          // vtable 0x01231710
    CBase() { }
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const;
    virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const;
    virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& hitter) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const;
    virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const;
    virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& from) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& from) const;   // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing() { }                                 // out-of-line copy 0x004AA840
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingAlive : public NScript::CPredicateBase {                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                   // 0x00E241B0
};
struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseIntelligentPointer { public:   // vtable 0x0127094C, 0x10 bytes
    virtual void h001() const; virtual void h002() const; virtual void h003() const; virtual void h004() const; virtual void h005() const; virtual void h006() const; virtual void h007() const; virtual void h008() const;
    virtual void h009() const; virtual void h010() const; virtual void h011() const; virtual void h012() const; virtual void h013() const; virtual void h014() const; virtual void h015() const; virtual void h016() const;
    virtual void h017() const; virtual void h018() const; virtual void h019() const;
    virtual void PlayLoopingAnimation(const CCharString& anim, long loops, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // slot 20 (+0x50)
    void* PImpData;                                     // +0x08
    void* PImpInfo;                                     // +0x0c
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CGameScriptInterface { public:                          // vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const;
    virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const;
    virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const;
    virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const;
    virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const;
    virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const;
    virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const;
    virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const;
    virtual void v079() const; virtual void v080() const;
    virtual CScriptThing GetNearestWithDefName(const CScriptThing& thing, const CCharString& defName) const;   // slot 81 (+0x144)
    virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const; virtual void v089() const;
    virtual void v090() const; virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const; virtual void v097() const;
    virtual void v098() const; virtual void v099() const; virtual void v100() const; virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const;
    virtual void v106() const; virtual void v107() const; virtual void v108() const; virtual void v109() const; virtual void v110() const; virtual void v111() const; virtual void v112() const; virtual void v113() const;
    virtual void v114() const; virtual void v115() const; virtual void v116() const; virtual void v117() const; virtual void v118() const; virtual void v119() const; virtual void v120() const; virtual void v121() const;
    virtual void v122() const; virtual void v123() const; virtual void v124() const; virtual void v125() const; virtual void v126() const; virtual void v127() const; virtual void v128() const; virtual void v129() const;
    virtual void v130() const; virtual void v131() const; virtual void v132() const; virtual void v133() const; virtual void v134() const; virtual void v135() const; virtual void v136() const; virtual void v137() const;
    virtual void v138() const; virtual void v139() const; virtual void v140() const; virtual void v141() const; virtual void v142() const; virtual void v143() const; virtual void v144() const; virtual void v145() const;
    virtual void v146() const; virtual void v147() const; virtual void v148() const; virtual void v149() const; virtual void v150() const; virtual void v151() const; virtual void v152() const; virtual void v153() const;
    virtual void v154() const; virtual void v155() const; virtual void v156() const; virtual void v157() const; virtual void v158() const; virtual void v159() const; virtual void v160() const; virtual void v161() const;
    virtual void v162() const; virtual void v163() const; virtual void v164() const; virtual void v165() const; virtual void v166() const; virtual void v167() const; virtual void v168() const; virtual void v169() const;
    virtual void v170() const; virtual void v171() const; virtual void v172() const; virtual void v173() const; virtual void v174() const; virtual void v175() const; virtual void v176() const; virtual void v177() const;
    virtual void v178() const; virtual void v179() const; virtual void v180() const; virtual void v181() const; virtual void v182() const; virtual void v183() const; virtual void v184() const; virtual void v185() const;
    virtual void v186() const; virtual void v187() const; virtual void v188() const; virtual void v189() const; virtual void v190() const; virtual void v191() const; virtual void v192() const; virtual void v193() const;
    virtual void v194() const; virtual void v195() const; virtual void v196() const; virtual void v197() const; virtual void v198() const; virtual void v199() const; virtual void v200() const; virtual void v201() const;
    virtual void v202() const; virtual void v203() const; virtual void v204() const; virtual void v205() const; virtual void v206() const; virtual void v207() const; virtual void v208() const; virtual void v209() const;
    virtual void v210() const; virtual void v211() const; virtual void v212() const; virtual void v213() const; virtual void v214() const; virtual void v215() const; virtual void v216() const; virtual void v217() const;
    virtual void v218() const; virtual void v219() const; virtual void v220() const; virtual void v221() const; virtual void v222() const; virtual void v223() const; virtual void v224() const; virtual void v225() const;
    virtual void v226() const; virtual void v227() const; virtual void v228() const; virtual void v229() const; virtual void v230() const; virtual void v231() const; virtual void v232() const; virtual void v233() const;
    virtual void v234() const; virtual void v235() const; virtual void v236() const; virtual void v237() const; virtual void v238() const; virtual void v239() const; virtual void v240() const; virtual void v241() const;
    virtual void v242() const; virtual void v243() const; virtual void v244() const; virtual void v245() const; virtual void v246() const; virtual void v247() const; virtual void v248() const; virtual void v249() const;
    virtual void v250() const; virtual void v251() const; virtual void v252() const; virtual void v253() const; virtual void v254() const; virtual void v255() const; virtual void v256() const; virtual void v257() const;
    virtual void v258() const; virtual void v259() const; virtual void v260() const; virtual void v261() const; virtual void v262() const; virtual void v263() const; virtual void v264() const; virtual void v265() const;
    virtual void v266() const; virtual void v267() const; virtual void v268() const; virtual void v269() const; virtual void v270() const; virtual void v271() const; virtual void v272() const; virtual void v273() const;
    virtual void v274() const; virtual void v275() const; virtual void v276() const; virtual void v277() const; virtual void v278() const; virtual void v279() const; virtual void v280() const; virtual void v281() const;
    virtual void v282() const; virtual void v283() const; virtual void v284() const; virtual void v285() const; virtual void v286() const; virtual void v287() const; virtual void v288() const; virtual void v289() const;
    virtual void v290() const; virtual void v291() const; virtual void v292() const; virtual void v293() const; virtual void v294() const; virtual void v295() const; virtual void v296() const; virtual void v297() const;
    virtual void v298() const; virtual void v299() const; virtual void v300() const; virtual void v301() const; virtual void v302() const; virtual void v303() const; virtual void v304() const; virtual void v305() const;
    virtual void v306() const; virtual void v307() const; virtual void v308() const; virtual void v309() const; virtual void v310() const; virtual void v311() const; virtual void v312() const; virtual void v313() const;
    virtual void v314() const; virtual void v315() const; virtual void v316() const; virtual void v317() const; virtual void v318() const; virtual void v319() const; virtual void v320() const;
    virtual void DisplayQuestInfo(bool display) const;   // slot 321 (+0x504)
    virtual void v322() const; virtual void v323() const; virtual void v324() const;
    virtual long AddQuestInfoBarHealth(const CScriptThing& thing, const CRGBColour& colour, const CCharString& icon, float scale) const;   // slot 325 (+0x514)
    virtual void v326() const; virtual void v327() const; virtual void v328() const; virtual void v329() const; virtual void v330() const; virtual void v331() const; virtual void v332() const; virtual void v333() const;
    virtual void v334() const; virtual void v335() const; virtual void v336() const; virtual void v337() const; virtual void v338() const; virtual void v339() const; virtual void v340() const; virtual void v341() const;
    virtual void v342() const; virtual void v343() const; virtual void v344() const; virtual void v345() const; virtual void v346() const; virtual void v347() const; virtual void v348() const; virtual void v349() const;
    virtual void v350() const; virtual void v351() const; virtual void v352() const; virtual void v353() const; virtual void v354() const; virtual void v355() const;
    virtual void EntitySetAsHiddenOnMiniMap(const CScriptThing& thing, bool hidden) const;   // slot 356 (+0x590)
    virtual void v357() const; virtual void v358() const; virtual void v359() const; virtual void v360() const; virtual void v361() const; virtual void v362() const; virtual void v363() const; virtual void v364() const;
    virtual void v365() const; virtual void v366() const; virtual void v367() const; virtual void v368() const; virtual void v369() const; virtual void v370() const; virtual void v371() const; virtual void v372() const;
    virtual void v373() const; virtual void v374() const; virtual void v375() const; virtual void v376() const; virtual void v377() const; virtual void v378() const; virtual void v379() const; virtual void v380() const;
    virtual void v381() const; virtual void v382() const; virtual void v383() const; virtual void v384() const; virtual void v385() const; virtual void v386() const; virtual void v387() const; virtual void v388() const;
    virtual void v389() const; virtual void v390() const; virtual void v391() const; virtual void v392() const; virtual void v393() const; virtual void v394() const; virtual void v395() const; virtual void v396() const;
    virtual void v397() const; virtual void v398() const; virtual void v399() const; virtual void v400() const; virtual void v401() const; virtual void v402() const; virtual void v403() const; virtual void v404() const;
    virtual void v405() const; virtual void v406() const; virtual void v407() const; virtual void v408() const; virtual void v409() const; virtual void v410() const; virtual void v411() const; virtual void v412() const;
    virtual void v413() const; virtual void v414() const; virtual void v415() const; virtual void v416() const; virtual void v417() const; virtual void v418() const; virtual void v419() const; virtual void v420() const;
    virtual void v421() const; virtual void v422() const; virtual void v423() const; virtual void v424() const; virtual void v425() const; virtual void v426() const; virtual void v427() const; virtual void v428() const;
    virtual void v429() const; virtual void v430() const; virtual void v431() const; virtual void v432() const; virtual void v433() const; virtual void v434() const; virtual void v435() const; virtual void v436() const;
    virtual void v437() const; virtual void v438() const; virtual void v439() const; virtual void v440() const; virtual void v441() const; virtual void v442() const; virtual void v443() const; virtual void v444() const;
    virtual void v445() const; virtual void v446() const; virtual void v447() const; virtual void v448() const; virtual void v449() const; virtual void v450() const; virtual void v451() const; virtual void v452() const;
    virtual void v453() const; virtual void v454() const; virtual void v455() const; virtual void v456() const; virtual void v457() const; virtual void v458() const; virtual void v459() const; virtual void v460() const;
    virtual void v461() const; virtual void v462() const; virtual void v463() const; virtual void v464() const; virtual void v465() const; virtual void v466() const; virtual void v467() const; virtual void v468() const;
    virtual void v469() const; virtual void v470() const; virtual void v471() const; virtual void v472() const; virtual void v473() const; virtual void v474() const; virtual void v475() const; virtual void v476() const;
    virtual void SetThingPersistent(const CScriptThing& thing, bool persistent) const;   // slot 477 (+0x774)
    virtual void v478() const; virtual void v479() const; virtual void v480() const; virtual void v481() const; virtual void v482() const; virtual void v483() const; virtual void v484() const; virtual void v485() const;
    virtual void v486() const; virtual void v487() const; virtual void v488() const; virtual void v489() const; virtual void v490() const; virtual void v491() const; virtual void v492() const; virtual void v493() const;
    virtual void v494() const; virtual void v495() const; virtual void v496() const; virtual void v497() const; virtual void v498() const; virtual void v499() const; virtual void v500() const; virtual void v501() const;
    virtual void v502() const; virtual void v503() const; virtual void v504() const; virtual void v505() const; virtual void v506() const; virtual void v507() const; virtual void v508() const; virtual void v509() const;
    virtual void v510() const; virtual void v511() const; virtual void v512() const; virtual void v513() const; virtual void v514() const; virtual void v515() const; virtual void v516() const; virtual void v517() const;
    virtual void v518() const; virtual void v519() const; virtual void v520() const; virtual void v521() const; virtual void v522() const; virtual void v523() const; virtual void v524() const; virtual void v525() const;
    virtual void v526() const; virtual void v527() const; virtual void v528() const; virtual void v529() const; virtual void v530() const; virtual void v531() const; virtual void v532() const; virtual void v533() const;
    virtual void v534() const; virtual void v535() const; virtual void v536() const; virtual void v537() const; virtual void v538() const; virtual void v539() const; virtual void v540() const; virtual void v541() const;
    virtual void v542() const; virtual void v543() const; virtual void v544() const; virtual void v545() const; virtual void v546() const; virtual void v547() const; virtual void v548() const; virtual void v549() const;
    virtual void v550() const; virtual void v551() const; virtual void v552() const; virtual void v553() const; virtual void v554() const; virtual void v555() const; virtual void v556() const; virtual void v557() const;
    virtual void v558() const; virtual void v559() const; virtual void v560() const; virtual void v561() const; virtual void v562() const; virtual void v563() const; virtual void v564() const; virtual void v565() const;
    virtual void v566() const;
    virtual void SetRegionExitAsActive(const CScriptThing& exit, bool active) const;   // slot 567 (+0x8dc)
    virtual void SetRegionEntranceAsActive(const CScriptThing& entrance, bool active) const;   // slot 568 (+0x8e0)
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern bool g_bScriptAnimationFlag;                    // 0x01375748
struct CQ_SnowTrollScript {
    unsigned char _pad00[0x48];
    CScriptGameResourceObjectScriptedThingBase RockPile;   // +0x48
    unsigned char _pad58[0x7d - 0x58];
    bool IntroPlayed;                                   // +0x7d
    void PlayCutscene(CCharString name);
    void ReleaseRockPile() { if (RockPile.IsScripting()) RockPile.Release(); }                // 0x00EE2DB0
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CStopDrawing : CEntityScriptBase {
    CQ_SnowTrollScript* ParentClass;                    // +0x14
    void*               PMasterData;                    // +0x18
    CScriptThing        EntrancePoint;                  // +0x1c
    CScriptThing        ExitPoint;                      // +0x28
    void Main();
};

void CStopDrawing::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingPersistent(Me, true);
    EntrancePoint = Game->GetNearestWithDefName(Me, "REGION_ENTRANCE_POINT");
    ExitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    Game->SetRegionEntranceAsActive(EntrancePoint, false);
    Game->SetRegionExitAsActive(ExitPoint, false);
    if (!ParentClass->IntroPlayed) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsHiddenOnMiniMap(Me, true);
        ParentClass->ReleaseRockPile();
        while (!Game->StartScriptingEntity(Me, ParentClass->RockPile, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ParentClass->RockPile.PlayLoopingAnimation("SCRIPT_ROCK_PILE", -1, false, false, false, true, g_bScriptAnimationFlag, false, false);
        for (;;) {
            bool waiting = !(IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)
                || Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO")));
            if (!waiting)
                break;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ParentClass->PlayCutscene("CS_SNOWTROLL_INTRO");
        Game->EntitySetAsHiddenOnMiniMap(Me, false);
        ParentClass->IntroPlayed = true;
        ParentClass->RockPile.Release();
    }
    Game->AddQuestInfoBarHealth(Me, CRGBColour(0xff, 0, 0, 0xff), "HUD_QUEST_ICON_ICE_TROLL", 1.0f);
    Game->DisplayQuestInfo(true);
    do
        Game->NewScriptFrame();
    while (!IsActiveThreadTerminating());
}