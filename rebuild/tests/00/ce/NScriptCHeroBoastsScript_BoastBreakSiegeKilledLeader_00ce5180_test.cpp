// Behaviour test for 00ce5180: the model below is a copy of src/00ce5180.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
template <class T> T* AddressOfTemporary(const T& t) { return const_cast<T*>(&t); }
class CCharString { public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};

class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012();
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString& quest) const;   // slot 28 (+0x70)
    virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString& quest) const;      // slot 29 (+0x74)
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164();
    virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
    virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189();
    virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194();
    virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204();
    virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224();
    virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229();
    virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234();
    virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244();
    virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254();
    virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259();
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264();
    virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269();
    virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274();
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284();
    virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289();
    virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299();
    virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
    virtual void SetBoastAsFailed(long boast, const CCharString& quest) const;      // slot 305 (+0x4c4)
    virtual void SetBoastAsCompleted(long boast, const CCharString& quest) const;   // slot 306 (+0x4c8)
};

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0x117];
    bool          BreakSiegeKilledLeader;               // +0x117
};

struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

enum { BOAST_BREAK_SIEGE_KILLED_LEADER = 26 };

class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    void BoastBreakSiegeKilledLeader(CCharString questName);
};

void CHeroBoastsScript::BoastBreakSiegeKilledLeader(CCharString questName)
{
    while (!IsActiveThreadTerminating()) {
        if (!GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
            if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName) && PMasterData->BreakSiegeKilledLeader) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->SetBoastAsCompleted(BOAST_BREAK_SIEGE_KILLED_LEADER, questName);
                return;
            }
            if (!GSI->MsgOnQuestCompletedBeforeScreenShown(questName)) {
                GSI->NewScriptFrame();
                continue;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->SetBoastAsFailed(BOAST_BREAK_SIEGE_KILLED_LEADER, questName);
        return;
    }
}
}

// ---- test doubles and driver ----
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
CCharString::~CCharString() {}
void NScript::CScriptBase::v00() {}
static int g_terminateAfter = 1000; static int g_termCalls = 0;
bool NScript::CScriptBase::IsActiveThreadTerminating() { return ++g_termCalls > g_terminateAfter; }
static bool g_completed = false, g_failed = false; static int g_frames = 0, g_completeAfterFrames = 0;
static long g_doneBoast = -1, g_failBoast = -1; static int g_reports = 0; static const void* g_quest = 0;
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (g_frames == g_completeAfterFrames) g_completed = true; }
bool CGameScriptInterface::MsgOnQuestCompletedBeforeScreenShown(const CCharString& q) const { g_quest = q.PStringData; return g_completed; }
bool CGameScriptInterface::MsgOnQuestFailedBeforeScreenShown(const CCharString& q) const { g_quest = q.PStringData; return g_failed; }
void CGameScriptInterface::SetBoastAsFailed(long b, const CCharString&) const { g_failBoast = b; ++g_reports; }
void CGameScriptInterface::SetBoastAsCompleted(long b, const CCharString&) const { g_doneBoast = b; ++g_reports; }
static void Reset() { g_terminateAfter = 1000; g_termCalls = 0; g_completed = g_failed = false; g_frames = 0; g_completeAfterFrames = 0; g_doneBoast = g_failBoast = -1; g_reports = 0; }
int main()
{
    using namespace NScript;
    static CGameScriptInterface gsi; static CQ_SunnyvaleMasterData md; static CHeroBoastsScript s;
    s.GSI = &gsi; s.PMasterData = &md;
    CCharString quest; quest.PStringData = (void*)"Q";
    bool ok = true;
    // success after two idle frames
    Reset(); md.BreakSiegeKilledLeader = true; g_completeAfterFrames = 2;
    s.BoastBreakSiegeKilledLeader(quest);
    if (g_frames != 2 || g_doneBoast != 26 || g_failBoast != -1 || g_reports != 1 || g_quest != quest.PStringData) ok = false;
    // completed with the wrong flag: failure
    Reset(); md.BreakSiegeKilledLeader = false; g_completed = true;
    s.BoastBreakSiegeKilledLeader(quest);
    if (g_failBoast != 26 || g_doneBoast != -1 || g_reports != 1 || g_frames) ok = false;
    // quest failed: failure
    Reset(); md.BreakSiegeKilledLeader = true; g_failed = true; g_completed = false;
    s.BoastBreakSiegeKilledLeader(quest);
    if (g_reports != 1 || g_failBoast != 26 || g_doneBoast != -1) ok = false;
    // the thread ends while waiting: no report
    Reset(); g_terminateAfter = 2;
    s.BoastBreakSiegeKilledLeader(quest);
    if (g_reports != 0 || g_frames != 2) ok = false;
    // the thread ends just as the quest completes: no report
    Reset(); md.BreakSiegeKilledLeader = true; g_completed = true; g_terminateAfter = 1;
    s.BoastBreakSiegeKilledLeader(quest);
    if (g_reports != 0) ok = false;
    if (!ok) { std::printf("00ce5180_TEST FAIL\n"); return 1; }
    std::printf("00ce5180_TEST PASS\n");
    return 0;
}