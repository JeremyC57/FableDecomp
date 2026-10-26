// Behaviour test for 00dfc920: final.cpp copied verbatim (32-bit `long` and calling-convention keywords under g++; build
// with `g++ -w -fpermissive`); the game interface, the string class, the thread-termination test and the region
// creature preparation are stubbed to record the call sequence, the by-value copies' reference counts and the early
// returns. CQ_TraderConflictGoodScript::WatchForRegionTransitions
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
// CQ_TraderConflictGoodScript::WatchForRegionTransitions() -- each region load: prepare the region creatures and set their combat range, sleep and Bandits faction
struct CCharString {
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CThing;
struct SCount { long m_references; };
struct CBaseClass {                                // vtable 0x01231710
    virtual void b00();
    CBaseClass() { }
};
struct CCPPointerImp {                             // CCPPointer<CScriptThingImp>: Data, Info (memberwise copy adds a reference)
    CThing* Data;
    SCount* Info;
    CCPPointerImp(const CCPPointerImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_references; }
};
struct CScriptThing : CBaseClass {                 // vtable 0x01238C8C, 12 bytes
    virtual void s00();
    CCPPointerImp PImp;                            // +4
    // the copy constructor is the compiler-generated memberwise one
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CGameScriptInterface {                     // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                       // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual bool MsgOnRegionLoaded(CCharString& region) const;                 // slot 22 (+0x58)
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046();
    virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182();
    virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198();
    virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206();
    virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222();
    virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230();
    virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238();
    virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246();
    virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254();
    virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262();
    virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270();
    virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278();
    virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286();
    virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302();
    virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310();
    virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318();
    virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326();
    virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342();
    virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350();
    virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358();
    virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366();
    virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374();
    virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382();
    virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390();
    virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398();
    virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406();
    virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414();
    virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422();
    virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430();
    virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438();
    virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446();
    virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453(); virtual void v454();
    virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461(); virtual void v462();
    virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470();
    virtual void v471(); virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477(); virtual void v478();
    virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486();
    virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493(); virtual void v494();
    virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501(); virtual void v502();
    virtual void v503(); virtual void v504(); virtual void v505(); virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510();
    virtual void v511(); virtual void v512(); virtual void v513(); virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517(); virtual void v518();
    virtual void v519(); virtual void v520(); virtual void v521(); virtual void v522(); virtual void v523(); virtual void v524(); virtual void v525(); virtual void v526();
    virtual void v527(); virtual void v528(); virtual void v529(); virtual void v530(); virtual void v531(); virtual void v532(); virtual void v533(); virtual void v534();
    virtual void v535(); virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541(); virtual void v542();
    virtual void v543(); virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547(); virtual void v548(); virtual void v549(); virtual void v550();
    virtual void v551(); virtual void v552(); virtual void v553(); virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557(); virtual void v558();
    virtual void v559(); virtual void v560(); virtual void v561(); virtual void v562(); virtual void v563(); virtual void v564(); virtual void v565(); virtual void v566();
    virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573(); virtual void v574();
    virtual void v575(); virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581(); virtual void v582();
    virtual void v583(); virtual void v584(); virtual void v585(); virtual void v586(); virtual void v587(); virtual void v588(); virtual void v589(); virtual void v590();
    virtual void v591(); virtual void v592();
    virtual void EntitySetSleepEnabled(const CScriptThing& thing, bool enabled);   // slot 593 (+0x944)
    virtual void v594(); virtual void v595(); virtual void v596(); virtual void v597(); virtual void v598(); virtual void v599(); virtual void v600();
    virtual void EntitySetInFaction(const CScriptThing& thing, const CCharString& faction) const;   // slot 601 (+0x964)
    virtual void v602(); virtual void v603(); virtual void v604(); virtual void v605(); virtual void v606(); virtual void v607(); virtual void v608(); virtual void v609();
    virtual void v610(); virtual void v611(); virtual void v612(); virtual void v613(); virtual void v614(); virtual void v615(); virtual void v616(); virtual void v617();
    virtual void v618(); virtual void v619(); virtual void v620(); virtual void v621(); virtual void v622(); virtual void v623(); virtual void v624(); virtual void v625();
    virtual void v626(); virtual void v627(); virtual void v628(); virtual void v629(); virtual void v630(); virtual void v631(); virtual void v632(); virtual void v633();
    virtual void v634(); virtual void v635(); virtual void v636(); virtual void v637(); virtual void v638(); virtual void v639(); virtual void v640(); virtual void v641();
    virtual void v642(); virtual void v643(); virtual void v644(); virtual void v645(); virtual void v646(); virtual void v647(); virtual void v648(); virtual void v649();
    virtual void v650(); virtual void v651(); virtual void v652(); virtual void v653(); virtual void v654(); virtual void v655(); virtual void v656(); virtual void v657();
    virtual void v658(); virtual void v659(); virtual void v660(); virtual void v661(); virtual void v662(); virtual void v663(); virtual void v664(); virtual void v665();
    virtual void v666(); virtual void v667(); virtual void v668(); virtual void v669(); virtual void v670(); virtual void v671(); virtual void v672(); virtual void v673();
    virtual void v674(); virtual void v675(); virtual void v676(); virtual void v677(); virtual void v678(); virtual void v679(); virtual void v680(); virtual void v681();
    virtual void v682(); virtual void v683(); virtual void v684(); virtual void v685(); virtual void v686(); virtual void v687(); virtual void v688(); virtual void v689();
    virtual void v690(); virtual void v691(); virtual void v692(); virtual void v693(); virtual void v694(); virtual void v695(); virtual void v696(); virtual void v697();
    virtual void v698(); virtual void v699(); virtual void v700(); virtual void v701(); virtual void v702(); virtual void v703(); virtual void v704(); virtual void v705();
    virtual void v706(); virtual void v707(); virtual void v708(); virtual void v709(); virtual void v710(); virtual void v711(); virtual void v712(); virtual void v713();
    virtual void v714(); virtual void v715(); virtual void v716(); virtual void v717(); virtual void v718(); virtual void v719(); virtual void v720(); virtual void v721();
    virtual void v722(); virtual void v723(); virtual void v724(); virtual void v725(); virtual void v726(); virtual void v727(); virtual void v728(); virtual void v729();
    virtual void v730(); virtual void v731(); virtual void v732(); virtual void v733(); virtual void v734(); virtual void v735(); virtual void v736(); virtual void v737();
    virtual void v738(); virtual void v739(); virtual void v740(); virtual void v741(); virtual void v742(); virtual void v743(); virtual void v744(); virtual void v745();
    virtual void v746(); virtual void v747(); virtual void v748(); virtual void v749(); virtual void v750(); virtual void v751(); virtual void v752(); virtual void v753();
    virtual void v754(); virtual void v755(); virtual void v756(); virtual void v757(); virtual void v758(); virtual void v759(); virtual void v760(); virtual void v761();
    virtual void v762(); virtual void v763(); virtual void v764(); virtual void v765(); virtual void v766(); virtual void v767(); virtual void v768(); virtual void v769();
    virtual void v770(); virtual void v771(); virtual void v772(); virtual void v773(); virtual void v774(); virtual void v775(); virtual void v776(); virtual void v777();
    virtual void v778(); virtual void v779(); virtual void v780(); virtual void v781(); virtual void v782(); virtual void v783(); virtual void v784(); virtual void v785();
    virtual void v786(); virtual void v787(); virtual void v788(); virtual void v789(); virtual void v790(); virtual void v791(); virtual void v792(); virtual void v793();
    virtual void v794(); virtual void v795(); virtual void v796(); virtual void v797(); virtual void v798(); virtual void v799(); virtual void v800(); virtual void v801();
    virtual void v802(); virtual void v803(); virtual void v804(); virtual void v805(); virtual void v806(); virtual void v807(); virtual void v808(); virtual void v809();
    virtual void v810(); virtual void v811(); virtual void v812(); virtual void v813(); virtual void v814(); virtual void v815(); virtual void v816();
    virtual void SetCombatNearbyBreakOffRange(CScriptThing thing, float range);   // slot 817 (+0xcc4)
};
extern const char g_factionBandits[];              // 0x0125FE68 "FACTION_BANDITS"
struct CScriptBase {
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
struct CQ_TraderConflictGoodScript : CScriptBase {
    unsigned char              _pad_0x00[0x40];
    CGameScriptInterface*      Game;                        // +0x40
    void*                      Data;                        // +0x44
    CRetailVector<CScriptThing> AllCreatures;               // +0x48
    long                       TradersFollowing;            // +0x54
    long                       TradersReachedTeleporter;    // +0x58
    long                       BanditSecurityLinesSaid;     // +0x5c
    bool                       FirstRegionDead;             // +0x60
    bool                       EnteredNewRegion;            // +0x61
    bool                       IntroDone;                   // +0x62
    bool                       OutroStart;                  // +0x63
    bool                       OutroDone;                   // +0x64
    void PrepareRegionCreatures();                          // 0x00DFC320
    void WatchForRegionTransitions();
};
void CQ_TraderConflictGoodScript::WatchForRegionTransitions()
{
    CCharString region;
    while (!OutroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Game->MsgOnRegionLoaded(region)) {
            if (IsActiveThreadTerminating())
                return;
            PrepareRegionCreatures();
            for (unsigned long i = 0; i < AllCreatures.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetCombatNearbyBreakOffRange(AllCreatures[i], 10.0f);
                Game->EntitySetSleepEnabled(AllCreatures[i], false);
                CCharString faction(g_factionBandits, -1);
                Game->EntitySetInFaction(AllCreatures[i], faction);
            }
            if (IsActiveThreadTerminating())
                return;
            EnteredNewRegion = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}

// ---- stubs ----
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
void CGameScriptInterface::v465() {}
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
void CGameScriptInterface::v594() {}
void CGameScriptInterface::v595() {}
void CGameScriptInterface::v596() {}
void CGameScriptInterface::v597() {}
void CGameScriptInterface::v598() {}
void CGameScriptInterface::v599() {}
void CGameScriptInterface::v600() {}
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
void CGameScriptInterface::v655() {}
void CGameScriptInterface::v656() {}
void CGameScriptInterface::v657() {}
void CGameScriptInterface::v658() {}
void CGameScriptInterface::v659() {}
void CGameScriptInterface::v660() {}
void CGameScriptInterface::v661() {}
void CGameScriptInterface::v662() {}
void CGameScriptInterface::v663() {}
void CGameScriptInterface::v664() {}
void CGameScriptInterface::v665() {}
void CGameScriptInterface::v666() {}
void CGameScriptInterface::v667() {}
void CGameScriptInterface::v668() {}
void CGameScriptInterface::v669() {}
void CGameScriptInterface::v670() {}
void CGameScriptInterface::v671() {}
void CGameScriptInterface::v672() {}
void CGameScriptInterface::v673() {}
void CGameScriptInterface::v674() {}
void CGameScriptInterface::v675() {}
void CGameScriptInterface::v676() {}
void CGameScriptInterface::v677() {}
void CGameScriptInterface::v678() {}
void CGameScriptInterface::v679() {}
void CGameScriptInterface::v680() {}
void CGameScriptInterface::v681() {}
void CGameScriptInterface::v682() {}
void CGameScriptInterface::v683() {}
void CGameScriptInterface::v684() {}
void CGameScriptInterface::v685() {}
void CGameScriptInterface::v686() {}
void CGameScriptInterface::v687() {}
void CGameScriptInterface::v688() {}
void CGameScriptInterface::v689() {}
void CGameScriptInterface::v690() {}
void CGameScriptInterface::v691() {}
void CGameScriptInterface::v692() {}
void CGameScriptInterface::v693() {}
void CGameScriptInterface::v694() {}
void CGameScriptInterface::v695() {}
void CGameScriptInterface::v696() {}
void CGameScriptInterface::v697() {}
void CGameScriptInterface::v698() {}
void CGameScriptInterface::v699() {}
void CGameScriptInterface::v700() {}
void CGameScriptInterface::v701() {}
void CGameScriptInterface::v702() {}
void CGameScriptInterface::v703() {}
void CGameScriptInterface::v704() {}
void CGameScriptInterface::v705() {}
void CGameScriptInterface::v706() {}
void CGameScriptInterface::v707() {}
void CGameScriptInterface::v708() {}
void CGameScriptInterface::v709() {}
void CGameScriptInterface::v710() {}
void CGameScriptInterface::v711() {}
void CGameScriptInterface::v712() {}
void CGameScriptInterface::v713() {}
void CGameScriptInterface::v714() {}
void CGameScriptInterface::v715() {}
void CGameScriptInterface::v716() {}
void CGameScriptInterface::v717() {}
void CGameScriptInterface::v718() {}
void CGameScriptInterface::v719() {}
void CGameScriptInterface::v720() {}
void CGameScriptInterface::v721() {}
void CGameScriptInterface::v722() {}
void CGameScriptInterface::v723() {}
void CGameScriptInterface::v724() {}
void CGameScriptInterface::v725() {}
void CGameScriptInterface::v726() {}
void CGameScriptInterface::v727() {}
void CGameScriptInterface::v728() {}
void CGameScriptInterface::v729() {}
void CGameScriptInterface::v730() {}
void CGameScriptInterface::v731() {}
void CGameScriptInterface::v732() {}
void CGameScriptInterface::v733() {}
void CGameScriptInterface::v734() {}
void CGameScriptInterface::v735() {}
void CGameScriptInterface::v736() {}
void CGameScriptInterface::v737() {}
void CGameScriptInterface::v738() {}
void CGameScriptInterface::v739() {}
void CGameScriptInterface::v740() {}
void CGameScriptInterface::v741() {}
void CGameScriptInterface::v742() {}
void CGameScriptInterface::v743() {}
void CGameScriptInterface::v744() {}
void CGameScriptInterface::v745() {}
void CGameScriptInterface::v746() {}
void CGameScriptInterface::v747() {}
void CGameScriptInterface::v748() {}
void CGameScriptInterface::v749() {}
void CGameScriptInterface::v750() {}
void CGameScriptInterface::v751() {}
void CGameScriptInterface::v752() {}
void CGameScriptInterface::v753() {}
void CGameScriptInterface::v754() {}
void CGameScriptInterface::v755() {}
void CGameScriptInterface::v756() {}
void CGameScriptInterface::v757() {}
void CGameScriptInterface::v758() {}
void CGameScriptInterface::v759() {}
void CGameScriptInterface::v760() {}
void CGameScriptInterface::v761() {}
void CGameScriptInterface::v762() {}
void CGameScriptInterface::v763() {}
void CGameScriptInterface::v764() {}
void CGameScriptInterface::v765() {}
void CGameScriptInterface::v766() {}
void CGameScriptInterface::v767() {}
void CGameScriptInterface::v768() {}
void CGameScriptInterface::v769() {}
void CGameScriptInterface::v770() {}
void CGameScriptInterface::v771() {}
void CGameScriptInterface::v772() {}
void CGameScriptInterface::v773() {}
void CGameScriptInterface::v774() {}
void CGameScriptInterface::v775() {}
void CGameScriptInterface::v776() {}
void CGameScriptInterface::v777() {}
void CGameScriptInterface::v778() {}
void CGameScriptInterface::v779() {}
void CGameScriptInterface::v780() {}
void CGameScriptInterface::v781() {}
void CGameScriptInterface::v782() {}
void CGameScriptInterface::v783() {}
void CGameScriptInterface::v784() {}
void CGameScriptInterface::v785() {}
void CGameScriptInterface::v786() {}
void CGameScriptInterface::v787() {}
void CGameScriptInterface::v788() {}
void CGameScriptInterface::v789() {}
void CGameScriptInterface::v790() {}
void CGameScriptInterface::v791() {}
void CGameScriptInterface::v792() {}
void CGameScriptInterface::v793() {}
void CGameScriptInterface::v794() {}
void CGameScriptInterface::v795() {}
void CGameScriptInterface::v796() {}
void CGameScriptInterface::v797() {}
void CGameScriptInterface::v798() {}
void CGameScriptInterface::v799() {}
void CGameScriptInterface::v800() {}
void CGameScriptInterface::v801() {}
void CGameScriptInterface::v802() {}
void CGameScriptInterface::v803() {}
void CGameScriptInterface::v804() {}
void CGameScriptInterface::v805() {}
void CGameScriptInterface::v806() {}
void CGameScriptInterface::v807() {}
void CGameScriptInterface::v808() {}
void CGameScriptInterface::v809() {}
void CGameScriptInterface::v810() {}
void CGameScriptInterface::v811() {}
void CGameScriptInterface::v812() {}
void CGameScriptInterface::v813() {}
void CGameScriptInterface::v814() {}
void CGameScriptInterface::v815() {}
void CGameScriptInterface::v816() {}

const char g_factionBandits[] = "FACTION_BANDITS";
static int g_liveStrings = 0;
CCharString::CCharString() : PStringData(0) { ++g_liveStrings; }
CCharString::CCharString(const char* text, long length)
{
    size_t n = length < 0 ? std::strlen(text) : (size_t)length;
    char* p = (char*)std::malloc(n + 1); std::memcpy(p, text, n); p[n] = 0; PStringData = p; ++g_liveStrings;
}
CCharString::~CCharString() { std::free(PStringData); --g_liveStrings; }
void CBaseClass::b00() {}
void CScriptThing::s00() {}
struct SCall { int slot; const void* thing; const void* data; long refs; float f; };
static SCall g_log[64]; static int g_logCount = 0;
static void Log(int slot, const void* thing, const void* data, long refs, float f)
{ if (g_logCount < 64) { SCall& c = g_log[g_logCount++]; c.slot = slot; c.thing = thing; c.data = data; c.refs = refs; c.f = f; } }
static CQ_TraderConflictGoodScript* g_script = 0;
static int g_frames = 0; static int g_frameOutroDone = 0; static int g_frameRegionLoaded = 0;
static int g_terminateCalls = 0; static int g_terminateAt = 0;
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; Log(7, 0, 0, 0, 0); if (g_frames == g_frameOutroDone) g_script->OutroDone = true; }
bool CGameScriptInterface::MsgOnRegionLoaded(CCharString& region) const { Log(22, &region, 0, 0, 0); return g_frames == g_frameRegionLoaded; }
void CGameScriptInterface::EntitySetSleepEnabled(const CScriptThing& thing, bool enabled) { Log(593, &thing, thing.PImp.Data, thing.PImp.Info->m_references, enabled ? 1.0f : 0.0f); }
void CGameScriptInterface::EntitySetInFaction(const CScriptThing& thing, const CCharString& faction) const { Log(601, &thing, thing.PImp.Data, thing.PImp.Info->m_references, (float)std::strcmp((const char*)faction.PStringData, "FACTION_BANDITS")); }
void CGameScriptInterface::SetCombatNearbyBreakOffRange(CScriptThing thing, float range)
{
    Log(817, &thing, thing.PImp.Data, thing.PImp.Info->m_references, range);
    --thing.PImp.Info->m_references;   // the callee releases its by-value copy
}
bool CScriptBase::IsActiveThreadTerminating() { ++g_terminateCalls; return g_terminateCalls == g_terminateAt; }
static SCount g_counts[2]; static int g_things[2];
static unsigned char g_storage[2][sizeof(CScriptThing)];
void CQ_TraderConflictGoodScript::PrepareRegionCreatures()
{
    Log(1, 0, 0, 0, 0);
    CScriptThing* v = (CScriptThing*)g_storage;
    for (int i = 0; i < 2; ++i) { g_counts[i].m_references = 1; v[i].PImp.Data = (CThing*)&g_things[i]; v[i].PImp.Info = &g_counts[i]; }
    AllCreatures._M_start = v; AllCreatures._M_finish = v + 2; AllCreatures._M_end_of_storage = v + 2;
}
static void Reset(CQ_TraderConflictGoodScript& s, int frameRegionLoaded, int frameOutroDone, int terminateAt)
{
    std::memset((void*)&s, 0, sizeof(s));
    static CGameScriptInterface game; s.Game = &game; g_script = &s;
    g_frames = 0; g_frameRegionLoaded = frameRegionLoaded; g_frameOutroDone = frameOutroDone;
    g_terminateCalls = 0; g_terminateAt = terminateAt; g_logCount = 0;
}
int main()
{
    CQ_TraderConflictGoodScript s;
    int stringsBefore = g_liveStrings;
    // region loaded on frame 2, outro done on frame 3, never terminating
    Reset(s, 2, 3, 0);
    s.WatchForRegionTransitions();
    CHECK(g_frames == 3 && s.OutroDone && s.EnteredNewRegion);
    CHECK(g_terminateCalls == 3 + 1 + 2 + 1 + 1);     // per frame, after the load, per creature, after the loop, at the end
    CHECK(g_liveStrings == stringsBefore);
    CScriptThing* v = (CScriptThing*)g_storage;
    // log: 7, 22, 7, 22, 1, then per creature 817/593/601, then 7, 22
    CHECK(g_logCount == 2 + 2 + 1 + 6 + 2);
    CHECK(g_log[0].slot == 7 && g_log[1].slot == 22 && g_log[2].slot == 7 && g_log[3].slot == 22 && g_log[4].slot == 1);
    for (int i = 0; i < 2; ++i) {
        const SCall* c = &g_log[5 + i * 3];
        CHECK(c[0].slot == 817 && c[0].thing != &v[i] && c[0].data == &g_things[i] && c[0].refs == 2 && c[0].f == 10.0f);
        CHECK(c[1].slot == 593 && c[1].thing == &v[i] && c[1].refs == 1 && c[1].f == 0.0f);
        CHECK(c[2].slot == 601 && c[2].thing == &v[i] && c[2].refs == 1 && c[2].f == 0.0f);
    }
    CHECK(g_log[11].slot == 7 && g_log[12].slot == 22);
    CHECK(g_counts[0].m_references == 1 && g_counts[1].m_references == 1);
    // terminating inside the loop (the 5th test: frame 1, frame 2, after the load, creature 0, creature 1)
    Reset(s, 2, 3, 5);
    s.WatchForRegionTransitions();
    CHECK(g_frames == 2 && !s.OutroDone && !s.EnteredNewRegion && g_terminateCalls == 5);
    CHECK(g_logCount == 2 + 2 + 1 + 3 && g_log[7].slot == 601);
    CHECK(g_counts[0].m_references == 1 && g_liveStrings == stringsBefore);
    // terminating on the very first test
    Reset(s, 2, 3, 1);
    s.WatchForRegionTransitions();
    CHECK(g_frames == 1 && g_logCount == 1 && g_liveStrings == stringsBefore);
    // outro already done: only the final test runs
    Reset(s, 2, 3, 0); s.OutroDone = true;
    s.WatchForRegionTransitions();
    CHECK(g_frames == 0 && g_terminateCalls == 1 && g_logCount == 0);
    if (fails == 0) printf("00dfc920_TEST PASS\n"); else printf("FAILS: %d\n", fails);
    return fails ? 1 : 0;
}