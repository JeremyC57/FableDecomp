// CDF_MarryLadyGrey::Main() -- demon-door face (marry Lady Grey): take scripted control of Me, make it persistent and immune, find the nearest REGION_EXIT_POINT and shut it; each time the hero talks to the face -- unless the master data says the hero married Lady Grey, speak the intro line; otherwise speak the opening line, fade the face out, open the door and reactivate the exit
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E73410, 1092 bytes (+ int3 padding), plain `ret`. Literals "TEXT_QST_088_MARRY_LADY_GREY_INTRO"
//   0x012E93E0, "TEXT_QST_088_MARRY_LADY_GREY_OPENING" 0x012E93B8, "DD_MarryLadyGrey" 0x012E93A4 (sibling of the matched
//   CDF_CombatMult::Main 0x00E6F710; only the test differs: CQ_SunnyvaleMasterData::HeroMarriedLadyGrey, PDB +0xD7 = retail).
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / out-of-line dtor 0x007E74D0, movie handle
//   out-of-line dtor 0x006E7B80, CIsThingAlive (vtable 0x012C32F0, out-of-line dtor 0x00E241B0), CScriptThing copy 0x004ABE90 /
//   dtor 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC, CCharString
//   0x0099EBF0 / 0x0099EAE0, CV_DemonDoorsScript::FadeOutFaceAndOpenDoor 0x00E6F040; GameCalls slots 7, 8, 70 GetHero,
//   81 GetNearestWithDefName, 149 (unused here), 264 GetHealth-sized float query, 370 StartMovieSequence,
//   379 PauseAllNonScriptedEntities, 477 SetThingPersistent, 516/517/526 damageable/killable/combo, 567 SetRegionExitAsActive;
//   CScriptThing slot 27 MsgIsTalkedToBy.
// Layout: Ego_r.pdb CDF_MarryLadyGrey (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_DemonDoorsScript),
//   PMasterData +0x18. CQ_SunnyvaleMasterData HeroMarriedLadyGrey +0xD7. Handle hierarchy as the landed
//   CMeleeThunder::Main 0x00D58080.
// Shape: the CMeleeThunder handle model (implicit handle dtor expanded only at the first return, inline_depth(0) elsewhere).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
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
struct CBase {
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    VPAD16(ma) VPAD8(mb) VPAD2(mc)                      // slots 1..26
    virtual bool MsgIsTalkedToBy(const CCharString& name) const;   // slot 27 (+0x6c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                                    // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                   // 0x00E241B0
};
}
using NScript::CIsThingAlive;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    // implicit destructor (stores no vptr); out-of-line copy 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    virtual void Speak(const CScriptThing&, const char*, ETextGroupSelectionMethod, bool, bool, bool);   // 0x007E7390
public:
    virtual CScriptThing GetScriptThing() const;   // 0x007E7490
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase { public:   // vtable 0x01260EF4, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectMovieBase() { }
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern const float g_fZero;                            // 0x0122DEDC (0.0f)
struct CV_DemonDoorsScript {
    void FadeOutFaceAndOpenDoor(const CScriptThing& face, const CCharString& doorName);   // 0x00E6F040
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0xd7];
    bool HeroMarriedLadyGrey;                           // +0xD7
};
struct CDF_MarryLadyGrey : CEntityScriptBase {
    CV_DemonDoorsScript*    ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    void Main();
};

void CDF_MarryLadyGrey::Main()
{
    CScriptGameResourceObjectScriptedThingBase face;
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (face.IsValid())
        face.Release();
    while (!Game->StartScriptingEntity(Me, face, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    Game->SetThingPersistent(Me, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    CScriptThing exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
    Game->SetRegionExitAsActive(exitPoint, false);
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                break;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (!PMasterData->HeroMarriedLadyGrey) {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                    face.Speak(Game->GetHero(), "TEXT_QST_088_MARRY_LADY_GREY_INTRO", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    }
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                }
                game->PauseAllNonScriptedEntities(false);
            } else {
                if (IsActiveThreadTerminating()) {
                    game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
                if (Game->GetHealth(face.GetScriptThing()) > g_fZero) {
                    face.Speak(Game->GetHero(), "TEXT_QST_088_MARRY_LADY_GREY_OPENING", (ETextGroupSelectionMethod)0, false, true, false);
                    while (face.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                            game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    }
                    if (IsActiveThreadTerminating()) {
                        game->PauseAllNonScriptedEntities(false);
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                }
                {
                    CCharString door("DD_MarryLadyGrey");
                    ParentClass->FadeOutFaceAndOpenDoor(Me, door);
                }
                Game->SetRegionExitAsActive(exitPoint, true);
                game->PauseAllNonScriptedEntities(false);
                break;
            }
        }
        Game->NewScriptFrame();
    }
#pragma inline_depth(0)
}
#pragma inline_depth()