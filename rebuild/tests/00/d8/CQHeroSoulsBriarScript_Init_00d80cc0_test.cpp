// Behaviour test for 00d80cc0: src/00d80cc0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CQ_HeroSoulsBriarScript::Init()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQ_HeroSoulsBriarScript::Init() -- reset the quest flags and both imposter timers, then register the quest region ("Q_HeroSoulsBriar", "NorthernWastes3")
// Shape note: CTimer::Set reads the index through a const reference (`const long& index = Index;`): VC7.1 then keeps
// the index load after the PlayedBriarCS byte store and the interface load (retail order); the plain member read is hoisted.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80CC0, length 130 bytes (padded to 144).
// Callees: CGameScriptInterface::SetTimer (slot 89, +0x164) through the global @ 0x0143E8F8 (x2),
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2: "NorthernWastes3" @ 0x012C4538, "Q_HeroSoulsBriar"
// @ 0x012D31AC), CGameScriptInterface::AddQuestRegion (slot 271, +0x43c) on the script's own Game (+0x40),
// CCharString::~CCharString @ 0x0099EAE0 (x2).
// Vtable 0x012D41BC slot 3. Layout (retail; PDB offsets -0x14): PlayedBriarCS +0x48, the two timers +0x68/+0x6c,
// KickOffQuestStartScreen +0x70.
// Shape (landed 0x00E1A870 form): both literals are implicit conversion temporaries built right to left (region first)
// in an 8-byte frame and destroyed after the call in reverse order; the KickOff byte store is scheduled into the first
// string's argument pushes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void v088();
    virtual void SetTimer(long timer, long time) const;   // slot 89 (+0x164)
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
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269();
    virtual void v270();
    virtual void AddQuestRegion(const CCharString& quest, const CCharString& region);   // slot 271 (+0x43c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
struct CTimer {                                    // a script timer: the interface's timer index
    long Index;
    void Set(long time) { const long& index = Index; g_pGameScriptInterface->SetTimer(index, time); }
};
struct CQ_HeroSoulsBriarScript {
    unsigned char         _pad_0x00[0x40];
    CGameScriptInterface* Game;                       // +0x40
    unsigned char         _pad_0x44[4];
    bool                  PlayedBriarCS;              // +0x48
    unsigned char         _pad_0x49[0x68 - 0x49];     // seh_BriarRose +0x4c, csargs +0x5c
    CTimer                BriarRoseImposterTauntTimer; // +0x68
    CTimer                BriarRoseImposterHereTimer;  // +0x6c
    bool                  KickOffQuestStartScreen;     // +0x70
    void Init();
};
void CQ_HeroSoulsBriarScript::Init()
{
    PlayedBriarCS = false;
    BriarRoseImposterTauntTimer.Set(0);
    BriarRoseImposterHereTimer.Set(0);
    KickOffQuestStartScreen = false;
    Game->AddQuestRegion("Q_HeroSoulsBriar", "NorthernWastes3");
}

// ---- stubs and driver ----
void CGameScriptInterface::v000() { }
void CGameScriptInterface::v001() { }
void CGameScriptInterface::v002() { }
void CGameScriptInterface::v003() { }
void CGameScriptInterface::v004() { }
void CGameScriptInterface::v005() { }
void CGameScriptInterface::v006() { }
void CGameScriptInterface::v007() { }
void CGameScriptInterface::v008() { }
void CGameScriptInterface::v009() { }
void CGameScriptInterface::v010() { }
void CGameScriptInterface::v011() { }
void CGameScriptInterface::v012() { }
void CGameScriptInterface::v013() { }
void CGameScriptInterface::v014() { }
void CGameScriptInterface::v015() { }
void CGameScriptInterface::v016() { }
void CGameScriptInterface::v017() { }
void CGameScriptInterface::v018() { }
void CGameScriptInterface::v019() { }
void CGameScriptInterface::v020() { }
void CGameScriptInterface::v021() { }
void CGameScriptInterface::v022() { }
void CGameScriptInterface::v023() { }
void CGameScriptInterface::v024() { }
void CGameScriptInterface::v025() { }
void CGameScriptInterface::v026() { }
void CGameScriptInterface::v027() { }
void CGameScriptInterface::v028() { }
void CGameScriptInterface::v029() { }
void CGameScriptInterface::v030() { }
void CGameScriptInterface::v031() { }
void CGameScriptInterface::v032() { }
void CGameScriptInterface::v033() { }
void CGameScriptInterface::v034() { }
void CGameScriptInterface::v035() { }
void CGameScriptInterface::v036() { }
void CGameScriptInterface::v037() { }
void CGameScriptInterface::v038() { }
void CGameScriptInterface::v039() { }
void CGameScriptInterface::v040() { }
void CGameScriptInterface::v041() { }
void CGameScriptInterface::v042() { }
void CGameScriptInterface::v043() { }
void CGameScriptInterface::v044() { }
void CGameScriptInterface::v045() { }
void CGameScriptInterface::v046() { }
void CGameScriptInterface::v047() { }
void CGameScriptInterface::v048() { }
void CGameScriptInterface::v049() { }
void CGameScriptInterface::v050() { }
void CGameScriptInterface::v051() { }
void CGameScriptInterface::v052() { }
void CGameScriptInterface::v053() { }
void CGameScriptInterface::v054() { }
void CGameScriptInterface::v055() { }
void CGameScriptInterface::v056() { }
void CGameScriptInterface::v057() { }
void CGameScriptInterface::v058() { }
void CGameScriptInterface::v059() { }
void CGameScriptInterface::v060() { }
void CGameScriptInterface::v061() { }
void CGameScriptInterface::v062() { }
void CGameScriptInterface::v063() { }
void CGameScriptInterface::v064() { }
void CGameScriptInterface::v065() { }
void CGameScriptInterface::v066() { }
void CGameScriptInterface::v067() { }
void CGameScriptInterface::v068() { }
void CGameScriptInterface::v069() { }
void CGameScriptInterface::v070() { }
void CGameScriptInterface::v071() { }
void CGameScriptInterface::v072() { }
void CGameScriptInterface::v073() { }
void CGameScriptInterface::v074() { }
void CGameScriptInterface::v075() { }
void CGameScriptInterface::v076() { }
void CGameScriptInterface::v077() { }
void CGameScriptInterface::v078() { }
void CGameScriptInterface::v079() { }
void CGameScriptInterface::v080() { }
void CGameScriptInterface::v081() { }
void CGameScriptInterface::v082() { }
void CGameScriptInterface::v083() { }
void CGameScriptInterface::v084() { }
void CGameScriptInterface::v085() { }
void CGameScriptInterface::v086() { }
void CGameScriptInterface::v088() { }
void CGameScriptInterface::v090() { }
void CGameScriptInterface::v091() { }
void CGameScriptInterface::v092() { }
void CGameScriptInterface::v093() { }
void CGameScriptInterface::v094() { }
void CGameScriptInterface::v095() { }
void CGameScriptInterface::v096() { }
void CGameScriptInterface::v097() { }
void CGameScriptInterface::v098() { }
void CGameScriptInterface::v099() { }
void CGameScriptInterface::v100() { }
void CGameScriptInterface::v101() { }
void CGameScriptInterface::v102() { }
void CGameScriptInterface::v103() { }
void CGameScriptInterface::v104() { }
void CGameScriptInterface::v105() { }
void CGameScriptInterface::v106() { }
void CGameScriptInterface::v107() { }
void CGameScriptInterface::v108() { }
void CGameScriptInterface::v109() { }
void CGameScriptInterface::v110() { }
void CGameScriptInterface::v111() { }
void CGameScriptInterface::v112() { }
void CGameScriptInterface::v113() { }
void CGameScriptInterface::v114() { }
void CGameScriptInterface::v115() { }
void CGameScriptInterface::v116() { }
void CGameScriptInterface::v117() { }
void CGameScriptInterface::v118() { }
void CGameScriptInterface::v119() { }
void CGameScriptInterface::v120() { }
void CGameScriptInterface::v121() { }
void CGameScriptInterface::v122() { }
void CGameScriptInterface::v123() { }
void CGameScriptInterface::v124() { }
void CGameScriptInterface::v125() { }
void CGameScriptInterface::v126() { }
void CGameScriptInterface::v127() { }
void CGameScriptInterface::v128() { }
void CGameScriptInterface::v129() { }
void CGameScriptInterface::v130() { }
void CGameScriptInterface::v131() { }
void CGameScriptInterface::v132() { }
void CGameScriptInterface::v133() { }
void CGameScriptInterface::v134() { }
void CGameScriptInterface::v135() { }
void CGameScriptInterface::v136() { }
void CGameScriptInterface::v137() { }
void CGameScriptInterface::v138() { }
void CGameScriptInterface::v139() { }
void CGameScriptInterface::v140() { }
void CGameScriptInterface::v141() { }
void CGameScriptInterface::v142() { }
void CGameScriptInterface::v143() { }
void CGameScriptInterface::v144() { }
void CGameScriptInterface::v145() { }
void CGameScriptInterface::v146() { }
void CGameScriptInterface::v147() { }
void CGameScriptInterface::v148() { }
void CGameScriptInterface::v149() { }
void CGameScriptInterface::v150() { }
void CGameScriptInterface::v151() { }
void CGameScriptInterface::v152() { }
void CGameScriptInterface::v153() { }
void CGameScriptInterface::v154() { }
void CGameScriptInterface::v155() { }
void CGameScriptInterface::v156() { }
void CGameScriptInterface::v157() { }
void CGameScriptInterface::v158() { }
void CGameScriptInterface::v159() { }
void CGameScriptInterface::v160() { }
void CGameScriptInterface::v161() { }
void CGameScriptInterface::v162() { }
void CGameScriptInterface::v163() { }
void CGameScriptInterface::v164() { }
void CGameScriptInterface::v165() { }
void CGameScriptInterface::v166() { }
void CGameScriptInterface::v167() { }
void CGameScriptInterface::v168() { }
void CGameScriptInterface::v169() { }
void CGameScriptInterface::v170() { }
void CGameScriptInterface::v171() { }
void CGameScriptInterface::v172() { }
void CGameScriptInterface::v173() { }
void CGameScriptInterface::v174() { }
void CGameScriptInterface::v175() { }
void CGameScriptInterface::v176() { }
void CGameScriptInterface::v177() { }
void CGameScriptInterface::v178() { }
void CGameScriptInterface::v179() { }
void CGameScriptInterface::v180() { }
void CGameScriptInterface::v181() { }
void CGameScriptInterface::v182() { }
void CGameScriptInterface::v183() { }
void CGameScriptInterface::v184() { }
void CGameScriptInterface::v185() { }
void CGameScriptInterface::v186() { }
void CGameScriptInterface::v187() { }
void CGameScriptInterface::v188() { }
void CGameScriptInterface::v189() { }
void CGameScriptInterface::v190() { }
void CGameScriptInterface::v191() { }
void CGameScriptInterface::v192() { }
void CGameScriptInterface::v193() { }
void CGameScriptInterface::v194() { }
void CGameScriptInterface::v195() { }
void CGameScriptInterface::v196() { }
void CGameScriptInterface::v197() { }
void CGameScriptInterface::v198() { }
void CGameScriptInterface::v199() { }
void CGameScriptInterface::v200() { }
void CGameScriptInterface::v201() { }
void CGameScriptInterface::v202() { }
void CGameScriptInterface::v203() { }
void CGameScriptInterface::v204() { }
void CGameScriptInterface::v205() { }
void CGameScriptInterface::v206() { }
void CGameScriptInterface::v207() { }
void CGameScriptInterface::v208() { }
void CGameScriptInterface::v209() { }
void CGameScriptInterface::v210() { }
void CGameScriptInterface::v211() { }
void CGameScriptInterface::v212() { }
void CGameScriptInterface::v213() { }
void CGameScriptInterface::v214() { }
void CGameScriptInterface::v215() { }
void CGameScriptInterface::v216() { }
void CGameScriptInterface::v217() { }
void CGameScriptInterface::v218() { }
void CGameScriptInterface::v219() { }
void CGameScriptInterface::v220() { }
void CGameScriptInterface::v221() { }
void CGameScriptInterface::v222() { }
void CGameScriptInterface::v223() { }
void CGameScriptInterface::v224() { }
void CGameScriptInterface::v225() { }
void CGameScriptInterface::v226() { }
void CGameScriptInterface::v227() { }
void CGameScriptInterface::v228() { }
void CGameScriptInterface::v229() { }
void CGameScriptInterface::v230() { }
void CGameScriptInterface::v231() { }
void CGameScriptInterface::v232() { }
void CGameScriptInterface::v233() { }
void CGameScriptInterface::v234() { }
void CGameScriptInterface::v235() { }
void CGameScriptInterface::v236() { }
void CGameScriptInterface::v237() { }
void CGameScriptInterface::v238() { }
void CGameScriptInterface::v239() { }
void CGameScriptInterface::v240() { }
void CGameScriptInterface::v241() { }
void CGameScriptInterface::v242() { }
void CGameScriptInterface::v243() { }
void CGameScriptInterface::v244() { }
void CGameScriptInterface::v245() { }
void CGameScriptInterface::v246() { }
void CGameScriptInterface::v247() { }
void CGameScriptInterface::v248() { }
void CGameScriptInterface::v249() { }
void CGameScriptInterface::v250() { }
void CGameScriptInterface::v251() { }
void CGameScriptInterface::v252() { }
void CGameScriptInterface::v253() { }
void CGameScriptInterface::v254() { }
void CGameScriptInterface::v255() { }
void CGameScriptInterface::v256() { }
void CGameScriptInterface::v257() { }
void CGameScriptInterface::v258() { }
void CGameScriptInterface::v259() { }
void CGameScriptInterface::v260() { }
void CGameScriptInterface::v261() { }
void CGameScriptInterface::v262() { }
void CGameScriptInterface::v263() { }
void CGameScriptInterface::v264() { }
void CGameScriptInterface::v265() { }
void CGameScriptInterface::v266() { }
void CGameScriptInterface::v267() { }
void CGameScriptInterface::v268() { }
void CGameScriptInterface::v269() { }
void CGameScriptInterface::v270() { }
static int g_setCalls = 0; static long g_setTimer[4]; static long g_setTime[4];
long CGameScriptInterface::RegisterTimer() { return 0; }
void CGameScriptInterface::SetTimer(long timer, long time) const { if (g_setCalls < 4) { g_setTimer[g_setCalls] = timer; g_setTime[g_setCalls] = time; } ++g_setCalls; }
static int g_regionCalls = 0; static const char* g_quest = 0; static const char* g_region = 0;
void CGameScriptInterface::AddQuestRegion(const CCharString& quest, const CCharString& region) { ++g_regionCalls; g_quest = (const char*)quest.PStringData; g_region = (const char*)region.PStringData; }
static int g_strings = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; ++g_strings; CHECK(length == -1); }
CCharString::~CCharString() { --g_strings; }
CGameScriptInterface* g_pGameScriptInterface = 0;
int main()
{
    CGameScriptInterface gsi; g_pGameScriptInterface = &gsi;
    CQ_HeroSoulsBriarScript s; memset(&s, 0x11, sizeof(s));
    s.Game = &gsi; s.BriarRoseImposterTauntTimer.Index = 7; s.BriarRoseImposterHereTimer.Index = 9;
    s.Init();
    CHECK(s.PlayedBriarCS == false); CHECK(s.KickOffQuestStartScreen == false);
    CHECK(g_setCalls == 2); CHECK(g_setTimer[0] == 7 && g_setTime[0] == 0); CHECK(g_setTimer[1] == 9 && g_setTime[1] == 0);
    CHECK(g_regionCalls == 1); CHECK(strcmp(g_quest, "Q_HeroSoulsBriar") == 0); CHECK(strcmp(g_region, "NorthernWastes3") == 0);
    CHECK(g_strings == 0);
    if (fails == 0) printf("00d80cc0_TEST PASS\n"); else printf("00d80cc0_TEST FAIL\n");
    return fails;
}
