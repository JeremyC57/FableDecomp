// Behaviour test for 00e38f80: final.cpp copied verbatim (32-bit `long`, calling-convention keywords and CRT names
// under g++; build with `g++ -w -fpermissive`), every declared callee stubbed below: the four game-interface slots record their arguments, CCharString is a counted strdup, Add records the spawned function, and the reference counts of the
// script thing are checked after each Init and after the spawned functions are deleted. CAttackingAssassin::Init
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#include <stdint.h>
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 int64_t
#define long int
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAttackingAssassin::Init() -- assassin-attack setup: clear Screamed, attach the thing to the V_AssassinAttacks script, make it a scripted-assassin opinion source, persistent, cutscene behaviour 1, then spawn the parent's WatchForAssassinDead watcher on it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E38F80, length 384 bytes (plain `ret`; vtable slot 2 = Init of the entity
//          script contract). Same TU family and model as the landed CAT_Phase3Archer::Init @ 0x00D169E0.
// Callees: the game interface's slots 465 EntityAttachToScript, 622 EntitySetAsOpinionSource, 477 SetThingPersistent, 512 EntitySetCutsceneBehaviour (cgsi_slots_aligned.tsv), CCharString(const char*, long) @ 0x0099EBF0 /
//          operator+ @ 0x0099F570 / ~CCharString @ 0x0099EAE0, operator new @ 0x00BFEA1A (0x48-byte CSpawnedFunc1<CV_AssassinAttacksScript,
//          CScriptThing>, vtable 0x012E37BC), CSpawnedFuncBase ctor @ 0x00CDD450, CScriptThing copy ctor @ 0x004ABE90 / ~CScriptThing
//          @ 0x004AA840, the parent script's Add(CSpawnedFuncBase*, const CCharString&) @ 0x00CB7E50 (manifest
//          "CGuiVarTransferStruct::Add" is propagated); the spawned function's SuspendableProcess is 0x00E39140.
// Shape: the nested new-expression keeps the callback-name temporaries alive through Add (their construction flags in
//          ebx, `mov ebx,7` then per-temporary `test bl,N` destruction), the by-value trigger parameter is destroyed after
//          construction; the interface calls take Me by const reference and the literals as implicit CCharString conversions.
// Layout: Ego_r.pdb CAttackingAssassin: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18, Screamed +0x1c.
#include <new>
struct CCharString {                                 // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

struct CCPPointerInfo { long References; };
struct C3DVector { float X, Y, Z; };

struct CCPPointerImp {                               // CCPPointer<CScriptThingImp>: Data, Info (memberwise copy adds a reference)
    void* Data;
    CCPPointerInfo* Info;
    CCPPointerImp() : Data(0), Info(0) {}
    CCPPointerImp(const CCPPointerImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
};
struct CScriptThingRuntime {
    virtual ~CScriptThingRuntime(); // CScriptThing RTTI slot 0, 0x004AB170
    CCPPointerImp PImp;
    virtual const CCharString& GetName() const;       // slot 1, 0x004AA8B0
    virtual CCharString GetDefName() const;           // slot 2, 0x004AA8D0
    virtual CCharString GetDataString() const;        // slot 3, 0x004AA900
    virtual void SetDataString(const CCharString&);   // slot 4, 0x004AA930
    virtual C3DVector GetFocalPos() const;            // slot 5, 0x004AA940
    virtual const C3DVector& GetPos() const;          // slot 6, 0x004AA980
    CScriptThingRuntime() {}
    // the copy constructor is the compiler-generated memberwise one
    CScriptThingRuntime(const struct CScriptThingStored& other);
};

// Same retail CScriptThing view, using the native out-of-line copy constructor
// for CSpawnedFunc1::Par1; the caller's by-value argument uses the inline copy above.
struct CScriptThingStored {
    virtual ~CScriptThingStored();
    CScriptThingRuntime* PImp_Data;
    CCPPointerInfo* PImp_Info;
    virtual const CCharString& GetName() const;
    virtual CCharString GetDefName() const;
    virtual CCharString GetDataString() const;
    virtual void SetDataString(const CCharString&);
    virtual C3DVector GetFocalPos() const;
    virtual const C3DVector& GetPos() const;
    CScriptThingStored(const CScriptThingRuntime& other); // retail 0x004ABE90
};
inline CScriptThingRuntime::CScriptThingRuntime(const CScriptThingStored& other)
{ PImp.Data = other.PImp_Data; PImp.Info = other.PImp_Info; if (PImp.Info) ++PImp.Info->References; }

struct CScriptProcessBaseRuntime {
    virtual ~CScriptProcessBaseRuntime();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    virtual void RuntimeHookCDD410(); // native one-byte ret at 0x00CDD410; original name/signature unresolved
    virtual void RuntimeHookCDD420(); // native one-byte ret at 0x00CDD420; original name/signature unresolved
    bool Running;
    bool StopRunning;
    bool TerminateUsingExceptions;
    unsigned char _pad_07;
    float TimeProcessSliceStarted;
    float TimesliceLength;
    CScriptProcessBaseRuntime(bool running, long stackSize, float fraction); // 0x00A44740
};

struct CSpawnedFuncBaseRuntime : CScriptProcessBaseRuntime {
    virtual ~CSpawnedFuncBaseRuntime(); // ordinary base destructor 0x00CDD4C0 cleans PPredicate
    unsigned long SpawnedFuncBaseState; // +0x10 word in the retail CSpawnedFuncBase subobject
    unsigned long ID;
    long LastScriptFrame;
    long CurrentScriptFrame;
    CCharString Name;
    CCharString AssociatedRegionName;
    bool FuncPaused;
    bool Interrupted;
    unsigned char _pad_2a[2];
    void* PPredicate_Data;
    CCPPointerInfo* PPredicate_Info;
    CSpawnedFuncBaseRuntime(const CCharString& name, unsigned long id); // 0x00CDD450
};

struct CSpawnedFuncPhase3;
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
    virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
    virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223();
    virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231();
    virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247();
    virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255();
    virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271();
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287();
    virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295();
    virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303();
    virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311();
    virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319();
    virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327();
    virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335();
    virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343();
    virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351();
    virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359();
    virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367();
    virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375();
    virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383();
    virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391();
    virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399();
    virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407();
    virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415();
    virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422(); virtual void v423();
    virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431();
    virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439();
    virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446(); virtual void v447();
    virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453(); virtual void v454(); virtual void v455();
    virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461(); virtual void v462(); virtual void v463();
    virtual void v464();
    virtual void EntityAttachToScript(const CScriptThingRuntime& thing, const CCharString& script) const;   // slot 465 (+0x744)
    virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470(); virtual void v471(); virtual void v472(); virtual void v473();
    virtual void v474(); virtual void v475(); virtual void v476();
    virtual void SetThingPersistent(const CScriptThingRuntime& thing, bool persistent) const;   // slot 477 (+0x774)
    virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485();
    virtual void v486(); virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493();
    virtual void v494(); virtual void v495();
    virtual void EntitySetAsScared(const CScriptThingRuntime& thing, bool scared) const;   // slot 496 (+0x7c0)
    virtual void v497(); virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501(); virtual void v502(); virtual void v503(); virtual void v504();
    virtual void v505(); virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510(); virtual void v511();
    virtual void EntitySetCutsceneBehaviour(const CScriptThingRuntime& thing, long behaviour) const;   // slot 512 (+0x800)
    virtual void v513(); virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517(); virtual void v518(); virtual void v519(); virtual void v520();
    virtual void v521(); virtual void v522(); virtual void v523(); virtual void v524(); virtual void v525(); virtual void v526(); virtual void v527(); virtual void v528();
    virtual void v529(); virtual void v530(); virtual void v531(); virtual void v532(); virtual void v533(); virtual void v534(); virtual void v535(); virtual void v536();
    virtual void v537(); virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541(); virtual void v542(); virtual void v543(); virtual void v544();
    virtual void v545(); virtual void v546(); virtual void v547(); virtual void v548(); virtual void v549(); virtual void v550(); virtual void v551(); virtual void v552();
    virtual void v553(); virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557(); virtual void v558(); virtual void v559(); virtual void v560();
    virtual void v561(); virtual void v562(); virtual void v563(); virtual void v564(); virtual void v565(); virtual void v566(); virtual void v567(); virtual void v568();
    virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573(); virtual void v574(); virtual void v575(); virtual void v576();
    virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581(); virtual void v582(); virtual void v583(); virtual void v584();
    virtual void v585(); virtual void v586(); virtual void v587(); virtual void v588(); virtual void v589(); virtual void v590(); virtual void v591(); virtual void v592();
    virtual void v593(); virtual void v594(); virtual void v595(); virtual void v596(); virtual void v597(); virtual void v598(); virtual void v599(); virtual void v600();
    virtual void v601(); virtual void v602(); virtual void v603(); virtual void v604(); virtual void v605(); virtual void v606(); virtual void v607(); virtual void v608();
    virtual void v609(); virtual void v610(); virtual void v611(); virtual void v612(); virtual void v613(); virtual void v614(); virtual void v615(); virtual void v616();
    virtual void EntitySetOpinionReactionEnabled(const CScriptThingRuntime& thing, long reaction, bool enabled) const;   // slot 617 (+0x9a4)
    virtual void v618(); virtual void v619(); virtual void v620(); virtual void v621();
    virtual void EntitySetAsOpinionSource(const CScriptThingRuntime& thing, const CCharString& source) const;   // slot 622 (+0x9b8)
};

struct CV_AssassinAttacksScript {
    void WatchForAssassinDead(CScriptThingRuntime thing);
    void Add(CSpawnedFuncPhase3* function, const CCharString& name); // 0x00CB7E50; observed function at +4, name at +8
};
typedef void (CV_AssassinAttacksScript::*SpawnedScriptCallback)(CScriptThingRuntime);
struct CSpawnedFuncPhase3 : CSpawnedFuncBaseRuntime {
    SpawnedScriptCallback Func;
    CV_AssassinAttacksScript* PClass;
    CScriptThingStored Par1;
    virtual void SuspendableProcess(); // 0x00E39140
    CSpawnedFuncPhase3(const CCharString& name, unsigned long id,
                       SpawnedScriptCallback func, CV_AssassinAttacksScript* owner,
                       CScriptThingRuntime par1);
};

struct CAttackingAssassin {
    void* __vftable;
    CGameScriptInterface* Game;
    CScriptThingRuntime Me;
    CV_AssassinAttacksScript* ParentClass;          // PDB +0x14
    void* PMasterData;                   // PDB +0x18
    bool Screamed;                       // PDB +0x1c
    void Init();
};

void CScriptProcessBaseRuntime::RuntimeHookCDD410() {}
void CScriptProcessBaseRuntime::RuntimeHookCDD420() {}
// Runtime slot body is external: retail D16780 copies Par1 directly into the
// callback's by-value argument, then sets StopRunning. The callback's native
// copy/destruction behavior is not synthesized into this Init translation unit.
CSpawnedFuncPhase3::CSpawnedFuncPhase3(const CCharString& name, unsigned long id,
                                       SpawnedScriptCallback func, CV_AssassinAttacksScript* owner,
                                       CScriptThingRuntime par1)
    : CSpawnedFuncBaseRuntime(name, id), Func(func), PClass(owner), Par1(par1)
{
}

void* __cdecl operator new(unsigned int bytes); // 0x00BFEA1A
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);

void CAttackingAssassin::Init()
{
    Screamed = false;
    Game->EntityAttachToScript(Me, "V_AssassinAttacks");
    Game->EntitySetAsOpinionSource(Me, "OPINION_SOURCE_SCRIPTED_ASSASSIN");
    Game->SetThingPersistent(Me, true);
    Game->EntitySetCutsceneBehaviour(Me, 1);
    ParentClass->Add(
        new CSpawnedFuncPhase3(
            CCharString("ParentClass.", -1) + CCharString("WatchForAssassinDead", -1),
            0, &CV_AssassinAttacksScript::WatchForAssassinDead, ParentClass, Me),
        CCharString("", -1));
}

// ---- stubs ----
static int g_liveStrings = 0;
CCharString::CCharString(const char* text, long length)
{
    size_t n = length < 0 ? std::strlen(text) : (size_t)length;
    char* p = (char*)std::malloc(n + 1); std::memcpy(p, text, n); p[n] = 0;
    PStringData = p; ++g_liveStrings;
}
CCharString::~CCharString() { std::free(PStringData); --g_liveStrings; }
CCharString __fastcall operator+(const CCharString& left, const CCharString& right)
{
    char t[256]; std::strcpy(t, (const char*)left.PStringData); std::strcat(t, (const char*)right.PStringData);
    return CCharString(t, -1);
}
#if defined(_MSC_VER)
void* __cdecl operator new(unsigned int n) { void* p = std::malloc(n); if (!p) throw std::bad_alloc(); return p; }
#else
void* operator new(size_t n) { void* p = std::malloc(n); if (!p) throw std::bad_alloc(); return p; }
void operator delete(void* p) throw() { std::free(p); }
#endif
CScriptThingRuntime::~CScriptThingRuntime() { if (PImp.Info) --PImp.Info->References; }
const CCharString& CScriptThingRuntime::GetName() const { static CCharString s("", -1); return s; }
CCharString CScriptThingRuntime::GetDefName() const { return CCharString("", -1); }
CCharString CScriptThingRuntime::GetDataString() const { return CCharString("", -1); }
void CScriptThingRuntime::SetDataString(const CCharString&) {}
C3DVector CScriptThingRuntime::GetFocalPos() const { C3DVector p = {0, 0, 0}; return p; }
const C3DVector& CScriptThingRuntime::GetPos() const { static C3DVector p = {0, 0, 0}; return p; }
static int g_par1Copies = 0;
CScriptThingStored::CScriptThingStored(const CScriptThingRuntime& o) : PImp_Data((CScriptThingRuntime*)o.PImp.Data), PImp_Info(o.PImp.Info)
{ ++g_par1Copies; if (PImp_Info) ++PImp_Info->References; }
CScriptThingStored::~CScriptThingStored() { if (PImp_Info) --PImp_Info->References; }
const CCharString& CScriptThingStored::GetName() const { static CCharString s("", -1); return s; }
CCharString CScriptThingStored::GetDefName() const { return CCharString("", -1); }
CCharString CScriptThingStored::GetDataString() const { return CCharString("", -1); }
void CScriptThingStored::SetDataString(const CCharString&) {}
C3DVector CScriptThingStored::GetFocalPos() const { C3DVector p = {0, 0, 0}; return p; }
const C3DVector& CScriptThingStored::GetPos() const { static C3DVector p = {0, 0, 0}; return p; }
CScriptProcessBaseRuntime::~CScriptProcessBaseRuntime() {}
void CScriptProcessBaseRuntime::RunSuspendableProcess() {}
void CScriptProcessBaseRuntime::SuspendSuspendableProcess(bool) {}
void CScriptProcessBaseRuntime::Restart() {}
CScriptProcessBaseRuntime::CScriptProcessBaseRuntime(bool r, long, float)
    : Running(r), StopRunning(false), TerminateUsingExceptions(false), _pad_07(0), TimeProcessSliceStarted(0), TimesliceLength(0) {}
CSpawnedFuncBaseRuntime::CSpawnedFuncBaseRuntime(const CCharString& n, unsigned long id)
    : CScriptProcessBaseRuntime(false, 0, 0), SpawnedFuncBaseState(0), ID(id), LastScriptFrame(0), CurrentScriptFrame(0),
      Name((const char*)n.PStringData, -1), AssociatedRegionName("", -1), FuncPaused(false), Interrupted(false),
      PPredicate_Data(0), PPredicate_Info(0) {}
CSpawnedFuncBaseRuntime::~CSpawnedFuncBaseRuntime() {}
static int g_callbacks = 0;
void CSpawnedFuncPhase3::SuspendableProcess() { (PClass->*Func)(CScriptThingRuntime(Par1)); StopRunning = true; }
static CScriptThingRuntime* g_expectedImp = 0;
void CV_AssassinAttacksScript::WatchForAssassinDead(CScriptThingRuntime thing) { ++g_callbacks; if (thing.PImp.Data != g_expectedImp) g_callbacks = -100; }
static int g_adds = 0; static CSpawnedFuncPhase3* g_added[2]; static long g_refAtAdd[2];
static CCPPointerInfo* g_info = 0;
void CV_AssassinAttacksScript::Add(CSpawnedFuncPhase3* f, const CCharString& n)
{
    if (g_adds >= 2 || !f || f->PClass != this || f->ID != 0 || f->Func != &CV_AssassinAttacksScript::WatchForAssassinDead ||
        f->Par1.PImp_Data != g_expectedImp || std::strcmp((const char*)f->Name.PStringData, "ParentClass.WatchForAssassinDead") ||
        std::strcmp((const char*)n.PStringData, "")) { g_adds = -100; return; }
    g_refAtAdd[g_adds] = g_info->References;
    g_added[g_adds++] = f;
}
// game interface: a concrete object whose slots record their arguments
struct SCall { int slot; const CScriptThingRuntime* thing; long a; const char* text; };
static SCall g_log[16]; static int g_logCount = 0;
static void Log(int slot, const CScriptThingRuntime& t, long a, const char* text) { if (g_logCount < 16) { SCall& c = g_log[g_logCount++]; c.slot = slot; c.thing = &t; c.a = a; c.text = text; } }
void CGameScriptInterface::v000() {}
void CGameScriptInterface::v001() {}
void CGameScriptInterface::v002() {}
void CGameScriptInterface::v003() {}
void CGameScriptInterface::v004() {}
void CGameScriptInterface::v005() {}
void CGameScriptInterface::v006() {}
void CGameScriptInterface::v007() {}
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
void CGameScriptInterface::v070() {}
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
void CGameScriptInterface::v264() {}
void CGameScriptInterface::v265() {}
void CGameScriptInterface::v266() {}
void CGameScriptInterface::v267() {}
void CGameScriptInterface::v268() {}
void CGameScriptInterface::v269() {}
void CGameScriptInterface::v270() {}
void CGameScriptInterface::v271() {}
void CGameScriptInterface::v272() {}
void CGameScriptInterface::v273() {}
void CGameScriptInterface::v274() {}
void CGameScriptInterface::v275() {}
void CGameScriptInterface::v276() {}
void CGameScriptInterface::v277() {}
void CGameScriptInterface::v278() {}
void CGameScriptInterface::v279() {}
void CGameScriptInterface::v280() {}
void CGameScriptInterface::v281() {}
void CGameScriptInterface::v282() {}
void CGameScriptInterface::v283() {}
void CGameScriptInterface::v284() {}
void CGameScriptInterface::v285() {}
void CGameScriptInterface::v286() {}
void CGameScriptInterface::v287() {}
void CGameScriptInterface::v288() {}
void CGameScriptInterface::v289() {}
void CGameScriptInterface::v290() {}
void CGameScriptInterface::v291() {}
void CGameScriptInterface::v292() {}
void CGameScriptInterface::v293() {}
void CGameScriptInterface::v294() {}
void CGameScriptInterface::v295() {}
void CGameScriptInterface::v296() {}
void CGameScriptInterface::v297() {}
void CGameScriptInterface::v298() {}
void CGameScriptInterface::v299() {}
void CGameScriptInterface::v300() {}
void CGameScriptInterface::v301() {}
void CGameScriptInterface::v302() {}
void CGameScriptInterface::v303() {}
void CGameScriptInterface::v304() {}
void CGameScriptInterface::v305() {}
void CGameScriptInterface::v306() {}
void CGameScriptInterface::v307() {}
void CGameScriptInterface::v308() {}
void CGameScriptInterface::v309() {}
void CGameScriptInterface::v310() {}
void CGameScriptInterface::v311() {}
void CGameScriptInterface::v312() {}
void CGameScriptInterface::v313() {}
void CGameScriptInterface::v314() {}
void CGameScriptInterface::v315() {}
void CGameScriptInterface::v316() {}
void CGameScriptInterface::v317() {}
void CGameScriptInterface::v318() {}
void CGameScriptInterface::v319() {}
void CGameScriptInterface::v320() {}
void CGameScriptInterface::v321() {}
void CGameScriptInterface::v322() {}
void CGameScriptInterface::v323() {}
void CGameScriptInterface::v324() {}
void CGameScriptInterface::v325() {}
void CGameScriptInterface::v326() {}
void CGameScriptInterface::v327() {}
void CGameScriptInterface::v328() {}
void CGameScriptInterface::v329() {}
void CGameScriptInterface::v330() {}
void CGameScriptInterface::v331() {}
void CGameScriptInterface::v332() {}
void CGameScriptInterface::v333() {}
void CGameScriptInterface::v334() {}
void CGameScriptInterface::v335() {}
void CGameScriptInterface::v336() {}
void CGameScriptInterface::v337() {}
void CGameScriptInterface::v338() {}
void CGameScriptInterface::v339() {}
void CGameScriptInterface::v340() {}
void CGameScriptInterface::v341() {}
void CGameScriptInterface::v342() {}
void CGameScriptInterface::v343() {}
void CGameScriptInterface::v344() {}
void CGameScriptInterface::v345() {}
void CGameScriptInterface::v346() {}
void CGameScriptInterface::v347() {}
void CGameScriptInterface::v348() {}
void CGameScriptInterface::v349() {}
void CGameScriptInterface::v350() {}
void CGameScriptInterface::v351() {}
void CGameScriptInterface::v352() {}
void CGameScriptInterface::v353() {}
void CGameScriptInterface::v354() {}
void CGameScriptInterface::v355() {}
void CGameScriptInterface::v356() {}
void CGameScriptInterface::v357() {}
void CGameScriptInterface::v358() {}
void CGameScriptInterface::v359() {}
void CGameScriptInterface::v360() {}
void CGameScriptInterface::v361() {}
void CGameScriptInterface::v362() {}
void CGameScriptInterface::v363() {}
void CGameScriptInterface::v364() {}
void CGameScriptInterface::v365() {}
void CGameScriptInterface::v366() {}
void CGameScriptInterface::v367() {}
void CGameScriptInterface::v368() {}
void CGameScriptInterface::v369() {}
void CGameScriptInterface::v370() {}
void CGameScriptInterface::v371() {}
void CGameScriptInterface::v372() {}
void CGameScriptInterface::v373() {}
void CGameScriptInterface::v374() {}
void CGameScriptInterface::v375() {}
void CGameScriptInterface::v376() {}
void CGameScriptInterface::v377() {}
void CGameScriptInterface::v378() {}
void CGameScriptInterface::v379() {}
void CGameScriptInterface::v380() {}
void CGameScriptInterface::v381() {}
void CGameScriptInterface::v382() {}
void CGameScriptInterface::v383() {}
void CGameScriptInterface::v384() {}
void CGameScriptInterface::v385() {}
void CGameScriptInterface::v386() {}
void CGameScriptInterface::v387() {}
void CGameScriptInterface::v388() {}
void CGameScriptInterface::v389() {}
void CGameScriptInterface::v390() {}
void CGameScriptInterface::v391() {}
void CGameScriptInterface::v392() {}
void CGameScriptInterface::v393() {}
void CGameScriptInterface::v394() {}
void CGameScriptInterface::v395() {}
void CGameScriptInterface::v396() {}
void CGameScriptInterface::v397() {}
void CGameScriptInterface::v398() {}
void CGameScriptInterface::v399() {}
void CGameScriptInterface::v400() {}
void CGameScriptInterface::v401() {}
void CGameScriptInterface::v402() {}
void CGameScriptInterface::v403() {}
void CGameScriptInterface::v404() {}
void CGameScriptInterface::v405() {}
void CGameScriptInterface::v406() {}
void CGameScriptInterface::v407() {}
void CGameScriptInterface::v408() {}
void CGameScriptInterface::v409() {}
void CGameScriptInterface::v410() {}
void CGameScriptInterface::v411() {}
void CGameScriptInterface::v412() {}
void CGameScriptInterface::v413() {}
void CGameScriptInterface::v414() {}
void CGameScriptInterface::v415() {}
void CGameScriptInterface::v416() {}
void CGameScriptInterface::v417() {}
void CGameScriptInterface::v418() {}
void CGameScriptInterface::v419() {}
void CGameScriptInterface::v420() {}
void CGameScriptInterface::v421() {}
void CGameScriptInterface::v422() {}
void CGameScriptInterface::v423() {}
void CGameScriptInterface::v424() {}
void CGameScriptInterface::v425() {}
void CGameScriptInterface::v426() {}
void CGameScriptInterface::v427() {}
void CGameScriptInterface::v428() {}
void CGameScriptInterface::v429() {}
void CGameScriptInterface::v430() {}
void CGameScriptInterface::v431() {}
void CGameScriptInterface::v432() {}
void CGameScriptInterface::v433() {}
void CGameScriptInterface::v434() {}
void CGameScriptInterface::v435() {}
void CGameScriptInterface::v436() {}
void CGameScriptInterface::v437() {}
void CGameScriptInterface::v438() {}
void CGameScriptInterface::v439() {}
void CGameScriptInterface::v440() {}
void CGameScriptInterface::v441() {}
void CGameScriptInterface::v442() {}
void CGameScriptInterface::v443() {}
void CGameScriptInterface::v444() {}
void CGameScriptInterface::v445() {}
void CGameScriptInterface::v446() {}
void CGameScriptInterface::v447() {}
void CGameScriptInterface::v448() {}
void CGameScriptInterface::v449() {}
void CGameScriptInterface::v450() {}
void CGameScriptInterface::v451() {}
void CGameScriptInterface::v452() {}
void CGameScriptInterface::v453() {}
void CGameScriptInterface::v454() {}
void CGameScriptInterface::v455() {}
void CGameScriptInterface::v456() {}
void CGameScriptInterface::v457() {}
void CGameScriptInterface::v458() {}
void CGameScriptInterface::v459() {}
void CGameScriptInterface::v460() {}
void CGameScriptInterface::v461() {}
void CGameScriptInterface::v462() {}
void CGameScriptInterface::v463() {}
void CGameScriptInterface::v464() {}
void CGameScriptInterface::v466() {}
void CGameScriptInterface::v467() {}
void CGameScriptInterface::v468() {}
void CGameScriptInterface::v469() {}
void CGameScriptInterface::v470() {}
void CGameScriptInterface::v471() {}
void CGameScriptInterface::v472() {}
void CGameScriptInterface::v473() {}
void CGameScriptInterface::v474() {}
void CGameScriptInterface::v475() {}
void CGameScriptInterface::v476() {}
void CGameScriptInterface::v478() {}
void CGameScriptInterface::v479() {}
void CGameScriptInterface::v480() {}
void CGameScriptInterface::v481() {}
void CGameScriptInterface::v482() {}
void CGameScriptInterface::v483() {}
void CGameScriptInterface::v484() {}
void CGameScriptInterface::v485() {}
void CGameScriptInterface::v486() {}
void CGameScriptInterface::v487() {}
void CGameScriptInterface::v488() {}
void CGameScriptInterface::v489() {}
void CGameScriptInterface::v490() {}
void CGameScriptInterface::v491() {}
void CGameScriptInterface::v492() {}
void CGameScriptInterface::v493() {}
void CGameScriptInterface::v494() {}
void CGameScriptInterface::v495() {}
void CGameScriptInterface::v497() {}
void CGameScriptInterface::v498() {}
void CGameScriptInterface::v499() {}
void CGameScriptInterface::v500() {}
void CGameScriptInterface::v501() {}
void CGameScriptInterface::v502() {}
void CGameScriptInterface::v503() {}
void CGameScriptInterface::v504() {}
void CGameScriptInterface::v505() {}
void CGameScriptInterface::v506() {}
void CGameScriptInterface::v507() {}
void CGameScriptInterface::v508() {}
void CGameScriptInterface::v509() {}
void CGameScriptInterface::v510() {}
void CGameScriptInterface::v511() {}
void CGameScriptInterface::v513() {}
void CGameScriptInterface::v514() {}
void CGameScriptInterface::v515() {}
void CGameScriptInterface::v516() {}
void CGameScriptInterface::v517() {}
void CGameScriptInterface::v518() {}
void CGameScriptInterface::v519() {}
void CGameScriptInterface::v520() {}
void CGameScriptInterface::v521() {}
void CGameScriptInterface::v522() {}
void CGameScriptInterface::v523() {}
void CGameScriptInterface::v524() {}
void CGameScriptInterface::v525() {}
void CGameScriptInterface::v526() {}
void CGameScriptInterface::v527() {}
void CGameScriptInterface::v528() {}
void CGameScriptInterface::v529() {}
void CGameScriptInterface::v530() {}
void CGameScriptInterface::v531() {}
void CGameScriptInterface::v532() {}
void CGameScriptInterface::v533() {}
void CGameScriptInterface::v534() {}
void CGameScriptInterface::v535() {}
void CGameScriptInterface::v536() {}
void CGameScriptInterface::v537() {}
void CGameScriptInterface::v538() {}
void CGameScriptInterface::v539() {}
void CGameScriptInterface::v540() {}
void CGameScriptInterface::v541() {}
void CGameScriptInterface::v542() {}
void CGameScriptInterface::v543() {}
void CGameScriptInterface::v544() {}
void CGameScriptInterface::v545() {}
void CGameScriptInterface::v546() {}
void CGameScriptInterface::v547() {}
void CGameScriptInterface::v548() {}
void CGameScriptInterface::v549() {}
void CGameScriptInterface::v550() {}
void CGameScriptInterface::v551() {}
void CGameScriptInterface::v552() {}
void CGameScriptInterface::v553() {}
void CGameScriptInterface::v554() {}
void CGameScriptInterface::v555() {}
void CGameScriptInterface::v556() {}
void CGameScriptInterface::v557() {}
void CGameScriptInterface::v558() {}
void CGameScriptInterface::v559() {}
void CGameScriptInterface::v560() {}
void CGameScriptInterface::v561() {}
void CGameScriptInterface::v562() {}
void CGameScriptInterface::v563() {}
void CGameScriptInterface::v564() {}
void CGameScriptInterface::v565() {}
void CGameScriptInterface::v566() {}
void CGameScriptInterface::v567() {}
void CGameScriptInterface::v568() {}
void CGameScriptInterface::v569() {}
void CGameScriptInterface::v570() {}
void CGameScriptInterface::v571() {}
void CGameScriptInterface::v572() {}
void CGameScriptInterface::v573() {}
void CGameScriptInterface::v574() {}
void CGameScriptInterface::v575() {}
void CGameScriptInterface::v576() {}
void CGameScriptInterface::v577() {}
void CGameScriptInterface::v578() {}
void CGameScriptInterface::v579() {}
void CGameScriptInterface::v580() {}
void CGameScriptInterface::v581() {}
void CGameScriptInterface::v582() {}
void CGameScriptInterface::v583() {}
void CGameScriptInterface::v584() {}
void CGameScriptInterface::v585() {}
void CGameScriptInterface::v586() {}
void CGameScriptInterface::v587() {}
void CGameScriptInterface::v588() {}
void CGameScriptInterface::v589() {}
void CGameScriptInterface::v590() {}
void CGameScriptInterface::v591() {}
void CGameScriptInterface::v592() {}
void CGameScriptInterface::v593() {}
void CGameScriptInterface::v594() {}
void CGameScriptInterface::v595() {}
void CGameScriptInterface::v596() {}
void CGameScriptInterface::v597() {}
void CGameScriptInterface::v598() {}
void CGameScriptInterface::v599() {}
void CGameScriptInterface::v600() {}
void CGameScriptInterface::v601() {}
void CGameScriptInterface::v602() {}
void CGameScriptInterface::v603() {}
void CGameScriptInterface::v604() {}
void CGameScriptInterface::v605() {}
void CGameScriptInterface::v606() {}
void CGameScriptInterface::v607() {}
void CGameScriptInterface::v608() {}
void CGameScriptInterface::v609() {}
void CGameScriptInterface::v610() {}
void CGameScriptInterface::v611() {}
void CGameScriptInterface::v612() {}
void CGameScriptInterface::v613() {}
void CGameScriptInterface::v614() {}
void CGameScriptInterface::v615() {}
void CGameScriptInterface::v616() {}
void CGameScriptInterface::v618() {}
void CGameScriptInterface::v619() {}
void CGameScriptInterface::v620() {}
void CGameScriptInterface::v621() {}

static char g_scriptText[64];
void CGameScriptInterface::EntityAttachToScript(const CScriptThingRuntime& t, const CCharString& script) const
{ std::strcpy(g_scriptText, (const char*)script.PStringData); Log(465, t, 0, g_scriptText); }
void CGameScriptInterface::SetThingPersistent(const CScriptThingRuntime& t, bool persistent) const { Log(477, t, persistent ? 1 : 0, 0); }
void CGameScriptInterface::EntitySetAsScared(const CScriptThingRuntime&, bool) const {}
void CGameScriptInterface::EntitySetCutsceneBehaviour(const CScriptThingRuntime& t, long behaviour) const { Log(512, t, behaviour, 0); }
void CGameScriptInterface::EntitySetOpinionReactionEnabled(const CScriptThingRuntime&, long, bool) const {}
static char g_sourceText[64];
void CGameScriptInterface::EntitySetAsOpinionSource(const CScriptThingRuntime& t, const CCharString& source) const
{ std::strcpy(g_sourceText, (const char*)source.PStringData); Log(622, t, 0, g_sourceText); }

int main()
{
    CCPPointerInfo info; info.References = 2; g_info = &info;
    CScriptThingRuntime imp; g_expectedImp = &imp;
    CGameScriptInterface game;
    CV_AssassinAttacksScript parent;
    CAttackingAssassin h; h.__vftable = 0; h.Game = &game; h.Me.PImp.Data = &imp; h.Me.PImp.Info = &info; h.ParentClass = &parent; h.PMasterData = 0;
    h.Screamed = true;
    int stringsBefore = g_liveStrings;
    h.Init();
    CHECK(!h.Screamed);
    CHECK(g_logCount == 4);
    CHECK(g_log[0].slot == 465 && g_log[0].thing == &h.Me && std::strcmp(g_log[0].text, "V_AssassinAttacks") == 0);
    CHECK(g_log[1].slot == 622 && g_log[1].thing == &h.Me && std::strcmp(g_log[1].text, "OPINION_SOURCE_SCRIPTED_ASSASSIN") == 0);
    CHECK(g_log[2].slot == 477 && g_log[2].thing == &h.Me && g_log[2].a == 1);
    CHECK(g_log[3].slot == 512 && g_log[3].thing == &h.Me && g_log[3].a == 1);
    CHECK(g_adds == 1);
    CHECK(info.References == 3);                 // the by-value temporary is gone, Par1 holds one
    CHECK(g_par1Copies == 1);
    h.Screamed = true;
    h.Init();
    CHECK(!h.Screamed && g_adds == 2 && g_logCount == 8 && info.References == 4);
#if defined(_MSC_VER)
    CHECK(g_refAtAdd[0] == 3 && g_refAtAdd[1] == 4);   // VC7.1 destroys the by-value temporary before Add (g++ after the full expression)
#else
    CHECK(g_refAtAdd[0] == 4 && g_refAtAdd[1] == 5);
#endif
    CHECK(g_liveStrings == stringsBefore + 4);
    g_added[0]->SuspendableProcess(); g_added[1]->SuspendableProcess();
    CHECK(g_callbacks == 2 && g_added[0]->StopRunning && g_added[1]->StopRunning);
    CHECK(info.References == 4);
    delete g_added[0]; delete g_added[1];
    CHECK(info.References == 2);
    CHECK(g_liveStrings == stringsBefore);
    if (fails == 0) printf("00e38f80_TEST PASS\n"); else printf("FAILS: %d\n", fails);
    return fails ? 1 : 0;
}