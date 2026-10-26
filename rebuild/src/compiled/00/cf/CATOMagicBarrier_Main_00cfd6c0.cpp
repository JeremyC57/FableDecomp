// CATO_MagicBarrier::Main() -- an Arena magic barrier: alive filter on Me, spawn the two red forcefield halves at Me turned 90 degrees; while the parent's stage is below 3 (else while its counter is positive) respawn both idle halves whenever both are gone; then remove them, play the two "_OFF" effects and remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CFD6C0, 1504 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CCountedPointer operator= 0x00704580, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0; CScriptThing slots 6 GetPos, 75 IsAlive;
//   the thing impl's slots 10 GetHeading, 75 IsAlive; GameCalls slots 7, 100 CreateEffect, 108 RemoveThing,
//   474 EntitySetFacingAngle.
// Data: "NEW_RED_FORCEFIELD_01/02" 0x012C7D6C / 0x012C7D54, "..._IDLE_01/02" 0x012C354C / 0x012C3530, "..._IDLE_01/02_OFF"
//   0x012C3470 / 0x012C3450, "" 0x0122D70E, -1.0f 0x0122DEDC, 90.0f 0x012B7B74.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (stage +0x4c, counter +0x60).
// Shape: the respawn test goes through an inline helper taking const CScriptThing& (BothGone), so its IsAlive calls stay
//   virtual; the first test on the locals is devirtualised and the inline IsAlive expands.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CScriptThingImp {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009();
    virtual float GetHeading() const;   // slot 10 (+0x28)
    virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015(); virtual void i016(); virtual void i017(); virtual void i018();
    virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026();
    virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034();
    virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047(); virtual void i048(); virtual void i049(); virtual void i050();
    virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055(); virtual void i056(); virtual void i057(); virtual void i058();
    virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063(); virtual void i064(); virtual void i065(); virtual void i066();
    virtual void i067(); virtual void i068(); virtual void i069(); virtual void i070(); virtual void i071(); virtual void i072(); virtual void i073(); virtual void i074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
struct CCountedPointer {
    CScriptThingImp* Data;
    void*            Info;
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580
};
class CBaseClass { public:
};
class CScriptThing : public CBaseClass { public:                     // vtable 0x01238C8C, 12 bytes
    virtual void t0(); virtual void t1(); virtual void t2(); virtual void t3(); virtual void t4(); virtual void t5();
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18)
    virtual void t07(); virtual void t08(); virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14();
    virtual void t15(); virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22();
    virtual void t23(); virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30();
    virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34(); virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38();
    virtual void t39(); virtual void t40(); virtual void t41(); virtual void t42(); virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46();
    virtual void t47(); virtual void t48(); virtual void t49(); virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54();
    virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58(); virtual void t59(); virtual void t60(); virtual void t61(); virtual void t62();
    virtual void t63(); virtual void t64(); virtual void t65(); virtual void t66(); virtual void t67(); virtual void t68(); virtual void t69(); virtual void t70();
    virtual void t71(); virtual void t72(); virtual void t73(); virtual void t74();
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
    CCountedPointer PImp;                              // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    float GetHeading() const { return !PImp.Data ? -1.0f : PImp.Data->GetHeading(); }
};
namespace NScript {
class CPredicateBase { public: virtual bool operator()() const = 0; virtual CPredicateBase* Clone() const = 0; };
class CIsThingAlive : public CPredicateBase { public:                // vtable 0x012C32F0
    CScriptThing Thing;
    __forceinline CIsThingAlive(const CScriptThing& t) : Thing(t) { }
    virtual bool operator()() const; virtual CPredicateBase* Clone() const;
    ~CIsThingAlive() { }
};
}
class CGameScriptInterface { public:
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
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual CScriptThing CreateEffect(const CCharString& name, const C3DVector& pos, const CCharString& bone, float angle, bool b1, bool b2) const;   // slot 100 (+0x190)
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool unused, bool fade) const;   // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156();
    virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164();
    virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172();
    virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180();
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
    virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276();
    virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284();
    virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292();
    virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300();
    virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308();
    virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316();
    virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324();
    virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332();
    virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340();
    virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348();
    virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356();
    virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364();
    virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372();
    virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380();
    virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388();
    virtual void v389(); virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396();
    virtual void v397(); virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404();
    virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412();
    virtual void v413(); virtual void v414(); virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420();
    virtual void v421(); virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428();
    virtual void v429(); virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436();
    virtual void v437(); virtual void v438(); virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444();
    virtual void v445(); virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452();
    virtual void v453(); virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460();
    virtual void v461(); virtual void v462(); virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468();
    virtual void v469(); virtual void v470(); virtual void v471(); virtual void v472(); virtual void v473();
    virtual void EntitySetFacingAngle(const CScriptThing& thing, float angle, bool b) const;   // slot 474 (+0x768)
};
inline bool BothGone(const CScriptThing& a, const CScriptThing& b) { return !a.IsAlive() && !b.IsAlive(); }
struct CArenaBarrierScript {
    unsigned char _pad[0x4c];
    long Stage;                                        // +0x4c
    unsigned char _pad50[0x10];
    long Counter;                                      // +0x60
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CATO_MagicBarrier : public CEntityScriptBase {
    void* VTable;
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing Me;                                   // +0x08
    CArenaBarrierScript* ParentClass;                  // +0x14
    void* PMasterData;                                 // +0x18
    void Main();
};
void CATO_MagicBarrier::Main()
{
    SetPredicate(NScript::CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptThing barrier1 = Game->CreateEffect("NEW_RED_FORCEFIELD_01", Me.GetPos(), "", 0.0f, false, false);
    CScriptThing barrier2 = Game->CreateEffect("NEW_RED_FORCEFIELD_02", Me.GetPos(), "", 0.0f, false, false);
    if (barrier1.IsAlive() && barrier2.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetFacingAngle(barrier1, barrier1.GetHeading() + 90.0f, true);
        Game->EntitySetFacingAngle(barrier2, barrier2.GetHeading() + 90.0f, true);
    }
    if (ParentClass->Stage < 3) {
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->Stage < 3) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (BothGone(barrier1, barrier2)) {
                if (IsActiveThreadTerminating()) return;
                barrier1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", 0.0f, false, false);
                barrier2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", 0.0f, false, false);
            }
        }
        if (IsActiveThreadTerminating()) return;
    } else {
        if (IsActiveThreadTerminating()) return;
        while (ParentClass->Counter > 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (BothGone(barrier1, barrier2)) {
                if (IsActiveThreadTerminating()) return;
                barrier1 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01", Me.GetPos(), "", 0.0f, false, false);
                barrier2 = Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02", Me.GetPos(), "", 0.0f, false, false);
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
    Game->RemoveThing(barrier1, false, true);
    Game->RemoveThing(barrier2, false, true);
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_01_OFF", Me.GetPos(), "", 0.0f, false, false);
    Game->CreateEffect("NEW_RED_FORCEFIELD_IDLE_02_OFF", Me.GetPos(), "", 0.0f, false, false);
    Game->RemoveThing(Me, false, true);
}