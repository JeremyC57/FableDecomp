// CFishCompCompetitor::Main() -- a fishing-competition competitor (his ID is his data string): attach to the quest and stay persistent; vanish if the competitors were already attacked; otherwise every frame until they are, flag the attack when the hero hits him (plain hit, or a special ability other than 14), and answer a talk with a paused movie line "TEXT_QST_B15_FISHER<id>" + _SCARED / _INTRO / _FISHCOMMENT_<prize> (once per new prize) / _TIP; finally become scared
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7E640, 1604 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, CBaseClass dtor 0x0099A2E0, scripted-thing handle IsValid
//   0x00CD23B9 / Release 0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   out-of-line dtor 0x007E74D0, movie PImp dtor 0x006E7AB0, IsActiveThreadTerminating 0x00F35B30, GFCharStringToInt 0x0099E7F0,
//   GFIntToCharString 0x0099F830, operator+ 0x0099F690 / 0x0099F600 / 0x0099F570, CCharString 0x0099EBF0 / 0x0099EAE0 / 0x0099E4C0 /
//   operator= 0x0099EFB0, operator delete 0x00BFE9BC; GameCalls slots 7, 8, 70, 108, 264, 370, 379, 465 EntityAttachToScript,
//   477 SetThingPersistent, 496 EntitySetAsScared, 655 GetActiveQuestName; CScriptThing slots 3, 21, 27, 41, 42.
// Layout: Ego_r.pdb CFishCompCompetitor (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CV_FishingCompetitionScript, retail = PDB - 0x14: CompetitorsAttacked +0x4f, CompetitorsJustGotAttacked +0x50, AttackCounter
//   +0x54, DoneIntro +0x60, PrizeNumber +0x64, SizeComment (vector<long>) +0x74), PMasterData +0x18.
enum EHeroAbility { HERO_ABILITY_NONE };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    operator const char*() const;                      // 0x0099E4C0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
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
    ~CCountedPointer()
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
    virtual void s001() const; virtual void s002() const;
    virtual CCharString GetDataString() const;   // slot 3 (+0xc)
    virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const;
    virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const;
    virtual void s020() const;
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
    // implicit destructor; out-of-line copy 0x007E74D0
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
CCharString __fastcall GFIntToCharString(long value);                   // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);   // 0x00CBE716
struct CV_FishingCompetitionScript {
    unsigned char _pad0[0x4f];
    bool  CompetitorsAttacked;                          // +0x4f
    bool  CompetitorsJustGotAttacked;                   // +0x50
    unsigned char _pad1[0x54 - 0x51];
    long  AttackCounter;                                // +0x54
    unsigned char _pad2[0x60 - 0x58];
    bool  DoneIntro;                                    // +0x60
    unsigned char _pad3[0x64 - 0x61];
    long  PrizeNumber;                                  // +0x64
    unsigned char _pad4[0x74 - 0x68];
    long* SizeComment;                                  // +0x74 (vector<long> _M_start)
};
extern const float g_fZero;                             // 0x0122DEDC (0.0f)
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CFishCompCompetitor : CEntityScriptBase {
    CV_FishingCompetitionScript* ParentClass;           // +0x14
    void*                PMasterData;                   // +0x18
    void Main();
};

void CFishCompCompetitor::Main()
{
    CCharString myName = Me.GetDataString();
    long myID = GFCharStringToInt(myName);
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
    Game->SetThingPersistent(Me, true);
    if (ParentClass->CompetitorsAttacked) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(Me, false, true);
    }
    while (!ParentClass->CompetitorsAttacked) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->CompetitorsAttacked = true;
            ParentClass->CompetitorsJustGotAttacked = true;
        } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CCharString line = "TEXT_QST_B15_FISHER" + myName;
            if (ParentClass->AttackCounter > 0) {
                if (IsActiveThreadTerminating())
                    return;
                line = line + "_SCARED";
            } else if (!ParentClass->DoneIntro) {
                if (IsActiveThreadTerminating())
                    return;
                line = line + "_INTRO";
            } else if (ParentClass->SizeComment[myID] < ParentClass->PrizeNumber) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->SizeComment[myID] = ParentClass->PrizeNumber;
                {
                    long* comments = ParentClass->SizeComment;
                    line = line + "_FISHCOMMENT_" + GFIntToCharString(comments[myID]);
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                line = line + "_TIP";
            }
            CScriptGameResourceObjectScriptedThingBase competitor;
            if (competitor.IsValid())
                competitor.Release();
            while (!Game->StartScriptingEntity(Me, competitor, (EScriptAIPriority)4)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                {
                    return;
                }
            CMovieStorage movieStorage;
            CScriptGameResourceObjectMovieBase& movie = *new (&movieStorage) CScriptGameResourceObjectMovieBase;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            bool alive = Game->GetHealth(competitor.GetScriptThing()) > g_fZero;
            if (alive) {
                competitor.Speak(Game->GetHero(), line, (ETextGroupSelectionMethod)2, false, true, false);
                while (competitor.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
                        movie.DestroyInline();
                        return;
                    }
                }
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
                    movie.DestroyInline();
                    return;
                }
            }
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyInline();
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsScared(Me, true);
}