#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
// CEndGameScarletRobe::Main() -- entity-script Main (vtable 0x012F701C slot 1): register an is-alive functor over Me, run one script frame, then (unless done) flag Me as having information and keep trying, a frame at a time, to start scripting Me through a fresh local handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFF4D0, 226 bytes (+ int3 padding).
// Identity: the catalog/BSim label "NScript::CQ_EndGameFocalSitesScript::CEndGameScarletRobe::Main" is a 0.58-similarity guess;
//   the body is slot 1 of vtable 0x012F701C, built by the 0x1c-byte factory 0x00EFF3F0 (registered under the name "Dummy"
//   at 0x00EFF14E). 28 bytes matches the PDB CEndGameScarletRobe, so the label is kept; the true class is unproven.
// Callees: CScriptThing copy ctor 0x004ABE90 (the functor's member), the functor registration 0x00F35B10 and IsDone 0x00F35B30
//   (on `this`), the functor's out-of-line dtor 0x00EF7AB0 (vtable 0x012C2FE8, landed as CScriptThingFunctor_00ef7ab0),
//   CBaseObject::CBaseObject() 0x0099A380, the handle's IsScripting 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0
//   (vtable 0x0127094C = CScriptGameResourceObjectScriptedThingBase), game interface slots 7 NewScriptFrame, 8
//   StartScriptingEntity, 360 SetThingHasInformation(thing, false, true, false).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: the functor is a scoped frame object (its own {} block); the handle is a function-scope local default-constructed
//   inline (CBaseObject ctor, own vtable, PImp {0,0} as an embedded member object so the zero stores follow the vtable store);
//   the `while (!Start(...)) { NewScriptFrame(); if (IsDone()) return; } IsDone();` loop is rotated with &Me in edi.
struct CScriptThing {                                  // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
};

struct CScriptThingFunctor_00ef7ab0 {                  // vtable 0x012C2FE8
    virtual void v00() {}
    CScriptThing m_thing;                              // +0x04
    CScriptThingFunctor_00ef7ab0(const CScriptThing& thing) : m_thing(thing) { }
    ~CScriptThingFunctor_00ef7ab0();                   // 0x00EF7AB0
};

struct CBaseObject {
    virtual void v00() {}
    void* IntelligentPointer;                          // +0x04
    CBaseObject();                                     // 0x0099A380
};

struct CCountedPointer_ScriptedThing {                 // PImp {Data, Info}
    void* Data;
    void* Info;
    CCountedPointer_ScriptedThing() : Data(0), Info(0) { }
};

struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer_ScriptedThing PImp;                // +0x08
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
};

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

struct CGameScriptInterface {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
    virtual void NewScriptFrame() const = 0;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority) = 0;   // slot 8 (+0x20)
    virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {}
    virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {}
    virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {} virtual void v032() {}
    virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {} virtual void v040() {}
    virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {} virtual void v048() {}
    virtual void v049() {} virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {} virtual void v056() {}
    virtual void v057() {} virtual void v058() {} virtual void v059() {} virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {} virtual void v064() {}
    virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {} virtual void v070() {} virtual void v071() {} virtual void v072() {}
    virtual void v073() {} virtual void v074() {} virtual void v075() {} virtual void v076() {} virtual void v077() {} virtual void v078() {} virtual void v079() {} virtual void v080() {}
    virtual void v081() {} virtual void v082() {} virtual void v083() {} virtual void v084() {} virtual void v085() {} virtual void v086() {} virtual void v087() {} virtual void v088() {}
    virtual void v089() {} virtual void v090() {} virtual void v091() {} virtual void v092() {} virtual void v093() {} virtual void v094() {} virtual void v095() {} virtual void v096() {}
    virtual void v097() {} virtual void v098() {} virtual void v099() {} virtual void v100() {} virtual void v101() {} virtual void v102() {} virtual void v103() {} virtual void v104() {}
    virtual void v105() {} virtual void v106() {} virtual void v107() {} virtual void v108() {} virtual void v109() {} virtual void v110() {} virtual void v111() {} virtual void v112() {}
    virtual void v113() {} virtual void v114() {} virtual void v115() {} virtual void v116() {} virtual void v117() {} virtual void v118() {} virtual void v119() {} virtual void v120() {}
    virtual void v121() {} virtual void v122() {} virtual void v123() {} virtual void v124() {} virtual void v125() {} virtual void v126() {} virtual void v127() {} virtual void v128() {}
    virtual void v129() {} virtual void v130() {} virtual void v131() {} virtual void v132() {} virtual void v133() {} virtual void v134() {} virtual void v135() {} virtual void v136() {}
    virtual void v137() {} virtual void v138() {} virtual void v139() {} virtual void v140() {} virtual void v141() {} virtual void v142() {} virtual void v143() {} virtual void v144() {}
    virtual void v145() {} virtual void v146() {} virtual void v147() {} virtual void v148() {} virtual void v149() {} virtual void v150() {} virtual void v151() {} virtual void v152() {}
    virtual void v153() {} virtual void v154() {} virtual void v155() {} virtual void v156() {} virtual void v157() {} virtual void v158() {} virtual void v159() {} virtual void v160() {}
    virtual void v161() {} virtual void v162() {} virtual void v163() {} virtual void v164() {} virtual void v165() {} virtual void v166() {} virtual void v167() {} virtual void v168() {}
    virtual void v169() {} virtual void v170() {} virtual void v171() {} virtual void v172() {} virtual void v173() {} virtual void v174() {} virtual void v175() {} virtual void v176() {}
    virtual void v177() {} virtual void v178() {} virtual void v179() {} virtual void v180() {} virtual void v181() {} virtual void v182() {} virtual void v183() {} virtual void v184() {}
    virtual void v185() {} virtual void v186() {} virtual void v187() {} virtual void v188() {} virtual void v189() {} virtual void v190() {} virtual void v191() {} virtual void v192() {}
    virtual void v193() {} virtual void v194() {} virtual void v195() {} virtual void v196() {} virtual void v197() {} virtual void v198() {} virtual void v199() {} virtual void v200() {}
    virtual void v201() {} virtual void v202() {} virtual void v203() {} virtual void v204() {} virtual void v205() {} virtual void v206() {} virtual void v207() {} virtual void v208() {}
    virtual void v209() {} virtual void v210() {} virtual void v211() {} virtual void v212() {} virtual void v213() {} virtual void v214() {} virtual void v215() {} virtual void v216() {}
    virtual void v217() {} virtual void v218() {} virtual void v219() {} virtual void v220() {} virtual void v221() {} virtual void v222() {} virtual void v223() {} virtual void v224() {}
    virtual void v225() {} virtual void v226() {} virtual void v227() {} virtual void v228() {} virtual void v229() {} virtual void v230() {} virtual void v231() {} virtual void v232() {}
    virtual void v233() {} virtual void v234() {} virtual void v235() {} virtual void v236() {} virtual void v237() {} virtual void v238() {} virtual void v239() {} virtual void v240() {}
    virtual void v241() {} virtual void v242() {} virtual void v243() {} virtual void v244() {} virtual void v245() {} virtual void v246() {} virtual void v247() {} virtual void v248() {}
    virtual void v249() {} virtual void v250() {} virtual void v251() {} virtual void v252() {} virtual void v253() {} virtual void v254() {} virtual void v255() {} virtual void v256() {}
    virtual void v257() {} virtual void v258() {} virtual void v259() {} virtual void v260() {} virtual void v261() {} virtual void v262() {} virtual void v263() {} virtual void v264() {}
    virtual void v265() {} virtual void v266() {} virtual void v267() {} virtual void v268() {} virtual void v269() {} virtual void v270() {} virtual void v271() {} virtual void v272() {}
    virtual void v273() {} virtual void v274() {} virtual void v275() {} virtual void v276() {} virtual void v277() {} virtual void v278() {} virtual void v279() {} virtual void v280() {}
    virtual void v281() {} virtual void v282() {} virtual void v283() {} virtual void v284() {} virtual void v285() {} virtual void v286() {} virtual void v287() {} virtual void v288() {}
    virtual void v289() {} virtual void v290() {} virtual void v291() {} virtual void v292() {} virtual void v293() {} virtual void v294() {} virtual void v295() {} virtual void v296() {}
    virtual void v297() {} virtual void v298() {} virtual void v299() {} virtual void v300() {} virtual void v301() {} virtual void v302() {} virtual void v303() {} virtual void v304() {}
    virtual void v305() {} virtual void v306() {} virtual void v307() {} virtual void v308() {} virtual void v309() {} virtual void v310() {} virtual void v311() {} virtual void v312() {}
    virtual void v313() {} virtual void v314() {} virtual void v315() {} virtual void v316() {} virtual void v317() {} virtual void v318() {} virtual void v319() {} virtual void v320() {}
    virtual void v321() {} virtual void v322() {} virtual void v323() {} virtual void v324() {} virtual void v325() {} virtual void v326() {} virtual void v327() {} virtual void v328() {}
    virtual void v329() {} virtual void v330() {} virtual void v331() {} virtual void v332() {} virtual void v333() {} virtual void v334() {} virtual void v335() {} virtual void v336() {}
    virtual void v337() {} virtual void v338() {} virtual void v339() {} virtual void v340() {} virtual void v341() {} virtual void v342() {} virtual void v343() {} virtual void v344() {}
    virtual void v345() {} virtual void v346() {} virtual void v347() {} virtual void v348() {} virtual void v349() {} virtual void v350() {} virtual void v351() {} virtual void v352() {}
    virtual void v353() {} virtual void v354() {} virtual void v355() {} virtual void v356() {} virtual void v357() {} virtual void v358() {} virtual void v359() {}
    virtual void SetThingHasInformation(const CScriptThing& thing, bool a, bool b, bool c) const = 0;   // slot 360 (+0x5a0)
};

struct CEndGameScarletRobe {
    void*                 __vftable;                   // +0x00
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void*                 ParentClass;                 // +0x14
    void*                 PMasterData;                 // +0x18
    void RegisterFunctor(const CScriptThingFunctor_00ef7ab0& functor);   // 0x00F35B10
    bool IsDone();                                     // 0x00F35B30
    void Main();
};

void CEndGameScarletRobe::Main()
{
    RegisterFunctor(CScriptThingFunctor_00ef7ab0(Me));
    Game->NewScriptFrame();
    if (IsDone())
        return;
    Game->SetThingHasInformation(Me, false, true, false);
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    IsDone();
}

// ---- test doubles and driver ----
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
CScriptThing::CScriptThing(const CScriptThing& other) { std::memcpy(_body, other._body, 12); Log(1); }
CScriptThingFunctor_00ef7ab0::~CScriptThingFunctor_00ef7ab0() { Log(2); }
CBaseObject::CBaseObject() : IntelligentPointer(0) { Log(3); }
CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase() { Log(4); }
static bool g_scripting = true;
bool CScriptGameResourceObjectScriptedThingBase::IsScripting() { Log(PImp.Data == 0 && PImp.Info == 0 ? 5 : 99); return g_scripting; }
void CScriptGameResourceObjectScriptedThingBase::Release() { Log(6); }
static int g_startFailures = 0; static int g_doneAfter = 100; static int g_doneCalls = 0;
struct TestGSI : CGameScriptInterface {
    const CScriptThing* expect;
    virtual void NewScriptFrame() const { Log(7); }
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority priority)
    { Log(thing._body[0] == 0x5a && &thing == expect && priority == 4 ? 8 : 98); if (g_startFailures > 0) { --g_startFailures; return false; } return true; }
    virtual void SetThingHasInformation(const CScriptThing& thing, bool a, bool b, bool c) const { Log(!a && b && !c && &thing == expect ? 9 : 97); }
};
void CEndGameScarletRobe::RegisterFunctor(const CScriptThingFunctor_00ef7ab0& f) { Log(f.m_thing._body[0] == 0x5a ? 10 : 96); }
bool CEndGameScarletRobe::IsDone() { Log(11); return ++g_doneCalls >= g_doneAfter; }
static bool Run(int failures, int doneAfter, bool scripting, const int* want, int nwant)
{
    TestGSI gsi; static unsigned long raw[8]; CEndGameScarletRobe& s = *reinterpret_cast<CEndGameScarletRobe*>(raw); std::memset(s.Me._body, 0x5a, 12); s.Game = &gsi; gsi.expect = &s.Me;
    g_n = 0; g_startFailures = failures; g_doneAfter = doneAfter; g_doneCalls = 0; g_scripting = scripting;
    s.Main();
    if (g_n != nwant) return false;
    for (int i = 0; i < nwant; ++i) if (g_log[i] != want[i]) return false;
    return true;
}
int main()
{
    // done right after the first frame: functor built/registered/destroyed, one frame, stop
    static const int a[] = { 1, 10, 2, 7, 11 };
    // succeeds first time: handle built, scripting released, started, IsDone once more, handle destroyed
    static const int b_info[] = { 1, 10, 2, 7, 11, 9, 3, 5, 6, 8, 11, 4 };
    static const int b_noinfo[] = { 1, 10, 2, 7, 11, 3, 5, 6, 8, 11, 4 };
    // two failures then done during the wait: no final IsDone after the loop, handle still destroyed
    static const int c_info[] = { 1, 10, 2, 7, 11, 9, 3, 5, 8, 7, 11, 8, 7, 11, 4 };
    static const int c_noinfo[] = { 1, 10, 2, 7, 11, 3, 5, 8, 7, 11, 8, 7, 11, 4 };
    bool ok = Run(0, 1, true, a, 5);
    if (1) {
        ok = ok && Run(0, 100, true, b_info, 12) && Run(5, 3, false, c_info, 15);
    } else {
        ok = ok && Run(0, 100, true, b_noinfo, 11) && Run(5, 3, false, c_noinfo, 14);
    }
    if (!ok) { std::printf("00eff4d0_TEST FAIL\n"); return 1; }
    std::printf("00eff4d0_TEST PASS\n");
    return 0;
}
