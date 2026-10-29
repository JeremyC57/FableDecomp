// CTriggerCrate::Main() -- test-script crate: each time the hero uses it, activate the "TestTheInventory" quest and drop a 10-point experience orb beside it; the next use deactivates the quest again
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF25E0, length 568 bytes.
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, the is-alive predicate (vtable 0x012C32F0, out-of-line dtor
//   0x00E241B0), SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0;
//   CScriptThing slots 6 GetPos, 20 MsgIsUsedBy; GameCalls slots 7 NewScriptFrame, 103 CreateExperienceOrb (by value,
//   result dropped), 276 ActivateQuest, 280 DeactivateQuest. Strings "SCRIPT_NAME_HERO", "TestTheInventory"; 1.0f 0x0122DED8.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} (as the landed CMuteGuardOutsideWoods::Main 0x00D86150).
// Shape: script WAIT loops; the orb position is `Me.GetPos() + C3DVector(1, 1, 0)` through an inline operator+: VC7.1
//   folds the `Z + 0.0f` term to a plain x87 load of Z, and the three sums stay on the FPU stack (Z loaded first) until the
//   temporary is stored after the argument pushes. The orb handle is a discarded by-value result.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CScriptThing { public:                                    // 12 bytes, vtable 0x01238C8C
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005();
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18) 0x004AA980
    virtual void s007(); virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012();
    virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019();
    virtual bool MsgIsUsedBy(const CCharString& user) const;   // slot 20 (+0x50) 0x004AAB10
    void* PImp_Data;                                     // +0x04
    void* PImp_Info;                                     // +0x08
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
};
struct GameCalls {                                       // CGameScriptInterface, retail vtable 0x01260F0C
 virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
 virtual void NewScriptFrame(void) const; //slot7 0x006e7410
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
 virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
 virtual CScriptThing CreateExperienceOrb(C3DVector const &, long) const; //slot103 0x0089fbb0
 virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
 virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
 virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
 virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
 virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
 virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
 virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
 virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
 virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
 virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
 virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
 virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
 virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
 virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
 virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223();
 virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231();
 virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
 virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247();
 virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255();
 virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
 virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271();
 virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275();
 virtual void ActivateQuest(CCharString const &) const; //slot276 0x00892e80
 virtual void v277(); virtual void v278(); virtual void v279();
 virtual void DeactivateQuest(CCharString const &, unsigned long) const; //slot280 0x00892f00
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                   // vtable 0x012C32F0: a predicate holding a script thing
    CScriptThing m_thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                    // 0x00E241B0
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:                               // retail 0x14 bytes
    virtual void v00();
    GameCalls*   Game;                                   // +0x04
    CScriptThing Me;                                     // +0x08
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CTriggerCrate : CEntityScriptBase {
    void* ParentClass;                                   // +0x14
    void* PMasterData;                                   // +0x18
    void Main();
};

void CTriggerCrate::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!IsActiveThreadTerminating()) {
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->ActivateQuest("TestTheInventory");
        Game->CreateExperienceOrb(Me.GetPos() + C3DVector(1.0f, 1.0f, 0.0f), 10);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->DeactivateQuest("TestTheInventory", 0);
        Game->NewScriptFrame();
    }
}