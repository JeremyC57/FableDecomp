#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_baseDtors, g_deleted;
struct CCPPointerInfo {                            // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                ::operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseClass {                                // polymorphic root of the script handles
    CBaseClass() {}
    virtual ~CBaseClass() { ++g_baseDtors; }
};
struct CScriptThing : CBaseClass {                 // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;            // +0x04 Data, +0x08 Info
    ~CScriptThing() { }
};

struct CGameScriptInterface {
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0; virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0; virtual void v006() = 0;
    virtual void NewScriptFrame() = 0;
    virtual void v008() = 0; virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0; virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0; virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0; virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0; virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0; virtual void v027() = 0;
    virtual void v028() = 0; virtual void v029() = 0; virtual void v030() = 0; virtual void v031() = 0; virtual void v032() = 0; virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0; virtual void v036() = 0; virtual void v037() = 0;
    virtual void v038() = 0; virtual void v039() = 0; virtual void v040() = 0; virtual void v041() = 0; virtual void v042() = 0; virtual void v043() = 0; virtual void v044() = 0; virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0; virtual void v051() = 0; virtual void v052() = 0; virtual void v053() = 0; virtual void v054() = 0; virtual void v055() = 0; virtual void v056() = 0; virtual void v057() = 0;
    virtual void v058() = 0; virtual void v059() = 0; virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0; virtual void v063() = 0; virtual void v064() = 0; virtual void v065() = 0; virtual void v066() = 0; virtual void v067() = 0;
    virtual void v068() = 0; virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0; virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0; virtual void v075() = 0; virtual void v076() = 0; virtual void v077() = 0;
    virtual void v078() = 0; virtual void v079() = 0; virtual void v080() = 0; virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0; virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0; virtual void v087() = 0;
    virtual void v088() = 0; virtual void v089() = 0; virtual void v090() = 0; virtual void v091() = 0; virtual void v092() = 0; virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0; virtual void v096() = 0; virtual void v097() = 0;
    virtual void v098() = 0; virtual void v099() = 0; virtual void v100() = 0; virtual void v101() = 0; virtual void v102() = 0; virtual void v103() = 0; virtual void v104() = 0; virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
    virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0; virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0; virtual void v114() = 0; virtual void v115() = 0; virtual void v116() = 0; virtual void v117() = 0;
    virtual void v118() = 0; virtual void v119() = 0; virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0; virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0; virtual void v126() = 0; virtual void v127() = 0;
    virtual void v128() = 0; virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0; virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0; virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0;
    virtual void v138() = 0; virtual void v139() = 0; virtual void v140() = 0; virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0; virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0; virtual void v147() = 0;
    virtual void v148() = 0; virtual void v149() = 0; virtual void v150() = 0; virtual void v151() = 0; virtual void v152() = 0; virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0; virtual void v156() = 0; virtual void v157() = 0;
    virtual void v158() = 0; virtual void v159() = 0; virtual void v160() = 0; virtual void v161() = 0; virtual void v162() = 0; virtual void v163() = 0; virtual void v164() = 0; virtual void v165() = 0; virtual void v166() = 0; virtual void v167() = 0;
    virtual void v168() = 0; virtual void v169() = 0; virtual void v170() = 0; virtual void v171() = 0; virtual void v172() = 0; virtual void v173() = 0; virtual void v174() = 0; virtual void v175() = 0; virtual void v176() = 0; virtual void v177() = 0;
    virtual void v178() = 0; virtual void v179() = 0; virtual void v180() = 0; virtual void v181() = 0; virtual void v182() = 0; virtual void v183() = 0; virtual void v184() = 0; virtual void v185() = 0; virtual void v186() = 0; virtual void v187() = 0;
    virtual void v188() = 0; virtual void v189() = 0; virtual void v190() = 0; virtual void v191() = 0; virtual void v192() = 0; virtual void v193() = 0; virtual void v194() = 0; virtual void v195() = 0; virtual void v196() = 0; virtual void v197() = 0;
    virtual void v198() = 0; virtual void v199() = 0; virtual void v200() = 0; virtual void v201() = 0; virtual void v202() = 0; virtual void v203() = 0; virtual void v204() = 0; virtual void v205() = 0; virtual void v206() = 0; virtual void v207() = 0;
    virtual void v208() = 0; virtual void v209() = 0; virtual void v210() = 0; virtual void v211() = 0; virtual void v212() = 0; virtual void v213() = 0; virtual void v214() = 0; virtual void v215() = 0; virtual void v216() = 0; virtual void v217() = 0;
    virtual void v218() = 0; virtual void v219() = 0; virtual void v220() = 0; virtual void v221() = 0; virtual void v222() = 0; virtual void v223() = 0; virtual void v224() = 0; virtual void v225() = 0; virtual void v226() = 0; virtual void v227() = 0;
    virtual void v228() = 0; virtual void v229() = 0; virtual void v230() = 0; virtual void v231() = 0; virtual void v232() = 0; virtual void v233() = 0; virtual void v234() = 0; virtual void v235() = 0; virtual void v236() = 0; virtual void v237() = 0;
    virtual void v238() = 0; virtual void v239() = 0; virtual void v240() = 0; virtual void v241() = 0; virtual void v242() = 0; virtual void v243() = 0; virtual void v244() = 0; virtual void v245() = 0; virtual void v246() = 0; virtual void v247() = 0;
    virtual void v248() = 0; virtual void v249() = 0; virtual void v250() = 0; virtual void v251() = 0; virtual void v252() = 0; virtual void v253() = 0; virtual void v254() = 0; virtual void v255() = 0; virtual void v256() = 0; virtual void v257() = 0;
    virtual void v258() = 0; virtual void v259() = 0; virtual void v260() = 0; virtual void v261() = 0; virtual void v262() = 0; virtual void v263() = 0; virtual void v264() = 0; virtual void v265() = 0; virtual void v266() = 0; virtual void v267() = 0;
    virtual void v268() = 0; virtual void v269() = 0; virtual void v270() = 0; virtual void v271() = 0; virtual void v272() = 0; virtual void v273() = 0; virtual void v274() = 0; virtual void v275() = 0; virtual void v276() = 0; virtual void v277() = 0;
    virtual void v278() = 0; virtual void v279() = 0; virtual void v280() = 0; virtual void v281() = 0; virtual void v282() = 0; virtual void v283() = 0; virtual void v284() = 0; virtual void v285() = 0; virtual void v286() = 0; virtual void v287() = 0;
    virtual void v288() = 0; virtual void v289() = 0; virtual void v290() = 0; virtual void v291() = 0; virtual void v292() = 0; virtual void v293() = 0; virtual void v294() = 0; virtual void v295() = 0; virtual void v296() = 0; virtual void v297() = 0;
    virtual void v298() = 0; virtual void v299() = 0; virtual void v300() = 0; virtual void v301() = 0; virtual void v302() = 0; virtual void v303() = 0; virtual void v304() = 0; virtual void v305() = 0; virtual void v306() = 0; virtual void v307() = 0;
    virtual void v308() = 0; virtual void v309() = 0; virtual void v310() = 0; virtual void v311() = 0; virtual void v312() = 0; virtual void v313() = 0; virtual void v314() = 0; virtual void v315() = 0; virtual void v316() = 0; virtual void v317() = 0;
    virtual void v318() = 0; virtual void v319() = 0; virtual void v320() = 0; virtual void v321() = 0; virtual void v322() = 0; virtual void v323() = 0; virtual void v324() = 0; virtual void v325() = 0; virtual void v326() = 0; virtual void v327() = 0;
    virtual void v328() = 0; virtual void v329() = 0; virtual void v330() = 0; virtual void v331() = 0; virtual void v332() = 0; virtual void v333() = 0; virtual void v334() = 0; virtual void v335() = 0; virtual void v336() = 0; virtual void v337() = 0;
    virtual void v338() = 0; virtual void v339() = 0; virtual void v340() = 0; virtual void v341() = 0; virtual void v342() = 0; virtual void v343() = 0; virtual void v344() = 0; virtual void v345() = 0; virtual void v346() = 0; virtual void v347() = 0;
    virtual void v348() = 0; virtual void v349() = 0; virtual void v350() = 0; virtual void v351() = 0; virtual void v352() = 0; virtual void v353() = 0; virtual void v354() = 0; virtual void v355() = 0; virtual void v356() = 0; virtual void v357() = 0;
    virtual void v358() = 0; virtual void v359() = 0; virtual void v360() = 0; virtual void v361() = 0; virtual void v362() = 0; virtual void v363() = 0; virtual void v364() = 0; virtual void v365() = 0; virtual void v366() = 0; virtual void v367() = 0;
    virtual void v368() = 0; virtual void v369() = 0; virtual void v370() = 0; virtual void v371() = 0; virtual void v372() = 0; virtual void v373() = 0; virtual void v374() = 0; virtual void v375() = 0; virtual void v376() = 0; virtual void v377() = 0;
    virtual void v378() = 0; virtual void v379() = 0; virtual void v380() = 0; virtual void v381() = 0; virtual void v382() = 0; virtual void v383() = 0; virtual void v384() = 0; virtual void v385() = 0; virtual void v386() = 0; virtual void v387() = 0;
    virtual void v388() = 0; virtual void v389() = 0; virtual void v390() = 0; virtual void v391() = 0; virtual void v392() = 0; virtual void v393() = 0; virtual void v394() = 0; virtual void v395() = 0; virtual void v396() = 0; virtual void v397() = 0;
    virtual void v398() = 0; virtual void v399() = 0; virtual void v400() = 0; virtual void v401() = 0; virtual void v402() = 0; virtual void v403() = 0; virtual void v404() = 0; virtual void v405() = 0; virtual void v406() = 0; virtual void v407() = 0;
    virtual void v408() = 0; virtual void v409() = 0; virtual void v410() = 0; virtual void v411() = 0; virtual void v412() = 0; virtual void v413() = 0; virtual void v414() = 0; virtual void v415() = 0; virtual void v416() = 0; virtual void v417() = 0;
    virtual void v418() = 0; virtual void v419() = 0; virtual void v420() = 0; virtual void v421() = 0; virtual void v422() = 0; virtual void v423() = 0; virtual void v424() = 0; virtual void v425() = 0; virtual void v426() = 0; virtual void v427() = 0;
    virtual void v428() = 0; virtual void v429() = 0; virtual void v430() = 0; virtual void v431() = 0; virtual void v432() = 0; virtual void v433() = 0; virtual void v434() = 0; virtual void v435() = 0; virtual void v436() = 0; virtual void v437() = 0;
    virtual void v438() = 0; virtual void v439() = 0; virtual void v440() = 0; virtual void v441() = 0; virtual void v442() = 0; virtual void v443() = 0; virtual void v444() = 0; virtual void v445() = 0; virtual void v446() = 0; virtual void v447() = 0;
    virtual void v448() = 0; virtual void v449() = 0; virtual void v450() = 0; virtual void v451() = 0; virtual void v452() = 0; virtual void v453() = 0; virtual void v454() = 0; virtual void v455() = 0; virtual void v456() = 0; virtual void v457() = 0;
    virtual void v458() = 0; virtual void v459() = 0; virtual void v460() = 0; virtual void v461() = 0; virtual void v462() = 0; virtual void v463() = 0; virtual void v464() = 0; virtual void v465() = 0; virtual void v466() = 0; virtual void v467() = 0;
    virtual void v468() = 0; virtual void v469() = 0; virtual void v470() = 0; virtual void v471() = 0; virtual void v472() = 0; virtual void v473() = 0; virtual void v474() = 0; virtual void v475() = 0; virtual void v476() = 0; virtual void v477() = 0;
    virtual void v478() = 0; virtual void v479() = 0; virtual void v480() = 0; virtual void v481() = 0; virtual void v482() = 0; virtual void v483() = 0; virtual void v484() = 0; virtual void v485() = 0; virtual void v486() = 0; virtual void v487() = 0;
    virtual void v488() = 0; virtual void v489() = 0; virtual void v490() = 0; virtual void v491() = 0; virtual void v492() = 0; virtual void v493() = 0; virtual void v494() = 0; virtual void v495() = 0; virtual void v496() = 0; virtual void v497() = 0;
    virtual void v498() = 0; virtual void v499() = 0; virtual void v500() = 0; virtual void v501() = 0; virtual void v502() = 0; virtual void v503() = 0; virtual void v504() = 0; virtual void v505() = 0; virtual void v506() = 0; virtual void v507() = 0;
    virtual void v508() = 0; virtual void v509() = 0;
    virtual void EntitySetAlpha(const CScriptThing& thing, float alpha, bool fade) = 0;
    virtual void v511() = 0; virtual void v512() = 0; virtual void v513() = 0; virtual void v514() = 0; virtual void v515() = 0; virtual void v516() = 0; virtual void v517() = 0; virtual void v518() = 0; virtual void v519() = 0; virtual void v520() = 0;
    virtual void v521() = 0; virtual void v522() = 0; virtual void v523() = 0; virtual void v524() = 0; virtual void v525() = 0; virtual void v526() = 0; virtual void v527() = 0; virtual void v528() = 0; virtual void v529() = 0; virtual void v530() = 0;
    virtual void v531() = 0; virtual void v532() = 0; virtual void v533() = 0; virtual void v534() = 0; virtual void v535() = 0; virtual void v536() = 0; virtual void v537() = 0; virtual void v538() = 0; virtual void v539() = 0; virtual void v540() = 0;
    virtual void v541() = 0; virtual void v542() = 0; virtual void v543() = 0; virtual void v544() = 0; virtual void v545() = 0; virtual void v546() = 0; virtual void v547() = 0; virtual void v548() = 0; virtual void v549() = 0; virtual void v550() = 0;
    virtual void v551() = 0; virtual void v552() = 0; virtual void v553() = 0; virtual void v554() = 0; virtual void v555() = 0; virtual void v556() = 0; virtual void v557() = 0; virtual void v558() = 0; virtual void v559() = 0; virtual void v560() = 0;
    virtual void v561() = 0; virtual void v562() = 0; virtual void v563() = 0; virtual void v564() = 0; virtual void v565() = 0; virtual void v566() = 0; virtual void v567() = 0; virtual void v568() = 0; virtual void v569() = 0; virtual void v570() = 0;
    virtual void v571() = 0; virtual void v572() = 0; virtual void v573() = 0; virtual void v574() = 0; virtual void v575() = 0; virtual void v576() = 0; virtual void v577() = 0; virtual void v578() = 0; virtual void v579() = 0; virtual void v580() = 0;
    virtual void v581() = 0; virtual void v582() = 0; virtual void v583() = 0; virtual void v584() = 0; virtual void v585() = 0; virtual void v586() = 0; virtual void v587() = 0; virtual void v588() = 0; virtual void v589() = 0; virtual void v590() = 0;
    virtual void v591() = 0; virtual void v592() = 0; virtual void v593() = 0; virtual void v594() = 0; virtual void v595() = 0; virtual void v596() = 0; virtual void v597() = 0; virtual void v598() = 0; virtual void v599() = 0; virtual void v600() = 0;
    virtual void v601() = 0; virtual void v602() = 0; virtual void v603() = 0; virtual void v604() = 0; virtual void v605() = 0; virtual void v606() = 0; virtual void v607() = 0; virtual void v608() = 0; virtual void v609() = 0; virtual void v610() = 0;
    virtual void v611() = 0; virtual void v612() = 0; virtual void v613() = 0; virtual void v614() = 0; virtual void v615() = 0; virtual void v616() = 0; virtual void v617() = 0; virtual void v618() = 0; virtual void v619() = 0; virtual void v620() = 0;
    virtual void v621() = 0; virtual void v622() = 0; virtual void v623() = 0; virtual void v624() = 0; virtual void v625() = 0; virtual void v626() = 0; virtual void v627() = 0; virtual void v628() = 0; virtual void v629() = 0; virtual void v630() = 0;
    virtual void v631() = 0; virtual void v632() = 0; virtual void v633() = 0; virtual void v634() = 0; virtual void v635() = 0; virtual void v636() = 0; virtual void v637() = 0; virtual void v638() = 0; virtual void v639() = 0; virtual void v640() = 0;
    virtual void v641() = 0; virtual void v642() = 0; virtual void v643() = 0; virtual void v644() = 0; virtual void v645() = 0; virtual void v646() = 0; virtual void v647() = 0; virtual void v648() = 0; virtual void v649() = 0; virtual void v650() = 0;
    virtual void v651() = 0; virtual void v652() = 0; virtual void v653() = 0;
    virtual long GetConstantFPS() = 0;
};

struct CScriptBase {
    unsigned char         _pad00[0x40];
    CGameScriptInterface* GSI;                     // +0x40 (the quest script's interface pointer)
    int framesLeft;
    bool IsActiveThreadTerminating() { return framesLeft-- <= 0; }
};

namespace NScript {
struct CPersonalScript_EmptyScript : CScriptBase {
    void ChangeAlpha(CScriptThing thing);
};

void CPersonalScript_EmptyScript::ChangeAlpha(CScriptThing thing)
{
    float alpha = 1.0f;
    float step = -0.5f / (float)GSI->GetConstantFPS();
    while (!IsActiveThreadTerminating()) {
        alpha += step;
        if (alpha < 0.0f) {
            alpha = 0.0f;
            step = -step;
        }
        else if (alpha > 1.0f) {
            alpha = 1.0f;
            step = -step;
        }
        GSI->EntitySetAlpha(thing, alpha, false);
        GSI->NewScriptFrame();
    }
}
}

struct CStubGSI : CGameScriptInterface {
    void v000() {} void v001() {} void v002() {} void v003() {} void v004() {} void v005() {} void v006() {} void v008() {} void v009() {} void v010() {} void v011() {} void v012() {} void v013() {} void v014() {} void v015() {} void v016() {} void v017() {} void v018() {} void v019() {} void v020() {} void v021() {} void v022() {} void v023() {} void v024() {} void v025() {} void v026() {} void v027() {} void v028() {} void v029() {} void v030() {} void v031() {} void v032() {} void v033() {} void v034() {} void v035() {} void v036() {} void v037() {} void v038() {} void v039() {} void v040() {} void v041() {} void v042() {} void v043() {} void v044() {} void v045() {} void v046() {} void v047() {} void v048() {} void v049() {} void v050() {} void v051() {} void v052() {} void v053() {} void v054() {} void v055() {} void v056() {} void v057() {} void v058() {} void v059() {} void v060() {} void v061() {} void v062() {} void v063() {} void v064() {} void v065() {} void v066() {} void v067() {} void v068() {} void v069() {} void v070() {} void v071() {} void v072() {} void v073() {} void v074() {} void v075() {} void v076() {} void v077() {} void v078() {} void v079() {} void v080() {} void v081() {} void v082() {} void v083() {} void v084() {} void v085() {} void v086() {} void v087() {} void v088() {} void v089() {} void v090() {} void v091() {} void v092() {} void v093() {} void v094() {} void v095() {} void v096() {} void v097() {} void v098() {} void v099() {} void v100() {} void v101() {} void v102() {} void v103() {} void v104() {} void v105() {} void v106() {} void v107() {} void v108() {} void v109() {} void v110() {} void v111() {} void v112() {} void v113() {} void v114() {} void v115() {} void v116() {} void v117() {} void v118() {} void v119() {} void v120() {} void v121() {} void v122() {} void v123() {} void v124() {} void v125() {} void v126() {} void v127() {} void v128() {} void v129() {} void v130() {} void v131() {} void v132() {} void v133() {} void v134() {} void v135() {} void v136() {} void v137() {} void v138() {} void v139() {} void v140() {} void v141() {} void v142() {} void v143() {} void v144() {} void v145() {} void v146() {} void v147() {} void v148() {} void v149() {} void v150() {} void v151() {} void v152() {} void v153() {} void v154() {} void v155() {} void v156() {} void v157() {} void v158() {} void v159() {} void v160() {} void v161() {} void v162() {} void v163() {} void v164() {} void v165() {} void v166() {} void v167() {} void v168() {} void v169() {} void v170() {} void v171() {} void v172() {} void v173() {} void v174() {} void v175() {} void v176() {} void v177() {} void v178() {} void v179() {} void v180() {} void v181() {} void v182() {} void v183() {} void v184() {} void v185() {} void v186() {} void v187() {} void v188() {} void v189() {} void v190() {} void v191() {} void v192() {} void v193() {} void v194() {} void v195() {} void v196() {} void v197() {} void v198() {} void v199() {} void v200() {} void v201() {} void v202() {} void v203() {} void v204() {} void v205() {} void v206() {} void v207() {} void v208() {} void v209() {} void v210() {} void v211() {} void v212() {} void v213() {} void v214() {} void v215() {} void v216() {} void v217() {} void v218() {} void v219() {} void v220() {} void v221() {} void v222() {} void v223() {} void v224() {} void v225() {} void v226() {} void v227() {} void v228() {} void v229() {} void v230() {} void v231() {} void v232() {} void v233() {} void v234() {} void v235() {} void v236() {} void v237() {} void v238() {} void v239() {} void v240() {} void v241() {} void v242() {} void v243() {} void v244() {} void v245() {} void v246() {} void v247() {} void v248() {} void v249() {} void v250() {} void v251() {} void v252() {} void v253() {} void v254() {} void v255() {} void v256() {} void v257() {} void v258() {} void v259() {} void v260() {} void v261() {} void v262() {} void v263() {} void v264() {} void v265() {} void v266() {} void v267() {} void v268() {} void v269() {} void v270() {} void v271() {} void v272() {} void v273() {} void v274() {} void v275() {} void v276() {} void v277() {} void v278() {} void v279() {} void v280() {} void v281() {} void v282() {} void v283() {} void v284() {} void v285() {} void v286() {} void v287() {} void v288() {} void v289() {} void v290() {} void v291() {} void v292() {} void v293() {} void v294() {} void v295() {} void v296() {} void v297() {} void v298() {} void v299() {} void v300() {} void v301() {} void v302() {} void v303() {} void v304() {} void v305() {} void v306() {} void v307() {} void v308() {} void v309() {} void v310() {} void v311() {} void v312() {} void v313() {} void v314() {} void v315() {} void v316() {} void v317() {} void v318() {} void v319() {} void v320() {} void v321() {} void v322() {} void v323() {} void v324() {} void v325() {} void v326() {} void v327() {} void v328() {} void v329() {} void v330() {} void v331() {} void v332() {} void v333() {} void v334() {} void v335() {} void v336() {} void v337() {} void v338() {} void v339() {} void v340() {} void v341() {} void v342() {} void v343() {} void v344() {} void v345() {} void v346() {} void v347() {} void v348() {} void v349() {} void v350() {} void v351() {} void v352() {} void v353() {} void v354() {} void v355() {} void v356() {} void v357() {} void v358() {} void v359() {} void v360() {} void v361() {} void v362() {} void v363() {} void v364() {} void v365() {} void v366() {} void v367() {} void v368() {} void v369() {} void v370() {} void v371() {} void v372() {} void v373() {} void v374() {} void v375() {} void v376() {} void v377() {} void v378() {} void v379() {} void v380() {} void v381() {} void v382() {} void v383() {} void v384() {} void v385() {} void v386() {} void v387() {} void v388() {} void v389() {} void v390() {} void v391() {} void v392() {} void v393() {} void v394() {} void v395() {} void v396() {} void v397() {} void v398() {} void v399() {} void v400() {} void v401() {} void v402() {} void v403() {} void v404() {} void v405() {} void v406() {} void v407() {} void v408() {} void v409() {} void v410() {} void v411() {} void v412() {} void v413() {} void v414() {} void v415() {} void v416() {} void v417() {} void v418() {} void v419() {} void v420() {} void v421() {} void v422() {} void v423() {} void v424() {} void v425() {} void v426() {} void v427() {} void v428() {} void v429() {} void v430() {} void v431() {} void v432() {} void v433() {} void v434() {} void v435() {} void v436() {} void v437() {} void v438() {} void v439() {} void v440() {} void v441() {} void v442() {} void v443() {} void v444() {} void v445() {} void v446() {} void v447() {} void v448() {} void v449() {} void v450() {} void v451() {} void v452() {} void v453() {} void v454() {} void v455() {} void v456() {} void v457() {} void v458() {} void v459() {} void v460() {} void v461() {} void v462() {} void v463() {} void v464() {} void v465() {} void v466() {} void v467() {} void v468() {} void v469() {} void v470() {} void v471() {} void v472() {} void v473() {} void v474() {} void v475() {} void v476() {} void v477() {} void v478() {} void v479() {} void v480() {} void v481() {} void v482() {} void v483() {} void v484() {} void v485() {} void v486() {} void v487() {} void v488() {} void v489() {} void v490() {} void v491() {} void v492() {} void v493() {} void v494() {} void v495() {} void v496() {} void v497() {} void v498() {} void v499() {} void v500() {} void v501() {} void v502() {} void v503() {} void v504() {} void v505() {} void v506() {} void v507() {} void v508() {} void v509() {} void v511() {} void v512() {} void v513() {} void v514() {} void v515() {} void v516() {} void v517() {} void v518() {} void v519() {} void v520() {} void v521() {} void v522() {} void v523() {} void v524() {} void v525() {} void v526() {} void v527() {} void v528() {} void v529() {} void v530() {} void v531() {} void v532() {} void v533() {} void v534() {} void v535() {} void v536() {} void v537() {} void v538() {} void v539() {} void v540() {} void v541() {} void v542() {} void v543() {} void v544() {} void v545() {} void v546() {} void v547() {} void v548() {} void v549() {} void v550() {} void v551() {} void v552() {} void v553() {} void v554() {} void v555() {} void v556() {} void v557() {} void v558() {} void v559() {} void v560() {} void v561() {} void v562() {} void v563() {} void v564() {} void v565() {} void v566() {} void v567() {} void v568() {} void v569() {} void v570() {} void v571() {} void v572() {} void v573() {} void v574() {} void v575() {} void v576() {} void v577() {} void v578() {} void v579() {} void v580() {} void v581() {} void v582() {} void v583() {} void v584() {} void v585() {} void v586() {} void v587() {} void v588() {} void v589() {} void v590() {} void v591() {} void v592() {} void v593() {} void v594() {} void v595() {} void v596() {} void v597() {} void v598() {} void v599() {} void v600() {} void v601() {} void v602() {} void v603() {} void v604() {} void v605() {} void v606() {} void v607() {} void v608() {} void v609() {} void v610() {} void v611() {} void v612() {} void v613() {} void v614() {} void v615() {} void v616() {} void v617() {} void v618() {} void v619() {} void v620() {} void v621() {} void v622() {} void v623() {} void v624() {} void v625() {} void v626() {} void v627() {} void v628() {} void v629() {} void v630() {} void v631() {} void v632() {} void v633() {} void v634() {} void v635() {} void v636() {} void v637() {} void v638() {} void v639() {} void v640() {} void v641() {} void v642() {} void v643() {} void v644() {} void v645() {} void v646() {} void v647() {} void v648() {} void v649() {} void v650() {} void v651() {} void v652() {} void v653() {}
    int frames, sets; float alphas[16]; const CScriptThing* lastThing; bool lastFade;
    void NewScriptFrame() { ++frames; }
    void EntitySetAlpha(const CScriptThing& thing, float alpha, bool fade) { if (sets < 16) alphas[sets] = alpha; ++sets; lastThing = &thing; lastFade = fade; }
    long GetConstantFPS() { return 4; }
};
static void __fastcall DeleteData(void* data) { (void)data; ++g_deleted; }
using namespace NScript;
int main()
{
    CStubGSI gsi; gsi.frames = gsi.sets = 0; gsi.lastThing = 0; gsi.lastFade = true;
    static CPersonalScript_EmptyScript script; script.GSI = &gsi; script.framesLeft = 6;
    CCPPointerInfo* info = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo));
    info->RefCount = 1; info->DeleteFunc = DeleteData; info->Data = 0;
    {
        CScriptThing t; t.PImp.Data = 0; t.PImp.Info = info;
        /* pass by value without a copy constructor: model the callee-owned argument by handing the bits over */
        script.ChangeAlpha(t);
        t.PImp.Info = 0; /* the callee released the shared block */
    }
    /* step = -0.5/4 = -0.125: 1 -> .875 .75 .625 .5 .375 .25 */
    const float want[6] = { 0.875f, 0.75f, 0.625f, 0.5f, 0.375f, 0.25f };
    if (gsi.sets != 6 || gsi.frames != 6 || gsi.lastFade != false) { std::printf("00ef8bc0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (gsi.alphas[i] != want[i]) { std::printf("00ef8bc0_TEST FAIL\n"); return 1; }
    if (g_deleted != 1) { std::printf("00ef8bc0_TEST FAIL\n"); return 1; }
    /* the bounce: start below zero after 9 steps -> clamp at 0 and reverse */
    gsi.sets = 0; gsi.frames = 0; script.framesLeft = 10;
    { CScriptThing t; t.PImp.Data = 0; t.PImp.Info = 0; script.ChangeAlpha(t); }
    if (gsi.sets != 10 || gsi.alphas[7] != 0.0f || gsi.alphas[8] != 0.0f || gsi.alphas[9] != 0.125f) { std::printf("00ef8bc0_TEST FAIL\n"); return 1; }
    std::printf("00ef8bc0_TEST PASS\n");
    return 0;
}