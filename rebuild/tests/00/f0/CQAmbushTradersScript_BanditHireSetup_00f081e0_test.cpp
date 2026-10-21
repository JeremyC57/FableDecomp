// Behaviour test for 00f081e0: src/00f081e0.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CQ_AmbushTradersScript::BanditHireSetup(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQ_AmbushTradersScript::BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject) -- a living bandit confirms the hire to the hero and finishes speaking; then the hire count rises, WatchForBanditSpeechMarkers is spawned for the bandit and its health bar is added
// Leaf: ?BanditHireSetup@CQ_AmbushTradersScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F081E0, 440 bytes, `ret 8`; Ego_r.pdb
//   ?BanditHireSetup@CQ_AmbushTradersScript@NScript@@QAEJABVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@@Z.
// Callees: the object's slots 12 GetScriptThing (hidden result), 13 Speak(const CScriptThing&, const char*, method, bool x3),
//   26 IsPerformingScriptTask; interface slots 264 GetHealth (> 0.0f 0x0122DEDC), 70 (+0x118, returns the hero; manifest
//   "GetHero"), 7 NewScriptFrame; IsActiveThreadTerminating 0x00CB7940; operator new 0x00BFEA1A (0x48 bytes),
//   CSpawnedFuncBase(name, 0) 0x00CDD450, CSpawnedFunc1 vtable 0x012F7EBC, WatchForBanditSpeechMarkers 0x00F07B80,
//   CScriptThing copy 0x004ABE90 / out-of-line dtor 0x004AA840 (the by-value parameter inside the inlined ctor),
//   AddSpawnedFunction 0x00CB7E50 ("" region), AddBanditHealthBar 0x00F07A80 (its result is returned).
// Layout (retail = Ego_r.pdb - 0x14 for the script; quest data +0xD0 counter, unnamed).
// Shape: SpawnFunction takes the region as `const CCharString& region = CCharString("")`: the default-argument temporary is
//   built at the call site and gets its own frame slot (frame+4) as in retail; a `const char*` default converted inside
//   the inline helper lets VC put the string in the dead `bandit` argument slot (0x10 frame).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CBaseClass {
    virtual ~CBaseClass();                                  // 0x0099A2E0
    CBaseClass();                                           // 0x0099A2D0
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
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
struct CScriptThing : CBaseClass {                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThing() { }
};
struct CScriptThingParam : CBaseClass {                     // the spawned function's by-value parameter: same object, out-of-line dtor
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThingParam(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThingParam();                                   // 0x004AA840
};
struct CScriptThingStored : CBaseClass {
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThingStored(const CScriptThingParam& other);     // 0x004ABE90
};

enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_2 = 2 };
struct CScriptGameResourceObjectScriptedThingBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CScriptThing GetScriptThing() const;            // slot 12 (+0x30)
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34)
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual bool IsPerformingScriptTask() const;            // slot 26 (+0x68)
};
struct CGameScriptInterface {
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
    virtual const CScriptThing& GetHero() const;            // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189();
    virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229();
    virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259();
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
};

struct CSpawnedFuncBase {                                    // 0x34 bytes
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
    virtual void v00();
    unsigned char _body[0x30];
};
template <class C, class P> struct CSpawnedFunc1 : CSpawnedFuncBase {
    typedef void (C::*TFunc)(P);
    TFunc               Func;                              // +0x34
    C*                  PClass;                            // +0x38
    CScriptThingStored  Par1;                              // +0x3c
    virtual void v00();
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, TFunc func, C* owner, CScriptThingParam par)
        : CSpawnedFuncBase(name, id), Func(func), PClass(owner), Par1(par) { }
};

struct CQuestData {
    unsigned char _pad00[0xd0];
    long          BanditsHired;                            // +0xd0
};
struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
    void Add(CSpawnedFuncBase* func, const CCharString& name);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const CCharString& region = CCharString("")) { Add(func, region); }
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    CQuestData*           Data;                             // +0x44
};
struct CQ_AmbushTradersScript : CQuestScriptBase {
    void WatchForBanditSpeechMarkers(CScriptThingParam bandit);   // 0x00F07B80
    long AddBanditHealthBar(const CScriptThing& bandit);    // 0x00F07A80
    long BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject);
};

long CQ_AmbushTradersScript::BanditHireSetup(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject)
{
    if (Game->GetHealth(banditObject.GetScriptThing()) > 0.0f) {
        banditObject.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_HIRE_CONFIRM", TEXT_GROUP_SELECTION_2, false, true, false);
        while (banditObject.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return 0;
        }
        if (IsActiveThreadTerminating())
            return 0;
    }
    ++Data->BanditsHired;
    SpawnFunction(new CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThingParam>("WatchForBanditSpeechMarkers", 0,
        &CQ_AmbushTradersScript::WatchForBanditSpeechMarkers, this, bandit));
    return AddBanditHealthBar(bandit);
}

// ---- stubs ----
static const char* g_lastStr = 0; static int g_strings = 0, g_strDtors = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; g_lastStr = text; ++g_strings; }
CCharString::~CCharString() { ++g_strDtors; }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CScriptThingParam::~CScriptThingParam() {}
static CCPPointerInfo* g_storedInfo = 0; static long g_storedRef = 0;
CScriptThingStored::CScriptThingStored(const CScriptThingParam& other) : PImp(other.PImp) { g_storedInfo = other.PImp.Info; g_storedRef = other.PImp.Info ? other.PImp.Info->RefCount : -1; }
void CGameScriptInterface::v000() {}
void CGameScriptInterface::v001() {}
void CGameScriptInterface::v002() {}
void CGameScriptInterface::v003() {}
void CGameScriptInterface::v004() {}
void CGameScriptInterface::v005() {}
void CGameScriptInterface::v006() {}
void CGameScriptInterface::v008() {}
void CGameScriptInterface::v009() {}
void CGameScriptInterface::v010() {}
void CGameScriptInterface::v011() {}
void CGameScriptInterface::v012() {}
void CGameScriptInterface::v013() {}
void CGameScriptInterface::v014() {}
void CGameScriptInterface::v015() {}
void CGameScriptInterface::v016() {}
void CGameScriptInterface::v017() {}
void CGameScriptInterface::v018() {}
void CGameScriptInterface::v019() {}
void CGameScriptInterface::v020() {}
void CGameScriptInterface::v021() {}
void CGameScriptInterface::v022() {}
void CGameScriptInterface::v023() {}
void CGameScriptInterface::v024() {}
void CGameScriptInterface::v025() {}
void CGameScriptInterface::v026() {}
void CGameScriptInterface::v027() {}
void CGameScriptInterface::v028() {}
void CGameScriptInterface::v029() {}
void CGameScriptInterface::v030() {}
void CGameScriptInterface::v031() {}
void CGameScriptInterface::v032() {}
void CGameScriptInterface::v033() {}
void CGameScriptInterface::v034() {}
void CGameScriptInterface::v035() {}
void CGameScriptInterface::v036() {}
void CGameScriptInterface::v037() {}
void CGameScriptInterface::v038() {}
void CGameScriptInterface::v039() {}
void CGameScriptInterface::v040() {}
void CGameScriptInterface::v041() {}
void CGameScriptInterface::v042() {}
void CGameScriptInterface::v043() {}
void CGameScriptInterface::v044() {}
void CGameScriptInterface::v045() {}
void CGameScriptInterface::v046() {}
void CGameScriptInterface::v047() {}
void CGameScriptInterface::v048() {}
void CGameScriptInterface::v049() {}
void CGameScriptInterface::v050() {}
void CGameScriptInterface::v051() {}
void CGameScriptInterface::v052() {}
void CGameScriptInterface::v053() {}
void CGameScriptInterface::v054() {}
void CGameScriptInterface::v055() {}
void CGameScriptInterface::v056() {}
void CGameScriptInterface::v057() {}
void CGameScriptInterface::v058() {}
void CGameScriptInterface::v059() {}
void CGameScriptInterface::v060() {}
void CGameScriptInterface::v061() {}
void CGameScriptInterface::v062() {}
void CGameScriptInterface::v063() {}
void CGameScriptInterface::v064() {}
void CGameScriptInterface::v065() {}
void CGameScriptInterface::v066() {}
void CGameScriptInterface::v067() {}
void CGameScriptInterface::v068() {}
void CGameScriptInterface::v069() {}
void CGameScriptInterface::v071() {}
void CGameScriptInterface::v072() {}
void CGameScriptInterface::v073() {}
void CGameScriptInterface::v074() {}
void CGameScriptInterface::v075() {}
void CGameScriptInterface::v076() {}
void CGameScriptInterface::v077() {}
void CGameScriptInterface::v078() {}
void CGameScriptInterface::v079() {}
void CGameScriptInterface::v080() {}
void CGameScriptInterface::v081() {}
void CGameScriptInterface::v082() {}
void CGameScriptInterface::v083() {}
void CGameScriptInterface::v084() {}
void CGameScriptInterface::v085() {}
void CGameScriptInterface::v086() {}
void CGameScriptInterface::v087() {}
void CGameScriptInterface::v088() {}
void CGameScriptInterface::v089() {}
void CGameScriptInterface::v090() {}
void CGameScriptInterface::v091() {}
void CGameScriptInterface::v092() {}
void CGameScriptInterface::v093() {}
void CGameScriptInterface::v094() {}
void CGameScriptInterface::v095() {}
void CGameScriptInterface::v096() {}
void CGameScriptInterface::v097() {}
void CGameScriptInterface::v098() {}
void CGameScriptInterface::v099() {}
void CGameScriptInterface::v100() {}
void CGameScriptInterface::v101() {}
void CGameScriptInterface::v102() {}
void CGameScriptInterface::v103() {}
void CGameScriptInterface::v104() {}
void CGameScriptInterface::v105() {}
void CGameScriptInterface::v106() {}
void CGameScriptInterface::v107() {}
void CGameScriptInterface::v108() {}
void CGameScriptInterface::v109() {}
void CGameScriptInterface::v110() {}
void CGameScriptInterface::v111() {}
void CGameScriptInterface::v112() {}
void CGameScriptInterface::v113() {}
void CGameScriptInterface::v114() {}
void CGameScriptInterface::v115() {}
void CGameScriptInterface::v116() {}
void CGameScriptInterface::v117() {}
void CGameScriptInterface::v118() {}
void CGameScriptInterface::v119() {}
void CGameScriptInterface::v120() {}
void CGameScriptInterface::v121() {}
void CGameScriptInterface::v122() {}
void CGameScriptInterface::v123() {}
void CGameScriptInterface::v124() {}
void CGameScriptInterface::v125() {}
void CGameScriptInterface::v126() {}
void CGameScriptInterface::v127() {}
void CGameScriptInterface::v128() {}
void CGameScriptInterface::v129() {}
void CGameScriptInterface::v130() {}
void CGameScriptInterface::v131() {}
void CGameScriptInterface::v132() {}
void CGameScriptInterface::v133() {}
void CGameScriptInterface::v134() {}
void CGameScriptInterface::v135() {}
void CGameScriptInterface::v136() {}
void CGameScriptInterface::v137() {}
void CGameScriptInterface::v138() {}
void CGameScriptInterface::v139() {}
void CGameScriptInterface::v140() {}
void CGameScriptInterface::v141() {}
void CGameScriptInterface::v142() {}
void CGameScriptInterface::v143() {}
void CGameScriptInterface::v144() {}
void CGameScriptInterface::v145() {}
void CGameScriptInterface::v146() {}
void CGameScriptInterface::v147() {}
void CGameScriptInterface::v148() {}
void CGameScriptInterface::v149() {}
void CGameScriptInterface::v150() {}
void CGameScriptInterface::v151() {}
void CGameScriptInterface::v152() {}
void CGameScriptInterface::v153() {}
void CGameScriptInterface::v154() {}
void CGameScriptInterface::v155() {}
void CGameScriptInterface::v156() {}
void CGameScriptInterface::v157() {}
void CGameScriptInterface::v158() {}
void CGameScriptInterface::v159() {}
void CGameScriptInterface::v160() {}
void CGameScriptInterface::v161() {}
void CGameScriptInterface::v162() {}
void CGameScriptInterface::v163() {}
void CGameScriptInterface::v164() {}
void CGameScriptInterface::v165() {}
void CGameScriptInterface::v166() {}
void CGameScriptInterface::v167() {}
void CGameScriptInterface::v168() {}
void CGameScriptInterface::v169() {}
void CGameScriptInterface::v170() {}
void CGameScriptInterface::v171() {}
void CGameScriptInterface::v172() {}
void CGameScriptInterface::v173() {}
void CGameScriptInterface::v174() {}
void CGameScriptInterface::v175() {}
void CGameScriptInterface::v176() {}
void CGameScriptInterface::v177() {}
void CGameScriptInterface::v178() {}
void CGameScriptInterface::v179() {}
void CGameScriptInterface::v180() {}
void CGameScriptInterface::v181() {}
void CGameScriptInterface::v182() {}
void CGameScriptInterface::v183() {}
void CGameScriptInterface::v184() {}
void CGameScriptInterface::v185() {}
void CGameScriptInterface::v186() {}
void CGameScriptInterface::v187() {}
void CGameScriptInterface::v188() {}
void CGameScriptInterface::v189() {}
void CGameScriptInterface::v190() {}
void CGameScriptInterface::v191() {}
void CGameScriptInterface::v192() {}
void CGameScriptInterface::v193() {}
void CGameScriptInterface::v194() {}
void CGameScriptInterface::v195() {}
void CGameScriptInterface::v196() {}
void CGameScriptInterface::v197() {}
void CGameScriptInterface::v198() {}
void CGameScriptInterface::v199() {}
void CGameScriptInterface::v200() {}
void CGameScriptInterface::v201() {}
void CGameScriptInterface::v202() {}
void CGameScriptInterface::v203() {}
void CGameScriptInterface::v204() {}
void CGameScriptInterface::v205() {}
void CGameScriptInterface::v206() {}
void CGameScriptInterface::v207() {}
void CGameScriptInterface::v208() {}
void CGameScriptInterface::v209() {}
void CGameScriptInterface::v210() {}
void CGameScriptInterface::v211() {}
void CGameScriptInterface::v212() {}
void CGameScriptInterface::v213() {}
void CGameScriptInterface::v214() {}
void CGameScriptInterface::v215() {}
void CGameScriptInterface::v216() {}
void CGameScriptInterface::v217() {}
void CGameScriptInterface::v218() {}
void CGameScriptInterface::v219() {}
void CGameScriptInterface::v220() {}
void CGameScriptInterface::v221() {}
void CGameScriptInterface::v222() {}
void CGameScriptInterface::v223() {}
void CGameScriptInterface::v224() {}
void CGameScriptInterface::v225() {}
void CGameScriptInterface::v226() {}
void CGameScriptInterface::v227() {}
void CGameScriptInterface::v228() {}
void CGameScriptInterface::v229() {}
void CGameScriptInterface::v230() {}
void CGameScriptInterface::v231() {}
void CGameScriptInterface::v232() {}
void CGameScriptInterface::v233() {}
void CGameScriptInterface::v234() {}
void CGameScriptInterface::v235() {}
void CGameScriptInterface::v236() {}
void CGameScriptInterface::v237() {}
void CGameScriptInterface::v238() {}
void CGameScriptInterface::v239() {}
void CGameScriptInterface::v240() {}
void CGameScriptInterface::v241() {}
void CGameScriptInterface::v242() {}
void CGameScriptInterface::v243() {}
void CGameScriptInterface::v244() {}
void CGameScriptInterface::v245() {}
void CGameScriptInterface::v246() {}
void CGameScriptInterface::v247() {}
void CGameScriptInterface::v248() {}
void CGameScriptInterface::v249() {}
void CGameScriptInterface::v250() {}
void CGameScriptInterface::v251() {}
void CGameScriptInterface::v252() {}
void CGameScriptInterface::v253() {}
void CGameScriptInterface::v254() {}
void CGameScriptInterface::v255() {}
void CGameScriptInterface::v256() {}
void CGameScriptInterface::v257() {}
void CGameScriptInterface::v258() {}
void CGameScriptInterface::v259() {}
void CGameScriptInterface::v260() {}
void CGameScriptInterface::v261() {}
void CGameScriptInterface::v262() {}
void CGameScriptInterface::v263() {}
void CScriptGameResourceObjectScriptedThingBase::v00() {}
void CScriptGameResourceObjectScriptedThingBase::v01() {}
void CScriptGameResourceObjectScriptedThingBase::v02() {}
void CScriptGameResourceObjectScriptedThingBase::v03() {}
void CScriptGameResourceObjectScriptedThingBase::v04() {}
void CScriptGameResourceObjectScriptedThingBase::v05() {}
void CScriptGameResourceObjectScriptedThingBase::v06() {}
void CScriptGameResourceObjectScriptedThingBase::v07() {}
void CScriptGameResourceObjectScriptedThingBase::v08() {}
void CScriptGameResourceObjectScriptedThingBase::v09() {}
void CScriptGameResourceObjectScriptedThingBase::v10() {}
void CScriptGameResourceObjectScriptedThingBase::v11() {}
void CScriptGameResourceObjectScriptedThingBase::v14() {}
void CScriptGameResourceObjectScriptedThingBase::v15() {}
void CScriptGameResourceObjectScriptedThingBase::v16() {}
void CScriptGameResourceObjectScriptedThingBase::v17() {}
void CScriptGameResourceObjectScriptedThingBase::v18() {}
void CScriptGameResourceObjectScriptedThingBase::v19() {}
void CScriptGameResourceObjectScriptedThingBase::v20() {}
void CScriptGameResourceObjectScriptedThingBase::v21() {}
void CScriptGameResourceObjectScriptedThingBase::v22() {}
void CScriptGameResourceObjectScriptedThingBase::v23() {}
void CScriptGameResourceObjectScriptedThingBase::v24() {}
void CScriptGameResourceObjectScriptedThingBase::v25() {}
static float g_health = 1.0f; static int g_frames = 0, g_healthCalls = 0; static const CScriptThing* g_hero = 0;
static int g_termAt = -1, g_termCalls = 0, g_busyFrames = 0, g_speaks = 0, g_adds = 0, g_bars = 0;
static const char* g_speakText = 0; static const CScriptThing* g_speakListener = 0; static int g_speakArgs[4];
static CSpawnedFuncBase* g_added = 0; static const char* g_addRegion = 0; static const char* g_funcName = 0; static unsigned long g_funcId = 99;
static const CScriptThing* g_barThing = 0;
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; }
const CScriptThing& CGameScriptInterface::GetHero() const { return *g_hero; }
float CGameScriptInterface::GetHealth(const CScriptThing& thing) const { ++g_healthCalls; return g_health; }
CScriptThing CScriptGameResourceObjectScriptedThingBase::GetScriptThing() const { return *g_hero; }
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c)
{ ++g_speaks; g_speakListener = &listener; g_speakText = text; g_speakArgs[0] = method; g_speakArgs[1] = a; g_speakArgs[2] = b; g_speakArgs[3] = c; }
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask() const { if (g_busyFrames > 0) { --g_busyFrames; return true; } return false; }
CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id) { g_funcName = (const char*)name.PStringData; g_funcId = id; }
void CSpawnedFuncBase::v00() {}
template <class C, class P> void CSpawnedFunc1<C, P>::v00() {}
void CScriptBase::v00() {}
bool CScriptBase::IsActiveThreadTerminating() { return g_termCalls++ == g_termAt; }
void CScriptBase::Add(CSpawnedFuncBase* func, const CCharString& name) { ++g_adds; g_added = func; g_addRegion = (const char*)name.PStringData; }
void CQ_AmbushTradersScript::WatchForBanditSpeechMarkers(CScriptThingParam bandit) {}
long CQ_AmbushTradersScript::AddBanditHealthBar(const CScriptThing& bandit) { ++g_bars; g_barThing = &bandit; return 77; }
static void Reset() { g_frames = g_healthCalls = g_termCalls = g_busyFrames = g_speaks = g_adds = g_bars = 0; g_termAt = -1; g_added = 0; g_addRegion = 0; g_funcName = 0; g_funcId = 99; g_barThing = 0; g_storedInfo = 0; g_health = 1.0f; }
typedef CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThingParam> TFunc;
struct TObject : CScriptGameResourceObjectScriptedThingBase { };
int main()
{
    static long sbuf[0x30]; CQ_AmbushTradersScript& script = *(CQ_AmbushTradersScript*)sbuf;
    static CQuestData data; script.Data = &data;
    static CGameScriptInterface game; script.Game = &game;
    static CCPPointerInfo info = { 5, 0, 0 };
    static long tb[3], hb[3];
    CScriptThing& bandit = *(CScriptThing*)tb; tb[1] = 0x1111; tb[2] = (long)&info;
    CScriptThing& hero = *(CScriptThing*)hb; g_hero = &hero;
    TObject obj;
    // living bandit, busy for two frames: speaks to the hero, waits, then the hire
    Reset(); data.BanditsHired = 3; g_busyFrames = 2;
    long r = script.BanditHireSetup(bandit, obj);
    CHECK(r == 77); CHECK(g_healthCalls == 1); CHECK(g_speaks == 1 && g_speakListener == &hero);
    CHECK(strcmp(g_speakText, "TEXT_QST_003_BANDIT_HIRE_CONFIRM") == 0);
    CHECK(g_speakArgs[0] == 2 && g_speakArgs[1] == 0 && g_speakArgs[2] == 1 && g_speakArgs[3] == 0);
    CHECK(g_frames == 2); CHECK(g_termCalls == 3); CHECK(data.BanditsHired == 4);
    CHECK(g_adds == 1 && g_added != 0); CHECK(g_addRegion && g_addRegion[0] == 0);
    CHECK(g_funcName && strcmp(g_funcName, "WatchForBanditSpeechMarkers") == 0); CHECK(g_funcId == 0);
    TFunc* f = (TFunc*)g_added;
    CHECK(f->PClass == &script); CHECK(f->Func == &CQ_AmbushTradersScript::WatchForBanditSpeechMarkers);
    CHECK(g_storedInfo == &info && g_storedRef == 7);   // the by-value parameter and the stored copy each hold one
    CHECK(info.RefCount == 6);                          // the stored copy keeps it; the parameter released its own
    CHECK(g_bars == 1 && g_barThing == &bandit);
    // dead bandit: no speech, straight to the hire
    Reset(); g_health = 0.0f; data.BanditsHired = 0;
    r = script.BanditHireSetup(bandit, obj);
    CHECK(r == 77); CHECK(g_speaks == 0); CHECK(g_termCalls == 0); CHECK(data.BanditsHired == 1); CHECK(g_adds == 1);
    // thread terminates while the bandit is still talking: 0, no hire
    Reset(); g_busyFrames = 5; g_termAt = 1; data.BanditsHired = 0;
    r = script.BanditHireSetup(bandit, obj);
    CHECK(r == 0); CHECK(g_frames == 2); CHECK(data.BanditsHired == 0); CHECK(g_adds == 0 && g_bars == 0);
    // thread terminates right after the speech: 0, no hire
    Reset(); g_busyFrames = 0; g_termAt = 0; data.BanditsHired = 0;
    r = script.BanditHireSetup(bandit, obj);
    CHECK(r == 0); CHECK(g_frames == 0); CHECK(g_speaks == 1); CHECK(data.BanditsHired == 0); CHECK(g_adds == 0);
    if (fails == 0) printf("00f081e0_TEST PASS\n"); else printf("00f081e0_TEST FAIL\n");
    return fails;
}