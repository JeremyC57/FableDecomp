// CKG_ChiefBreakSiege::Main() -- Break the Siege, the bandit chief: wait for the siege to start or end; take the chief under
//   scripted control and walk him to his hut, telling the hero to stop bothering him whenever spoken to; when the siege is
//   over play CS_BREAKSIEGE_DEFEAT (hero alive, bandits beaten) or CS_BREAKSIEGE_VICTORY, then make the chief killable
// Leaf: ?Main@CKG_ChiefBreakSiege@
// Shape: CMKGHCHERO3 0x00D29DB0 model; the chief handle ctor is plain inline (the per-function inline budget expands it here
//   but calls 0x007E72A0 for the later hero handles); hut position via a __forceinline null-test accessor; Me / the hero
//   reached through reference helpers so MsgIsTalkedToBy / IsAlive stay virtual; the two cut-scene blocks share a tail.
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
int __cdecl rand();                                     // 0x00BFEB16
#include "engine/CCountedPointer.h"
struct CScriptThingImpl {
    virtual void i00(); virtual void i01(); virtual void i02(); virtual void i03(); virtual void i04(); virtual void i05();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
};
extern const C3DVector g_ZeroPos;                       // 0x0143E8E0
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11(); virtual void s12();
    virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18();
    virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24();
    virtual void s25(); virtual void s26();
    virtual bool MsgIsTalkedToBy(const CCharString& talker) const;   // slot 27 (+0x6c) 0x004AABD0
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33();
    virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39();
    virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45();
    virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51();
    virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57();
    virtual void s58(); virtual void s59(); virtual void s60(); virtual void s61(); virtual void s62(); virtual void s63();
    virtual void s64(); virtual void s65(); virtual void s66(); virtual void s67(); virtual void s68(); virtual void s69();
    virtual void s70(); virtual void s71(); virtual void s72(); virtual void s73(); virtual void s74();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    __forceinline const C3DVector& PosOrZero() const { return PImp.Data == 0 ? g_ZeroPos : PImp.Data->GetPos(); }
};
class CBaseObject { public:                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                      // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);   // 0x0099A3B0
public:
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CHandlePImp {
    void*           Data;
    CCPPointerInfo* Info;
    CHandlePImp() : Data(0), Info(0) { }
    CHandlePImp& operator=(const CHandlePImp& other);   // 0x008AB1E0
};
enum EScriptEntityMoveType;
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CHandlePImp PImp;                                   // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }    // out of line 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    virtual void MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool);   // 0x007E72F0
    virtual bool IsPerformingScriptTask() const;                // 0x007E7450
    virtual void ClearCommands();                               // 0x007E7360
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
class CScriptGameResourceObjectMovieBase : public CBaseObject { public:   // vtable 0x01260EF4; destructor-less view, destroyed by hand
    CMovieImpWords PImp;                                // +0x08
    __forceinline CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    virtual void m001();
    __forceinline void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBaseDtor*>(this)->CScriptGameResourceObjectBaseDtor::~CScriptGameResourceObjectBaseDtor();
    }
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
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:                              // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CQBreakSiegeScript {
    unsigned char _pad[0x48];
    bool          BanditsBeaten;                        // +0x48 (descriptive)
    bool          SiegeLost;                            // +0x49
    unsigned char _pad4a;
    bool          ChiefTaken;                           // +0x4b
    unsigned char _pad4c[5];
    bool          SiegeStarted;                         // +0x51
    unsigned char _pad52;
    bool          SiegeOver;                            // +0x53
    bool          ChiefDone;                            // +0x54
};
struct CKG_ChiefBreakSiege : CEntityScriptBase {
    CQBreakSiegeScript* ParentClass;                    // +0x14
    void Main();
};
static inline bool IsTalkedToBy(const CScriptThing& thing, const CCharString& talker) { return thing.MsgIsTalkedToBy(talker); }
static inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }

void CKG_ChiefBreakSiege::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase chief;
    while (!ParentClass->SiegeStarted && !ParentClass->SiegeOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    ParentClass->ChiefTaken = true;
    if (chief.IsValid())
        chief.Release();
    while (!Game->StartScriptingEntity(Me, chief, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing hut = Game->GetThingWithScriptName("MB_ChiefHut");
    CScriptThing leader = Game->GetThingWithScriptName("ImprisonedBanditLeader");
    while (IsDistanceBetweenThingsOver(Me, hut, 2.0f) && !ParentClass->SiegeOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!chief.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating())
                return;
            chief.MoveToPosition(hut.PosOrZero(), 0.0f, (EScriptEntityMoveType)0, false, true);
        }
        if (IsTalkedToBy(Me, "SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            chief.ClearCommands();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_018_CHIEF_STOP_BOTHERING_ME_10", false, Me, Game->GetHero());
            Game->Pause(3.0f);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->SiegeOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsThingAlive(Game->GetHero()) && ParentClass->SiegeLost) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
        Game->SheatheHeroWeapons();
        CCutsceneCastMap cast;
        cast["HERO"] = hero;
        cast["CHIEF"] = chief;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_BREAKSIEGE_DEFEAT", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        chief.Release();
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    } else if (ParentClass->BanditsBeaten) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
        Game->SheatheHeroWeapons();
        CCutsceneCastMap cast;
        cast["HERO"] = hero;
        cast["CHIEF"] = chief;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_BREAKSIEGE_VICTORY", cast, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        chief.Release();
        game->PauseAllNonScriptedEntities(false);
        movie.DestroyInline();
    }
    ParentClass->ChiefDone = true;
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
}