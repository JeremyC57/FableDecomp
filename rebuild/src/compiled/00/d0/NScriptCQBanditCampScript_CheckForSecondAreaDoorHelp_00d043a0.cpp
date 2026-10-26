// Leaf: CheckForSecondAreaDoorHelp
// Grade: RELOCATION_MATCH 948/948 on the corrected span 0x00D043A0+948 (code + embedded jump table; bounds audit) -- catalog row 976 B over-captured 0x00D04760; really CQ_BanditCampScript::CheckForSecondAreaDoorHelp (catalog: EndMission, propagated)
// CQ_BanditCampScript::CheckForSecondAreaDoorHelp() -- once the bandit raid is under way, while the hero lingers outside BanditCampResidential the guildmaster cycles through his THROUGH_GATE hints every 20 timer ticks; stops when the hero reaches BanditCampBoss
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D043A0, length 964 bytes, plain `ret` (+ a five-entry switch table).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CTimer (inline ctor/dtor on the global interface 0x0143E8F8, slots 87..90; out-of-line ~CTimer 0x00CD4470
//   at the return before the switch); GameCalls slots 7 NewScriptFrame, 11 IsLevelLoaded, 274
//   HeroReceiveMessageFromGuildMaster. Strings "BanditCampResidential", "BanditCampBoss",
//   "TEXT_QST_009_GUILDMASTER_THROUGH_GATE_10" .. "_50".
// Layout: Ego_r.pdb CQ_BanditCampScript minus 0x14 (retail CScriptBase): ActivatedBanditRaid +0x4d.
// Identity: spawned as "CheckForSecondAreaDoorHelp" (string 0x012C8910) by the CQ_BanditCampScript spawn list at
//   0x00D041B9 (member pointer stored at 0x00D041E3); the catalog's CV_ChapelOfEvilScript::EndMission is a propagated label.
// Shape: generated script code; `for (;;)` with the terminate return first; the hint switch's last case loops back to
//   stage 3 (cases 2 and 4 share the store).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct GameCalls {                                       // CGameScriptInterface, retail vtable 0x01260F0C
 virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
 virtual void NewScriptFrame(void) const; //slot7 0x006e7410
 virtual void v008(); virtual void v009(); virtual void v010();
 virtual bool IsLevelLoaded(CCharString const &) const; //slot11 0x0088e2a0
 virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
 virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
 virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
 virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
 virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
 virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
 virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
 virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
 virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
 virtual void v084(); virtual void v085(); virtual void v086();
 virtual long RegisterTimer(void); //slot87 0x006e79e0
 virtual void DeregisterTimer(long); //slot88 0x006e7690
 virtual void SetTimer(long, long); //slot89 0x006e76e0
 virtual long GetTimer(long) const; //slot90 0x006e7710
 virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
 virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
 virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
 virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
 virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
 virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
 virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146();
 virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
 virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
 virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170();
 virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
 virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186();
 virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194();
 virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202();
 virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210();
 virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218();
 virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226();
 virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234();
 virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242();
 virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250();
 virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258();
 virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266();
 virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273();
 virtual void HeroReceiveMessageFromGuildMaster(CCharString const &, CCharString const &, bool, bool) const; //slot274 0x008918c0
};
extern GameCalls* g_pGameScriptInterface;                   // 0x0143E8F8
struct CTimer {                                             // a script timer: the interface's timer index
    long Index;
    CTimer() { Index = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }   // out of line 0x00CD4470
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
};
struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
namespace NScript {
class CQ_BanditCampScript : public CScriptBase {
public:
    GameCalls*    Game;                                     // +0x40
    void*         Data;                                     // +0x44
    unsigned char _pad48[0x4d - 0x48];
    bool          ActivatedBanditRaid;                      // +0x4d
    void CheckForSecondAreaDoorHelp();
};
} // namespace NScript
using NScript::CQ_BanditCampScript;

void NScript::CQ_BanditCampScript::CheckForSecondAreaDoorHelp()
{
    while (!ActivatedBanditRaid) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    timer.SetTime(5);
    long stage = 0;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("BanditCampResidential")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsLevelLoaded("BanditCampBoss")) {
                if (IsActiveThreadTerminating())
                    return;
                return;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() <= 0) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            switch (stage) {
            case 0:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_10", "", true, true);
                stage = 1;
                break;
            case 1:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_20", "", true, true);
                stage = 2;
                break;
            case 2:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_30", "", true, true);
                stage = 3;
                break;
            case 3:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_40", "", true, true);
                stage = 4;
                break;
            case 4:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_50", "", true, true);
                stage = 3;
                break;
            }
            timer.SetTime(20);
        }
        Game->NewScriptFrame();
    }
}