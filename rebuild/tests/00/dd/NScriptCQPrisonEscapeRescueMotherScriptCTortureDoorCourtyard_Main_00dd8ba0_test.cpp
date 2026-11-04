#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CTortureDoorCourtyard::Main() -- keep the courtyard torture door unlocked until the mother is rescued; then lock it and, for as long as the mother stays rescued, show the "escape via the secret passage" hint whenever the hero uses it
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD8BA0, length 348 bytes (+4 int3). Entity-script slot 1 (Main) of the
//   CQ_PrisonEscapeRescueMotherScript nested CTortureDoorCourtyard (Ego_r.pdb has two CTortureDoorCourtyard classes; the
//   other, CQ_SecretPassageScript's, is landed at 0x00DEEFC0 and has no flag at +0x67).
// Callees: CGameScriptInterface slots 518 EntitySetAsLocked (+0x818), 7 NewScriptFrame (+0x1c), 115 DisplayGameInfo
//   (+0x1cc), 40 MsgIsGameInfoClickedPast (+0xa0); CScriptThing slot 20 MsgIsUsedBy (+0x50, 0x004AAB10); the entity
//   script's IsActiveThreadTerminating @ 0x00F35B30 (manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is
//   propagated); CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0.
// Strings: 0x0125D1C8 "SCRIPT_NAME_HERO", 0x012DC4C8 "TEXT_QST_055_HINT_ESCAPE_VIA_SECRET_PASSAGE".
// Layout: Ego_r.pdb CTortureDoorCourtyard (28 bytes, = retail): Game +0x04, Me +0x08, ParentClass +0x14, PMasterData +0x18;
//   the parent's mother-rescued flag at +0x67 as the landed sibling CTortureDoorPassage::Main (0x00DD8990) of the same TU.
// Shape: the landed CTortureDoorPassage::Main with the lock order reversed and a second (inverse) flag loop; the trailing
//   `if (IsActiveThreadTerminating()) return;` leaves a bare call before the epilogue.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing { public:                                    // 12 bytes, vtable 0x01238C8C
    virtual void s000() const; virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual void s006() const; virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const;
    virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const;
    virtual void s018() const; virtual void s019() const;
    virtual bool MsgIsUsedBy(const CCharString& user) const;   // slot 20 (+0x50) 0x004AAB10
    void* PImpData;                                      // +0x04
    void* PImpInfo;                                      // +0x08
};
class CGameScriptInterface { public:
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const;
    virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const;
    virtual void v014() const; virtual void v015() const; virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const;
    virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const; virtual void v025() const;
    virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const;
    virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const;
    virtual void v038() const; virtual void v039() const;
    virtual bool MsgIsGameInfoClickedPast() const;   // slot 40 (+0xa0)
    virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const;
    virtual void v047() const; virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const;
    virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const;
    virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const;
    virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const; virtual void v070() const;
    virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const;
    virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const;
    virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const;
    virtual void v089() const; virtual void v090() const; virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const;
    virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const; virtual void v100() const;
    virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const;
    virtual void v107() const; virtual void v108() const; virtual void v109() const; virtual void v110() const; virtual void v111() const; virtual void v112() const;
    virtual void v113() const; virtual void v114() const;
    virtual void DisplayGameInfo(const CCharString& text) const;   // slot 115 (+0x1cc)
    virtual void v116() const; virtual void v117() const; virtual void v118() const; virtual void v119() const; virtual void v120() const; virtual void v121() const;
    virtual void v122() const; virtual void v123() const; virtual void v124() const; virtual void v125() const; virtual void v126() const; virtual void v127() const;
    virtual void v128() const; virtual void v129() const; virtual void v130() const; virtual void v131() const; virtual void v132() const; virtual void v133() const;
    virtual void v134() const; virtual void v135() const; virtual void v136() const; virtual void v137() const; virtual void v138() const; virtual void v139() const;
    virtual void v140() const; virtual void v141() const; virtual void v142() const; virtual void v143() const; virtual void v144() const; virtual void v145() const;
    virtual void v146() const; virtual void v147() const; virtual void v148() const; virtual void v149() const; virtual void v150() const; virtual void v151() const;
    virtual void v152() const; virtual void v153() const; virtual void v154() const; virtual void v155() const; virtual void v156() const; virtual void v157() const;
    virtual void v158() const; virtual void v159() const; virtual void v160() const; virtual void v161() const; virtual void v162() const; virtual void v163() const;
    virtual void v164() const; virtual void v165() const; virtual void v166() const; virtual void v167() const; virtual void v168() const; virtual void v169() const;
    virtual void v170() const; virtual void v171() const; virtual void v172() const; virtual void v173() const; virtual void v174() const; virtual void v175() const;
    virtual void v176() const; virtual void v177() const; virtual void v178() const; virtual void v179() const; virtual void v180() const; virtual void v181() const;
    virtual void v182() const; virtual void v183() const; virtual void v184() const; virtual void v185() const; virtual void v186() const; virtual void v187() const;
    virtual void v188() const; virtual void v189() const; virtual void v190() const; virtual void v191() const; virtual void v192() const; virtual void v193() const;
    virtual void v194() const; virtual void v195() const; virtual void v196() const; virtual void v197() const; virtual void v198() const; virtual void v199() const;
    virtual void v200() const; virtual void v201() const; virtual void v202() const; virtual void v203() const; virtual void v204() const; virtual void v205() const;
    virtual void v206() const; virtual void v207() const; virtual void v208() const; virtual void v209() const; virtual void v210() const; virtual void v211() const;
    virtual void v212() const; virtual void v213() const; virtual void v214() const; virtual void v215() const; virtual void v216() const; virtual void v217() const;
    virtual void v218() const; virtual void v219() const; virtual void v220() const; virtual void v221() const; virtual void v222() const; virtual void v223() const;
    virtual void v224() const; virtual void v225() const; virtual void v226() const; virtual void v227() const; virtual void v228() const; virtual void v229() const;
    virtual void v230() const; virtual void v231() const; virtual void v232() const; virtual void v233() const; virtual void v234() const; virtual void v235() const;
    virtual void v236() const; virtual void v237() const; virtual void v238() const; virtual void v239() const; virtual void v240() const; virtual void v241() const;
    virtual void v242() const; virtual void v243() const; virtual void v244() const; virtual void v245() const; virtual void v246() const; virtual void v247() const;
    virtual void v248() const; virtual void v249() const; virtual void v250() const; virtual void v251() const; virtual void v252() const; virtual void v253() const;
    virtual void v254() const; virtual void v255() const; virtual void v256() const; virtual void v257() const; virtual void v258() const; virtual void v259() const;
    virtual void v260() const; virtual void v261() const; virtual void v262() const; virtual void v263() const; virtual void v264() const; virtual void v265() const;
    virtual void v266() const; virtual void v267() const; virtual void v268() const; virtual void v269() const; virtual void v270() const; virtual void v271() const;
    virtual void v272() const; virtual void v273() const; virtual void v274() const; virtual void v275() const; virtual void v276() const; virtual void v277() const;
    virtual void v278() const; virtual void v279() const; virtual void v280() const; virtual void v281() const; virtual void v282() const; virtual void v283() const;
    virtual void v284() const; virtual void v285() const; virtual void v286() const; virtual void v287() const; virtual void v288() const; virtual void v289() const;
    virtual void v290() const; virtual void v291() const; virtual void v292() const; virtual void v293() const; virtual void v294() const; virtual void v295() const;
    virtual void v296() const; virtual void v297() const; virtual void v298() const; virtual void v299() const; virtual void v300() const; virtual void v301() const;
    virtual void v302() const; virtual void v303() const; virtual void v304() const; virtual void v305() const; virtual void v306() const; virtual void v307() const;
    virtual void v308() const; virtual void v309() const; virtual void v310() const; virtual void v311() const; virtual void v312() const; virtual void v313() const;
    virtual void v314() const; virtual void v315() const; virtual void v316() const; virtual void v317() const; virtual void v318() const; virtual void v319() const;
    virtual void v320() const; virtual void v321() const; virtual void v322() const; virtual void v323() const; virtual void v324() const; virtual void v325() const;
    virtual void v326() const; virtual void v327() const; virtual void v328() const; virtual void v329() const; virtual void v330() const; virtual void v331() const;
    virtual void v332() const; virtual void v333() const; virtual void v334() const; virtual void v335() const; virtual void v336() const; virtual void v337() const;
    virtual void v338() const; virtual void v339() const; virtual void v340() const; virtual void v341() const; virtual void v342() const; virtual void v343() const;
    virtual void v344() const; virtual void v345() const; virtual void v346() const; virtual void v347() const; virtual void v348() const; virtual void v349() const;
    virtual void v350() const; virtual void v351() const; virtual void v352() const; virtual void v353() const; virtual void v354() const; virtual void v355() const;
    virtual void v356() const; virtual void v357() const; virtual void v358() const; virtual void v359() const; virtual void v360() const; virtual void v361() const;
    virtual void v362() const; virtual void v363() const; virtual void v364() const; virtual void v365() const; virtual void v366() const; virtual void v367() const;
    virtual void v368() const; virtual void v369() const; virtual void v370() const; virtual void v371() const; virtual void v372() const; virtual void v373() const;
    virtual void v374() const; virtual void v375() const; virtual void v376() const; virtual void v377() const; virtual void v378() const; virtual void v379() const;
    virtual void v380() const; virtual void v381() const; virtual void v382() const; virtual void v383() const; virtual void v384() const; virtual void v385() const;
    virtual void v386() const; virtual void v387() const; virtual void v388() const; virtual void v389() const; virtual void v390() const; virtual void v391() const;
    virtual void v392() const; virtual void v393() const; virtual void v394() const; virtual void v395() const; virtual void v396() const; virtual void v397() const;
    virtual void v398() const; virtual void v399() const; virtual void v400() const; virtual void v401() const; virtual void v402() const; virtual void v403() const;
    virtual void v404() const; virtual void v405() const; virtual void v406() const; virtual void v407() const; virtual void v408() const; virtual void v409() const;
    virtual void v410() const; virtual void v411() const; virtual void v412() const; virtual void v413() const; virtual void v414() const; virtual void v415() const;
    virtual void v416() const; virtual void v417() const; virtual void v418() const; virtual void v419() const; virtual void v420() const; virtual void v421() const;
    virtual void v422() const; virtual void v423() const; virtual void v424() const; virtual void v425() const; virtual void v426() const; virtual void v427() const;
    virtual void v428() const; virtual void v429() const; virtual void v430() const; virtual void v431() const; virtual void v432() const; virtual void v433() const;
    virtual void v434() const; virtual void v435() const; virtual void v436() const; virtual void v437() const; virtual void v438() const; virtual void v439() const;
    virtual void v440() const; virtual void v441() const; virtual void v442() const; virtual void v443() const; virtual void v444() const; virtual void v445() const;
    virtual void v446() const; virtual void v447() const; virtual void v448() const; virtual void v449() const; virtual void v450() const; virtual void v451() const;
    virtual void v452() const; virtual void v453() const; virtual void v454() const; virtual void v455() const; virtual void v456() const; virtual void v457() const;
    virtual void v458() const; virtual void v459() const; virtual void v460() const; virtual void v461() const; virtual void v462() const; virtual void v463() const;
    virtual void v464() const; virtual void v465() const; virtual void v466() const; virtual void v467() const; virtual void v468() const; virtual void v469() const;
    virtual void v470() const; virtual void v471() const; virtual void v472() const; virtual void v473() const; virtual void v474() const; virtual void v475() const;
    virtual void v476() const; virtual void v477() const; virtual void v478() const; virtual void v479() const; virtual void v480() const; virtual void v481() const;
    virtual void v482() const; virtual void v483() const; virtual void v484() const; virtual void v485() const; virtual void v486() const; virtual void v487() const;
    virtual void v488() const; virtual void v489() const; virtual void v490() const; virtual void v491() const; virtual void v492() const; virtual void v493() const;
    virtual void v494() const; virtual void v495() const; virtual void v496() const; virtual void v497() const; virtual void v498() const; virtual void v499() const;
    virtual void v500() const; virtual void v501() const; virtual void v502() const; virtual void v503() const; virtual void v504() const; virtual void v505() const;
    virtual void v506() const; virtual void v507() const; virtual void v508() const; virtual void v509() const; virtual void v510() const; virtual void v511() const;
    virtual void v512() const; virtual void v513() const; virtual void v514() const; virtual void v515() const; virtual void v516() const; virtual void v517() const;
    virtual void EntitySetAsLocked(const CScriptThing& thing, bool locked) const;   // slot 518 (+0x818)
};
class CQ_PrisonEscapeRescueMotherScript {
public:
    unsigned char _pad00[0x67];
    bool          MotherRescued;                         // +0x67
};
class CQ_SunnyvaleMasterData;
class CTortureDoorCourtyard {
public:
    void*                              __vftable;        // +0x00
    CGameScriptInterface*              Game;             // +0x04
    CScriptThing                       Me;               // +0x08
    CQ_PrisonEscapeRescueMotherScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData*            PMasterData;      // +0x18
    bool IsActiveThreadTerminating();                    // 0x00F35B30
    void Main();
};

void CTortureDoorCourtyard::Main()
{
    Game->EntitySetAsLocked(Me, false);
    while (!ParentClass->MotherRescued) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsLocked(Me, true);
    while (ParentClass->MotherRescued) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_055_HINT_ESCAPE_VIA_SECRET_PASSAGE");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}

void CGameScriptInterface::v000() const {}
void CGameScriptInterface::v001() const {}
void CGameScriptInterface::v002() const {}
void CGameScriptInterface::v003() const {}
void CGameScriptInterface::v004() const {}
void CGameScriptInterface::v005() const {}
void CGameScriptInterface::v006() const {}
void CGameScriptInterface::v008() const {}
void CGameScriptInterface::v009() const {}
void CGameScriptInterface::v010() const {}
void CGameScriptInterface::v011() const {}
void CGameScriptInterface::v012() const {}
void CGameScriptInterface::v013() const {}
void CGameScriptInterface::v014() const {}
void CGameScriptInterface::v015() const {}
void CGameScriptInterface::v016() const {}
void CGameScriptInterface::v017() const {}
void CGameScriptInterface::v018() const {}
void CGameScriptInterface::v019() const {}
void CGameScriptInterface::v020() const {}
void CGameScriptInterface::v021() const {}
void CGameScriptInterface::v022() const {}
void CGameScriptInterface::v023() const {}
void CGameScriptInterface::v024() const {}
void CGameScriptInterface::v025() const {}
void CGameScriptInterface::v026() const {}
void CGameScriptInterface::v027() const {}
void CGameScriptInterface::v028() const {}
void CGameScriptInterface::v029() const {}
void CGameScriptInterface::v030() const {}
void CGameScriptInterface::v031() const {}
void CGameScriptInterface::v032() const {}
void CGameScriptInterface::v033() const {}
void CGameScriptInterface::v034() const {}
void CGameScriptInterface::v035() const {}
void CGameScriptInterface::v036() const {}
void CGameScriptInterface::v037() const {}
void CGameScriptInterface::v038() const {}
void CGameScriptInterface::v039() const {}
void CGameScriptInterface::v041() const {}
void CGameScriptInterface::v042() const {}
void CGameScriptInterface::v043() const {}
void CGameScriptInterface::v044() const {}
void CGameScriptInterface::v045() const {}
void CGameScriptInterface::v046() const {}
void CGameScriptInterface::v047() const {}
void CGameScriptInterface::v048() const {}
void CGameScriptInterface::v049() const {}
void CGameScriptInterface::v050() const {}
void CGameScriptInterface::v051() const {}
void CGameScriptInterface::v052() const {}
void CGameScriptInterface::v053() const {}
void CGameScriptInterface::v054() const {}
void CGameScriptInterface::v055() const {}
void CGameScriptInterface::v056() const {}
void CGameScriptInterface::v057() const {}
void CGameScriptInterface::v058() const {}
void CGameScriptInterface::v059() const {}
void CGameScriptInterface::v060() const {}
void CGameScriptInterface::v061() const {}
void CGameScriptInterface::v062() const {}
void CGameScriptInterface::v063() const {}
void CGameScriptInterface::v064() const {}
void CGameScriptInterface::v065() const {}
void CGameScriptInterface::v066() const {}
void CGameScriptInterface::v067() const {}
void CGameScriptInterface::v068() const {}
void CGameScriptInterface::v069() const {}
void CGameScriptInterface::v070() const {}
void CGameScriptInterface::v071() const {}
void CGameScriptInterface::v072() const {}
void CGameScriptInterface::v073() const {}
void CGameScriptInterface::v074() const {}
void CGameScriptInterface::v075() const {}
void CGameScriptInterface::v076() const {}
void CGameScriptInterface::v077() const {}
void CGameScriptInterface::v078() const {}
void CGameScriptInterface::v079() const {}
void CGameScriptInterface::v080() const {}
void CGameScriptInterface::v081() const {}
void CGameScriptInterface::v082() const {}
void CGameScriptInterface::v083() const {}
void CGameScriptInterface::v084() const {}
void CGameScriptInterface::v085() const {}
void CGameScriptInterface::v086() const {}
void CGameScriptInterface::v087() const {}
void CGameScriptInterface::v088() const {}
void CGameScriptInterface::v089() const {}
void CGameScriptInterface::v090() const {}
void CGameScriptInterface::v091() const {}
void CGameScriptInterface::v092() const {}
void CGameScriptInterface::v093() const {}
void CGameScriptInterface::v094() const {}
void CGameScriptInterface::v095() const {}
void CGameScriptInterface::v096() const {}
void CGameScriptInterface::v097() const {}
void CGameScriptInterface::v098() const {}
void CGameScriptInterface::v099() const {}
void CGameScriptInterface::v100() const {}
void CGameScriptInterface::v101() const {}
void CGameScriptInterface::v102() const {}
void CGameScriptInterface::v103() const {}
void CGameScriptInterface::v104() const {}
void CGameScriptInterface::v105() const {}
void CGameScriptInterface::v106() const {}
void CGameScriptInterface::v107() const {}
void CGameScriptInterface::v108() const {}
void CGameScriptInterface::v109() const {}
void CGameScriptInterface::v110() const {}
void CGameScriptInterface::v111() const {}
void CGameScriptInterface::v112() const {}
void CGameScriptInterface::v113() const {}
void CGameScriptInterface::v114() const {}
void CGameScriptInterface::v116() const {}
void CGameScriptInterface::v117() const {}
void CGameScriptInterface::v118() const {}
void CGameScriptInterface::v119() const {}
void CGameScriptInterface::v120() const {}
void CGameScriptInterface::v121() const {}
void CGameScriptInterface::v122() const {}
void CGameScriptInterface::v123() const {}
void CGameScriptInterface::v124() const {}
void CGameScriptInterface::v125() const {}
void CGameScriptInterface::v126() const {}
void CGameScriptInterface::v127() const {}
void CGameScriptInterface::v128() const {}
void CGameScriptInterface::v129() const {}
void CGameScriptInterface::v130() const {}
void CGameScriptInterface::v131() const {}
void CGameScriptInterface::v132() const {}
void CGameScriptInterface::v133() const {}
void CGameScriptInterface::v134() const {}
void CGameScriptInterface::v135() const {}
void CGameScriptInterface::v136() const {}
void CGameScriptInterface::v137() const {}
void CGameScriptInterface::v138() const {}
void CGameScriptInterface::v139() const {}
void CGameScriptInterface::v140() const {}
void CGameScriptInterface::v141() const {}
void CGameScriptInterface::v142() const {}
void CGameScriptInterface::v143() const {}
void CGameScriptInterface::v144() const {}
void CGameScriptInterface::v145() const {}
void CGameScriptInterface::v146() const {}
void CGameScriptInterface::v147() const {}
void CGameScriptInterface::v148() const {}
void CGameScriptInterface::v149() const {}
void CGameScriptInterface::v150() const {}
void CGameScriptInterface::v151() const {}
void CGameScriptInterface::v152() const {}
void CGameScriptInterface::v153() const {}
void CGameScriptInterface::v154() const {}
void CGameScriptInterface::v155() const {}
void CGameScriptInterface::v156() const {}
void CGameScriptInterface::v157() const {}
void CGameScriptInterface::v158() const {}
void CGameScriptInterface::v159() const {}
void CGameScriptInterface::v160() const {}
void CGameScriptInterface::v161() const {}
void CGameScriptInterface::v162() const {}
void CGameScriptInterface::v163() const {}
void CGameScriptInterface::v164() const {}
void CGameScriptInterface::v165() const {}
void CGameScriptInterface::v166() const {}
void CGameScriptInterface::v167() const {}
void CGameScriptInterface::v168() const {}
void CGameScriptInterface::v169() const {}
void CGameScriptInterface::v170() const {}
void CGameScriptInterface::v171() const {}
void CGameScriptInterface::v172() const {}
void CGameScriptInterface::v173() const {}
void CGameScriptInterface::v174() const {}
void CGameScriptInterface::v175() const {}
void CGameScriptInterface::v176() const {}
void CGameScriptInterface::v177() const {}
void CGameScriptInterface::v178() const {}
void CGameScriptInterface::v179() const {}
void CGameScriptInterface::v180() const {}
void CGameScriptInterface::v181() const {}
void CGameScriptInterface::v182() const {}
void CGameScriptInterface::v183() const {}
void CGameScriptInterface::v184() const {}
void CGameScriptInterface::v185() const {}
void CGameScriptInterface::v186() const {}
void CGameScriptInterface::v187() const {}
void CGameScriptInterface::v188() const {}
void CGameScriptInterface::v189() const {}
void CGameScriptInterface::v190() const {}
void CGameScriptInterface::v191() const {}
void CGameScriptInterface::v192() const {}
void CGameScriptInterface::v193() const {}
void CGameScriptInterface::v194() const {}
void CGameScriptInterface::v195() const {}
void CGameScriptInterface::v196() const {}
void CGameScriptInterface::v197() const {}
void CGameScriptInterface::v198() const {}
void CGameScriptInterface::v199() const {}
void CGameScriptInterface::v200() const {}
void CGameScriptInterface::v201() const {}
void CGameScriptInterface::v202() const {}
void CGameScriptInterface::v203() const {}
void CGameScriptInterface::v204() const {}
void CGameScriptInterface::v205() const {}
void CGameScriptInterface::v206() const {}
void CGameScriptInterface::v207() const {}
void CGameScriptInterface::v208() const {}
void CGameScriptInterface::v209() const {}
void CGameScriptInterface::v210() const {}
void CGameScriptInterface::v211() const {}
void CGameScriptInterface::v212() const {}
void CGameScriptInterface::v213() const {}
void CGameScriptInterface::v214() const {}
void CGameScriptInterface::v215() const {}
void CGameScriptInterface::v216() const {}
void CGameScriptInterface::v217() const {}
void CGameScriptInterface::v218() const {}
void CGameScriptInterface::v219() const {}
void CGameScriptInterface::v220() const {}
void CGameScriptInterface::v221() const {}
void CGameScriptInterface::v222() const {}
void CGameScriptInterface::v223() const {}
void CGameScriptInterface::v224() const {}
void CGameScriptInterface::v225() const {}
void CGameScriptInterface::v226() const {}
void CGameScriptInterface::v227() const {}
void CGameScriptInterface::v228() const {}
void CGameScriptInterface::v229() const {}
void CGameScriptInterface::v230() const {}
void CGameScriptInterface::v231() const {}
void CGameScriptInterface::v232() const {}
void CGameScriptInterface::v233() const {}
void CGameScriptInterface::v234() const {}
void CGameScriptInterface::v235() const {}
void CGameScriptInterface::v236() const {}
void CGameScriptInterface::v237() const {}
void CGameScriptInterface::v238() const {}
void CGameScriptInterface::v239() const {}
void CGameScriptInterface::v240() const {}
void CGameScriptInterface::v241() const {}
void CGameScriptInterface::v242() const {}
void CGameScriptInterface::v243() const {}
void CGameScriptInterface::v244() const {}
void CGameScriptInterface::v245() const {}
void CGameScriptInterface::v246() const {}
void CGameScriptInterface::v247() const {}
void CGameScriptInterface::v248() const {}
void CGameScriptInterface::v249() const {}
void CGameScriptInterface::v250() const {}
void CGameScriptInterface::v251() const {}
void CGameScriptInterface::v252() const {}
void CGameScriptInterface::v253() const {}
void CGameScriptInterface::v254() const {}
void CGameScriptInterface::v255() const {}
void CGameScriptInterface::v256() const {}
void CGameScriptInterface::v257() const {}
void CGameScriptInterface::v258() const {}
void CGameScriptInterface::v259() const {}
void CGameScriptInterface::v260() const {}
void CGameScriptInterface::v261() const {}
void CGameScriptInterface::v262() const {}
void CGameScriptInterface::v263() const {}
void CGameScriptInterface::v264() const {}
void CGameScriptInterface::v265() const {}
void CGameScriptInterface::v266() const {}
void CGameScriptInterface::v267() const {}
void CGameScriptInterface::v268() const {}
void CGameScriptInterface::v269() const {}
void CGameScriptInterface::v270() const {}
void CGameScriptInterface::v271() const {}
void CGameScriptInterface::v272() const {}
void CGameScriptInterface::v273() const {}
void CGameScriptInterface::v274() const {}
void CGameScriptInterface::v275() const {}
void CGameScriptInterface::v276() const {}
void CGameScriptInterface::v277() const {}
void CGameScriptInterface::v278() const {}
void CGameScriptInterface::v279() const {}
void CGameScriptInterface::v280() const {}
void CGameScriptInterface::v281() const {}
void CGameScriptInterface::v282() const {}
void CGameScriptInterface::v283() const {}
void CGameScriptInterface::v284() const {}
void CGameScriptInterface::v285() const {}
void CGameScriptInterface::v286() const {}
void CGameScriptInterface::v287() const {}
void CGameScriptInterface::v288() const {}
void CGameScriptInterface::v289() const {}
void CGameScriptInterface::v290() const {}
void CGameScriptInterface::v291() const {}
void CGameScriptInterface::v292() const {}
void CGameScriptInterface::v293() const {}
void CGameScriptInterface::v294() const {}
void CGameScriptInterface::v295() const {}
void CGameScriptInterface::v296() const {}
void CGameScriptInterface::v297() const {}
void CGameScriptInterface::v298() const {}
void CGameScriptInterface::v299() const {}
void CGameScriptInterface::v300() const {}
void CGameScriptInterface::v301() const {}
void CGameScriptInterface::v302() const {}
void CGameScriptInterface::v303() const {}
void CGameScriptInterface::v304() const {}
void CGameScriptInterface::v305() const {}
void CGameScriptInterface::v306() const {}
void CGameScriptInterface::v307() const {}
void CGameScriptInterface::v308() const {}
void CGameScriptInterface::v309() const {}
void CGameScriptInterface::v310() const {}
void CGameScriptInterface::v311() const {}
void CGameScriptInterface::v312() const {}
void CGameScriptInterface::v313() const {}
void CGameScriptInterface::v314() const {}
void CGameScriptInterface::v315() const {}
void CGameScriptInterface::v316() const {}
void CGameScriptInterface::v317() const {}
void CGameScriptInterface::v318() const {}
void CGameScriptInterface::v319() const {}
void CGameScriptInterface::v320() const {}
void CGameScriptInterface::v321() const {}
void CGameScriptInterface::v322() const {}
void CGameScriptInterface::v323() const {}
void CGameScriptInterface::v324() const {}
void CGameScriptInterface::v325() const {}
void CGameScriptInterface::v326() const {}
void CGameScriptInterface::v327() const {}
void CGameScriptInterface::v328() const {}
void CGameScriptInterface::v329() const {}
void CGameScriptInterface::v330() const {}
void CGameScriptInterface::v331() const {}
void CGameScriptInterface::v332() const {}
void CGameScriptInterface::v333() const {}
void CGameScriptInterface::v334() const {}
void CGameScriptInterface::v335() const {}
void CGameScriptInterface::v336() const {}
void CGameScriptInterface::v337() const {}
void CGameScriptInterface::v338() const {}
void CGameScriptInterface::v339() const {}
void CGameScriptInterface::v340() const {}
void CGameScriptInterface::v341() const {}
void CGameScriptInterface::v342() const {}
void CGameScriptInterface::v343() const {}
void CGameScriptInterface::v344() const {}
void CGameScriptInterface::v345() const {}
void CGameScriptInterface::v346() const {}
void CGameScriptInterface::v347() const {}
void CGameScriptInterface::v348() const {}
void CGameScriptInterface::v349() const {}
void CGameScriptInterface::v350() const {}
void CGameScriptInterface::v351() const {}
void CGameScriptInterface::v352() const {}
void CGameScriptInterface::v353() const {}
void CGameScriptInterface::v354() const {}
void CGameScriptInterface::v355() const {}
void CGameScriptInterface::v356() const {}
void CGameScriptInterface::v357() const {}
void CGameScriptInterface::v358() const {}
void CGameScriptInterface::v359() const {}
void CGameScriptInterface::v360() const {}
void CGameScriptInterface::v361() const {}
void CGameScriptInterface::v362() const {}
void CGameScriptInterface::v363() const {}
void CGameScriptInterface::v364() const {}
void CGameScriptInterface::v365() const {}
void CGameScriptInterface::v366() const {}
void CGameScriptInterface::v367() const {}
void CGameScriptInterface::v368() const {}
void CGameScriptInterface::v369() const {}
void CGameScriptInterface::v370() const {}
void CGameScriptInterface::v371() const {}
void CGameScriptInterface::v372() const {}
void CGameScriptInterface::v373() const {}
void CGameScriptInterface::v374() const {}
void CGameScriptInterface::v375() const {}
void CGameScriptInterface::v376() const {}
void CGameScriptInterface::v377() const {}
void CGameScriptInterface::v378() const {}
void CGameScriptInterface::v379() const {}
void CGameScriptInterface::v380() const {}
void CGameScriptInterface::v381() const {}
void CGameScriptInterface::v382() const {}
void CGameScriptInterface::v383() const {}
void CGameScriptInterface::v384() const {}
void CGameScriptInterface::v385() const {}
void CGameScriptInterface::v386() const {}
void CGameScriptInterface::v387() const {}
void CGameScriptInterface::v388() const {}
void CGameScriptInterface::v389() const {}
void CGameScriptInterface::v390() const {}
void CGameScriptInterface::v391() const {}
void CGameScriptInterface::v392() const {}
void CGameScriptInterface::v393() const {}
void CGameScriptInterface::v394() const {}
void CGameScriptInterface::v395() const {}
void CGameScriptInterface::v396() const {}
void CGameScriptInterface::v397() const {}
void CGameScriptInterface::v398() const {}
void CGameScriptInterface::v399() const {}
void CGameScriptInterface::v400() const {}
void CGameScriptInterface::v401() const {}
void CGameScriptInterface::v402() const {}
void CGameScriptInterface::v403() const {}
void CGameScriptInterface::v404() const {}
void CGameScriptInterface::v405() const {}
void CGameScriptInterface::v406() const {}
void CGameScriptInterface::v407() const {}
void CGameScriptInterface::v408() const {}
void CGameScriptInterface::v409() const {}
void CGameScriptInterface::v410() const {}
void CGameScriptInterface::v411() const {}
void CGameScriptInterface::v412() const {}
void CGameScriptInterface::v413() const {}
void CGameScriptInterface::v414() const {}
void CGameScriptInterface::v415() const {}
void CGameScriptInterface::v416() const {}
void CGameScriptInterface::v417() const {}
void CGameScriptInterface::v418() const {}
void CGameScriptInterface::v419() const {}
void CGameScriptInterface::v420() const {}
void CGameScriptInterface::v421() const {}
void CGameScriptInterface::v422() const {}
void CGameScriptInterface::v423() const {}
void CGameScriptInterface::v424() const {}
void CGameScriptInterface::v425() const {}
void CGameScriptInterface::v426() const {}
void CGameScriptInterface::v427() const {}
void CGameScriptInterface::v428() const {}
void CGameScriptInterface::v429() const {}
void CGameScriptInterface::v430() const {}
void CGameScriptInterface::v431() const {}
void CGameScriptInterface::v432() const {}
void CGameScriptInterface::v433() const {}
void CGameScriptInterface::v434() const {}
void CGameScriptInterface::v435() const {}
void CGameScriptInterface::v436() const {}
void CGameScriptInterface::v437() const {}
void CGameScriptInterface::v438() const {}
void CGameScriptInterface::v439() const {}
void CGameScriptInterface::v440() const {}
void CGameScriptInterface::v441() const {}
void CGameScriptInterface::v442() const {}
void CGameScriptInterface::v443() const {}
void CGameScriptInterface::v444() const {}
void CGameScriptInterface::v445() const {}
void CGameScriptInterface::v446() const {}
void CGameScriptInterface::v447() const {}
void CGameScriptInterface::v448() const {}
void CGameScriptInterface::v449() const {}
void CGameScriptInterface::v450() const {}
void CGameScriptInterface::v451() const {}
void CGameScriptInterface::v452() const {}
void CGameScriptInterface::v453() const {}
void CGameScriptInterface::v454() const {}
void CGameScriptInterface::v455() const {}
void CGameScriptInterface::v456() const {}
void CGameScriptInterface::v457() const {}
void CGameScriptInterface::v458() const {}
void CGameScriptInterface::v459() const {}
void CGameScriptInterface::v460() const {}
void CGameScriptInterface::v461() const {}
void CGameScriptInterface::v462() const {}
void CGameScriptInterface::v463() const {}
void CGameScriptInterface::v464() const {}
void CGameScriptInterface::v465() const {}
void CGameScriptInterface::v466() const {}
void CGameScriptInterface::v467() const {}
void CGameScriptInterface::v468() const {}
void CGameScriptInterface::v469() const {}
void CGameScriptInterface::v470() const {}
void CGameScriptInterface::v471() const {}
void CGameScriptInterface::v472() const {}
void CGameScriptInterface::v473() const {}
void CGameScriptInterface::v474() const {}
void CGameScriptInterface::v475() const {}
void CGameScriptInterface::v476() const {}
void CGameScriptInterface::v477() const {}
void CGameScriptInterface::v478() const {}
void CGameScriptInterface::v479() const {}
void CGameScriptInterface::v480() const {}
void CGameScriptInterface::v481() const {}
void CGameScriptInterface::v482() const {}
void CGameScriptInterface::v483() const {}
void CGameScriptInterface::v484() const {}
void CGameScriptInterface::v485() const {}
void CGameScriptInterface::v486() const {}
void CGameScriptInterface::v487() const {}
void CGameScriptInterface::v488() const {}
void CGameScriptInterface::v489() const {}
void CGameScriptInterface::v490() const {}
void CGameScriptInterface::v491() const {}
void CGameScriptInterface::v492() const {}
void CGameScriptInterface::v493() const {}
void CGameScriptInterface::v494() const {}
void CGameScriptInterface::v495() const {}
void CGameScriptInterface::v496() const {}
void CGameScriptInterface::v497() const {}
void CGameScriptInterface::v498() const {}
void CGameScriptInterface::v499() const {}
void CGameScriptInterface::v500() const {}
void CGameScriptInterface::v501() const {}
void CGameScriptInterface::v502() const {}
void CGameScriptInterface::v503() const {}
void CGameScriptInterface::v504() const {}
void CGameScriptInterface::v505() const {}
void CGameScriptInterface::v506() const {}
void CGameScriptInterface::v507() const {}
void CGameScriptInterface::v508() const {}
void CGameScriptInterface::v509() const {}
void CGameScriptInterface::v510() const {}
void CGameScriptInterface::v511() const {}
void CGameScriptInterface::v512() const {}
void CGameScriptInterface::v513() const {}
void CGameScriptInterface::v514() const {}
void CGameScriptInterface::v515() const {}
void CGameScriptInterface::v516() const {}
void CGameScriptInterface::v517() const {}
void CScriptThing::s000() const {}
void CScriptThing::s001() const {}
void CScriptThing::s002() const {}
void CScriptThing::s003() const {}
void CScriptThing::s004() const {}
void CScriptThing::s005() const {}
void CScriptThing::s006() const {}
void CScriptThing::s007() const {}
void CScriptThing::s008() const {}
void CScriptThing::s009() const {}
void CScriptThing::s010() const {}
void CScriptThing::s011() const {}
void CScriptThing::s012() const {}
void CScriptThing::s013() const {}
void CScriptThing::s014() const {}
void CScriptThing::s015() const {}
void CScriptThing::s016() const {}
void CScriptThing::s017() const {}
void CScriptThing::s018() const {}
void CScriptThing::s019() const {}

static int g_frames, g_hints, g_clicksNeeded, g_terminateAfter, g_lockCalls, g_live;
static bool g_locked;
static CQ_PrisonEscapeRescueMotherScript* g_parent;
static int g_rescueAt, g_unrescueAt;
CCharString::CCharString(const char* text, long length) : PStringData((void*)text) { ++g_live; }
CCharString::~CCharString() { --g_live; }
void CGameScriptInterface::NewScriptFrame() const
{
    ++g_frames;
    if (g_frames == g_rescueAt) g_parent->MotherRescued = true;
    if (g_frames == g_unrescueAt) g_parent->MotherRescued = false;
}
bool CGameScriptInterface::MsgIsGameInfoClickedPast() const { return g_clicksNeeded-- <= 0; }
void CGameScriptInterface::DisplayGameInfo(const CCharString& t) const
{ if (std::strcmp((const char*)t.PStringData, "TEXT_QST_055_HINT_ESCAPE_VIA_SECRET_PASSAGE") == 0) ++g_hints; }
void CGameScriptInterface::EntitySetAsLocked(const CScriptThing&, bool l) const { g_locked = l; ++g_lockCalls; }
bool CScriptThing::MsgIsUsedBy(const CCharString& who) const
{ return g_frames == 3 && std::strcmp((const char*)who.PStringData, "SCRIPT_NAME_HERO") == 0; }
bool CTortureDoorCourtyard::IsActiveThreadTerminating() { return g_terminateAfter >= 0 && g_frames >= g_terminateAfter; }
#define CHECK(c) if (!(c)) { std::printf("00dd8ba0_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    static unsigned char parentBytes[0x68];
    CQ_PrisonEscapeRescueMotherScript* parent = (CQ_PrisonEscapeRescueMotherScript*)parentBytes;
    g_parent = parent;
    static CGameScriptInterface gsi;
    static CTortureDoorCourtyard d;
    d.Game = &gsi; d.ParentClass = parent;
    // unlocked until frame 2 rescues the mother, locked, hero uses it on frame 3 (hint, one click wait), unrescued on 6
    g_rescueAt = 2; g_unrescueAt = 6; g_clicksNeeded = 1; g_terminateAfter = -1;
    d.Main();
    CHECK(g_locked && g_lockCalls == 2 && g_hints == 1 && g_frames == 6 && g_live == 0);
    // terminated while waiting for the rescue: never locked
    parent->MotherRescued = false; g_frames = 0; g_hints = 0; g_lockCalls = 0; g_rescueAt = 100; g_terminateAfter = 3;
    d.Main();
    CHECK(!g_locked && g_lockCalls == 1 && g_frames == 3 && g_hints == 0);
    // already rescued, terminated at the hero's use: locked, no hint
    parent->MotherRescued = true; g_frames = 0; g_lockCalls = 0; g_terminateAfter = 3; g_unrescueAt = 100;
    d.Main();
    CHECK(g_locked && g_lockCalls == 2 && g_frames == 3 && g_hints == 0 && g_live == 0);
    std::printf("00dd8ba0_TEST PASS\n");
    return 0;
}