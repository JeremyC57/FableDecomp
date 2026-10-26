// CQ_OrchardFarmRaidScript::Init() -- register the farm as a quest region of whichever side's quest is active, reset the master-data flags, the crate count, timers and mission state, pair the two crate teams as enemies with no carriers, set the fail reasons, then pick the hero's team, text prefix, objective and the minimap routes for the evil or the good quest
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCC140, 1298 bytes (int3 padded to 1312).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0, CWideString::operator=(const wchar_t*)
//   0x0099B800, CBaseClass 0x0099A2D0 / ~ 0x0099A2E0, operator delete 0x00BFE9BC; g_pGameScriptInterface 0x0143E8F8
//   SetTimer (slot 89); Game slots 271 AddQuestRegion, 284 IsQuestActive, 296 SetQuestCardObjective, 354
//   MiniMapAllowRouteBetweenRegions.
// Layout (retail = Ego_r.pdb - 0x14, and -4 more after the 12-byte STLport CrateList): CrateCount +0x48, CommentTimer +0x4c,
//   DoneIntroduction +0x50 .. WhisperSpawned +0x52, MissionSucceeded +0x60, MissionFailed +0x64, FailReasons +0x68, HeroTeam
//   +0x78, BanditWavesSpawned +0x7c, TextSystemScriptCode +0x84, HeroMetWhisperBeforeFarm +0x88 .. WhisperInCutscene +0x8b,
//   Teams[2] +0x8c (0x40 each: TeamCrateCarrier +0x30, EnemyTeam +0x3c). CQ_SunnyvaleMasterData (no shift): OFBRCratesStolen
//   +0x109, OFBR_NoCratesWereStolen +0x10a, OrchardFarmBanditKilled +0x118, OrchardFarmGuardKilled +0x119.
// Shape: `X = CScriptThing();` is the landed CV_BordelloScript (0x00E399D0) temporary + inline counted operator= model.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CWideString {
    void* PStringData;
    CWideString& operator=(const unsigned short* text);        // 0x0099B800
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
struct CBase {
    virtual ~CBase();                                   // 0x0099A2E0
    CBase();                                            // 0x0099A2D0
};
struct SScriptThingInfo {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CScriptThing : CBase {                           // vtable 0x01238C8C, 12 bytes
    void* PImp_Data;
    SScriptThingInfo* PImp_Info;
    CScriptThing() : PImp_Data(0), PImp_Info(0) { }
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
    CScriptThing& operator=(const CScriptThing& other)
    {
        SScriptThingInfo* info = other.PImp_Info;
        void* data = other.PImp_Data;
        if (PImp_Info != info) {
            if (PImp_Info) {
                --PImp_Info->m_references;
                if (PImp_Info->m_references == 0) {
                    PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                    delete PImp_Info;
                }
            }
            PImp_Data = data;
            PImp_Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
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
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164)
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
    virtual void v282(); virtual void v283();
    virtual bool IsQuestActive(const CCharString& quest) const;   // slot 284 (+0x470)
    virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295();
    virtual void SetQuestCardObjective(const CCharString& quest, const CCharString& text, const CCharString& region1, const CCharString& region2) const;   // slot 296 (+0x4a0)
    virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306();
    virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316();
    virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326();
    virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336();
    virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346();
    virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353();
    virtual void MiniMapAllowRouteBetweenRegions(const CCharString& a, const CCharString& b, bool allow) const;   // slot 354 (+0x588)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {
    long Index;
    void Set(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x109];
    bool OFBRCratesStolen;                              // +0x109
    bool OFBR_NoCratesWereStolen;                       // +0x10a
    unsigned char _pad10b[0x118 - 0x10b];
    bool OrchardFarmBanditKilled;                       // +0x118
    bool OrchardFarmGuardKilled;                        // +0x119
};
struct CCrateTeamManager {                              // 0x40 bytes
    long StateCounter[6];                               // +0x00
    long MemberCount;                                   // +0x18
    CTimer TeamReinforcementsTimer;                     // +0x1c
    long MaxTeamMembers;                                // +0x20
    unsigned char CrateDropPos[0x0c];                   // +0x24 (CScriptThing)
    CScriptThing TeamCrateCarrier;                      // +0x30
    CCrateTeamManager* EnemyTeam;                       // +0x3c
};
struct CScriptBase {
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_OrchardFarmRaidScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    long                    CrateCount;                 // +0x48
    CTimer                  CommentTimer;               // +0x4c
    bool                    DoneIntroduction;           // +0x50
    bool                    HeroAtWrongEntrance;        // +0x51
    bool                    WhisperSpawned;             // +0x52
    void*                   CrateList[3];               // +0x54 (12-byte STLport vector<CScriptThing>)
    bool                    MissionSucceeded;           // +0x60
    long                    MissionFailed;              // +0x64
    CWideString             FailReasons[4];             // +0x68
    long                    HeroTeam;                   // +0x78
    long                    BanditWavesSpawned;         // +0x7c
    CTimer                  RemindHeroOfObjectivesTimer;// +0x80
    CCharString             TextSystemScriptCode;       // +0x84
    bool                    HeroMetWhisperBeforeFarm;   // +0x88
    bool                    ShownCombatMultiplierTutorial;  // +0x89
    bool                    ShownCombatFlourishTutorial;    // +0x8a
    bool                    WhisperInCutscene;          // +0x8b
    CCrateTeamManager       Teams[2];                   // +0x8c
    void Init();
};
} // namespace NScript
using NScript::CQ_OrchardFarmRaidScript;

void NScript::CQ_OrchardFarmRaidScript::Init()
{
    if (Game->IsQuestActive("Q_OrchardFarmRaidGood"))
        Game->AddQuestRegion("Q_OrchardFarmRaidGood", "OrchardFarm");
    else
        Game->AddQuestRegion("Q_OrchardFarmRaidEvil", "OrchardFarm");
    PMasterData->OrchardFarmGuardKilled = false;
    PMasterData->OrchardFarmBanditKilled = false;
    HeroMetWhisperBeforeFarm = false;
    CrateCount = 0;
    PMasterData->OFBRCratesStolen = false;
    PMasterData->OFBR_NoCratesWereStolen = false;
    CommentTimer.Set(0);
    DoneIntroduction = false;
    HeroAtWrongEntrance = false;
    WhisperSpawned = false;
    MissionSucceeded = false;
    MissionFailed = 0;
    BanditWavesSpawned = 0;
    ShownCombatMultiplierTutorial = false;
    ShownCombatFlourishTutorial = false;
    WhisperInCutscene = false;
    Teams[0].EnemyTeam = &Teams[1];
    Teams[1].EnemyTeam = &Teams[0];
    Teams[0].TeamCrateCarrier = CScriptThing();
    Teams[1].TeamCrateCarrier = CScriptThing();
    FailReasons[0] = L"PROBLEM: Tell Ben problem with Orchard Farm fail reasons";
    FailReasons[1] = L"TEXT_QST_051_FAILED_HERO_KILLED";
    FailReasons[2] = L"TEXT_QST_051_FAILED_CRATES_STOLEN";
    FailReasons[3] = L"TEXT_QST_051_FAILED_TEAM_KILLED";
    if (Game->IsQuestActive("Q_OrchardFarmRaidEvil")) {
        HeroTeam = 1;
        TextSystemScriptCode = "TEXT_QST_051_";
        Game->SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01", "", "HeroGuildComplexInside");
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", false);
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", true);
    } else if (Game->IsQuestActive("Q_OrchardFarmRaidGood")) {
        HeroTeam = 0;
        TextSystemScriptCode = "TEXT_QST_052_";
        Game->SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_01", "", "Greatwood");
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", false);
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", true);
    }
}