// CFH_Villager::Main() -- a Fire Heart prisoner villager: leave at once if the hero already hit his group; otherwise take scripted control and, until the mission succeeds or his group is hit, mark his group when the hero hits him (plain hit, or a special ability other than 14), make his "_COMMENT_NEW" remark when the quest asks for it, and answer a talk with a paused "_ON_TALK" movie; when his group was hit run to the escape marker and fade out there (or vanish when he did not get within 16 units)
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D37530, 1783 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, CBaseClass dtor 0x0099A2E0, scripted-thing handle IsValid
//   0x00CD23B9 / Release 0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   MoveToPosition 0x007E72F0 / dtor 0x007E74D0, movie PImp dtor 0x006E7AB0 / out-of-line dtor 0x006E7B80, IsActiveThreadTerminating
//   0x00F35B30, GFIntToCharString 0x0099F830, operator+ 0x0099F690 / 0x0099F600, CCharString 0x0099EBF0 / 0x0099EAE0 / 0x0099E4C0,
//   GetSquaredDistanceBetweenThings 0x00CBE583, FadeOutAndKillEntity 0x00CBE716, operator delete 0x00BFE9BC; GameCalls slots 7, 8, 70,
//   108 RemoveThing, 264, 364-366, 370, 379; CScriptThing slots 6, 21, 27, 41, 42.
// Layout: Ego_r.pdb CFH_Villager (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_FireHeartScript),
//   PMasterData +0x18, MyID +0x1c. The parent's retail offsets do not follow the PDB (its maps/vectors are smaller in retail):
//   MissionSucceeded +0x49 (PDB +0x5d), the two "hero hit villagers" group flags +0x97/+0x98, the per-villager speak flags
//   (first word of a container) +0xec and the escape-marker CScriptThing +0x18c are taken from the code; names from the PDB.
enum EHeroAbility { HERO_ABILITY_NONE };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    operator const char*() const;                      // 0x0099E4C0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const;
    virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const;
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    virtual void s028() const; virtual void s029() const; virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const;
    virtual void s036() const; virtual void s037() const; virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
    ~CScriptThing() { }                                 // out-of-line copy 0x004AA840
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    void MoveToPosition(const C3DVector& pos, float speed, long moveType, bool a, bool b);   // 0x007E72F0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    CScriptThing GetScriptThing() const;                // 0x007E7490
    void Speak(const CScriptThing& listener, const char* text, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // 0x007E7390
    bool IsPerformingScriptTask() const;                // 0x007E7450
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
struct CScriptGameResourceObjectMovieFull : CScriptGameResourceObjectBase {
    CMovieImpWords PImp;
    ~CScriptGameResourceObjectMovieFull();              // 0x006E7B80
};
struct CMovieStorage { void* Words[4]; };              // raw frame storage of one movie handle (0x10 bytes)
inline void* operator new(unsigned int, void* where) { return where; }   // placement form: the handle is destroyed by hand
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; built in place in CMovieStorage, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBase*>(this)->CScriptGameResourceObjectBase::~CScriptGameResourceObjectBase();
    }
    __forceinline void Destroy()
    {
        reinterpret_cast<CScriptGameResourceObjectMovieFull*>(this)->CScriptGameResourceObjectMovieFull::~CScriptGameResourceObjectMovieFull();
    }
};
class CGameScriptInterface { public:                          // vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const;
    virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const;
    virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const;
    virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const;
    virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const;
    virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const;
    virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const;
    virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) 0x00891CA0
    virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const;
    virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const;
    virtual void v087() const; virtual void v088() const; virtual void v089() const; virtual void v090() const; virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const;
    virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const; virtual void v100() const; virtual void v101() const; virtual void v102() const;
    virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const; virtual void v107() const;
    virtual void RemoveThing(const CScriptThing& a0, bool a1, bool a2) const;   // slot 108 (+0x1b0) 0x008910D0
    virtual void v109() const; virtual void v110() const; virtual void v111() const; virtual void v112() const; virtual void v113() const; virtual void v114() const; virtual void v115() const; virtual void v116() const;
    virtual void v117() const; virtual void v118() const; virtual void v119() const; virtual void v120() const; virtual void v121() const; virtual void v122() const; virtual void v123() const; virtual void v124() const;
    virtual void v125() const; virtual void v126() const; virtual void v127() const; virtual void v128() const; virtual void v129() const; virtual void v130() const; virtual void v131() const; virtual void v132() const;
    virtual void v133() const; virtual void v134() const; virtual void v135() const; virtual void v136() const; virtual void v137() const; virtual void v138() const; virtual void v139() const; virtual void v140() const;
    virtual void v141() const; virtual void v142() const; virtual void v143() const; virtual void v144() const; virtual void v145() const; virtual void v146() const; virtual void v147() const; virtual void v148() const;
    virtual void v149() const; virtual void v150() const; virtual void v151() const; virtual void v152() const; virtual void v153() const; virtual void v154() const; virtual void v155() const; virtual void v156() const;
    virtual void v157() const; virtual void v158() const; virtual void v159() const; virtual void v160() const; virtual void v161() const; virtual void v162() const; virtual void v163() const; virtual void v164() const;
    virtual void v165() const; virtual void v166() const; virtual void v167() const; virtual void v168() const; virtual void v169() const; virtual void v170() const; virtual void v171() const; virtual void v172() const;
    virtual void v173() const; virtual void v174() const; virtual void v175() const; virtual void v176() const; virtual void v177() const; virtual void v178() const; virtual void v179() const; virtual void v180() const;
    virtual void v181() const; virtual void v182() const; virtual void v183() const; virtual void v184() const; virtual void v185() const; virtual void v186() const; virtual void v187() const; virtual void v188() const;
    virtual void v189() const; virtual void v190() const; virtual void v191() const; virtual void v192() const; virtual void v193() const; virtual void v194() const; virtual void v195() const; virtual void v196() const;
    virtual void v197() const; virtual void v198() const; virtual void v199() const; virtual void v200() const; virtual void v201() const; virtual void v202() const; virtual void v203() const; virtual void v204() const;
    virtual void v205() const; virtual void v206() const; virtual void v207() const; virtual void v208() const; virtual void v209() const; virtual void v210() const; virtual void v211() const; virtual void v212() const;
    virtual void v213() const; virtual void v214() const; virtual void v215() const; virtual void v216() const; virtual void v217() const; virtual void v218() const; virtual void v219() const; virtual void v220() const;
    virtual void v221() const; virtual void v222() const; virtual void v223() const; virtual void v224() const; virtual void v225() const; virtual void v226() const; virtual void v227() const; virtual void v228() const;
    virtual void v229() const; virtual void v230() const; virtual void v231() const; virtual void v232() const; virtual void v233() const; virtual void v234() const; virtual void v235() const; virtual void v236() const;
    virtual void v237() const; virtual void v238() const; virtual void v239() const; virtual void v240() const; virtual void v241() const; virtual void v242() const; virtual void v243() const; virtual void v244() const;
    virtual void v245() const; virtual void v246() const; virtual void v247() const; virtual void v248() const; virtual void v249() const; virtual void v250() const; virtual void v251() const; virtual void v252() const;
    virtual void v253() const; virtual void v254() const; virtual void v255() const; virtual void v256() const; virtual void v257() const; virtual void v258() const; virtual void v259() const; virtual void v260() const;
    virtual void v261() const; virtual void v262() const; virtual void v263() const;
    virtual float GetHealth(const CScriptThing& a0) const;   // slot 264 (+0x420) 0x00890400
    virtual void v265() const; virtual void v266() const; virtual void v267() const; virtual void v268() const; virtual void v269() const; virtual void v270() const; virtual void v271() const; virtual void v272() const;
    virtual void v273() const; virtual void v274() const; virtual void v275() const; virtual void v276() const; virtual void v277() const; virtual void v278() const; virtual void v279() const; virtual void v280() const;
    virtual void v281() const; virtual void v282() const; virtual void v283() const; virtual void v284() const; virtual void v285() const; virtual void v286() const; virtual void v287() const; virtual void v288() const;
    virtual void v289() const; virtual void v290() const; virtual void v291() const; virtual void v292() const; virtual void v293() const; virtual void v294() const; virtual void v295() const; virtual void v296() const;
    virtual void v297() const; virtual void v298() const; virtual void v299() const; virtual void v300() const; virtual void v301() const; virtual void v302() const; virtual void v303() const; virtual void v304() const;
    virtual void v305() const; virtual void v306() const; virtual void v307() const; virtual void v308() const; virtual void v309() const; virtual void v310() const; virtual void v311() const; virtual void v312() const;
    virtual void v313() const; virtual void v314() const; virtual void v315() const; virtual void v316() const; virtual void v317() const; virtual void v318() const; virtual void v319() const; virtual void v320() const;
    virtual void v321() const; virtual void v322() const; virtual void v323() const; virtual void v324() const; virtual void v325() const; virtual void v326() const; virtual void v327() const; virtual void v328() const;
    virtual void v329() const; virtual void v330() const; virtual void v331() const; virtual void v332() const; virtual void v333() const; virtual void v334() const; virtual void v335() const; virtual void v336() const;
    virtual void v337() const; virtual void v338() const; virtual void v339() const; virtual void v340() const; virtual void v341() const; virtual void v342() const; virtual void v343() const; virtual void v344() const;
    virtual void v345() const; virtual void v346() const; virtual void v347() const; virtual void v348() const; virtual void v349() const; virtual void v350() const; virtual void v351() const; virtual void v352() const;
    virtual void v353() const; virtual void v354() const; virtual void v355() const; virtual void v356() const; virtual void v357() const; virtual void v358() const; virtual void v359() const; virtual void v360() const;
    virtual void v361() const; virtual void v362() const; virtual void v363() const;
    virtual long AddNewConversation(const CScriptThing& a0, bool a1, bool a2) const;   // slot 364 (+0x5b0) 0x008906C0
    virtual void AddPersonToConversation(long a0, const CScriptThing& a1) const;   // slot 365 (+0x5b4) 0x00890710
    virtual void AddLineToConversation(long a0, const CCharString& a1, bool a2, const CScriptThing& a3, const CScriptThing& a4) const;   // slot 366 (+0x5b8) 0x00890750
    virtual void v367() const; virtual void v368() const; virtual void v369() const;
    virtual void StartMovieSequence(const CCharString& a0, CScriptGameResourceObjectMovieBase& a1);   // slot 370 (+0x5c8) 0x0089B110
    virtual void v371() const; virtual void v372() const; virtual void v373() const; virtual void v374() const; virtual void v375() const; virtual void v376() const; virtual void v377() const; virtual void v378() const;
    virtual void PauseAllNonScriptedEntities(bool a0);   // slot 379 (+0x5ec) 0x00890AB0
};
CCharString __fastcall GFIntToCharString(long value);                   // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);   // 0x00CBE716
struct CQ_FireHeartScript {
    unsigned char _pad0[0x49];
    bool MissionSucceeded;                              // +0x49
    unsigned char _pad1[0x97 - 0x4a];
    bool HasHeroHitVillagersGroupOne;                   // +0x97
    bool HasHeroHitVillagersGroupTwo;                   // +0x98
    unsigned char _pad2[0xec - 0x99];
    bool* ShouldVillagerSpeak;                          // +0xec (container start)
    unsigned char _pad3[0x18c - 0xf0];
    CScriptThing VillagerEscapeMarker;                  // +0x18c
};
extern const float g_fZero;                             // 0x0122DEDC (0.0f)
extern const float g_fSixteen;                          // 0x01238164 (16.0f)
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CFH_Villager : CEntityScriptBase {
    CQ_FireHeartScript*  ParentClass;                   // +0x14
    void*                PMasterData;                   // +0x18
    long                 MyID;                          // +0x1c
    void Main();
};

void CFH_Villager::Main()
{
    if ((MyID < 3 && ParentClass->HasHeroHitVillagersGroupOne) || (MyID >= 3 && ParentClass->HasHeroHitVillagersGroupTwo)) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
        return;
    }
    CScriptGameResourceObjectScriptedThingBase villager;
    if (villager.IsValid())
        villager.Release();
    while (!Game->StartScriptingEntity(Me, villager, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->MissionSucceeded) {
        if (MyID < 3) {
            if (ParentClass->HasHeroHitVillagersGroupOne)
                break;
        } else if (ParentClass->HasHeroHitVillagersGroupTwo)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            if (MyID < 3) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->HasHeroHitVillagersGroupOne = true;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->HasHeroHitVillagersGroupTwo = true;
            }
        } else if (ParentClass->ShouldVillagerSpeak[MyID]) {
            if (IsActiveThreadTerminating())
                return;
            CCharString line = "TEXT_QST_B01_VILLAGER" + GFIntToCharString(MyID) + "_COMMENT_NEW";
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, line, false, Me, Game->GetHero());
            ParentClass->ShouldVillagerSpeak[MyID] = false;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CMovieStorage movieStorage;
            CScriptGameResourceObjectMovieBase& movie = *new (&movieStorage) CScriptGameResourceObjectMovieBase;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            bool alive = Game->GetHealth(villager.GetScriptThing()) > g_fZero;
            if (alive) {
                villager.Speak(Game->GetHero(), "TEXT_QST_B01_VILLAGER" + GFIntToCharString(MyID) + "_ON_TALK", (ETextGroupSelectionMethod)2, false, true, false);
                while (villager.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        movie.DestroyInline();
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.Destroy();
                    return;
                }
            }
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyInline();
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if ((MyID < 3 && ParentClass->HasHeroHitVillagersGroupOne) || (MyID >= 3 && ParentClass->HasHeroHitVillagersGroupTwo)) {
        if (IsActiveThreadTerminating())
            return;
        villager.MoveToPosition(ParentClass->VillagerEscapeMarker.GetPos(), 1.0f, 1, false, true);
        while (!villager.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (villager.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (GetSquaredDistanceBetweenThings(Me, ParentClass->VillagerEscapeMarker) <= g_fSixteen) {
            if (IsActiveThreadTerminating())
                return;
            FadeOutAndKillEntity(Me, true, 2.5f, true);
            return;
        }
    }
    Game->RemoveThing(Me, false, true);
}