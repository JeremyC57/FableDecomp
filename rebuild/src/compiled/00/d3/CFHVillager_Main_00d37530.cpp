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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
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