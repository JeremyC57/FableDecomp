// CEM_FlashLightPoint::Main() -- Expose-the-Mayor lamp point entity: waits for stage 2, spawns the parent's CheckLampSlot thread, then until the hero has flashed the lights correctly waits for the hero to come within MaxFlashDistFromStables, waits for the hand lamp to go off and on again, spawns HeroFlashesLamp(5) and waits for a result or a lamp reset
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) NScript::CV_ExposeMayorScript::CEM_FlashLightPoint::Main @ 0x00E76920, 851 bytes (+ int3 padding).
// Callees: IsActiveThreadTerminating 0x00F35B30 (on this), operator new 0x00BFEA1A (0x3c / 0x40 bytes), CCharString(const
// char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, operator+ 0x0099F570 (__fastcall), CSpawnedFuncBase ctor 0x00CDD450,
// CScriptBase::AddSpawnedFunction 0x00CB7E50 (catalog "CGuiVarTransferStruct::Add" is propagated), IsDistanceBetweenThingsUnder
// 0x00CBE2FF (__fastcall); GameCalls slots 7 NewScriptFrame (+0x1c), 70 GetHero (+0x118), 185 IsHeroHandLampLit (+0x2e4),
// 376 Pause (+0x5e0). Spawned threads: CSpawnedFunc0<CV_ExposeMayorScript> (vtable 0x012E96E0, CheckLampSlot 0x00E76CA0)
// and CSpawnedFunc1<CV_ExposeMayorScript, long> (vtable 0x012E9924, HeroFlashesLamp 0x00E76E90, Par1 = 5).
// Strings: "CheckLampSlot" 0x012E9950, "HeroFlashesLamp" 0x012E9940, "ParentClass." 0x012C9934, "" 0x0122D70E.
// Layout: CEM_FlashLightPoint (Ego_r.pdb, 28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18. CV_ExposeMayorScript members sit 0x14 below their PDB offsets (quest-script base 0x40 in retail, as
// for the other quest scripts): StageNumber +0x60 -> +0x4c, HeroFlashedLightsCorrectly +0x64 -> +0x50, ResetLamp +0x66 ->
// +0x52, MaxFlashDistFromStables +0xb0 -> +0x9c.
// Shape: generated script code (`if (IsActiveThreadTerminating()) return;` after every statement, WAIT_UNTIL loops of
// NewScriptFrame + terminate test); the spawned threads are the landed CFishCompOrganiser::OnPredicateFail
// (0x00E7F030) model: the thread name temporaries exist only when the new succeeded, so VC guards them with flag
// masks (7, then 0x38) in ebx.
class CScriptThing;
class CGameScriptInterface { public:                          // GameCalls vtable 0x01260F0C (slots used here named)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) 0x00891CA0
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
    virtual void v183(); virtual void v184();
    virtual bool IsHeroHandLampLit() const;   // slot 185 (+0x2e4) 0x00898E60
    virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193();
    virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201();
    virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217();
    virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225();
    virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233();
    virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241();
    virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257();
    virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265();
    virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273();
    virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281();
    virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289();
    virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297();
    virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305();
    virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313();
    virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321();
    virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329();
    virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337();
    virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345();
    virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353();
    virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361();
    virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369();
    virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375();
    virtual void Pause(float seconds) const;   // slot 376 (+0x5e0) 0x0088E4F0
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

class CScriptThing { public:                                   // 12 bytes {vptr, PImp_Data, PImp_Info}
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

class CSpawnedFuncBase { public:                               // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];                          // +0x04 .. +0x34
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
template <class T, class P1> struct CSpawnedFunc1 : CSpawnedFuncBase {
    void (T::*Func)(P1);                                // +0x34
    T* PClass;                                          // +0x38
    P1 Par1;                                            // +0x3c
    CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), P1 par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) {}
    virtual void SuspendableProcess() { (PClass->*Func)(Par1); }
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
};
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase { public:
    unsigned char _pad40[0x48 - 0x40];
    long  Path;                                         // +0x48 PATH_ID
    long  StageNumber;                                  // +0x4c STAGE_IN_PATH
    bool  HeroFlashedLightsCorrectly;                   // +0x50
    bool  PrisonerTalkedAboutLamp;                      // +0x51
    bool  ResetLamp;                                    // +0x52
    unsigned char _pad53[0x9c - 0x53];
    float MaxFlashDistFromStables;                      // +0x9c
    void CheckLampSlot();                               // 0x00E76CA0
    void HeroFlashesLamp(long slot);                    // 0x00E76E90
    class CEM_FlashLightPoint;
};
class CV_ExposeMayorScript::CEM_FlashLightPoint {
public:
    void*                  __vftable;                   // +0x00
    CGameScriptInterface*  Game;                        // +0x04
    CScriptThing           Me;                          // +0x08
    CV_ExposeMayorScript*  ParentClass;                 // +0x14
    void*                  PMasterData;                 // +0x18
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};

void CV_ExposeMayorScript::CEM_FlashLightPoint::Main()
{
    while (ParentClass->StageNumber != 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass->AddSpawnedFunction(
        new CSpawnedFunc0<CV_ExposeMayorScript>(CCharString("ParentClass.") + CCharString("CheckLampSlot"), 0,
                                                ParentClass, &CV_ExposeMayorScript::CheckLampSlot),
        CCharString("", -1));
    while (!ParentClass->HeroFlashedLightsCorrectly) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, ParentClass->MaxFlashDistFromStables)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (Game->IsHeroHandLampLit()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsHeroHandLampLit()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        ParentClass->AddSpawnedFunction(
            new CSpawnedFunc1<CV_ExposeMayorScript, long>(CCharString("ParentClass.") + CCharString("HeroFlashesLamp"), 0,
                                                          ParentClass, &CV_ExposeMayorScript::HeroFlashesLamp, 5),
            CCharString("", -1));
        while (!ParentClass->HeroFlashedLightsCorrectly && !ParentClass->ResetLamp) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        ParentClass->ResetLamp = false;
        Game->Pause(0.5f);
    }
    if (IsActiveThreadTerminating()) return;
}
}