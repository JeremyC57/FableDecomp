// Behaviour test for 00cf0180: src/00cf0180.cpp copied verbatim; while fewer than the maximum warnings were given, each frame
// (hero player-controlled, tutorials on) counts the cooldown down and, once it has run out and the will energy is below the
// threshold, sends the guild-master warning and re-arms the cooldown.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NScript::CGameflowAssistanceScript::Tutorial_LowMagic() -- tutorial thread: while fewer than the def's maximum low-will warnings were given, each script frame (while the hero is player controlled) warn the hero through the guild master when his will energy drops below the def's threshold, then wait the def's cooldown before warning again
// Retry 10b (statement-permutation sweep): the warning count is incremented AFTER the cooldown is re-armed from the def;
// that order gives retail's registers (cooldown in edi, the block-local def in ebx pushed at entry).
// Leaf: Tutorial_LowMagic
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CF0180, length 282 bytes (+ int3 padding), plain `ret`.
// Label correction: the catalog says Tutorial_LowHealth; the spawned-thread registration at 0x00CEFBD0 stores this
//   address under the name "Tutorial_LowMagic" (0x012C5E30), and the body tests the will energy (Ego_r.pdb
//   ?Tutorial_LowMagic@CGameflowAssistanceScript@NScript@@QAEXXZ).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (the last call's result unused), the game script interface
//   (+0x40) slots 7 NewScriptFrame, 394 IsHeroControlledByPlayer, 180 GetHeroWillEnergyLevel, 274
//   HeroReceiveMessageFromGuildMaster (cgsi_slots_aligned.tsv); CCharString(const char*, long) @ 0x0099EBF0
//   ("TEXT_QST_080_WILL_LOW" @ 0x012C5EF8, "" @ 0x0122D70E) / ~CCharString @ 0x0099EAE0.
// Globals: the script def @ 0x0143E90C (low-will threshold +0xff4, warning cooldown in frames +0xff8, maximum warnings
//   +0xffc).
// Layout: retail quest base (0x14 below Ego_r.pdb): GSI +0x40, master data +0x44 (its tutorials-off flag +0x67),
//   WillEnergyWarningsGiven +0x5c (PDB +0x70).
// Shape: a rotated `while` over the warning count with the cooldown counter in edi; the def is read into a block-local
//   pointer before the will-energy call (kept in ebx for the compare); both message strings are implicit
//   conversion temporaries of the guild-master call (built before the pushes, destroyed after it).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CGameScriptInterface_Calls {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
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
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual float GetHeroWillEnergyLevel() const;   // slot 180 (+0x2d0)
    virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188();
    virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196();
    virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204();
    virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212();
    virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220();
    virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228();
    virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236();
    virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244();
    virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252();
    virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260();
    virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268();
    virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273();
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString& speaker, bool important, bool immediate) const;   // slot 274 (+0x448)
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282();
    virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290();
    virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298();
    virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306();
    virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322();
    virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330();
    virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338();
    virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346();
    virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362();
    virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370();
    virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378();
    virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386();
    virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393();
    virtual bool IsHeroControlledByPlayer() const;   // slot 394 (+0x628)
};
class CScriptDef { public:
    unsigned char _pad000[0xff4];
    float LowWillEnergyThreshold;                       // +0xff4
    long  WillEnergyWarningCooldown;                    // +0xff8
    long  MaxWillEnergyWarnings;                        // +0xffc
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CQuestMasterData {
    unsigned char _pad00[0x67];
    bool TutorialsDisabled;                             // +0x67
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
class CGameflowAssistanceScript : public CScriptBase { public:
    CGameScriptInterface_Calls* GSI;                    // +0x40
    CQuestMasterData*           PMasterData;            // +0x44
    unsigned char               _pad48[0x5c - 0x48];
    long                        WillEnergyWarningsGiven;   // +0x5c
    void Tutorial_LowMagic();
};

void CGameflowAssistanceScript::Tutorial_LowMagic()
{
    long cooldown = 0;
    while (WillEnergyWarningsGiven < g_pScriptDef->MaxWillEnergyWarnings) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (PMasterData->TutorialsDisabled)
            break;
        if (GSI->IsHeroControlledByPlayer()) {
            if (IsActiveThreadTerminating())
                return;
            if (cooldown <= 0) {
                CScriptDef* def = g_pScriptDef;
                if (GSI->GetHeroWillEnergyLevel() < def->LowWillEnergyThreshold) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_WILL_LOW", "", true, false);
                cooldown = g_pScriptDef->WillEnergyWarningCooldown;
                ++WillEnergyWarningsGiven;
                }
            }
            --cooldown;
        }
    }
    IsActiveThreadTerminating();
}
}
// ---- end source ----
void CGameScriptInterface_Calls::v000() {} void CGameScriptInterface_Calls::v001() {} void CGameScriptInterface_Calls::v002() {} void CGameScriptInterface_Calls::v003() {} void CGameScriptInterface_Calls::v004() {} void CGameScriptInterface_Calls::v005() {} void CGameScriptInterface_Calls::v006() {} void CGameScriptInterface_Calls::v008() {} void CGameScriptInterface_Calls::v009() {} void CGameScriptInterface_Calls::v010() {} void CGameScriptInterface_Calls::v011() {} void CGameScriptInterface_Calls::v012() {} void CGameScriptInterface_Calls::v013() {} void CGameScriptInterface_Calls::v014() {} void CGameScriptInterface_Calls::v015() {} void CGameScriptInterface_Calls::v016() {} void CGameScriptInterface_Calls::v017() {} void CGameScriptInterface_Calls::v018() {} void CGameScriptInterface_Calls::v019() {} void CGameScriptInterface_Calls::v020() {} void CGameScriptInterface_Calls::v021() {} void CGameScriptInterface_Calls::v022() {} void CGameScriptInterface_Calls::v023() {} void CGameScriptInterface_Calls::v024() {} void CGameScriptInterface_Calls::v025() {} void CGameScriptInterface_Calls::v026() {} void CGameScriptInterface_Calls::v027() {} void CGameScriptInterface_Calls::v028() {} void CGameScriptInterface_Calls::v029() {} void CGameScriptInterface_Calls::v030() {} void CGameScriptInterface_Calls::v031() {} void CGameScriptInterface_Calls::v032() {} void CGameScriptInterface_Calls::v033() {} void CGameScriptInterface_Calls::v034() {} void CGameScriptInterface_Calls::v035() {} void CGameScriptInterface_Calls::v036() {} void CGameScriptInterface_Calls::v037() {} void CGameScriptInterface_Calls::v038() {} void CGameScriptInterface_Calls::v039() {} void CGameScriptInterface_Calls::v040() {} void CGameScriptInterface_Calls::v041() {} void CGameScriptInterface_Calls::v042() {} void CGameScriptInterface_Calls::v043() {} void CGameScriptInterface_Calls::v044() {} void CGameScriptInterface_Calls::v045() {} void CGameScriptInterface_Calls::v046() {} void CGameScriptInterface_Calls::v047() {} void CGameScriptInterface_Calls::v048() {} void CGameScriptInterface_Calls::v049() {} void CGameScriptInterface_Calls::v050() {} void CGameScriptInterface_Calls::v051() {} void CGameScriptInterface_Calls::v052() {} void CGameScriptInterface_Calls::v053() {} void CGameScriptInterface_Calls::v054() {} void CGameScriptInterface_Calls::v055() {} void CGameScriptInterface_Calls::v056() {} void CGameScriptInterface_Calls::v057() {} void CGameScriptInterface_Calls::v058() {} void CGameScriptInterface_Calls::v059() {} void CGameScriptInterface_Calls::v060() {} void CGameScriptInterface_Calls::v061() {} void CGameScriptInterface_Calls::v062() {} void CGameScriptInterface_Calls::v063() {} void CGameScriptInterface_Calls::v064() {} void CGameScriptInterface_Calls::v065() {} void CGameScriptInterface_Calls::v066() {} void CGameScriptInterface_Calls::v067() {} void CGameScriptInterface_Calls::v068() {} void CGameScriptInterface_Calls::v069() {} void CGameScriptInterface_Calls::v070() {} void CGameScriptInterface_Calls::v071() {} void CGameScriptInterface_Calls::v072() {} void CGameScriptInterface_Calls::v073() {} void CGameScriptInterface_Calls::v074() {} void CGameScriptInterface_Calls::v075() {} void CGameScriptInterface_Calls::v076() {} void CGameScriptInterface_Calls::v077() {} void CGameScriptInterface_Calls::v078() {} void CGameScriptInterface_Calls::v079() {} void CGameScriptInterface_Calls::v080() {} void CGameScriptInterface_Calls::v081() {} void CGameScriptInterface_Calls::v082() {} void CGameScriptInterface_Calls::v083() {} void CGameScriptInterface_Calls::v084() {} void CGameScriptInterface_Calls::v085() {} void CGameScriptInterface_Calls::v086() {} void CGameScriptInterface_Calls::v087() {} void CGameScriptInterface_Calls::v088() {} void CGameScriptInterface_Calls::v089() {} void CGameScriptInterface_Calls::v090() {} void CGameScriptInterface_Calls::v091() {} void CGameScriptInterface_Calls::v092() {} void CGameScriptInterface_Calls::v093() {} void CGameScriptInterface_Calls::v094() {} void CGameScriptInterface_Calls::v095() {} void CGameScriptInterface_Calls::v096() {} void CGameScriptInterface_Calls::v097() {} void CGameScriptInterface_Calls::v098() {} void CGameScriptInterface_Calls::v099() {} void CGameScriptInterface_Calls::v100() {} void CGameScriptInterface_Calls::v101() {} void CGameScriptInterface_Calls::v102() {} void CGameScriptInterface_Calls::v103() {} void CGameScriptInterface_Calls::v104() {} void CGameScriptInterface_Calls::v105() {} void CGameScriptInterface_Calls::v106() {} void CGameScriptInterface_Calls::v107() {} void CGameScriptInterface_Calls::v108() {} void CGameScriptInterface_Calls::v109() {} void CGameScriptInterface_Calls::v110() {} void CGameScriptInterface_Calls::v111() {} void CGameScriptInterface_Calls::v112() {} void CGameScriptInterface_Calls::v113() {} void CGameScriptInterface_Calls::v114() {} void CGameScriptInterface_Calls::v115() {} void CGameScriptInterface_Calls::v116() {} void CGameScriptInterface_Calls::v117() {} void CGameScriptInterface_Calls::v118() {} void CGameScriptInterface_Calls::v119() {} void CGameScriptInterface_Calls::v120() {} void CGameScriptInterface_Calls::v121() {} void CGameScriptInterface_Calls::v122() {} void CGameScriptInterface_Calls::v123() {} void CGameScriptInterface_Calls::v124() {} void CGameScriptInterface_Calls::v125() {} void CGameScriptInterface_Calls::v126() {} void CGameScriptInterface_Calls::v127() {} void CGameScriptInterface_Calls::v128() {} void CGameScriptInterface_Calls::v129() {} void CGameScriptInterface_Calls::v130() {} void CGameScriptInterface_Calls::v131() {} void CGameScriptInterface_Calls::v132() {} void CGameScriptInterface_Calls::v133() {} void CGameScriptInterface_Calls::v134() {} void CGameScriptInterface_Calls::v135() {} void CGameScriptInterface_Calls::v136() {} void CGameScriptInterface_Calls::v137() {} void CGameScriptInterface_Calls::v138() {} void CGameScriptInterface_Calls::v139() {} void CGameScriptInterface_Calls::v140() {} void CGameScriptInterface_Calls::v141() {} void CGameScriptInterface_Calls::v142() {} void CGameScriptInterface_Calls::v143() {} void CGameScriptInterface_Calls::v144() {} void CGameScriptInterface_Calls::v145() {} void CGameScriptInterface_Calls::v146() {} void CGameScriptInterface_Calls::v147() {} void CGameScriptInterface_Calls::v148() {} void CGameScriptInterface_Calls::v149() {} void CGameScriptInterface_Calls::v150() {} void CGameScriptInterface_Calls::v151() {} void CGameScriptInterface_Calls::v152() {} void CGameScriptInterface_Calls::v153() {} void CGameScriptInterface_Calls::v154() {} void CGameScriptInterface_Calls::v155() {} void CGameScriptInterface_Calls::v156() {} void CGameScriptInterface_Calls::v157() {} void CGameScriptInterface_Calls::v158() {} void CGameScriptInterface_Calls::v159() {} void CGameScriptInterface_Calls::v160() {} void CGameScriptInterface_Calls::v161() {} void CGameScriptInterface_Calls::v162() {} void CGameScriptInterface_Calls::v163() {} void CGameScriptInterface_Calls::v164() {} void CGameScriptInterface_Calls::v165() {} void CGameScriptInterface_Calls::v166() {} void CGameScriptInterface_Calls::v167() {} void CGameScriptInterface_Calls::v168() {} void CGameScriptInterface_Calls::v169() {} void CGameScriptInterface_Calls::v170() {} void CGameScriptInterface_Calls::v171() {} void CGameScriptInterface_Calls::v172() {} void CGameScriptInterface_Calls::v173() {} void CGameScriptInterface_Calls::v174() {} void CGameScriptInterface_Calls::v175() {} void CGameScriptInterface_Calls::v176() {} void CGameScriptInterface_Calls::v177() {} void CGameScriptInterface_Calls::v178() {} void CGameScriptInterface_Calls::v179() {} void CGameScriptInterface_Calls::v181() {} void CGameScriptInterface_Calls::v182() {} void CGameScriptInterface_Calls::v183() {} void CGameScriptInterface_Calls::v184() {} void CGameScriptInterface_Calls::v185() {} void CGameScriptInterface_Calls::v186() {} void CGameScriptInterface_Calls::v187() {} void CGameScriptInterface_Calls::v188() {} void CGameScriptInterface_Calls::v189() {} void CGameScriptInterface_Calls::v190() {} void CGameScriptInterface_Calls::v191() {} void CGameScriptInterface_Calls::v192() {} void CGameScriptInterface_Calls::v193() {} void CGameScriptInterface_Calls::v194() {} void CGameScriptInterface_Calls::v195() {} void CGameScriptInterface_Calls::v196() {} void CGameScriptInterface_Calls::v197() {} void CGameScriptInterface_Calls::v198() {} void CGameScriptInterface_Calls::v199() {} void CGameScriptInterface_Calls::v200() {} void CGameScriptInterface_Calls::v201() {} void CGameScriptInterface_Calls::v202() {} void CGameScriptInterface_Calls::v203() {} void CGameScriptInterface_Calls::v204() {} void CGameScriptInterface_Calls::v205() {} void CGameScriptInterface_Calls::v206() {} void CGameScriptInterface_Calls::v207() {} void CGameScriptInterface_Calls::v208() {} void CGameScriptInterface_Calls::v209() {} void CGameScriptInterface_Calls::v210() {} void CGameScriptInterface_Calls::v211() {} void CGameScriptInterface_Calls::v212() {} void CGameScriptInterface_Calls::v213() {} void CGameScriptInterface_Calls::v214() {} void CGameScriptInterface_Calls::v215() {} void CGameScriptInterface_Calls::v216() {} void CGameScriptInterface_Calls::v217() {} void CGameScriptInterface_Calls::v218() {} void CGameScriptInterface_Calls::v219() {} void CGameScriptInterface_Calls::v220() {} void CGameScriptInterface_Calls::v221() {} void CGameScriptInterface_Calls::v222() {} void CGameScriptInterface_Calls::v223() {} void CGameScriptInterface_Calls::v224() {} void CGameScriptInterface_Calls::v225() {} void CGameScriptInterface_Calls::v226() {} void CGameScriptInterface_Calls::v227() {} void CGameScriptInterface_Calls::v228() {} void CGameScriptInterface_Calls::v229() {} void CGameScriptInterface_Calls::v230() {} void CGameScriptInterface_Calls::v231() {} void CGameScriptInterface_Calls::v232() {} void CGameScriptInterface_Calls::v233() {} void CGameScriptInterface_Calls::v234() {} void CGameScriptInterface_Calls::v235() {} void CGameScriptInterface_Calls::v236() {} void CGameScriptInterface_Calls::v237() {} void CGameScriptInterface_Calls::v238() {} void CGameScriptInterface_Calls::v239() {} void CGameScriptInterface_Calls::v240() {} void CGameScriptInterface_Calls::v241() {} void CGameScriptInterface_Calls::v242() {} void CGameScriptInterface_Calls::v243() {} void CGameScriptInterface_Calls::v244() {} void CGameScriptInterface_Calls::v245() {} void CGameScriptInterface_Calls::v246() {} void CGameScriptInterface_Calls::v247() {} void CGameScriptInterface_Calls::v248() {} void CGameScriptInterface_Calls::v249() {} void CGameScriptInterface_Calls::v250() {} void CGameScriptInterface_Calls::v251() {} void CGameScriptInterface_Calls::v252() {} void CGameScriptInterface_Calls::v253() {} void CGameScriptInterface_Calls::v254() {} void CGameScriptInterface_Calls::v255() {} void CGameScriptInterface_Calls::v256() {} void CGameScriptInterface_Calls::v257() {} void CGameScriptInterface_Calls::v258() {} void CGameScriptInterface_Calls::v259() {} void CGameScriptInterface_Calls::v260() {} void CGameScriptInterface_Calls::v261() {} void CGameScriptInterface_Calls::v262() {} void CGameScriptInterface_Calls::v263() {} void CGameScriptInterface_Calls::v264() {} void CGameScriptInterface_Calls::v265() {} void CGameScriptInterface_Calls::v266() {} void CGameScriptInterface_Calls::v267() {} void CGameScriptInterface_Calls::v268() {} void CGameScriptInterface_Calls::v269() {} void CGameScriptInterface_Calls::v270() {} void CGameScriptInterface_Calls::v271() {} void CGameScriptInterface_Calls::v272() {} void CGameScriptInterface_Calls::v273() {} void CGameScriptInterface_Calls::v275() {} void CGameScriptInterface_Calls::v276() {} void CGameScriptInterface_Calls::v277() {} void CGameScriptInterface_Calls::v278() {} void CGameScriptInterface_Calls::v279() {} void CGameScriptInterface_Calls::v280() {} void CGameScriptInterface_Calls::v281() {} void CGameScriptInterface_Calls::v282() {} void CGameScriptInterface_Calls::v283() {} void CGameScriptInterface_Calls::v284() {} void CGameScriptInterface_Calls::v285() {} void CGameScriptInterface_Calls::v286() {} void CGameScriptInterface_Calls::v287() {} void CGameScriptInterface_Calls::v288() {} void CGameScriptInterface_Calls::v289() {} void CGameScriptInterface_Calls::v290() {} void CGameScriptInterface_Calls::v291() {} void CGameScriptInterface_Calls::v292() {} void CGameScriptInterface_Calls::v293() {} void CGameScriptInterface_Calls::v294() {} void CGameScriptInterface_Calls::v295() {} void CGameScriptInterface_Calls::v296() {} void CGameScriptInterface_Calls::v297() {} void CGameScriptInterface_Calls::v298() {} void CGameScriptInterface_Calls::v299() {} void CGameScriptInterface_Calls::v300() {} void CGameScriptInterface_Calls::v301() {} void CGameScriptInterface_Calls::v302() {} void CGameScriptInterface_Calls::v303() {} void CGameScriptInterface_Calls::v304() {} void CGameScriptInterface_Calls::v305() {} void CGameScriptInterface_Calls::v306() {} void CGameScriptInterface_Calls::v307() {} void CGameScriptInterface_Calls::v308() {} void CGameScriptInterface_Calls::v309() {} void CGameScriptInterface_Calls::v310() {} void CGameScriptInterface_Calls::v311() {} void CGameScriptInterface_Calls::v312() {} void CGameScriptInterface_Calls::v313() {} void CGameScriptInterface_Calls::v314() {} void CGameScriptInterface_Calls::v315() {} void CGameScriptInterface_Calls::v316() {} void CGameScriptInterface_Calls::v317() {} void CGameScriptInterface_Calls::v318() {} void CGameScriptInterface_Calls::v319() {} void CGameScriptInterface_Calls::v320() {} void CGameScriptInterface_Calls::v321() {} void CGameScriptInterface_Calls::v322() {} void CGameScriptInterface_Calls::v323() {} void CGameScriptInterface_Calls::v324() {} void CGameScriptInterface_Calls::v325() {} void CGameScriptInterface_Calls::v326() {} void CGameScriptInterface_Calls::v327() {} void CGameScriptInterface_Calls::v328() {} void CGameScriptInterface_Calls::v329() {} void CGameScriptInterface_Calls::v330() {} void CGameScriptInterface_Calls::v331() {} void CGameScriptInterface_Calls::v332() {} void CGameScriptInterface_Calls::v333() {} void CGameScriptInterface_Calls::v334() {} void CGameScriptInterface_Calls::v335() {} void CGameScriptInterface_Calls::v336() {} void CGameScriptInterface_Calls::v337() {} void CGameScriptInterface_Calls::v338() {} void CGameScriptInterface_Calls::v339() {} void CGameScriptInterface_Calls::v340() {} void CGameScriptInterface_Calls::v341() {} void CGameScriptInterface_Calls::v342() {} void CGameScriptInterface_Calls::v343() {} void CGameScriptInterface_Calls::v344() {} void CGameScriptInterface_Calls::v345() {} void CGameScriptInterface_Calls::v346() {} void CGameScriptInterface_Calls::v347() {} void CGameScriptInterface_Calls::v348() {} void CGameScriptInterface_Calls::v349() {} void CGameScriptInterface_Calls::v350() {} void CGameScriptInterface_Calls::v351() {} void CGameScriptInterface_Calls::v352() {} void CGameScriptInterface_Calls::v353() {} void CGameScriptInterface_Calls::v354() {} void CGameScriptInterface_Calls::v355() {} void CGameScriptInterface_Calls::v356() {} void CGameScriptInterface_Calls::v357() {} void CGameScriptInterface_Calls::v358() {} void CGameScriptInterface_Calls::v359() {} void CGameScriptInterface_Calls::v360() {} void CGameScriptInterface_Calls::v361() {} void CGameScriptInterface_Calls::v362() {} void CGameScriptInterface_Calls::v363() {} void CGameScriptInterface_Calls::v364() {} void CGameScriptInterface_Calls::v365() {} void CGameScriptInterface_Calls::v366() {} void CGameScriptInterface_Calls::v367() {} void CGameScriptInterface_Calls::v368() {} void CGameScriptInterface_Calls::v369() {} void CGameScriptInterface_Calls::v370() {} void CGameScriptInterface_Calls::v371() {} void CGameScriptInterface_Calls::v372() {} void CGameScriptInterface_Calls::v373() {} void CGameScriptInterface_Calls::v374() {} void CGameScriptInterface_Calls::v375() {} void CGameScriptInterface_Calls::v376() {} void CGameScriptInterface_Calls::v377() {} void CGameScriptInterface_Calls::v378() {} void CGameScriptInterface_Calls::v379() {} void CGameScriptInterface_Calls::v380() {} void CGameScriptInterface_Calls::v381() {} void CGameScriptInterface_Calls::v382() {} void CGameScriptInterface_Calls::v383() {} void CGameScriptInterface_Calls::v384() {} void CGameScriptInterface_Calls::v385() {} void CGameScriptInterface_Calls::v386() {} void CGameScriptInterface_Calls::v387() {} void CGameScriptInterface_Calls::v388() {} void CGameScriptInterface_Calls::v389() {} void CGameScriptInterface_Calls::v390() {} void CGameScriptInterface_Calls::v391() {} void CGameScriptInterface_Calls::v392() {} void CGameScriptInterface_Calls::v393() {}

static int g_frames = 0, g_msgs = 0, g_term = 0, g_termAfter = 1000; static float g_will = 10.0f; static bool g_controlled = true;
static char g_lastText[64];
CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
void CGameScriptInterface_Calls::NewScriptFrame() const { ++g_frames; }
float CGameScriptInterface_Calls::GetHeroWillEnergyLevel() const { return g_will; }
bool CGameScriptInterface_Calls::IsHeroControlledByPlayer() const { return g_controlled; }
void CGameScriptInterface_Calls::HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString&, bool, bool) const
{ ++g_msgs; strcpy(g_lastText, (const char*)text.PStringData); }
bool NScript::CScriptBase::IsActiveThreadTerminating() { return ++g_term > g_termAfter; }
static CScriptDef g_def; CScriptDef* g_pScriptDef = &g_def;
int main()
{
    g_def.LowWillEnergyThreshold = 50.0f; g_def.WillEnergyWarningCooldown = 3; g_def.MaxWillEnergyWarnings = 2;
    static CGameScriptInterface_Calls gsi; static CQuestMasterData master; master.TutorialsDisabled = false;
    static NScript::CGameflowAssistanceScript s; s.GSI = &gsi; s.PMasterData = &master; s.WillEnergyWarningsGiven = 0;
    s.Tutorial_LowMagic();
    CHECK(g_msgs == 2 && s.WillEnergyWarningsGiven == 2 && g_frames == 4);
    CHECK(strcmp(g_lastText, "TEXT_QST_080_WILL_LOW") == 0);
    g_frames = g_msgs = 0; s.WillEnergyWarningsGiven = 0; master.TutorialsDisabled = true;
    s.Tutorial_LowMagic(); CHECK(g_frames == 1 && g_msgs == 0);          // tutorials off: leave after one frame
    master.TutorialsDisabled = false; g_frames = 0; g_term = 0; g_termAfter = 0;
    s.Tutorial_LowMagic(); CHECK(g_frames == 1 && g_msgs == 0);          // terminating thread
    if (fails == 0) printf("00cf0180_TEST PASS\n"); else printf("00cf0180_TEST FAIL\n");
    return fails;
}