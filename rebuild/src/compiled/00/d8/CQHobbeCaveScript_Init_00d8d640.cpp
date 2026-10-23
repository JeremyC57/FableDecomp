// CQ_HobbeCaveScript::Init() -- register the quest's eleven regions, reset the mission state (the timer from the script def), flag the master data and register the six boasts
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8D640, 1472 bytes (int3 padded).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; CGameScriptInterface AddQuestRegion (slot 271,
//   +0x43c), AddBoast (slot 303, +0x4bc); script def g_pScriptDef 0x0143E90C (boast cost/reward pairs +0xd8.. and +0xcf0..,
//   HobbeCave timer +0xccc).
// Layout: Game +0x40, PMasterData +0x44 (+0x10d / +0x110 / +0x114), bools +0x48..+0x50, longs +0x58/+0x5c, bool +0x60,
//   timer +0x64, bools +0x68..+0x6c, longs +0x70/+0x74 (descriptive names: the PDB does not cover the quest scripts).
// Shape: landed CQ_HangingTreeGoodScript::Init 0x00D6F020 (literals straight to the `const CCharString&` parameters).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CGameScriptInterface {
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
    virtual void AddQuestRegion(const CCharString& quest, const CCharString& region) const;   // slot 271 (+0x43c)
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281();
    virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291();
    virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301();
    virtual void v302();
    virtual void AddBoast(const CCharString& description, long boastID, long cost, long reward, bool hasAmount, const CCharString& extra, long extraValue) const;   // slot 303 (+0x4bc)
};
struct CScriptDef {
    unsigned char _pad0[0xd8];
    long NakedBoastCost;                                // +0xd8
    long NakedBoastReward;                              // +0xdc
    long NoDamageBoastCost;                             // +0xe0
    long NoDamageBoastReward;                           // +0xe4
    long NoWeaponsBoastCost;                            // +0xe8
    long NoWeaponsBoastReward;                          // +0xec
    unsigned char _pad0f0[0xccc - 0xf0];
    long HobbeCaveTimer;                                // +0xccc
    unsigned char _pad0cd0[0xcf0 - 0xcd0];
    long KillNymphBoastCost;                            // +0xcf0
    long KillNymphBoastReward;                          // +0xcf4
    long SacrificeBoastCost;                            // +0xcf8
    long SacrificeBoastReward;                          // +0xcfc
    long BoyNoHarmBoastCost;                            // +0xd00
    long BoyNoHarmBoastReward;                          // +0xd04
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CHobbeCaveMasterData {
    unsigned char _pad[0x10d];
    bool HobbeCaveDone;                                 // +0x10d
    unsigned char _pad10e[2];
    long HobbeCaveState;                                // +0x110
    bool HobbeCaveActive;                               // +0x114
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
struct CQ_HobbeCaveScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    CHobbeCaveMasterData* PMasterData;                  // +0x44
    bool Flag48, Flag49, Flag4A, Flag4B, Flag4C, Flag4D, Flag4E, Flag4F, Flag50;   // +0x48..+0x50
    unsigned char _pad51[7];
    long Count58;                                       // +0x58
    long Count5C;                                       // +0x5c
    bool Flag60;                                        // +0x60
    unsigned char _pad61[3];
    long Timer;                                         // +0x64
    bool Flag68, Flag69, Flag6A, Flag6B, Flag6C;        // +0x68..+0x6c
    unsigned char _pad6d[3];
    long Count70;                                       // +0x70
    long Count74;                                       // +0x74
    void Init();
};

void CQ_HobbeCaveScript::Init()
{
    Game->AddQuestRegion("Q_HobbeCave", "GrannysHouse");
    Game->AddQuestRegion("Q_HobbeCave", "Greatwood");
    Game->AddQuestRegion("Q_HobbeCave", "GreatwoodCaves");
    Game->AddQuestRegion("Q_HobbeCave", "DemonDoor_GreatwoodCaves");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveEntranceTunnel");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveSideRoom");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveLarder");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveNymphChamber");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveMainChamber");
    Game->AddQuestRegion("Q_HobbeCave", "HobbeCaveNymphFocal");
    Game->AddQuestRegion("Q_HobbeCave", "DemonDoor_GreatwoodGrannysHouse");
    Flag48 = false;
    Flag49 = false;
    Flag4A = false;
    Flag4B = false;
    Flag4C = false;
    Flag4D = false;
    Flag4E = false;
    Flag4F = false;
    Flag50 = false;
    Count58 = 0;
    Count5C = 0;
    Flag60 = false;
    Timer = g_pScriptDef->HobbeCaveTimer;
    Flag68 = false;
    Flag69 = false;
    Flag6A = false;
    Flag6B = false;
    Flag6C = false;
    Count70 = 0;
    Count74 = 0;
    PMasterData->HobbeCaveDone = false;
    PMasterData->HobbeCaveState = 1;
    PMasterData->HobbeCaveActive = true;
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_pScriptDef->NakedBoastCost, g_pScriptDef->NakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_pScriptDef->NoDamageBoastCost, g_pScriptDef->NoDamageBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_pScriptDef->NoWeaponsBoastCost, g_pScriptDef->NoWeaponsBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLNYMPH", 0x15, g_pScriptDef->KillNymphBoastCost, g_pScriptDef->KillNymphBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_SACRIFICE", 0x16, g_pScriptDef->SacrificeBoastCost, g_pScriptDef->SacrificeBoastReward, true, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_BOYNOHARM", 0x17, g_pScriptDef->BoyNoHarmBoastCost, g_pScriptDef->BoyNoHarmBoastReward, false, "", 0);
}