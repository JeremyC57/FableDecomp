// NScript::CQ_OpeningGraveyardSecretPassageScript::CGraveyardDemon::Main() -- the graveyard secret-passage demon door: take scripted control of Me, then until the door changes, each time the hero talks to it play a paused movie in which the demon speaks its first, second, then (every later time) third line
// Leaf: ?Main@CGraveyardDemon@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line, as retail calls them here).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC3FC0, 1111 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CBaseIntelligentPointer ctor 0x0099A380 / dtor 0x0099A430, scripted-thing handle IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / out-of-line dtor 0x007E74D0, movie PImp dtor
//   0x006E7AB0 / movie out-of-line dtor 0x006E7B80, CScriptThing copy 0x004ABE90 / dtor 0x004AA840, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, operator delete 0x00BFE9BC, CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 8,
//   70 GetHero, 264 GetHealth, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities; CScriptThing slot 27 MsgIsTalkedToBy.
// Layout: Ego_r.pdb CGraveyardDemon (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_OpeningGraveyardSecretPassageScript: DoorChange PDB +0x5d = retail +0x49, the usual 0x14 quest-script shift),
//   PMasterData +0x18, TimesSpoken +0x1c. Handle model as the landed CDF_ShineLight::Main 0x00E6EA30; movie as a
//   destructor-less view destroyed by hand (landed CTargetForKey::Main 0x00CE15E0): retail expands its destructor inline
//   on the normal path and at the in-loop exit, and calls the out-of-line copy at the after-loop exit.
// Naming wave 35: identity reg 00dc2ce8 "GraveyardDemon" in 00dc27c0; Alloc 00dc3ee0; vtable 012da540 slot 1; PDB ?Main@CGraveyardDemon@CQ_OpeningGraveyardSecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CGraveyardDemon::Main (wave3/labels_alloc.tsv); PDB ?Main@CGraveyardDemon@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CGraveyardDemon@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_OpeningGraveyardSecretPassageScript moved into NScript); same bytes.
#include "fable_compiler.h"
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
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
#include "engine/C3DVector.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    FABLE_FORCEINLINE ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
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
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    // implicit destructor; out-of-line copy 0x007E74D0
    bool IsValid() const;                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    virtual void Speak(const CScriptThing&, const char*, ETextGroupSelectionMethod, bool, bool, bool);   // 0x007E7390
public:
    virtual CScriptThing GetScriptThing() const;   // 0x007E7490
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
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
    FABLE_FORCEINLINE CScriptGameResourceObjectMovieBase() { PImp.Data = 0; PImp.Info = 0; }
    FABLE_FORCEINLINE void DestroyInline()
    {
        reinterpret_cast<CMovieImpPointer*>(&PImp)->~CMovieImpPointer();
        reinterpret_cast<CScriptGameResourceObjectBase*>(this)->CScriptGameResourceObjectBase::~CScriptGameResourceObjectBase();
    }
    FABLE_FORCEINLINE void Destroy()
    {
        reinterpret_cast<CScriptGameResourceObjectMovieFull*>(this)->CScriptGameResourceObjectMovieFull::~CScriptGameResourceObjectMovieFull();
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript {
public:
    unsigned char _pad[0x49];
    bool DoorChange;                                    // +0x49 (PDB +0x5d)
public:
    class CGraveyardDemon;           // the entity class below (naming wave 35)
};
}
using NScript::CQ_OpeningGraveyardSecretPassageScript;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript::CGraveyardDemon : public CEntityScriptBase {
public:
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;   // +0x14
    void*                PMasterData;                   // +0x18
    long                 TimesSpoken;                   // +0x1c
private:
    virtual void Main();                          // PDB ?Main@CGraveyardDemon@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
};
}

void NScript::CQ_OpeningGraveyardSecretPassageScript::CGraveyardDemon::Main()
{
    CScriptGameResourceObjectScriptedThingBase demon;
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (demon.IsValid())
        demon.Release();
    while (!Game->StartScriptingEntity(Me, demon, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
                return;
            }
    }
    if (IsActiveThreadTerminating())
        {
            return;
        }
    while (!ParentClass->DoorChange) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
                return;
            }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                {
                    return;
                }
            CMovieStorage movieStorage;
            CScriptGameResourceObjectMovieBase& movie = *new (&movieStorage) CScriptGameResourceObjectMovieBase;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            switch (TimesSpoken++) {
            case 0:
                if (Game->GetHealth(demon.GetScriptThing()) > 0.0f) {
                    demon.Speak(Game->GetHero(), "TEXT_QST_049_DEMON_DOOR_KEEPER_FIRST", (ETextGroupSelectionMethod)0, false, true, false);
                    while (demon.IsPerformingScriptTask()) {
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
                break;
            case 1:
                if (Game->GetHealth(demon.GetScriptThing()) > 0.0f) {
                    demon.Speak(Game->GetHero(), "TEXT_QST_049_DEMON_DOOR_KEEPER_SECOND", (ETextGroupSelectionMethod)0, false, true, false);
                    while (demon.IsPerformingScriptTask()) {
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
                break;
            default:
                if (Game->GetHealth(demon.GetScriptThing()) > 0.0f) {
                    demon.Speak(Game->GetHero(), "TEXT_QST_049_DEMON_DOOR_KEEPER_THIRD", (ETextGroupSelectionMethod)0, false, true, false);
                    while (demon.IsPerformingScriptTask()) {
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
                break;
            }
            game->PauseAllNonScriptedEntities(false);
            movie.DestroyInline();
        }
    }
    if (IsActiveThreadTerminating())
        {
            return;
        }
}