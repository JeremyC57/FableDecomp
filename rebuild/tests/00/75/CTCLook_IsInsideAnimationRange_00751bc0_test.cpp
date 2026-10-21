// Behaviour test for 00751bc0: src/00751bc0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCLook::IsInsideAnimationRange()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCLook::IsInsideAnimationRange() -- whether the creature is close enough to the camera (in the XY plane) for its look animation to matter, and is either on screen or in a movie sequence
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751BC0, length 176 bytes (+ int3 padding); speed TU (no frame pointer, esp-relative frame).
// Callees: CThing::GetPosition @ 0x004C73D0, CTCBase::GetWorld @ 0x00686A70 (twice), CWorld virtual slot 8 (+0x20, GetViewPosition --
// a C3DVector through the hidden pointer), Distance2D(const C2DVector&, const C2DVector&) @ 0x009E2460 (`__fastcall`, float in st0),
// CTCGraphicAppearance::WasRenderedLastFrame @ 0x004BD200 on CThing::PTCGraphicAppearance (+0x64), CGameScriptInterface::
// IsInMovieSequence() const (slot 395, +0x62C) through CWorld::PGameScriptInterface (+0x38). CLookDef::CutOffNoLooking +0x50 (PDB).
// Shape: the thing's position is copied into a C3DVector local (member-wise copy ctor, so the dead Z store drops and the
// 12-byte slot keeps retail's 4-byte hole above X/Y) and the view position into a second 3D local; both reach Distance2D as 2D
// conversion temporaries in the frame. The range test is `!(distance <= cutoff)` (`fcomp ; test ah,41h ; jp` on the false edge);
// the `||` of the two queries is materialised as the 32-bit 0/1 tails. The script interface comes through an inline CWorld getter
// over the scoped_ptr (`mov ecx,[eax+38h]`); the scoped_ptr's operator-> or get() at the call site gives `add eax,38h` / `mov ecx,eax`.
// Layout: CThing::PTCGraphicAppearance at +0x64 (PDB +0x74; retail loads [eax+64h]); CWorld is modelled locally because the shared
// header exposes the scoped_ptr as a raw pointer and has no inline script-interface getter (the getter is what fixes the codegen).
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
float __fastcall Distance2D(const C2DVector& a, const C2DVector& b);   // 0x009E2460
struct CTCGraphicAppearance {
    bool WasRenderedLastFrame() const;               // 0x004BD200
};
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64 (PDB +0x74)
    const C3DVector& GetPosition() const;            // 0x004C73D0
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
    virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289();
    virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299();
    virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309();
    virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319();
    virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329();
    virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339();
    virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349();
    virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359();
    virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369();
    virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379();
    virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389();
    virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394();
    virtual bool IsInMovieSequence() const;          // slot 395 (+0x62c)
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual C3DVector GetViewPosition() const;       // slot 8 (+0x20)
    unsigned char _pad04[0x38 - 0x04];
    scoped_ptr<CGameScriptInterface> PGameScriptInterface;   // +0x38
    CGameScriptInterface* GetGameScriptInterface() const { return PGameScriptInterface.get(); }
};
struct CLookDef {
    unsigned char _pad00[0x50];
    float CutOffNoLooking;                           // +0x50
};
template <class T> struct CDefPointer { T* Object; T* operator->() const { return Object; } };

struct CTCLook {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x0c];
    CDefPointer<CLookDef> PDef;                      // +0x14
    CWorld* GetWorld();                              // 0x00686A70 (CTCBase)
    bool IsInsideAnimationRange();
};
bool CTCLook::IsInsideAnimationRange()
{
    C3DVector position = ParentThing->GetPosition();
    C3DVector view = GetWorld()->GetViewPosition();
    CLookDef* def = PDef.Object;
    if (Distance2D(C2DVector(position), C2DVector(view)) <= def->CutOffNoLooking)
        return ParentThing->PTCGraphicAppearance->WasRenderedLastFrame() || GetWorld()->GetGameScriptInterface()->IsInMovieSequence();
    return false;
}

// ---- stubs and driver ----
#include <cmath>
void CGameScriptInterface::v000() {}
void CGameScriptInterface::v001() {}
void CGameScriptInterface::v002() {}
void CGameScriptInterface::v003() {}
void CGameScriptInterface::v004() {}
void CGameScriptInterface::v005() {}
void CGameScriptInterface::v006() {}
void CGameScriptInterface::v007() {}
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

void CWorld::v00() {}
void CWorld::v01() {}
void CWorld::v02() {}
void CWorld::v03() {}
void CWorld::v04() {}
void CWorld::v05() {}
void CWorld::v06() {}
void CWorld::v07() {}

static bool g_inMovie = false, g_rendered = false;
static int g_movieCalls = 0, g_renderCalls = 0, g_worldCalls = 0;
static C3DVector g_thingPos, g_viewPos;
static CWorld* g_world = 0;
bool CGameScriptInterface::IsInMovieSequence() const { ++g_movieCalls; return g_inMovie; }
C3DVector CWorld::GetViewPosition() const { return g_viewPos; }
bool CTCGraphicAppearance::WasRenderedLastFrame() const { ++g_renderCalls; return g_rendered; }
const C3DVector& CThing::GetPosition() const { return g_thingPos; }
CWorld* CTCLook::GetWorld() { ++g_worldCalls; return g_world; }
float __fastcall Distance2D(const C2DVector& a, const C2DVector& b)
{
    float dx = a.X - b.X, dy = a.Y - b.Y;
    return (float)sqrt(dx * dx + dy * dy);
}
int main()
{
    CGameScriptInterface gsi;
    CWorld world; world.PGameScriptInterface.ptr = &gsi; g_world = &world;
    CTCGraphicAppearance appearance;
    CThing thing; memset(thing._pad00, 0, sizeof(thing._pad00)); thing.PTCGraphicAppearance = &appearance;
    CLookDef def; memset(&def, 0, sizeof(def)); def.CutOffNoLooking = 10.0f;
    CTCLook look; memset(&look, 0, sizeof(look)); look.ParentThing = &thing; look.PDef.Object = &def;
    g_thingPos.X = 3.0f; g_thingPos.Y = 4.0f; g_thingPos.Z = 1000.0f;
    g_viewPos.X = 0.0f; g_viewPos.Y = 0.0f; g_viewPos.Z = -1000.0f;
    // distance 5 (Z ignored), rendered: inside, no movie query
    g_rendered = true; g_inMovie = false;
    CHECK(look.IsInsideAnimationRange() && g_renderCalls == 1 && g_movieCalls == 0 && g_worldCalls == 1);
    // not rendered, not in a movie: outside
    g_rendered = false;
    CHECK(!look.IsInsideAnimationRange() && g_renderCalls == 2 && g_movieCalls == 1 && g_worldCalls == 3);
    // not rendered but in a movie: inside
    g_inMovie = true;
    CHECK(look.IsInsideAnimationRange() && g_movieCalls == 2);
    // exactly at the cut-off counts as inside
    def.CutOffNoLooking = 5.0f;
    CHECK(look.IsInsideAnimationRange());
    // beyond the cut-off: false without any appearance or movie query
    def.CutOffNoLooking = 4.99f; g_rendered = true;
    int r = g_renderCalls, m = g_movieCalls;
    CHECK(!look.IsInsideAnimationRange() && g_renderCalls == r && g_movieCalls == m);
    if (fails == 0) printf("00751bc0_TEST PASS\n");
    return fails;
}
