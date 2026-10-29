// NGraphicAppearance::CTCGraphicAppearance::WasRenderedLastFrame() const -- true when the owner's def forces it or a movie sequence runs; otherwise refresh the stale rendered flag once a frame has passed and report it
// Leaf: WasRenderedLastFrame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BD200, length 211 bytes (+ int3 padding). __fastcall, `ret`.
//         Ego_r.pdb: `?WasRenderedLastFrame@CTCGraphicAppearance@NGraphicAppearance@@QBE_NXZ`.
// Callees: CTCBase::GetWorld() const @ 0x00686A70 (twice), CGameScriptInterface::IsInMovieSequence() const (slot 395, +0x62C),
//          CWorld::GetWorldFrame (static) @ 0x0049D870 (twice), CTCGraphicAppearance::UpdateRenderedLastFrameFlag @ 0x004BC620,
//          the def's virtual slot 1.
// Shape: `forced = GetDef() && GetDef()->ForceRenderedLastFrameOverride` over a by-value CDefPointer getter: two
//        conditional temporaries tracked by construction flags in ebx (1, then 3), destroyed in reverse at the end of the
//        full expression, the result kept in a frame byte; the movie test shares the `mov al,1` exit
//        (two separate early returns) and reaches the script interface through an inline accessor (straight into ecx);
//        the last render frame is read into a local before the frame call (the +1 follows the call).
// Layout: retail CThing PBaseDef +0x70; retail CThingBaseDef ForceRenderedLastFrameOverride +0x43 bit 2 (PDB +0x47);
//         retail CTCGraphicAppearance FrameUpdatedWasRenderedFlag +0x54, RenderedLastFrame +0x59 bit 6; retail CWorld
//         PGameScriptInterface +0x38 and the last render frame +0x8C.
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
};
class CThingBaseDef : public CDefClassBase {
public:
    unsigned char _pad08[0x43 - 0x08];
    unsigned char DisableOcclusionTesting : 1;      // +0x43 bit 0
    unsigned char DrawAfterWater : 1;
    unsigned char ForceRenderedLastFrameOverride : 1;   // bit 2
    unsigned char AddToComboMultiplierOnHit : 1;
    unsigned char GiveHeroStatChangesOnBeingHit : 1;
};

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

class CThing {
public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;      // +0x70
    CDefPointer<const CThingBaseDef> GetDef() const { return PBaseDef; }
};

class CGameScriptInterface {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164();
    virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
    virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189();
    virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194();
    virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204();
    virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224();
    virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229();
    virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234();
    virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244();
    virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254();
    virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259();
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264();
    virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269();
    virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274();
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284();
    virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289();
    virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299();
    virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
    virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309();
    virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319();
    virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324();
    virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329();
    virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339();
    virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344();
    virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349();
    virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359();
    virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364();
    virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369();
    virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374();
    virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379();
    virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384();
    virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389();
    virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394();
    virtual bool IsInMovieSequence() const;         // slot 395 (+0x62C)
};

class CWorld { public:
    unsigned char _pad00[0x38];
    CGameScriptInterface* PGameScriptInterface;     // +0x38
    unsigned char _pad3c[0x8c - 0x3c];
    long LastRenderFrame;                           // +0x8C
    static long __fastcall GetFrame();                    // 0x0049D870
    CGameScriptInterface* GetGameScriptInterface() const { return PGameScriptInterface; }
};

class CTCBase { protected:
    CWorld& GetWorld() const;                       // 0x00686A70 (Ego_r.pdb CTCBase, protected)
};

namespace NGraphicAppearance {

class CTCGraphicAppearance : public ::CTCBase {
public:
    void*         __vfptr;                          // +0x00
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x54 - 0x08];
    long          FrameUpdatedWasRenderedFlag;      // +0x54
    unsigned char UpdatePrimitiveReasons;           // +0x58
    unsigned char SelfIlluminating : 1;             // +0x59 bit 0
    unsigned char ForceResendEveryRenderFrame : 1;
    unsigned char ResendPrimsEveryRenderFrame : 1;
    unsigned char DisableOcclusionTesting : 1;
    unsigned char DrawAfterWater : 1;
    unsigned char CachedStaticFocalPos : 1;
    unsigned char RenderedLastFrame : 1;            // bit 6
    unsigned char PrimitivesDrawn : 1;
    void UpdateRenderedLastFrameFlag() const;       // 0x004BC620
    bool WasRenderedLastFrame() const;
};

bool CTCGraphicAppearance::WasRenderedLastFrame() const
{
    bool forced = ParentThing->GetDef().Object && ParentThing->GetDef()->ForceRenderedLastFrameOverride;
    if (forced)
        return true;
    if (GetWorld().GetGameScriptInterface()->IsInMovieSequence())
        return true;
    if (FrameUpdatedWasRenderedFlag < CWorld::GetFrame()) {
        long lastRenderFrame = GetWorld().LastRenderFrame;
        if (lastRenderFrame + 1 < CWorld::GetFrame())
            UpdateRenderedLastFrameFlag();
    }
    return RenderedLastFrame;
}

}