// CQ_BreakSiegeScript::Init() -- register the two Break Siege quest regions, clear the quest flags, reset the master
// Leaf: ?Init@CQ_BreakSiegeScript@@
// data's siege boast results and offer the five Break Siege boasts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D1CC40, length 696 bytes (padded to 704).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's
// interface (+0x40): AddQuestRegion (slot 271, +0x43c) x2 and AddBoast (slot 303, +0x4bc) x5.
// Data: the script def 0x0143E90C: BreakSiegeNaked/NoDamage/NoWeapons boasts +0xa8..+0xbc (Ego_r.pdb -4); the
// hire-no-guards / kill-bandit-leader boast values +0xb94..+0xba0 are TLC additions Ego_r.pdb lacks (names
// descriptive). The quest master data (+0x44, CQ_SunnyvaleMasterData): BreakSiegeNoAssistance +0x116,
// BreakSiegeKilledLeader +0x117 (Ego_r.pdb offsets).
// Layout (Ego_r.pdb - 0x14): flags +0x48..+0x56, ReasonForFailure +0x58.
// Shape: literals are implicit conversion temporaries; every clear and the boasts' false/0 arguments share ebx.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CGameScriptInterface {                      // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
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
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269();
    virtual void v270();
    virtual void AddQuestRegion(const CCharString& quest, const CCharString& region) const;   // slot 271 (+0x43c) 0x00891820 public: virtual void __fastcall CGameScriptInterface::AddQuestRegion(class CCharString const &, class CCharString const &) const
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281();
    virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291();
    virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301();
    virtual void v302();
    virtual void AddBoast(const CCharString& description, long boastID, long cost, long reward, bool flag, const CCharString& extra, long extraValue) const;   // slot 303 (+0x4bc) 0x00893060 public: virtual void __fastcall CGameScriptInterface::AddBoast(class CCharString const &, long, long, long, bool, class CCharString const &, long) const
};

struct CScriptDef {
    unsigned char _pad_0x000[0xa8];
    long BreakSiegeNakedBoastCost;                     // +0xa8
    long BreakSiegeNakedBoastReward;                   // +0xac
    long BreakSiegeNoDamageBoastCost;                  // +0xb0
    long BreakSiegeNoDamageBoastReward;                // +0xb4
    long BreakSiegeNoWeaponsBoastCost;                 // +0xb8
    long BreakSiegeNoWeaponsBoastReward;               // +0xbc
    unsigned char _pad_0x0c0[0xb94 - 0xc0];
    long BreakSiegeHireNoGuardsBoastCost;              // +0xb94
    long BreakSiegeKillBanditLeaderBoastCost;          // +0xb98
    long BreakSiegeHireNoGuardsBoastReward;            // +0xb9c
    long BreakSiegeKillBanditLeaderBoastReward;        // +0xba0
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CQ_SunnyvaleMasterData {
    unsigned char _pad_0x000[0x116];
    bool BreakSiegeNoAssistance;                       // +0x116
    bool BreakSiegeKilledLeader;                       // +0x117
};

struct CQ_BreakSiegeScript {
    unsigned char           _pad_0x00[0x40];           // CQuestScriptBase / CScriptBase
    CGameScriptInterface*   Game;                      // +0x40
    CQ_SunnyvaleMasterData* Data;                      // +0x44
    bool MissionSucceeded;                             // +0x48
    bool MissionFailed;                                // +0x49
    bool BanditDemandsKnown;                           // +0x4a
    bool ChiefRefusesDemands;                          // +0x4b
    bool BanditLeaderFree;                             // +0x4c
    bool BanditSiegeStart;                             // +0x4d
    bool HasFirstWaveAttacked;                         // +0x4e
    bool StartSecondWave;                              // +0x4f
    bool BanditsAttackHero;                            // +0x50
    bool HeroLetInTown;                                // +0x51
    bool GuardFollowingHero;                           // +0x52
    bool EndCutsceneStarts;                            // +0x53
    bool EndCutsceneDone;                              // +0x54
    bool AttackedBanditLeader;                         // +0x55
    bool QuestStartScreened;                           // +0x56
    long ReasonForFailure;                             // +0x58
    void Init();
};

void CQ_BreakSiegeScript::Init()
{
    Game->AddQuestRegion("Q_BreakSiege", "KnotholeGlade");
    Game->AddQuestRegion("Q_BreakSiege", "DemonDoor_KnotholeGlade");
    MissionSucceeded = false;
    MissionFailed = false;
    BanditDemandsKnown = false;
    ChiefRefusesDemands = false;
    BanditLeaderFree = false;
    BanditSiegeStart = false;
    BanditsAttackHero = false;
    HasFirstWaveAttacked = false;
    StartSecondWave = false;
    HeroLetInTown = false;
    GuardFollowingHero = false;
    EndCutsceneStarts = false;
    EndCutsceneDone = false;
    AttackedBanditLeader = false;
    QuestStartScreened = false;
    ReasonForFailure = 0;
    Data->BreakSiegeNoAssistance = true;
    Data->BreakSiegeKilledLeader = false;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->BreakSiegeNakedBoastCost,
                   pGlobalScriptDef->BreakSiegeNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->BreakSiegeNoDamageBoastCost,
                   pGlobalScriptDef->BreakSiegeNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->BreakSiegeNoWeaponsBoastCost,
                   pGlobalScriptDef->BreakSiegeNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_HIRENOGUARDS", 25, pGlobalScriptDef->BreakSiegeHireNoGuardsBoastCost,
                   pGlobalScriptDef->BreakSiegeHireNoGuardsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLBANDITLEADER", 26, pGlobalScriptDef->BreakSiegeKillBanditLeaderBoastCost,
                   pGlobalScriptDef->BreakSiegeKillBanditLeaderBoastReward, false, "", 0);
}