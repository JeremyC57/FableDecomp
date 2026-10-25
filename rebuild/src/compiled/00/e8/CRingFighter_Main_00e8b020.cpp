// CRingFighter::Main() -- punch-club ring fighter: filter on Me alive while the crowd is not all attacking; wait for the round to start and target the hero; each frame let the crowd cheer when the hero lands a hit and boo when the ring fighter hits the hero, stop on a time-out; once Me's health falls to the def's losing threshold, take scripted control of Me and the hero, play the ROUNDWON cutscene, walk Me to the loser marker, flag the ring cleared and wait for the next round, then walk Me out to the marker and remove Me
// Leaf: ?Main@CRingFighter@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8B020, 1748 bytes (int3 padded to 1760).
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840, CIsThingAlive (vtable 0x012C32F0, ~ 0x00E241B0), flag predicate
//   (vtable 0x012EA43C), and-predicate (vtable 0x012EA430, ctor 0x00E7E020), predicate base vtable 0x012C2FF4,
//   SetPredicate 0x00F35B10, IsDone 0x00F35B30 (manifest "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated),
//   CBaseObject 0x0099A380, scripted-thing handle ctor 0x007E72A0 / ~ 0x007E74D0 / IsScripting 0x00CD23B9 / Release 0x00CD2770
//   / IsPerformingScriptTask 0x007E7450 / MoveToThing 0x007E7300 / MoveToPosition 0x007E72F0 / GetScriptThing 0x007E7490,
//   cast map ctor 0x00CDBF70 / ~ 0x00CDBFB0 / operator[] 0x00CD3D2E, handle operator= 0x008ABD10, movie ctor 0x006E7B60 / ~
//   0x006E7B80, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CV_FisticuffsClubScript::GetCutsceneName 0x00E8A1D0, RunCutsceneMacro
//   0x00CBFB7D, IsDistanceFromThingToPositionOver 0x00CBE45C; CScriptThing slots 21, 41, 42 (impl slot 6 GetPos inline);
//   pGlobalScriptDef 0x0143E90C (+0x5E0 losing health), zero vector 0x0143E8E0; Game slots 7, 8, 70, 72, 108, 264, 370, 379, 451, 452.
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8}; Ego_r.pdb CRingFighter ParentClass +0x14; parent
//   CV_FisticuffsClubScript (PDB - 0x14) TylerInFight +0x54, TimeOut +0x56, RingCleared +0x57, RoundStarted +0x58,
//   EverybodyAttack +0x8c, CheerNumber +0xac, BooNumber +0xb0.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
void operator delete(void* p);                          // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
struct C3DVector { float X, Y, Z; };
struct CScriptThingImpl {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005();
    virtual const C3DVector& GetPos() const;            // imp slot 6 (+0x18)
};
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
struct CBaseClass {
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    __forceinline virtual const C3DVector& GetPos() const   // slot 6 (+0x18) 0x004AA980
    {
        if (!PImp.Data)
            return g_ZeroVector;
        return PImp.Data->GetPos();
    }
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020();
    virtual bool MsgIsHitBy(const CCharString& by) const;   // slot 21 (+0x54)
    virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& by) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& by) const;              // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
};
struct CScriptPredicate {                               // vtable 0x012C2FF4
    virtual bool Evaluate() const = 0;
    virtual CScriptPredicate* Clone() const = 0;
    virtual ~CScriptPredicate() {}
};
struct CIsThingAlive : CScriptPredicate {               // vtable 0x012C32F0
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    ~CIsThingAlive();                                   // 0x00E241B0
    virtual bool Evaluate() const;
    virtual CScriptPredicate* Clone() const;
};
struct CIsFlagFalse : CScriptPredicate {                // vtable 0x012EA43C
    const bool* Flag;                                   // +0x04
    CIsFlagFalse(const bool* flag) : Flag(flag) {}
    virtual bool Evaluate() const;
    virtual CScriptPredicate* Clone() const;
};
struct CAndPredicate : CScriptPredicate {               // vtable 0x012EA430
    CIsThingAlive First;                                // +0x04
    CIsFlagFalse  Second;                               // +0x14
    CAndPredicate(const CIsThingAlive& first, const CIsFlagFalse& second);   // 0x00E7E020
    ~CAndPredicate() {}
    virtual bool Evaluate() const;
    virtual CScriptPredicate* Clone() const;
};
struct CBaseObject {                                    // vtable 0x0129A7D8
    CBaseObject();                                      // 0x0099A380
    virtual ~CBaseObject();                             // 0x0099A430
    void* IntelligentPointer;                           // +0x04
};
struct CScriptGameResourceObjectBase : CBaseObject {    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
enum EScriptEntityMoveType { MOVE_TYPE_WALK };
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C
    CCountedPointerWords PImp;
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    virtual bool IsPerformingScriptTask() const;        // 0x007E7450
    virtual CScriptThing GetScriptThing() const;        // 0x007E7490
    virtual void MoveToThing(const CScriptThing& thing, float distance, EScriptEntityMoveType type, bool a, bool b, bool c, bool d);   // 0x007E7300
    virtual void MoveToPosition(const C3DVector& pos, float distance, EScriptEntityMoveType type, bool a, bool b);   // 0x007E72F0
};
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4
    CCountedPointerWords PImp;
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, long priority);   // slot 8 (+0x20)
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168();
    virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
    virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188();
    virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198();
    virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208();
    virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218();
    virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228();
    virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238();
    virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248();
    virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258();
    virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
    virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274();
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284();
    virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
    virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324();
    virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344();
    virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364();
    virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369();
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378();
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec)
    virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389();
    virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399();
    virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409();
    virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419();
    virtual void v420(); virtual void v421(); virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429();
    virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439();
    virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449();
    virtual void v450();
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;   // slot 451 (+0x70c)
    virtual void ClearThingBestEnemyTarget(const CScriptThing& thing) const;   // slot 452 (+0x710)
};
struct CScriptDef {
    unsigned char _pad[0x5e0];
    long FisticuffsLosingHealth;                        // +0x5e0
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CV_FisticuffsClubScript {
    unsigned char _pad[0x54];
    bool TylerInFight;                                  // +0x54
    bool HeroBeatTyler;                                 // +0x55
    bool TimeOut;                                       // +0x56
    bool RingCleared;                                   // +0x57
    bool RoundStarted;                                  // +0x58
    unsigned char _pad59[0x8c - 0x59];
    bool EverybodyAttack;                               // +0x8c
    unsigned char _pad8d[0xac - 0x8d];
    float CheerNumber;                                  // +0xac
    float BooNumber;                                    // +0xb0
    CCharString GetCutsceneName(CCharString name);      // 0x00E8A1D0
};
struct CEntityScriptBase {
    virtual void Main() = 0;
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void SetPredicate(const CScriptPredicate& predicate);   // 0x00F35B10
    bool IsDone();                                      // 0x00F35B30
};
struct CRingFighter : CEntityScriptBase {
    CV_FisticuffsClubScript* ParentClass;               // +0x14
    void*                    PMasterData;               // +0x18
    bool                     FirstRingFighterFought;    // +0x1c
    virtual void Main();
};

void CRingFighter::Main()
{
    {
        CIsFlagFalse notAttacking(&ParentClass->EverybodyAttack);
        CIsThingAlive alive(Me);
        CAndPredicate both(alive, notAttacking);
        SetPredicate(both);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    CScriptGameResourceObjectScriptedThingBase fighter;
    while (!ParentClass->RoundStarted) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!IsDone()) {
        bool hit = Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO"));
        if (hit) {
            if (IsDone())
                return;
            ParentClass->CheerNumber += 0.25f;
        }
        if (Game->GetHero().MsgIsHitBy("KHGRingFighter")) {
            if (IsDone())
                return;
            ParentClass->BooNumber += 4.0f;
        }
        if (ParentClass->TimeOut) {
            if (IsDone())
                return;
            Game->ClearThingBestEnemyTarget(Me);
            return;
        }
        const CScriptDef* def = pGlobalScriptDef;
        if (Game->GetHealth(Me) <= def->FisticuffsLosingHealth) {
            if (IsDone())
                return;
            CScriptThing loserPos = Game->GetThingWithScriptName("LoserFighterPos");
            ParentClass->RoundStarted = false;
            {
#pragma inline_depth(0)
                CScriptGameResourceObjectScriptedThingBase hero;
#pragma inline_depth()
                if (fighter.IsScripting())
                    fighter.Release();
                while (!Game->StartScriptingEntity(Me, fighter, 4)) {
                    Game->NewScriptFrame();
                    if (IsDone())
                        return;
                }
                if (IsDone())
                    return;
                Game->StartScriptingEntity(Game->GetHero(), hero, 4);
                CCutsceneCastMap cast;
                cast["FIGHTER"] = fighter;
                cast["HERO"] = hero;
#pragma inline_depth(0)
                CScriptGameResourceObjectMovieBase movie;
#pragma inline_depth()
                Game->StartMovieSequence("", movie);
                CGameScriptInterface* game = Game;
                game->PauseAllNonScriptedEntities(true);
                RunCutsceneMacro(ParentClass->GetCutsceneName("ROUNDWON"), cast, 0, 0, false, true);
                while (fighter.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsDone()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsDone()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                fighter.MoveToThing(loserPos, 1.0f, MOVE_TYPE_WALK, false, true, false, true);
                ParentClass->RingCleared = true;
                while (!ParentClass->RoundStarted && !ParentClass->TylerInFight) {
                    Game->NewScriptFrame();
                    if (IsDone()) {
                        game->PauseAllNonScriptedEntities(false);
                        return;
                    }
                }
                if (IsDone()) {
                    game->PauseAllNonScriptedEntities(false);
                    return;
                }
                hero.Release();
                game->PauseAllNonScriptedEntities(false);
            }
            {
                C3DVector pos = loserPos.GetPos();
                while (IsDistanceFromThingToPositionOver(fighter.GetScriptThing(), pos, 2.0f)) {
                    Game->NewScriptFrame();
                    if (IsDone())
                        return;
                    fighter.MoveToPosition(pos, 0.0f, MOVE_TYPE_WALK, false, true);
                    while (fighter.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsDone())
                            return;
                    }
                    if (IsDone())
                        return;
                }
            }
            if (IsDone())
                return;
            Game->RemoveThing(Me, false, true);
            return;
        }
        Game->NewScriptFrame();
    }
}