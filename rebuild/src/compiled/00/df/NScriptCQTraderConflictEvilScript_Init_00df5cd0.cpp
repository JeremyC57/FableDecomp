// CQ_TraderConflictEvilScript::Init() -- reset the evil trader-conflict quest state, start the scream-out timer, register the BarrowFields quest region and the five boasts, then arm the bandit-follower boast flags in the master data
// Leaf: ?Init@CQ_TraderConflictEvilScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF5CD0, 625 bytes (640 with int3 padding).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the global game interface 0x0143E8F8
//   SetTimer (slot 89, +0x164) through the inline CTimer; through the script's interface (+0x40): AddQuestRegion
//   (slot 271, +0x43c) and AddBoast (slot 303, +0x4bc) -- slot names of the landed CQ_WhiteBalverineKnotholeGladeScript::Init.
// Data: the script def at 0x0143E90C, 4 bytes below Ego_r.pdb's CScriptDef offsets (TraderConflictEvilNakedBoastCost
//   PDB +0x1CC -> retail +0x1C8, TCEKeepBanditFollowerAliveCost PDB +0x24C -> +0x248). The master data (+0x44) is
//   CQ_SunnyvaleMasterData at the PDB offsets (TCEKeepBanditFollowerAlive +0x128, TCEMadeTimeLimit +0x129).
// Layout (retail = Ego_r PDB -0x18, after the 12-byte retail AllCreatures vector at +0x48): CounterID +0x54,
//   NumberSpawned +0x58, InitialNumberInRegion +0x5C, QuestStartScreened +0x60, NextTimeToSpawnGuards +0x64,
//   MissionSucceeded +0x68, HeroAttackedBandit +0x69, PlayerEngaged +0x6A, ScreamOutTimer +0x6C.
// Shape: the zero is promoted into ebx and reused for the boasts' `false` / 0 pushes; PlayerEngaged is cleared after
//   the timer call (it is scheduled into the first CCharString build); every literal is passed straight to a
//   `const CCharString&` parameter, so the temporaries are built right to left before the pushes. The timer index is
//   read through `const CTimer* const& self = this;` (the 00703590 view): a plain member read is hoisted above the
//   member stores, retail reads it after the interface global.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
    virtual void v088();
    virtual void SetTimer(long timer, long seconds);   // slot 89 (+0x164)
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145();
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153();
    virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161();
    virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177();
    virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185();
    virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193();
    virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201();
    virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217();
    virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225();
    virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233();
    virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241();
    virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257();
    virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265();
    virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270();
    virtual void AddQuestRegion(const CCharString& quest, const CCharString& region);   // slot 271 (+0x43c)
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287();
    virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295();
    virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302();
    virtual void AddBoast(const CCharString& description, long boastID, long cost, long reward, bool flag, const CCharString& extra, long extraValue);   // slot 303 (+0x4bc)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {                                         // a script timer: the interface's timer index
    long Index;
    void SetTime(long seconds) { const CTimer* const& self = this; g_pGameScriptInterface->SetTimer(self->Index, seconds); }
};
struct CScriptDef {
    unsigned char _pad0[0x1c8];
    long TraderConflictEvilNakedBoastCost;              // +0x1C8
    long TraderConflictEvilNakedBoastReward;            // +0x1CC
    long TraderConflictEvilNoDamageBoastCost;           // +0x1D0
    long TraderConflictEvilNoDamageBoastReward;         // +0x1D4
    long TraderConflictEvilNoWeaponsBoastCost;          // +0x1D8
    long TraderConflictEvilNoWeaponsBoastReward;        // +0x1DC
    unsigned char _pad1e0[0x68];
    long TCEKeepBanditFollowerAliveCost;                // +0x248
    long TCEKeepBanditFollowerAliveReward;              // +0x24C
    long TCEMadeTimeLimitCost;                          // +0x250
    long TCEMadeTimeLimitReward;                        // +0x254
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x128];
    bool TCEKeepBanditFollowerAlive;                    // +0x128
    bool TCEMadeTimeLimit;                              // +0x129
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_TraderConflictEvilScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* Data;                       // +0x44
    unsigned char           AllCreatures[0xc];          // +0x48 vector<CScriptThing>
    long                    CounterID;                  // +0x54
    long                    NumberSpawned;              // +0x58
    long                    InitialNumberInRegion;      // +0x5C
    bool                    QuestStartScreened;         // +0x60
    long                    NextTimeToSpawnGuards;      // +0x64
    bool                    MissionSucceeded;           // +0x68
    bool                    HeroAttackedBandit;         // +0x69
    bool                    PlayerEngaged;              // +0x6A
    CTimer                  ScreamOutTimer;             // +0x6C
    void Init();
};
} // namespace NScript
using NScript::CQ_TraderConflictEvilScript;

void NScript::CQ_TraderConflictEvilScript::Init()
{
    CounterID = 0;
    NumberSpawned = 0;
    InitialNumberInRegion = 0;
    QuestStartScreened = false;
    NextTimeToSpawnGuards = 24;
    MissionSucceeded = false;
    HeroAttackedBandit = false;
    ScreamOutTimer.SetTime(15);
    PlayerEngaged = false;
    Game->AddQuestRegion("Q_TraderConflictEvil", "BarrowFields");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->TraderConflictEvilNakedBoastCost,
                   pGlobalScriptDef->TraderConflictEvilNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->TraderConflictEvilNoDamageBoastCost,
                   pGlobalScriptDef->TraderConflictEvilNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, pGlobalScriptDef->TraderConflictEvilNoWeaponsBoastCost,
                   pGlobalScriptDef->TraderConflictEvilNoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCEBANDITALIVE", 0x24, pGlobalScriptDef->TCEKeepBanditFollowerAliveCost,
                   pGlobalScriptDef->TCEKeepBanditFollowerAliveReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_TCETIMELIMIT", 0x25, pGlobalScriptDef->TCEMadeTimeLimitCost,
                   pGlobalScriptDef->TCEMadeTimeLimitReward, false, "", 0);
    Data->TCEKeepBanditFollowerAlive = true;
    Data->TCEMadeTimeLimit = false;
}