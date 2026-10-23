// CMurderTwistSheriff::DoSuspicious() -- the pleased sheriff: talked to by the hero -> paused movie sequence, the "pleased" line (first time,
//   then HasSaidWellDoneToHero) or that line numbered 1..2 (NextLine); hit by the hero (or a special ability other than 14) ->
//   wait for the enflame reaction, "pleased, attacked" line, SheriffState 2
// Leaf: DoSuspicious
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB19C0, 1274 bytes (`ret`).
// Label: the catalog repeats DoBecomingSuspiciousOfHero for this second handler; the landed CMurderTwistSheriff::Main 0x00EAEF30
//   binds 0x00EB19C0 as DoSuspicious, so that name is kept (one symbol per address); by its strings and flag the original is
//   probably DoSayingWellDoneToHero.
// Twin: the landed CMurderTwistSheriff::DoRemindingHeroToKillSheriff 0x00EB15C0 (same model); 0x00EB10C0 and 0x00EB19C0 differ
//   only in the flag (+0x36 HasExpressedSuspicionToHero / +0x37 HasSaidWellDoneToHero) and the three line strings.
// Callees: as 0x00EB15C0, plus GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690,
//   CCharString::operator const char* 0x0099E4C0.
// Data: "SCRIPT_NAME_HERO" 0x0125D1C8, "" 0x0122D70E, "TEXT_QST_B07_SHERIFF_PLEASED", "TEXT_QST_B07_SHERIFF_PLEASED_ATTACKED".
// Layout: PDB CMurderTwistSheriff: ParentClass +0x14, MyEntityHandle +0x1c, NextLine +0x30, flags +0x34..+0x37.
// Shape: talk block: first time the single line and the flag set, afterwards a cycling line (NextLine 1..2); the cycling
//   text lives in an inner block inside its arm, left by goto on the terminate exits (text destructor before the unpause).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    operator const char*() const;                      // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830 (hidden result in ecx)
CCharString __fastcall operator+(const char* left, const CCharString& right);  // 0x0099F690 (hidden result in ecx)
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570 (hidden result in ecx)
enum EHeroAbility { HERO_ABILITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
struct CScriptThing {                                  // 12 bytes, vtable 0x01238C8C
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020();
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54) 0x004AAB30
    virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026();
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c) 0x004AABD0
    virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035();
    virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4) 0x004AAD20
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8) 0x004AAD40
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
};
struct CBaseObject {                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
    CBaseObject();                                      // 0x0099A380
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
struct CScriptGameResourceObjectMovieBase : CBaseObject {   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
    void DestroyOutOfLine();                            // the implicit destructor's out-of-line copy 0x006E7B80
};
struct CScriptGameResourceObjectScriptedThingBase {    // 0x10 bytes, vtable 0x0127094C
    virtual void h000(); virtual void h001(); virtual void h002(); virtual void h003(); virtual void h004(); virtual void h005(); virtual void h006(); virtual void h007();
    virtual void h008(); virtual void h009(); virtual void h010(); virtual void h011();
    virtual CScriptThing GetScriptThing() const;   // slot 12 (+0x30) 0x007E7490
    virtual void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34) 0x007E7390
    virtual void h014(); virtual void h015(); virtual void h016(); virtual void h017(); virtual void h018(); virtual void h019(); virtual void h020(); virtual void h021();
    virtual void h022(); virtual void h023(); virtual void h024(); virtual void h025();
    virtual bool IsPerformingScriptTask() const;   // slot 26 (+0x68) 0x007E7450
    unsigned char _body[0xc];
};
struct CGameScriptInterface {                          // vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
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
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void GiveHeroGold(long amount) const;   // slot 126 (+0x1f8) 0x00898F40
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
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420) 0x00890400
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
    virtual void v369();
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8) 0x0089B110
    virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378();
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec) 0x00890AB0
};
struct CEntityScriptBase {                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
enum ESheriffState { SHERIFF_STATE_0, SHERIFF_STATE_1, SHERIFF_STATE_2, SHERIFF_STATE_3 };   // enumerator names not in the PDB dump
enum EAssassinState { ASSASSIN_STATE_0, ASSASSIN_STATE_1, ASSASSIN_STATE_2, ASSASSIN_STATE_3 };
struct CV_MurderTwistFinalScript {                     // retail sits 0x14 below the PDB (SheriffState PDB +0x5c)
    unsigned char _pad00[0x48];
    ESheriffState  SheriffState;                       // +0x48
    EAssassinState AssassinState;                      // +0x4c
    void WaitForEnflameReactionIfNecessary(const CScriptThing& thing);   // 0x00EB0700
    bool HasHeroEnflamedEntity(const CScriptThing& thing);               // 0x00EB07D0
};
struct CTimer { unsigned long _handle; };
struct CMurderTwistSheriff : CEntityScriptBase {
    CV_MurderTwistFinalScript* ParentClass;            // +0x14
    void*                      PMasterData;            // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    CTimer                     AttractTimer;           // +0x2c
    unsigned long              NextLine;               // +0x30
    bool HasGivenReminderToHero;                       // +0x34
    bool HasBeingRumbledCutscenePlayed;                // +0x35
    bool HasExpressedSuspicionToHero;                  // +0x36
    bool HasSaidWellDoneToHero;                        // +0x37
    void DoSuspicious();
};

void CMurderTwistSheriff::DoSuspicious()
{
    if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        if (HasSaidWellDoneToHero) {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyInline();
                return;
            }
            if (NextLine > 2)
                NextLine = 1;
            {
                CCharString text = "TEXT_QST_B07_SHERIFF_PLEASED" + GFIntToCharString(NextLine);
                if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                    MyEntityHandle.Speak(Game->GetHero(), text, TEXT_GROUP_SELECTION_RANDOM, false, true, false);
                    while (MyEntityHandle.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            goto terminatedInLoop;
                    }
                    if (IsActiveThreadTerminating())
                        goto terminatedAfterLoop;
                }
                NextLine++;
            }
            if (0) {
terminatedInLoop:
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyOutOfLine();
                return;
terminatedAfterLoop:
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyOutOfLine();
                return;
            }
        } else {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyInline();
                return;
            }
            if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
                MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_PLEASED", TEXT_GROUP_SELECTION_RANDOM, false, true, false);
                while (MyEntityHandle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        movie.DestroyOutOfLine();
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyOutOfLine();
                    return;
                }
            }
            HasSaidWellDoneToHero = true;
        }
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    }
    if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
        || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->WaitForEnflameReactionIfNecessary(Me);
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        if (Game->GetHealth(MyEntityHandle.GetScriptThing()) > 0.0f) {
            MyEntityHandle.Speak(Game->GetHero(), "TEXT_QST_B07_SHERIFF_PLEASED_ATTACKED", TEXT_GROUP_SELECTION_RANDOM, false, true, false);
            while (MyEntityHandle.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyOutOfLine();
                    return;
                }
            }
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                movie.DestroyOutOfLine();
                return;
            }
        }
        ParentClass->SheriffState = SHERIFF_STATE_2;
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    }
}