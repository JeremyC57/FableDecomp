// Behaviour test for 00f0f910 (CQ_AmbushTradersScript::WatchForArcherSpawnConditions): final.cpp body verbatim, every callee
// stubbed. The archers are created dead and faceMe alive (and vice versa), so the test sees which thing the IsAlive check reads.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
extern "C" int __cdecl strcmp(const char*, const char*);
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0F910, length 599 bytes. Spawned as "WatchForArcherSpawnConditions"
//   (0x012F8450) by CQ_AmbushTradersScript::ProcessInitialSpyExtraction 0x00F0F680; the manifest label
//   "CQ_GuildTrainingScript::WatchForSparrowKilled" is propagated.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line),
//   CScriptThing::IsAlive 0x004AB130 (slot 75; called directly on the local), ~vector<CScriptThing> 0x008AC970 (out of
//   line), CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40): NewScriptFrame (7),
//   IsRegionLoaded (12), GetThingWithScriptName (72), GetAllThingsWithScriptName (75), CreateCreatureOnEntity (93),
//   EntityAttachToScript (465), EntitySetFacingAngleTowardsThing (475), GetActiveQuestName (655).
// Layout: SpyContactMade +0x5f (retail = Ego_r.pdb CQ_AmbushTradersScript - 0x14).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                       // slot 0; complete destructor 0x004AA840
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031();
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055();
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063();
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071();
    virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                  // slot 75 (+0x12c) 0x004AB130
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                     // 0x008AC970 for CScriptThing
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class CGameScriptInterface { public:                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30) IsRegionLoaded
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
    virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const;   // slot 75 (+0x12c) GetAllThingsWithScriptName
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void v092();
    virtual CScriptThing CreateCreatureOnEntity(const CCharString& def, const CScriptThing& entity, const CCharString& scriptName) const;   // slot 93 (+0x174) CreateCreatureOnEntity
    virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157();
    virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165();
    virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173();
    virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181();
    virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189();
    virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197();
    virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205();
    virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213();
    virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221();
    virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229();
    virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237();
    virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245();
    virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253();
    virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261();
    virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269();
    virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277();
    virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285();
    virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293();
    virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301();
    virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309();
    virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317();
    virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325();
    virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333();
    virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341();
    virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349();
    virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357();
    virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365();
    virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373();
    virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381();
    virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389();
    virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397();
    virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405();
    virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413();
    virtual void v414(); virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421();
    virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429();
    virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437();
    virtual void v438(); virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445();
    virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453();
    virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461();
    virtual void v462(); virtual void v463(); virtual void v464();
    virtual void EntityAttachToScript(const CScriptThing& thing, const CCharString& script) const;   // slot 465 (+0x744) EntityAttachToScript
    virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470(); virtual void v471(); virtual void v472(); virtual void v473();
    virtual void v474();
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing& thing, const CScriptThing& target, bool a) const;   // slot 475 (+0x76c) EntitySetFacingAngleTowardsThing
    virtual void v476(); virtual void v477(); virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483();
    virtual void v484(); virtual void v485(); virtual void v486(); virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491();
    virtual void v492(); virtual void v493(); virtual void v494(); virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499();
    virtual void v500(); virtual void v501(); virtual void v502(); virtual void v503(); virtual void v504(); virtual void v505(); virtual void v506(); virtual void v507();
    virtual void v508(); virtual void v509(); virtual void v510(); virtual void v511(); virtual void v512(); virtual void v513(); virtual void v514(); virtual void v515();
    virtual void v516(); virtual void v517(); virtual void v518(); virtual void v519(); virtual void v520(); virtual void v521(); virtual void v522(); virtual void v523();
    virtual void v524(); virtual void v525(); virtual void v526(); virtual void v527(); virtual void v528(); virtual void v529(); virtual void v530(); virtual void v531();
    virtual void v532(); virtual void v533(); virtual void v534(); virtual void v535(); virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539();
    virtual void v540(); virtual void v541(); virtual void v542(); virtual void v543(); virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547();
    virtual void v548(); virtual void v549(); virtual void v550(); virtual void v551(); virtual void v552(); virtual void v553(); virtual void v554(); virtual void v555();
    virtual void v556(); virtual void v557(); virtual void v558(); virtual void v559(); virtual void v560(); virtual void v561(); virtual void v562(); virtual void v563();
    virtual void v564(); virtual void v565(); virtual void v566(); virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571();
    virtual void v572(); virtual void v573(); virtual void v574(); virtual void v575(); virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579();
    virtual void v580(); virtual void v581(); virtual void v582(); virtual void v583(); virtual void v584(); virtual void v585(); virtual void v586(); virtual void v587();
    virtual void v588(); virtual void v589(); virtual void v590(); virtual void v591(); virtual void v592(); virtual void v593(); virtual void v594(); virtual void v595();
    virtual void v596(); virtual void v597(); virtual void v598(); virtual void v599(); virtual void v600(); virtual void v601(); virtual void v602(); virtual void v603();
    virtual void v604(); virtual void v605(); virtual void v606(); virtual void v607(); virtual void v608(); virtual void v609(); virtual void v610(); virtual void v611();
    virtual void v612(); virtual void v613(); virtual void v614(); virtual void v615(); virtual void v616(); virtual void v617(); virtual void v618(); virtual void v619();
    virtual void v620(); virtual void v621(); virtual void v622(); virtual void v623(); virtual void v624(); virtual void v625(); virtual void v626(); virtual void v627();
    virtual void v628(); virtual void v629(); virtual void v630(); virtual void v631(); virtual void v632(); virtual void v633(); virtual void v634(); virtual void v635();
    virtual void v636(); virtual void v637(); virtual void v638(); virtual void v639(); virtual void v640(); virtual void v641(); virtual void v642(); virtual void v643();
    virtual void v644(); virtual void v645(); virtual void v646(); virtual void v647(); virtual void v648(); virtual void v649(); virtual void v650(); virtual void v651();
    virtual void v652(); virtual void v653(); virtual void v654();
    virtual CCharString GetActiveQuestName() const;   // slot 655 (+0xa3c) GetActiveQuestName
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
class CQ_AmbushTradersScript : public CScriptBase { public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x5f - 0x48];
    bool                  SpyContactMade;               // +0x5f
    void WatchForArcherSpawnConditions();
};
void CQ_AmbushTradersScript::WatchForArcherSpawnConditions()
{
    while (!SpyContactMade) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("DesertedFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    std::vector<CScriptThing> spawnPoints;
    long count = Game->GetAllThingsWithScriptName("M_AT_ArcherSpawn", spawnPoints);
    CScriptThing faceMe = Game->GetThingWithScriptName("M_AT_ArcherSpawnFaceMe");
    for (long i = 0; i < count; ++i) {
        if (spawnPoints[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing archer = Game->CreateCreatureOnEntity("CREATURE_BS_GUARD_CROSSBOW", spawnPoints[i], "AT_Phase3Archer");
            Game->EntityAttachToScript(archer, Game->GetActiveQuestName());
            if (faceMe.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(archer, faceMe, false);
            }
        }
    }
}
}
void CScriptThing::t001() {}
void CScriptThing::t002() {}
void CScriptThing::t003() {}
void CScriptThing::t004() {}
void CScriptThing::t005() {}
void CScriptThing::t006() {}
void CScriptThing::t007() {}
void CScriptThing::t008() {}
void CScriptThing::t009() {}
void CScriptThing::t010() {}
void CScriptThing::t011() {}
void CScriptThing::t012() {}
void CScriptThing::t013() {}
void CScriptThing::t014() {}
void CScriptThing::t015() {}
void CScriptThing::t016() {}
void CScriptThing::t017() {}
void CScriptThing::t018() {}
void CScriptThing::t019() {}
void CScriptThing::t020() {}
void CScriptThing::t021() {}
void CScriptThing::t022() {}
void CScriptThing::t023() {}
void CScriptThing::t024() {}
void CScriptThing::t025() {}
void CScriptThing::t026() {}
void CScriptThing::t027() {}
void CScriptThing::t028() {}
void CScriptThing::t029() {}
void CScriptThing::t030() {}
void CScriptThing::t031() {}
void CScriptThing::t032() {}
void CScriptThing::t033() {}
void CScriptThing::t034() {}
void CScriptThing::t035() {}
void CScriptThing::t036() {}
void CScriptThing::t037() {}
void CScriptThing::t038() {}
void CScriptThing::t039() {}
void CScriptThing::t040() {}
void CScriptThing::t041() {}
void CScriptThing::t042() {}
void CScriptThing::t043() {}
void CScriptThing::t044() {}
void CScriptThing::t045() {}
void CScriptThing::t046() {}
void CScriptThing::t047() {}
void CScriptThing::t048() {}
void CScriptThing::t049() {}
void CScriptThing::t050() {}
void CScriptThing::t051() {}
void CScriptThing::t052() {}
void CScriptThing::t053() {}
void CScriptThing::t054() {}
void CScriptThing::t055() {}
void CScriptThing::t056() {}
void CScriptThing::t057() {}
void CScriptThing::t058() {}
void CScriptThing::t059() {}
void CScriptThing::t060() {}
void CScriptThing::t061() {}
void CScriptThing::t062() {}
void CScriptThing::t063() {}
void CScriptThing::t064() {}
void CScriptThing::t065() {}
void CScriptThing::t066() {}
void CScriptThing::t067() {}
void CScriptThing::t068() {}
void CScriptThing::t069() {}
void CScriptThing::t070() {}
void CScriptThing::t071() {}
void CScriptThing::t072() {}
void CScriptThing::t073() {}
void CScriptThing::t074() {}
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
void CGameScriptInterface::v073() {}
void CGameScriptInterface::v074() {}
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
void CGameScriptInterface::v476() {}
void CGameScriptInterface::v477() {}
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
void CGameScriptInterface::v496() {}
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
void CGameScriptInterface::v512() {}
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
void CGameScriptInterface::v617() {}
void CGameScriptInterface::v618() {}
void CGameScriptInterface::v619() {}
void CGameScriptInterface::v620() {}
void CGameScriptInterface::v621() {}
void CGameScriptInterface::v622() {}
void CGameScriptInterface::v623() {}
void CGameScriptInterface::v624() {}
void CGameScriptInterface::v625() {}
void CGameScriptInterface::v626() {}
void CGameScriptInterface::v627() {}
void CGameScriptInterface::v628() {}
void CGameScriptInterface::v629() {}
void CGameScriptInterface::v630() {}
void CGameScriptInterface::v631() {}
void CGameScriptInterface::v632() {}
void CGameScriptInterface::v633() {}
void CGameScriptInterface::v634() {}
void CGameScriptInterface::v635() {}
void CGameScriptInterface::v636() {}
void CGameScriptInterface::v637() {}
void CGameScriptInterface::v638() {}
void CGameScriptInterface::v639() {}
void CGameScriptInterface::v640() {}
void CGameScriptInterface::v641() {}
void CGameScriptInterface::v642() {}
void CGameScriptInterface::v643() {}
void CGameScriptInterface::v644() {}
void CGameScriptInterface::v645() {}
void CGameScriptInterface::v646() {}
void CGameScriptInterface::v647() {}
void CGameScriptInterface::v648() {}
void CGameScriptInterface::v649() {}
void CGameScriptInterface::v650() {}
void CGameScriptInterface::v651() {}
void CGameScriptInterface::v652() {}
void CGameScriptInterface::v653() {}
void CGameScriptInterface::v654() {}

using namespace NScript;
static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
static bool Eq(const CCharString& s, const char* t) { return strcmp((const char*)s.PStringData, t) == 0; }
CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
CScriptThing::~CScriptThing() {}
bool CScriptThing::IsAlive() const { return PImp_Info != 0; }
static CScriptThing MakeThing(long id, bool alive) { CScriptThing t; t.PImp_Data = (CScriptThing*)id; t.PImp_Info = (CCPPointerInfo*)(alive ? 1 : 0); return t; }
namespace std { template<class T, class A> vector<T, A>::~vector() { delete[] _M_start; } }
static int g_frames = 0, g_creates = 0, g_attaches = 0, g_faces = 0, g_terminating = 0;
static bool g_archerAlive = false, g_faceMeAlive = true;
static long g_lastEntity = 0, g_faceThing = 0, g_faceTarget = 0;
static CQ_AmbushTradersScript* g_script = 0;
bool CScriptBase::IsActiveThreadTerminating() { ++g_terminating; return false; }
void CGameScriptInterface::NewScriptFrame() const { if (++g_frames == 2) g_script->SpyContactMade = true; }
bool CGameScriptInterface::IsRegionLoaded(const CCharString& region) const { return Eq(region, "DesertedFarm") && g_frames >= 3; }
CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& name) const
{ Check(Eq(name, "M_AT_ArcherSpawnFaceMe"), "faceMe name"); return MakeThing(99, g_faceMeAlive); }
long CGameScriptInterface::GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const
{
    Check(Eq(name, "M_AT_ArcherSpawn"), "spawn name");
    things._M_start = new CScriptThing[3];
    things._M_start[0] = MakeThing(1, true); things._M_start[1] = MakeThing(2, false); things._M_start[2] = MakeThing(3, true);
    things._M_finish = things._M_end_of_storage = things._M_start + 3;
    return 3;
}
CScriptThing CGameScriptInterface::CreateCreatureOnEntity(const CCharString& def, const CScriptThing& entity, const CCharString& scriptName) const
{
    Check(Eq(def, "CREATURE_BS_GUARD_CROSSBOW") && Eq(scriptName, "AT_Phase3Archer"), "create args");
    ++g_creates; g_lastEntity = (long)entity.PImp_Data;
    return MakeThing(100 + g_lastEntity, g_archerAlive);
}
void CGameScriptInterface::EntityAttachToScript(const CScriptThing& thing, const CCharString& script) const
{ Check((long)thing.PImp_Data == 100 + g_lastEntity && Eq(script, "QUEST"), "attach"); ++g_attaches; }
void CGameScriptInterface::EntitySetFacingAngleTowardsThing(const CScriptThing& thing, const CScriptThing& target, bool a) const
{ ++g_faces; g_faceThing = (long)thing.PImp_Data; g_faceTarget = (long)target.PImp_Data; Check(!a, "facing flag"); }
CCharString CGameScriptInterface::GetActiveQuestName() const { return CCharString("QUEST"); }
int main()
{
    static CQ_AmbushTradersScript script; static CGameScriptInterface game;
    script.Game = &game; script.SpyContactMade = false; g_script = &script;
    // archers dead, faceMe alive -> both live spawn points get an archer, both are turned
    script.WatchForArcherSpawnConditions();
    Check(g_frames == 3, "waited for contact then region");
    Check(g_creates == 2 && g_lastEntity == 3 && g_attaches == 2, "archers on live spawn points");
    Check(g_faces == 2 && g_faceThing == 103 && g_faceTarget == 99, "faced faceMe (faceMe alive)");
    // archers alive, faceMe dead -> no facing
    g_creates = g_attaches = g_faces = 0; g_archerAlive = true; g_faceMeAlive = false;
    script.WatchForArcherSpawnConditions();
    Check(g_frames == 3 && g_creates == 2 && g_attaches == 2 && g_faces == 0, "faceMe dead -> no facing");
    printf(g_fail ? "00f0f910_TEST FAIL\n" : "00f0f910_TEST PASS\n");
    return g_fail;
}