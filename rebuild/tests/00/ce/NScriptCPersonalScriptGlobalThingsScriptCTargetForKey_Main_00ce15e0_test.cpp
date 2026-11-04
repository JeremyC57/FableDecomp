// Behaviour test for 0x00CE15E0 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
// CTargetForKey::Main() -- the Minion Camp silver-key target: register an alive filter over Me; once the target is done
// or the Minion Camp quest is active, remove Me (and later code still runs); otherwise wait until Me is hit, then
// spawn the silver key at "GenerateKeyHere", play CS_STANDING_STONE with the hero cast inside a paused movie sequence,
// make the end-game focal site persistent and give it the "nothing stuck" click text, mark the target done and remove Me
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE15E0, plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CIsThingAlive filter (vtable 0x012C32F0) over CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840,
//   RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0,
//   CBaseIntelligentPointer ctor 0x0099A380 / operator= 0x0099A3B0 / dtor 0x0099A430, handle PImp operator= 0x008AB1E0,
//   handle dtor 0x007E74D0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, movie PImp dtor
//   0x006E7AB0, RunCutsceneMacro 0x00CBFB7D (__fastcall); CScriptThing slot 21 MsgIsHitBy and its PImp's slot 4;
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName,
//   77 GetNearestWithScriptName, 99 CreateObjectOnEntity, 108 RemoveThing, 284 IsQuestActive, 370 StartMovieSequence,
//   376 Pause, 379 PauseAllNonScriptedEntities, 477 SetThingPersistent.
// Layout: Ego_r.pdb CTargetForKey (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, TargetForKeyDone +0x1c. Handle / map / movie classes as the landed CMK_GHC_HERO3::Main 0x00D29DB0
//   and CRockTrollTrigger::Main 0x00E0AD40.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CCPPointerInfo { public: long RefCount; void (__fastcall* DeleteFunc)(void* data); void* Data; };
struct CScriptThingImpl {
    virtual void i000() { } virtual void i001() { } virtual void i002() { } virtual void i003() { }
    virtual void SetClickText(const CCharString& text);   // slot 4 (+0x10)
};
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                           // slot 0 (out of line 0x004AA840)
    virtual void t001() { } virtual void t002() { } virtual void t003() { } virtual void t004() { } virtual void t005() { } virtual void t006() { } virtual void t007() { } virtual void t008() { }
    virtual void t009() { } virtual void t010() { } virtual void t011() { } virtual void t012() { } virtual void t013() { } virtual void t014() { } virtual void t015() { } virtual void t016() { }
    virtual void t017() { } virtual void t018() { } virtual void t019() { } virtual void t020() { }
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    CScriptThingImpl* PImp_Data;                       // +0x04
    CCPPointerInfo*   PImp_Info;                       // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    void SetClickText(const CCharString& text) { if (PImp_Data) PImp_Data->SetClickText(text); }
};
struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00() { }
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
class CBaseObject { public:                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000() { }
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                      // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);   // 0x0099A3B0
public:
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
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CHandlePImp PImp;                                   // +0x08 Data, +0x0c Info
    __forceinline CScriptGameResourceObjectScriptedThingBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void h001() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001() { }
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
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CGameScriptInterface { public:                           // vtable 0x01260F0C
    virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { }
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009() { } virtual void v010() { } virtual void v011() { } virtual void v012() { } virtual void v013() { } virtual void v014() { } virtual void v015() { } virtual void v016() { }
    virtual void v017() { } virtual void v018() { } virtual void v019() { } virtual void v020() { } virtual void v021() { } virtual void v022() { } virtual void v023() { } virtual void v024() { }
    virtual void v025() { } virtual void v026() { } virtual void v027() { } virtual void v028() { } virtual void v029() { } virtual void v030() { } virtual void v031() { } virtual void v032() { }
    virtual void v033() { } virtual void v034() { } virtual void v035() { } virtual void v036() { } virtual void v037() { } virtual void v038() { } virtual void v039() { } virtual void v040() { }
    virtual void v041() { } virtual void v042() { } virtual void v043() { } virtual void v044() { } virtual void v045() { } virtual void v046() { } virtual void v047() { } virtual void v048() { }
    virtual void v049() { } virtual void v050() { } virtual void v051() { } virtual void v052() { } virtual void v053() { } virtual void v054() { } virtual void v055() { } virtual void v056() { }
    virtual void v057() { } virtual void v058() { } virtual void v059() { } virtual void v060() { } virtual void v061() { } virtual void v062() { } virtual void v063() { } virtual void v064() { }
    virtual void v065() { } virtual void v066() { } virtual void v067() { } virtual void v068() { } virtual void v069() { }
    virtual CScriptThing& GetHero();                    // slot 70 (+0x118)
    virtual void v071() { }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073() { } virtual void v074() { } virtual void v075() { } virtual void v076() { }
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const;   // slot 77 (+0x134)
    virtual void v078() { } virtual void v079() { } virtual void v080() { } virtual void v081() { } virtual void v082() { } virtual void v083() { } virtual void v084() { } virtual void v085() { }
    virtual void v086() { } virtual void v087() { } virtual void v088() { } virtual void v089() { } virtual void v090() { } virtual void v091() { } virtual void v092() { } virtual void v093() { }
    virtual void v094() { } virtual void v095() { } virtual void v096() { } virtual void v097() { } virtual void v098() { }
    virtual CScriptThing CreateObjectOnEntity(const CCharString& name, const CScriptThing& thing, const CCharString& boneName) const;   // slot 99 (+0x18c)
    virtual void v100() { } virtual void v101() { } virtual void v102() { } virtual void v103() { } virtual void v104() { } virtual void v105() { } virtual void v106() { } virtual void v107() { }
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    virtual void v109() { } virtual void v110() { } virtual void v111() { } virtual void v112() { } virtual void v113() { } virtual void v114() { } virtual void v115() { } virtual void v116() { }
    virtual void v117() { } virtual void v118() { } virtual void v119() { } virtual void v120() { } virtual void v121() { } virtual void v122() { } virtual void v123() { } virtual void v124() { }
    virtual void v125() { } virtual void v126() { } virtual void v127() { } virtual void v128() { } virtual void v129() { } virtual void v130() { } virtual void v131() { } virtual void v132() { }
    virtual void v133() { } virtual void v134() { } virtual void v135() { } virtual void v136() { } virtual void v137() { } virtual void v138() { } virtual void v139() { } virtual void v140() { }
    virtual void v141() { } virtual void v142() { } virtual void v143() { } virtual void v144() { } virtual void v145() { } virtual void v146() { } virtual void v147() { } virtual void v148() { }
    virtual void v149() { } virtual void v150() { } virtual void v151() { } virtual void v152() { } virtual void v153() { } virtual void v154() { } virtual void v155() { } virtual void v156() { }
    virtual void v157() { } virtual void v158() { } virtual void v159() { } virtual void v160() { } virtual void v161() { } virtual void v162() { } virtual void v163() { } virtual void v164() { }
    virtual void v165() { } virtual void v166() { } virtual void v167() { } virtual void v168() { } virtual void v169() { } virtual void v170() { } virtual void v171() { } virtual void v172() { }
    virtual void v173() { } virtual void v174() { } virtual void v175() { } virtual void v176() { } virtual void v177() { } virtual void v178() { } virtual void v179() { } virtual void v180() { }
    virtual void v181() { } virtual void v182() { } virtual void v183() { } virtual void v184() { } virtual void v185() { } virtual void v186() { } virtual void v187() { } virtual void v188() { }
    virtual void v189() { } virtual void v190() { } virtual void v191() { } virtual void v192() { } virtual void v193() { } virtual void v194() { } virtual void v195() { } virtual void v196() { }
    virtual void v197() { } virtual void v198() { } virtual void v199() { } virtual void v200() { } virtual void v201() { } virtual void v202() { } virtual void v203() { } virtual void v204() { }
    virtual void v205() { } virtual void v206() { } virtual void v207() { } virtual void v208() { } virtual void v209() { } virtual void v210() { } virtual void v211() { } virtual void v212() { }
    virtual void v213() { } virtual void v214() { } virtual void v215() { } virtual void v216() { } virtual void v217() { } virtual void v218() { } virtual void v219() { } virtual void v220() { }
    virtual void v221() { } virtual void v222() { } virtual void v223() { } virtual void v224() { } virtual void v225() { } virtual void v226() { } virtual void v227() { } virtual void v228() { }
    virtual void v229() { } virtual void v230() { } virtual void v231() { } virtual void v232() { } virtual void v233() { } virtual void v234() { } virtual void v235() { } virtual void v236() { }
    virtual void v237() { } virtual void v238() { } virtual void v239() { } virtual void v240() { } virtual void v241() { } virtual void v242() { } virtual void v243() { } virtual void v244() { }
    virtual void v245() { } virtual void v246() { } virtual void v247() { } virtual void v248() { } virtual void v249() { } virtual void v250() { } virtual void v251() { } virtual void v252() { }
    virtual void v253() { } virtual void v254() { } virtual void v255() { } virtual void v256() { } virtual void v257() { } virtual void v258() { } virtual void v259() { } virtual void v260() { }
    virtual void v261() { } virtual void v262() { } virtual void v263() { } virtual void v264() { } virtual void v265() { } virtual void v266() { } virtual void v267() { } virtual void v268() { }
    virtual void v269() { } virtual void v270() { } virtual void v271() { } virtual void v272() { } virtual void v273() { } virtual void v274() { } virtual void v275() { } virtual void v276() { }
    virtual void v277() { } virtual void v278() { } virtual void v279() { } virtual void v280() { } virtual void v281() { } virtual void v282() { } virtual void v283() { }
    virtual bool IsQuestActive(const CCharString& name) const;   // slot 284 (+0x470)
    virtual void v285() { } virtual void v286() { } virtual void v287() { } virtual void v288() { } virtual void v289() { } virtual void v290() { } virtual void v291() { } virtual void v292() { }
    virtual void v293() { } virtual void v294() { } virtual void v295() { } virtual void v296() { } virtual void v297() { } virtual void v298() { } virtual void v299() { } virtual void v300() { }
    virtual void v301() { } virtual void v302() { } virtual void v303() { } virtual void v304() { } virtual void v305() { } virtual void v306() { } virtual void v307() { } virtual void v308() { }
    virtual void v309() { } virtual void v310() { } virtual void v311() { } virtual void v312() { } virtual void v313() { } virtual void v314() { } virtual void v315() { } virtual void v316() { }
    virtual void v317() { } virtual void v318() { } virtual void v319() { } virtual void v320() { } virtual void v321() { } virtual void v322() { } virtual void v323() { } virtual void v324() { }
    virtual void v325() { } virtual void v326() { } virtual void v327() { } virtual void v328() { } virtual void v329() { } virtual void v330() { } virtual void v331() { } virtual void v332() { }
    virtual void v333() { } virtual void v334() { } virtual void v335() { } virtual void v336() { } virtual void v337() { } virtual void v338() { } virtual void v339() { } virtual void v340() { }
    virtual void v341() { } virtual void v342() { } virtual void v343() { } virtual void v344() { } virtual void v345() { } virtual void v346() { } virtual void v347() { } virtual void v348() { }
    virtual void v349() { } virtual void v350() { } virtual void v351() { } virtual void v352() { } virtual void v353() { } virtual void v354() { } virtual void v355() { } virtual void v356() { }
    virtual void v357() { } virtual void v358() { } virtual void v359() { } virtual void v360() { } virtual void v361() { } virtual void v362() { } virtual void v363() { } virtual void v364() { }
    virtual void v365() { } virtual void v366() { } virtual void v367() { } virtual void v368() { } virtual void v369() { }
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void v371() { } virtual void v372() { } virtual void v373() { } virtual void v374() { } virtual void v375() { }
    virtual void Pause(float seconds) const;            // slot 376 (+0x5e0)
    virtual void v377() { } virtual void v378() { }
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec)
    virtual void v380() { } virtual void v381() { } virtual void v382() { } virtual void v383() { } virtual void v384() { } virtual void v385() { } virtual void v386() { } virtual void v387() { }
    virtual void v388() { } virtual void v389() { } virtual void v390() { } virtual void v391() { } virtual void v392() { } virtual void v393() { } virtual void v394() { } virtual void v395() { }
    virtual void v396() { } virtual void v397() { } virtual void v398() { } virtual void v399() { } virtual void v400() { } virtual void v401() { } virtual void v402() { } virtual void v403() { }
    virtual void v404() { } virtual void v405() { } virtual void v406() { } virtual void v407() { } virtual void v408() { } virtual void v409() { } virtual void v410() { } virtual void v411() { }
    virtual void v412() { } virtual void v413() { } virtual void v414() { } virtual void v415() { } virtual void v416() { } virtual void v417() { } virtual void v418() { } virtual void v419() { }
    virtual void v420() { } virtual void v421() { } virtual void v422() { } virtual void v423() { } virtual void v424() { } virtual void v425() { } virtual void v426() { } virtual void v427() { }
    virtual void v428() { } virtual void v429() { } virtual void v430() { } virtual void v431() { } virtual void v432() { } virtual void v433() { } virtual void v434() { } virtual void v435() { }
    virtual void v436() { } virtual void v437() { } virtual void v438() { } virtual void v439() { } virtual void v440() { } virtual void v441() { } virtual void v442() { } virtual void v443() { }
    virtual void v444() { } virtual void v445() { } virtual void v446() { } virtual void v447() { } virtual void v448() { } virtual void v449() { } virtual void v450() { } virtual void v451() { }
    virtual void v452() { } virtual void v453() { } virtual void v454() { } virtual void v455() { } virtual void v456() { } virtual void v457() { } virtual void v458() { } virtual void v459() { }
    virtual void v460() { } virtual void v461() { } virtual void v462() { } virtual void v463() { } virtual void v464() { } virtual void v465() { } virtual void v466() { } virtual void v467() { }
    virtual void v468() { } virtual void v469() { } virtual void v470() { } virtual void v471() { } virtual void v472() { } virtual void v473() { } virtual void v474() { } virtual void v475() { }
    virtual void v476() { }
    virtual void SetThingPersistent(const CScriptThing& thing, bool persistent) const;   // slot 477 (+0x774)
};
class CEntityScriptBase { public:
    virtual void e00() { }
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void RegisterFilter(CIsThingAlive& filter);        // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CTargetForKey : CEntityScriptBase {
    void* ParentClass;                                 // +0x14 CPersonalScript_GlobalThingsScript*
    void* PMasterData;                                 // +0x18 CQ_SunnyvaleMasterData*
    bool  TargetForKeyDone;                            // +0x1c
    void Main();
};

void CTargetForKey::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (TargetForKeyDone || Game->IsQuestActive("Q_MinionCamp")) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
    }
    while (!Me.MsgIsHitBy("")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing keyHere = Game->GetNearestWithScriptName(Me, "GenerateKeyHere");
    {
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    Game->CreateObjectOnEntity("OBJECT_SILVER_KEY", keyHere, "");
    CCutsceneCastMap cast;
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->Pause(1.0f);
    RunCutsceneMacro("CS_STANDING_STONE", cast, 0, 0, false, true);
    game->PauseAllNonScriptedEntities(false);
    movie.DestroyInline();
    }
    CScriptThing site = Game->GetThingWithScriptName("EndGameFocalSite");
    Game->SetThingPersistent(site, true);
    site.SetClickText("TXT_CLICK_NOTHING_STUCK");
    TargetForKeyDone = true;
    Game->RemoveThing(Me, false, true);
}

static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
static const char* g_lastStr[16]; static int g_ns = 0;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() { }
static const char* Str(const CCharString& s) { return (const char*)s.PStringData; }
static bool Eq(const CCharString& s, const char* t) { return std::strcmp(Str(s), t) == 0; }
static int g_scriptThings = 0;
CScriptThing::CScriptThing(const CScriptThing& other) : PImp_Data(other.PImp_Data), PImp_Info(other.PImp_Info) { ++g_scriptThings; }
CScriptThing::~CScriptThing() { --g_scriptThings; }
static int g_hitAfter = 0;
bool CScriptThing::MsgIsHitBy(const CCharString& name) const { if (!Eq(name, "")) return false; Log(20); return g_hitAfter-- <= 0; }
static int g_clickOk = 0;
struct TestImpl : CScriptThingImpl { };
void CScriptThingImpl::SetClickText(const CCharString& text) { if (Eq(text, "TXT_CLICK_NOTHING_STUCK")) g_clickOk = 1; Log(90); }
CBaseObject::CBaseObject() : m_pIntelligentPointer(0) { }
CBaseObject& CBaseObject::operator=(const CBaseObject& other) { m_pIntelligentPointer = other.m_pIntelligentPointer; return *this; }
CBaseObjectDtor::~CBaseObjectDtor() { }
CHandlePImp& CHandlePImp::operator=(const CHandlePImp& other) { Data = other.Data; Info = other.Info; return *this; }
static int g_heroDtors = 0, g_movieDtors = 0, g_castDtors = 0;
CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase() { ++g_heroDtors; Log(80); }
CMovieImpPointer::~CMovieImpPointer() { ++g_movieDtors; Log(81); }
static void* g_castHeroData = 0;
CCutsceneCastMap::CCutsceneCastMap() { _body[0] = 0; }
CCutsceneCastMap::~CCutsceneCastMap() { ++g_castDtors; Log(82); }
static CScriptGameResourceObjectScriptedThingBase* g_castSlot;
CScriptGameResourceObjectScriptedThingBase& CCutsceneCastMap::operator[](const CCharString& key) { if (!Eq(key, "HERO")) std::abort(); return *g_castSlot; }
static int g_cutsceneOk = 0;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap&, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6)
{ if (Eq(name, "CS_STANDING_STONE") && !flags && !args && !a5 && a6 && g_castSlot->PImp.Data == (void*)0x1234) g_cutsceneOk = 1; Log(60); }
static int g_frames = 0, g_termAt = 1000, g_checks = 0, g_quest = 0, g_removes = 0, g_persist = 0, g_paused = -1, g_prio = -1;
static int g_filterReg = 0;
static CScriptThing* g_hero; static CScriptThing* g_key; static CScriptThing* g_site;
void CEntityScriptBase::RegisterFilter(CIsThingAlive& filter) { if (filter.m_thing.PImp_Data == Me.PImp_Data) g_filterReg = 1; Log(1); }
bool CEntityScriptBase::IsActiveThreadTerminating() const { return ++g_checks >= g_termAt; }
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; Log(2); }
bool CGameScriptInterface::StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority)
{ if (&thing == g_hero) { object.PImp.Data = (void*)0x1234; g_prio = (int)priority; } Log(30); return true; }
CScriptThing& CGameScriptInterface::GetHero() { return *g_hero; }
CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& name) const { Log(Eq(name, "EndGameFocalSite") ? 70 : 71); return *g_site; }
CScriptThing CGameScriptInterface::GetNearestWithScriptName(const CScriptThing&, const CCharString& name) const { Log(Eq(name, "GenerateKeyHere") ? 25 : 26); return *g_key; }
CScriptThing CGameScriptInterface::CreateObjectOnEntity(const CCharString& name, const CScriptThing& thing, const CCharString& bone) const
{ Log(Eq(name, "OBJECT_SILVER_KEY") && thing.PImp_Data == g_key->PImp_Data && Eq(bone, "") ? 40 : 41); return *g_key; }
void CGameScriptInterface::RemoveThing(const CScriptThing&, bool a, bool b) const { if (!a && b) ++g_removes; Log(99); }
bool CGameScriptInterface::IsQuestActive(const CCharString& name) const { Log(Eq(name, "Q_MinionCamp") ? 10 : 11); return g_quest != 0; }
void CGameScriptInterface::StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase&) { Log(Eq(name, "") ? 50 : 51); }
static float g_pause = 0.0f;
void CGameScriptInterface::Pause(float seconds) const { g_pause = seconds; Log(55); }
void CGameScriptInterface::PauseAllNonScriptedEntities(bool pause) { g_paused = pause ? 1 : 0; Log(pause ? 52 : 61); }
void CGameScriptInterface::SetThingPersistent(const CScriptThing& thing, bool persistent) const { if (thing.PImp_Data == g_site->PImp_Data && persistent) g_persist = 1; Log(85); }
struct TestThing : CScriptThing { TestThing(const CScriptThing& t) : CScriptThing(t) { } };
static void Reset() { g_n = 0; g_frames = 0; g_termAt = 1000; g_checks = 0; g_quest = 0; g_removes = 0; g_persist = 0; g_paused = -1; g_prio = -1;
    g_filterReg = 0; g_hitAfter = 0; g_clickOk = 0; g_cutsceneOk = 0; g_heroDtors = 0; g_movieDtors = 0; g_castDtors = 0; g_pause = 0.0f; }

int main()
{
    static TestImpl meImpl, heroImpl, keyImpl, siteImpl;
    static CScriptThing* proto = (CScriptThing*)std::calloc(1, sizeof(CScriptThing) + 16);
    CScriptThing* mk = 0; (void)mk;
    static CGameScriptInterface game;
    static CScriptGameResourceObjectScriptedThingBase castSlot; g_castSlot = &castSlot;
    CTargetForKey& script = *(CTargetForKey*)std::calloc(1, sizeof(CTargetForKey));
    // build things by copy-constructing from a raw seed with the right PImp_Data
    std::memset(proto, 0, sizeof(CScriptThing));
    proto->PImp_Data = &heroImpl; static TestThing hero(*proto); g_hero = &hero;
    proto->PImp_Data = &keyImpl; static TestThing key(*proto); g_key = &key;
    proto->PImp_Data = &siteImpl; static TestThing site(*proto); g_site = &site;
    proto->PImp_Data = &meImpl; new (&script.Me) TestThing(*proto);
    script.Game = &game;
    // 1) terminating right after the first frame: only the filter and the frame
    Reset(); g_termAt = 1; script.TargetForKeyDone = false;
    script.Main();
    if (!g_filterReg || g_frames != 1 || g_removes != 0 || g_n != 2) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
    // 2) not done, quest inactive, hit after two frames: full sequence
    Reset(); g_hitAfter = 2; castSlot.PImp.Data = 0;
    script.Main();
    if (!script.TargetForKeyDone || g_removes != 1 || g_frames != 3) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
    if (g_prio != 4 || !g_cutsceneOk || g_paused != 0 || g_pause != 1.0f || !g_persist || !g_clickOk) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
    if (g_heroDtors != 1 || g_movieDtors != 1 || g_castDtors != 1) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
    {
        static const int want[] = { 1, 2, 10, 20, 2, 20, 2, 20, 25, 30, 40, 50, 52, 55, 60, 61, 81, 82, 80, 70, 85, 90, 99 };
        if (g_n != (int)(sizeof(want) / sizeof(want[0]))) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
        for (int i = 0; i < g_n; ++i) if (g_log[i] != want[i]) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
    }
    // 3) already done: removed early, then (hit at once) the sequence still runs; quest not queried
    Reset(); script.TargetForKeyDone = true;
    script.Main();
    if (g_removes != 2 || g_log[2] != 99 || g_log[3] != 20) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
    // 4) quest active and terminating at the inner check: no remove
    Reset(); script.TargetForKeyDone = false; g_quest = 1; g_termAt = 2;
    script.Main();
    if (g_removes != 0 || g_log[2] != 10 || g_n != 3) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
    // 5) waiting for the hit and terminating inside the loop
    Reset(); g_hitAfter = 5; g_termAt = 3;
    script.Main();
    if (g_removes != 0 || g_frames != 3 || script.TargetForKeyDone != false) { std::printf("00ce15e0_TEST FAIL\n"); return 1; }
    std::printf("00ce15e0_TEST PASS\n");
    return 0;
}