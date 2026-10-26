// CQ_ArenaScript::Init() -- register the four Arena quest regions, offer the four Arena boasts and reset the arena
// Leaf: ?Init@CQ_ArenaScript@NScript@@
// state (rounds, gold, creature counts, crowd, the sixteen spawn-needed flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFA700, length 845 bytes (padded to 848).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; through the script's
// interface (+0x40): AddQuestRegion (slot 271, +0x43c) x4 and AddBoast (slot 303, +0x4bc) x4;
// CScriptBase::IsActiveThreadTerminating 0x00CB7940.
// Data: the script def 0x0143E90C: ArenaNaked/NoDamage boasts +0x70..+0x7c (Ego_r.pdb -4); the kill/spare-opponents
// boast values +0xadc..+0xae8 are TLC additions that Ego_r.pdb does not have (names descriptive).
// Layout: Ego_r.pdb - 0x14 up to Rounds (+0x98); retail's STLport vector is 12 bytes (PDB 16), so every member after it
// sits a further 4 bytes lower (ArenaState +0xa4 .. WhisperNeededForCutscene +0x115).
// Shape: literals are implicit conversion temporaries; the clears share ebx with the boasts' false/0 arguments; the
// spawn-flag loop checks for thread termination before every store (and once more after the loop).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CGameScriptInterface { public:                      // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
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

class CScriptDef { public:
    unsigned char _pad_0x000[0x70];
    long ArenaNakedBoastCost;                          // +0x70
    long ArenaNakedBoastReward;                        // +0x74
    long ArenaNoDamageBoastCost;                       // +0x78
    long ArenaNoDamageBoastReward;                     // +0x7c
    unsigned char _pad_0x080[0xadc - 0x80];
    long ArenaKillOpponentsBoastCost;                  // +0xadc
    long ArenaSpareOpponentsBoastCost;                 // +0xae0
    long ArenaKillOpponentsBoastReward;                // +0xae4
    long ArenaSpareOpponentsBoastReward;               // +0xae8
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

class CScriptBase { public:                                   // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
};

namespace NScript {
class CQ_ArenaScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                        // +0x40
    void*                 Data;                        // +0x44
    unsigned char CrowdLoopTags[0x50];                 // +0x48 CCharString[4][5]
    long Rounds[3];                                   // +0x98 vector<CArenaRoundDef>
    long ArenaState;                                   // +0xa4
    long ArenaRound;                                   // +0xa8
    long ArenaRoundWave;                               // +0xac
    long GoldMultiplier;                               // +0xb0
    bool ChamLeaving;                                  // +0xb4
    long GoldTotal;                                    // +0xb8
    bool InHitCutsceneAlready;                         // +0xbc
    bool NeedBertForSpeech;                            // +0xbd
    long CellsVillage[3];                             // +0xc0 CScriptThing
    bool PlayerLeaving;                                // +0xcc
    bool PlayerWon;                                    // +0xcd
    long TotalCreatures[3];                            // +0xd0
    long ExtraCreatures;                               // +0xdc
    bool MissionSucceeded;                             // +0xe0
    bool MissionFailed;                                // +0xe1
    bool ArenaSpawnNeeded[16];                         // +0xe2
    long NewCrowdPoints;                               // +0xf4
    long NewCrowdHappiness;                            // +0xf8
    long NewCrowdBaseLevel;                            // +0xfc
    long GlobalCrowdTimer;                             // +0x100 CTimer
    bool FinalBattleCS;                                // +0x104
    long Whisper[3];                                  // +0x108 CScriptThing
    bool PauseCrowdChecker;                            // +0x114
    bool WhisperNeededForCutscene;                     // +0x115
    long StopTimeIndex;                                // +0x118
    void Init();
};
} // namespace NScript
using NScript::CQ_ArenaScript;

void NScript::CQ_ArenaScript::Init()
{
    Game->AddQuestRegion("Q_Arena", "ArenaExterior");
    Game->AddQuestRegion("Q_Arena", "ArenaHallOfHeroes");
    Game->AddQuestRegion("Q_Arena", "ArenaCells");
    Game->AddQuestRegion("Q_Arena", "Arena");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->ArenaNakedBoastCost,
                   pGlobalScriptDef->ArenaNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->ArenaNoDamageBoastCost,
                   pGlobalScriptDef->ArenaNoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLARENAOPPONENTS", 30, pGlobalScriptDef->ArenaKillOpponentsBoastCost,
                   pGlobalScriptDef->ArenaKillOpponentsBoastReward, true, "", 1);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_SPAREARENAOPPONENTS", 31, pGlobalScriptDef->ArenaSpareOpponentsBoastCost,
                   pGlobalScriptDef->ArenaSpareOpponentsBoastReward, false, "", 1);
    ChamLeaving = false;
    ArenaRound = 0;
    ArenaRoundWave = 0;
    NeedBertForSpeech = false;
    PlayerLeaving = false;
    PlayerWon = false;
    TotalCreatures[0] = 0;
    TotalCreatures[1] = 0;
    TotalCreatures[2] = 0;
    ExtraCreatures = 0;
    MissionSucceeded = false;
    MissionFailed = false;
    GoldMultiplier = 0;
    GoldTotal = 0;
    InHitCutsceneAlready = false;
    NewCrowdHappiness = 0;
    NewCrowdBaseLevel = 0;
    NewCrowdPoints = 200;
    PauseCrowdChecker = true;
    for (long i = 0; i < 16; ++i) {
        if (IsActiveThreadTerminating()) return;
        ArenaSpawnNeeded[i] = false;
    }
    if (IsActiveThreadTerminating()) return;
    FinalBattleCS = false;
    ArenaState = 0;
    WhisperNeededForCutscene = true;
}