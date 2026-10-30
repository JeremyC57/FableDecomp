// CUndeadRisingZombieAttackerBoss::Main() -- Undead Rising boss zombie: conscious filter on Me; once the quest flags the boss cutscene, spawn the quest's LookAfterHealth thread on Me, take scripting control, retreat to the start position whenever health drops below half until it recovers, then attack the hero and idle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0E2E0, length 698 bytes, plain `ret`.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840; the CIsThingConscious filter (vtable
//   0x012C2FE8, inline destructor) and SetPredicate 0x00F35B10; IsActiveThreadTerminating 0x00F35B30; operator new
//   0x00BFEA1A and the inlined CSpawnedFunc1<CQ_UndeadRisingScript, CScriptThing> constructor (CSpawnedFuncBase ctor
//   0x00CDD450, vtable 0x012E0088, callback 0x00E0DBB0); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   operator+(const CCharString&, const CCharString&) 0x0099F570 (__fastcall); CScriptBase::Add 0x00CB7E50; the scripted-thing
//   handle (CBaseObject ctor 0x0099A380, vtable 0x0127094C, IsValid 0x00CD23B9, Release 0x00CD2770, MoveToPosition
//   0x007E72F0, dtor 0x007E74D0); GameCalls slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 264 GetHealth, 451
//   GiveThingBestEnemyTarget. Strings "LookAfterHealth", "ParentClass.", ""; 0.5f 0x0122F59C.
// Layout: Ego_r.pdb CUndeadRisingZombieAttackerBoss: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, StartPos (C3DVector) +0x1c, MaxHealth +0x28 (descriptive names for the last two). CQ_UndeadRisingScript
//   retail = Ego_r.pdb - 0x14: BossCutscenePlayed +0x51. The spawned function as the landed CAttackingBandit::Init 0x00E1F3E0
//   (0x48 bytes: base 0x34, Func +0x34, PClass +0x38, Par1 +0x3c).
// Shape: generated script code; the spawn is `ParentClass->Add(new CSpawnedFunc1(...), "")` (conditional name temporaries
//   with frame flags, the by-value thing parameter destroyed before Add).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
class C3DVector { public: float X, Y, Z; };
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
struct CPredicateBase {
    virtual bool IsTrue() = 0;
};
struct CIsThingConscious : CPredicateBase {             // vtable 0x012C2FE8
    CScriptThing Thing;                                 // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingConscious() { }
    virtual bool IsTrue();
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
enum EScriptEntityMoveType { };
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                          // +0x08
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    virtual void MoveToPosition(const C3DVector& position, float a2, EScriptEntityMoveType moveType, bool a4, bool a5);   // 0x007E72F0 (header slot 4; called directly on the local)
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_3 = 3 };
struct GameCalls {                                       // CGameScriptInterface, retail vtable 0x01260F0C
 virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
 virtual void NewScriptFrame(void) const; //slot7 0x006e7410
 virtual bool StartScriptingEntity(CScriptThing const &, CScriptGameResourceObjectScriptedThingBase &, EScriptAIPriority); //slot8 0x0089b5b0
 virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
 virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
 virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
 virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
 virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
 virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
 virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
 virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
 virtual CScriptThing & GetHero(void); //slot70 0x00891ca0
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
 virtual void v263();
 virtual float GetHealth(CScriptThing const &) const; //slot264 0x00890400
 virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272();
 virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280();
 virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288();
 virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296();
 virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
 virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312();
 virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
 virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328();
 virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336();
 virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344();
 virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352();
 virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360();
 virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368();
 virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376();
 virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384();
 virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391(); virtual void v392();
 virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399(); virtual void v400();
 virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408();
 virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415(); virtual void v416();
 virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422(); virtual void v423(); virtual void v424();
 virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431(); virtual void v432();
 virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439(); virtual void v440();
 virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446(); virtual void v447(); virtual void v448();
 virtual void v449(); virtual void v450();
 virtual void GiveThingBestEnemyTarget(CScriptThing const &, CScriptThing const &) const; //slot451 0x008a0260
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
struct CQ_UndeadRisingScript {
    unsigned char _pad00[0x51];
    bool          BossCutscenePlayed;                   // +0x51
    void LookAfterHealth(CScriptThing thing);           // 0x00E0DBB0
    void Add(CSpawnedFuncBase* function, const CCharString& name);   // 0x00CB7E50
};
typedef void (CQ_UndeadRisingScript::*SpawnedThingCallback)(CScriptThing);
struct CSpawnedFunc1 : CSpawnedFuncBase {               // CSpawnedFunc1<CQ_UndeadRisingScript, CScriptThing>, vtable 0x012E0088
    SpawnedThingCallback   Func;                        // +0x34
    CQ_UndeadRisingScript* PClass;                      // +0x38
    CScriptThing           Par1;                        // +0x3c
    __forceinline CSpawnedFunc1(const CCharString& name, unsigned long id, SpawnedThingCallback func, CQ_UndeadRisingScript* owner, CScriptThing par1)
        : CSpawnedFuncBase(name, id), Func(func), PClass(owner), Par1(par1) { }
    virtual void SuspendableProcess();
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A
struct CUndeadRisingZombieAttackerBoss {
    void*                  VTable;
    GameCalls*             Game;                        // +0x04
    CScriptThing           Me;                          // +0x08
    CQ_UndeadRisingScript* ParentClass;                 // +0x14
    void*                  PMasterData;                 // +0x18
    C3DVector              StartPos;                    // +0x1c
    float                  MaxHealth;                   // +0x28
    void SetPredicate(const CPredicateBase& predicate); // 0x00F35B10
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};

void CUndeadRisingZombieAttackerBoss::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    while (!ParentClass->BossCutscenePlayed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->Add(new CSpawnedFunc1(CCharString("ParentClass.") + CCharString("LookAfterHealth"), 0,
                                       &CQ_UndeadRisingScript::LookAfterHealth, ParentClass, Me), CCharString(""));
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->GetHealth(Me) < MaxHealth * 0.5f) {
        if (IsActiveThreadTerminating())
            return;
        handle.MoveToPosition(StartPos, 0.0f, (EScriptEntityMoveType)0, true, true);
        while (Game->GetHealth(Me) < MaxHealth) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}