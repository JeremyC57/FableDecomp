// CUndeadSoldier::Main() -- undead soldier entity script: alive filter on Me, then invulnerable and hostile until the hero comes within 15 units, then damageable for the rest of the script
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D88F90, length 265 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840 (the CIsThingAlive filter, vtable
// 0x012C32F0), the filter registration @ 0x00F35B10, IsActiveThreadTerminating @ 0x00F35B30, CGameScriptInterface slots 7
// NewScriptFrame (+0x1c), 516 EntitySetAsDamageable (+0x810), 601 EntitySetInFaction (+0x964), 70 GetHero (+0x118),
// IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF (`__fastcall`),
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0.
// Strings: 0x012D4F1C "FACTION_GUARDS_ENEMY".
// Layout: the entity-script base {vptr, Game +4, Me +8} (the landed CSummonerToKill::Main 0x00D83DC0 model).
// Shape: the scoped filter object of the landed entity-script Mains; the faction name is an implicit conversion; the
// distance test's float is pushed before GetHero() is evaluated (right-to-left); the final wait is an endless frame loop
// that only leaves when the thread terminates.
namespace NScript { class CPredicateBase; }
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing { public:                                    // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
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
    virtual void v038() const; virtual void v039() const; virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const;
    virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const; virtual void v049() const;
    virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const;
    virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const;
    virtual void v062() const; virtual void v063() const; virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const;
    virtual void v068() const; virtual void v069() const;
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const;
    virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const;
    virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const;
    virtual void v089() const; virtual void v090() const; virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const;
    virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const; virtual void v100() const;
    virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const;
    virtual void v107() const; virtual void v108() const; virtual void v109() const; virtual void v110() const; virtual void v111() const; virtual void v112() const;
    virtual void v113() const; virtual void v114() const; virtual void v115() const; virtual void v116() const; virtual void v117() const; virtual void v118() const;
    virtual void v119() const; virtual void v120() const; virtual void v121() const; virtual void v122() const; virtual void v123() const; virtual void v124() const;
    virtual void v125() const; virtual void v126() const; virtual void v127() const; virtual void v128() const; virtual void v129() const; virtual void v130() const;
    virtual void v131() const; virtual void v132() const; virtual void v133() const; virtual void v134() const; virtual void v135() const; virtual void v136() const;
    virtual void v137() const; virtual void v138() const; virtual void v139() const; virtual void v140() const; virtual void v141() const; virtual void v142() const;
    virtual void v143() const; virtual void v144() const; virtual void v145() const; virtual void v146() const; virtual void v147() const; virtual void v148() const;
    virtual void v149() const; virtual void v150() const; virtual void v151() const; virtual void v152() const; virtual void v153() const; virtual void v154() const;
    virtual void v155() const; virtual void v156() const; virtual void v157() const; virtual void v158() const; virtual void v159() const; virtual void v160() const;
    virtual void v161() const; virtual void v162() const; virtual void v163() const; virtual void v164() const; virtual void v165() const; virtual void v166() const;
    virtual void v167() const; virtual void v168() const; virtual void v169() const; virtual void v170() const; virtual void v171() const; virtual void v172() const;
    virtual void v173() const; virtual void v174() const; virtual void v175() const; virtual void v176() const; virtual void v177() const; virtual void v178() const;
    virtual void v179() const; virtual void v180() const; virtual void v181() const; virtual void v182() const; virtual void v183() const; virtual void v184() const;
    virtual void v185() const; virtual void v186() const; virtual void v187() const; virtual void v188() const; virtual void v189() const; virtual void v190() const;
    virtual void v191() const; virtual void v192() const; virtual void v193() const; virtual void v194() const; virtual void v195() const; virtual void v196() const;
    virtual void v197() const; virtual void v198() const; virtual void v199() const; virtual void v200() const; virtual void v201() const; virtual void v202() const;
    virtual void v203() const; virtual void v204() const; virtual void v205() const; virtual void v206() const; virtual void v207() const; virtual void v208() const;
    virtual void v209() const; virtual void v210() const; virtual void v211() const; virtual void v212() const; virtual void v213() const; virtual void v214() const;
    virtual void v215() const; virtual void v216() const; virtual void v217() const; virtual void v218() const; virtual void v219() const; virtual void v220() const;
    virtual void v221() const; virtual void v222() const; virtual void v223() const; virtual void v224() const; virtual void v225() const; virtual void v226() const;
    virtual void v227() const; virtual void v228() const; virtual void v229() const; virtual void v230() const; virtual void v231() const; virtual void v232() const;
    virtual void v233() const; virtual void v234() const; virtual void v235() const; virtual void v236() const; virtual void v237() const; virtual void v238() const;
    virtual void v239() const; virtual void v240() const; virtual void v241() const; virtual void v242() const; virtual void v243() const; virtual void v244() const;
    virtual void v245() const; virtual void v246() const; virtual void v247() const; virtual void v248() const; virtual void v249() const; virtual void v250() const;
    virtual void v251() const; virtual void v252() const; virtual void v253() const; virtual void v254() const; virtual void v255() const; virtual void v256() const;
    virtual void v257() const; virtual void v258() const; virtual void v259() const; virtual void v260() const; virtual void v261() const; virtual void v262() const;
    virtual void v263() const; virtual void v264() const; virtual void v265() const; virtual void v266() const; virtual void v267() const; virtual void v268() const;
    virtual void v269() const; virtual void v270() const; virtual void v271() const; virtual void v272() const; virtual void v273() const; virtual void v274() const;
    virtual void v275() const; virtual void v276() const; virtual void v277() const; virtual void v278() const; virtual void v279() const; virtual void v280() const;
    virtual void v281() const; virtual void v282() const; virtual void v283() const; virtual void v284() const; virtual void v285() const; virtual void v286() const;
    virtual void v287() const; virtual void v288() const; virtual void v289() const; virtual void v290() const; virtual void v291() const; virtual void v292() const;
    virtual void v293() const; virtual void v294() const; virtual void v295() const; virtual void v296() const; virtual void v297() const; virtual void v298() const;
    virtual void v299() const; virtual void v300() const; virtual void v301() const; virtual void v302() const; virtual void v303() const; virtual void v304() const;
    virtual void v305() const; virtual void v306() const; virtual void v307() const; virtual void v308() const; virtual void v309() const; virtual void v310() const;
    virtual void v311() const; virtual void v312() const; virtual void v313() const; virtual void v314() const; virtual void v315() const; virtual void v316() const;
    virtual void v317() const; virtual void v318() const; virtual void v319() const; virtual void v320() const; virtual void v321() const; virtual void v322() const;
    virtual void v323() const; virtual void v324() const; virtual void v325() const; virtual void v326() const; virtual void v327() const; virtual void v328() const;
    virtual void v329() const; virtual void v330() const; virtual void v331() const; virtual void v332() const; virtual void v333() const; virtual void v334() const;
    virtual void v335() const; virtual void v336() const; virtual void v337() const; virtual void v338() const; virtual void v339() const; virtual void v340() const;
    virtual void v341() const; virtual void v342() const; virtual void v343() const; virtual void v344() const; virtual void v345() const; virtual void v346() const;
    virtual void v347() const; virtual void v348() const; virtual void v349() const; virtual void v350() const; virtual void v351() const; virtual void v352() const;
    virtual void v353() const; virtual void v354() const; virtual void v355() const; virtual void v356() const; virtual void v357() const; virtual void v358() const;
    virtual void v359() const; virtual void v360() const; virtual void v361() const; virtual void v362() const; virtual void v363() const; virtual void v364() const;
    virtual void v365() const; virtual void v366() const; virtual void v367() const; virtual void v368() const; virtual void v369() const; virtual void v370() const;
    virtual void v371() const; virtual void v372() const; virtual void v373() const; virtual void v374() const; virtual void v375() const; virtual void v376() const;
    virtual void v377() const; virtual void v378() const; virtual void v379() const; virtual void v380() const; virtual void v381() const; virtual void v382() const;
    virtual void v383() const; virtual void v384() const; virtual void v385() const; virtual void v386() const; virtual void v387() const; virtual void v388() const;
    virtual void v389() const; virtual void v390() const; virtual void v391() const; virtual void v392() const; virtual void v393() const; virtual void v394() const;
    virtual void v395() const; virtual void v396() const; virtual void v397() const; virtual void v398() const; virtual void v399() const; virtual void v400() const;
    virtual void v401() const; virtual void v402() const; virtual void v403() const; virtual void v404() const; virtual void v405() const; virtual void v406() const;
    virtual void v407() const; virtual void v408() const; virtual void v409() const; virtual void v410() const; virtual void v411() const; virtual void v412() const;
    virtual void v413() const; virtual void v414() const; virtual void v415() const; virtual void v416() const; virtual void v417() const; virtual void v418() const;
    virtual void v419() const; virtual void v420() const; virtual void v421() const; virtual void v422() const; virtual void v423() const; virtual void v424() const;
    virtual void v425() const; virtual void v426() const; virtual void v427() const; virtual void v428() const; virtual void v429() const; virtual void v430() const;
    virtual void v431() const; virtual void v432() const; virtual void v433() const; virtual void v434() const; virtual void v435() const; virtual void v436() const;
    virtual void v437() const; virtual void v438() const; virtual void v439() const; virtual void v440() const; virtual void v441() const; virtual void v442() const;
    virtual void v443() const; virtual void v444() const; virtual void v445() const; virtual void v446() const; virtual void v447() const; virtual void v448() const;
    virtual void v449() const; virtual void v450() const; virtual void v451() const; virtual void v452() const; virtual void v453() const; virtual void v454() const;
    virtual void v455() const; virtual void v456() const; virtual void v457() const; virtual void v458() const; virtual void v459() const; virtual void v460() const;
    virtual void v461() const; virtual void v462() const; virtual void v463() const; virtual void v464() const; virtual void v465() const; virtual void v466() const;
    virtual void v467() const; virtual void v468() const; virtual void v469() const; virtual void v470() const; virtual void v471() const; virtual void v472() const;
    virtual void v473() const; virtual void v474() const; virtual void v475() const; virtual void v476() const; virtual void v477() const; virtual void v478() const;
    virtual void v479() const; virtual void v480() const; virtual void v481() const; virtual void v482() const; virtual void v483() const; virtual void v484() const;
    virtual void v485() const; virtual void v486() const; virtual void v487() const; virtual void v488() const; virtual void v489() const; virtual void v490() const;
    virtual void v491() const; virtual void v492() const; virtual void v493() const; virtual void v494() const; virtual void v495() const; virtual void v496() const;
    virtual void v497() const; virtual void v498() const; virtual void v499() const; virtual void v500() const; virtual void v501() const; virtual void v502() const;
    virtual void v503() const; virtual void v504() const; virtual void v505() const; virtual void v506() const; virtual void v507() const; virtual void v508() const;
    virtual void v509() const; virtual void v510() const; virtual void v511() const; virtual void v512() const; virtual void v513() const; virtual void v514() const;
    virtual void v515() const;
    virtual void EntitySetAsDamageable(const CScriptThing& thing, bool damageable) const;   // slot 516 (+0x810)
    virtual void v517() const; virtual void v518() const; virtual void v519() const; virtual void v520() const; virtual void v521() const; virtual void v522() const;
    virtual void v523() const; virtual void v524() const; virtual void v525() const; virtual void v526() const; virtual void v527() const; virtual void v528() const;
    virtual void v529() const; virtual void v530() const; virtual void v531() const; virtual void v532() const; virtual void v533() const; virtual void v534() const;
    virtual void v535() const; virtual void v536() const; virtual void v537() const; virtual void v538() const; virtual void v539() const; virtual void v540() const;
    virtual void v541() const; virtual void v542() const; virtual void v543() const; virtual void v544() const; virtual void v545() const; virtual void v546() const;
    virtual void v547() const; virtual void v548() const; virtual void v549() const; virtual void v550() const; virtual void v551() const; virtual void v552() const;
    virtual void v553() const; virtual void v554() const; virtual void v555() const; virtual void v556() const; virtual void v557() const; virtual void v558() const;
    virtual void v559() const; virtual void v560() const; virtual void v561() const; virtual void v562() const; virtual void v563() const; virtual void v564() const;
    virtual void v565() const; virtual void v566() const; virtual void v567() const; virtual void v568() const; virtual void v569() const; virtual void v570() const;
    virtual void v571() const; virtual void v572() const; virtual void v573() const; virtual void v574() const; virtual void v575() const; virtual void v576() const;
    virtual void v577() const; virtual void v578() const; virtual void v579() const; virtual void v580() const; virtual void v581() const; virtual void v582() const;
    virtual void v583() const; virtual void v584() const; virtual void v585() const; virtual void v586() const; virtual void v587() const; virtual void v588() const;
    virtual void v589() const; virtual void v590() const; virtual void v591() const; virtual void v592() const; virtual void v593() const; virtual void v594() const;
    virtual void v595() const; virtual void v596() const; virtual void v597() const; virtual void v598() const; virtual void v599() const; virtual void v600() const;
    virtual void EntitySetInFaction(const CScriptThing& thing, const CCharString& faction) const;   // slot 601 (+0x964)
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingAlive : public NScript::CPredicateBase {                                   // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CUndeadSoldier : public CEntityScriptBase {
public:
    void*                 __vftable;                     // +0x00
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    void Main();
};

void CUndeadSoldier::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_GUARDS_ENEMY");
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsDamageable(Me, true);
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}