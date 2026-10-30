// CFireHeartHolder::Main() -- until the fire heart has been taken: each frame, when the hero uses the holder, play the (unnamed) movie sequence, pause the world, mark the heart taken, take the fire heart object from the hero, remove the holder's minimap marker and unpause; then idle until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF2D30, length 349 bytes (`ret`).
// Callees: the entity script's IsDone @ 0x00F35B30, CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, CScriptThing slot 20
//   MsgIsUsedBy(const CCharString&) (0x004AAB10), the movie handle's root ctor/dtor @ 0x0099A380 / 0x0099A430 and its PImp
//   counted-pointer dtor @ 0x006E7AB0 (vtables 0x01260EF4 / 0x0126008C); CGameScriptInterface slots 7 NewScriptFrame,
//   125 TakeObjectFromHero, 352 MiniMapRemoveMarker, 370 StartMovieSequence, 379 PauseAllNonScriptedEntities.
// Layout (the CEntityScriptBase prefix of the landed entity scripts): vptr, Game +4, CScriptThing Me +8, the owning quest
//   script +0x14 (fire heart taken flag +0x50).
// Shape: the pause/unpause pair goes through one named copy of Game (edi); the movie handle is a block local whose
//   implicit destructor releases PImp out of line and then runs the inline base destructor (vptr store + root dtor).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectBase : public CBaseObject {
public:
    CScriptGameResourceObjectBase() : CBaseObject() { }
    virtual ~CScriptGameResourceObjectBase() { }
};
class CScriptGameResourceObjectMovieBase : public CScriptGameResourceObjectBase {
public:
    CScriptGameResourceObjectMovieBase() : CScriptGameResourceObjectBase(), PImp() { }
    void Release();
    CCountedPointer<CScriptGameResourceObjectMovieBase> PImp;
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQuestScript_Retail {
    unsigned char _pad00[0x50];
    bool          FireHeartTaken;                       // +0x50
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CFireHeartHolder : public CEntityScriptBase {
public:
    void*                   __vftable;                  // +0x00
    CGameScriptInterface*   Game;                       // +0x04
    CScriptThing            Me;                         // +0x08
    CQuestScript_Retail*    Quest;                      // +0x14
    void Main();
};

void CFireHeartHolder::Main()
{
    while (!Quest->FireHeartTaken) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        bool used;
        {
            CCharString hero("SCRIPT_NAME_HERO");
            used = Me.MsgIsUsedBy(hero);
        }
        if (used) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase movie;
            {
                CCharString name("");
                Game->StartMovieSequence(name, movie);
            }
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Quest->FireHeartTaken = true;
            {
                CCharString fireHeart("OBJECT_LPDD_FIREHEART_01");
                Game->TakeObjectFromHero(fireHeart);
            }
            Game->MiniMapRemoveMarker(Me);
            game->PauseAllNonScriptedEntities(false);
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}