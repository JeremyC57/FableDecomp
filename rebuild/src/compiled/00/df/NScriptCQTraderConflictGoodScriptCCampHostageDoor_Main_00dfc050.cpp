// NScript::CQ_TraderConflictGoodScript::CCampHostageDoor::Main() -- the bandit-camp hostage cage door: closed until the hero uses it, then opened and the parent's OpenedCage set
// Leaf: ?Main@CCampHostageDoor@CQ_TraderConflictGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFC050, 134 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): slot 1 (Main) of the RTTI-less entity vtable 0x012DEE30, which the landed
//   NScript::CQ_TraderConflictGoodScript::CCampHostageDoor::Alloc 0x00DFBFA0 stores (site 0x00DFBFD6); Ego_r.pdb has ?Main@CCampHostageDoor@CQ_TraderConflictGoodScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CCampHostageDoor (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 426 CloseDoor, 425 OpenDoor, 7 NewScriptFrame; CScriptThing slot 20 MsgIsUsedBy;
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; IsActiveThreadTerminating 0x00F35B30.
//   Parent CQ_TraderConflictGoodScript: Ego_r OpenedCage +0x7d = retail +0x65 (- 0x14, - 4 for the vector AllCreatures).
// Shape: retail's single-test loop is `do { if (used) { ...; return; } frame; } while (!terminating);`.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderConflictGoodScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    std::vector<CScriptThing> AllCreatures;             // +0x48
    long TradersFollowing;                              // +0x54
    long TradersReachedTeleporter;                      // +0x58
    long BanditSecurityLinesSaid;                       // +0x5c
    bool FirstRegionDead;                               // +0x60
    bool EnteredNewRegion;                              // +0x61
    bool IntroDone;                                     // +0x62
    bool OutroStart;                                    // +0x63
    bool OutroDone;                                     // +0x64
    bool OpenedCage;                                    // +0x65
    bool CommentedOnBanditCostume;                      // +0x66
    bool MissionSucceeded;                              // +0x67
    class CCampHostageDoor;
};
class CQ_TraderConflictGoodScript::CCampHostageDoor : public CEntityScriptBase {
public:
    CQ_TraderConflictGoodScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Main();
};

void CQ_TraderConflictGoodScript::CCampHostageDoor::Main()
{
    Game->CloseDoor(Me);
    if (IsActiveThreadTerminating())
        return;
    do {
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->OpenDoor(Me);
            ParentClass->OpenedCage = true;
            return;
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}