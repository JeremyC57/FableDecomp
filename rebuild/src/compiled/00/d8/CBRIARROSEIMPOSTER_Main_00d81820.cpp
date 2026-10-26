// CBRIAR_ROSE_IMPOSTER::Main() -- a Briar Rose imposter in the Hero Souls quest: alive filter on Me, reset the two taunt timers, then every frame: when the hero hits it (or hits it with any special ability but number 14) and the taunt timer has run out, run a one-line taunt conversation and restart the timers; when the "here" timer has run out, the taunt timer is above 7 and the hero is over 13 units away, say "here" and restart that timer
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D81820, length 822 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy @ 0x004ABE90 / dtor @ 0x004AA840, SetPredicate @ 0x00F35B10, IsDone @ 0x00F35B30,
//          CScriptThing slots 21 MsgIsHitBy (+0x54), 41 MsgIsHitBySpecialAbilityFrom (+0xa4), 42
//          MsgIsHitByAnySpecialAbilityFrom (+0xa8); CGameScriptInterface slots 7, 70 GetHero, 89 SetTimer, 90 GetTimer,
//          364/365/366 AddNewConversation / AddPersonToConversation / AddLineToConversation (Game +4 and the timer
//          interface global 0x0143E8F8); IsDistanceBetweenThingsOver @ 0x00CBE3EA; CCharString @ 0x0099EBF0 / 0x0099EAE0.
//          Strings "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_B04_BRIAR_COPY_TAUNT" 0x012D4350,
//          "TEXT_QST_B04_BRIAR_COPY_HERE" 0x012D4330.
// Layout: Ego_r.pdb CBRIAR_ROSE_IMPOSTER (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//         (CQ_HeroSoulsBriarScript: BriarRoseImposterTauntTimer PDB +0x80 / retail +0x68, BriarRoseImposterHereTimer
//         PDB +0x84 / retail +0x6c), PMasterData +0x18.
// Shape: the hit test is one bool whose CCharString temporaries are destroyed under the flag word (spilled to the
//         frame); the timers are CTimer members with inline Get/Set through the interface global.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830

struct CScriptThingImpl;
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    CScriptThingImpl* PImp_Data;                       // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& name) const;                                  // slot 21 (+0x54)
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40();
    virtual bool MsgIsHitBySpecialAbilityFrom(long ability, const CCharString& name) const; // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;            // slot 42 (+0xa8)
    bool IsAlive() const;                              // slot 75, inlined below
};
struct CScriptThingImpl {
    virtual void s00();
    virtual void s01();
    virtual void s02();
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual bool IsAlive() const;                      // slot 75 (+0x12c)
};
inline bool CScriptThing::IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }

bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA

class CBaseObject { public:                                   // CBaseIntelligentPointer root
    virtual void v00();
    void* m_pIntelligentPointer;                       // +0x04
    CBaseObject();                                     // 0x0099A380
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void FollowThing(const CScriptThing& thing, float distance, bool run);   // slot 7 (+0x1c), 0x007E7320
    virtual void StopFollowingThing(const CScriptThing& thing);   // slot 8 (+0x20), 0x007E7330
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
};

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

class CGameScriptInterface { public:                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088();
    virtual void SetTimer(long index, long time);   // slot 89 (+0x164)
    virtual long GetTimer(long index) const;   // slot 90 (+0x168)
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146();
    virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
    virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170();
    virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
    virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186();
    virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194();
    virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202();
    virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210();
    virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218();
    virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226();
    virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234();
    virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242();
    virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250();
    virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258();
    virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266();
    virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274();
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
    virtual void v363();
    virtual long AddNewConversation(const CScriptThing& thing, bool a, bool b) const;   // slot 364 (+0x5b0)
    virtual void AddPersonToConversation(long conversation, const CScriptThing& person) const;   // slot 365 (+0x5b4)
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
};

struct CIsThingAlive {                                 // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};

class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void SetPredicate(CIsThingAlive& predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};




extern CGameScriptInterface* g_pScriptTimers;          // 0x0143E8F8
class CTimer { public:
    long Index;
    long GetTime() const { return g_pScriptTimers->GetTimer(Index); }
    void SetTime(long time) { g_pScriptTimers->SetTimer(Index, time); }
};
struct CQ_HeroSoulsBriarScript {
    unsigned char _pad[0x68];
    CTimer BriarRoseImposterTauntTimer;                // +0x68
    CTimer BriarRoseImposterHereTimer;                 // +0x6c
};

struct CBRIAR_ROSE_IMPOSTER : CEntityScriptBase {
    CQ_HeroSoulsBriarScript* ParentClass;              // +0x14
    void* PMasterData;                                 // +0x18
    void Main();
};

void CBRIAR_ROSE_IMPOSTER::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ParentClass->BriarRoseImposterTauntTimer.SetTime(0);
    ParentClass->BriarRoseImposterHereTimer.SetTime(4);
    while (!IsActiveThreadTerminating()) {
        bool hit = (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                    || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                        && !Me.MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO")))
                   && ParentClass->BriarRoseImposterTauntTimer.GetTime() == 0;
        if (hit) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_BRIAR_COPY_TAUNT", false, Me, Game->GetHero());
            ParentClass->BriarRoseImposterTauntTimer.SetTime(15);
            ParentClass->BriarRoseImposterHereTimer.SetTime(4);
        }
        if (ParentClass->BriarRoseImposterHereTimer.GetTime() == 0
            && ParentClass->BriarRoseImposterTauntTimer.GetTime() > 7
            && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 13.0f)) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_BRIAR_COPY_HERE", false, Me, Game->GetHero());
            ParentClass->BriarRoseImposterHereTimer.SetTime(15);
        }
        Game->NewScriptFrame();
    }
}