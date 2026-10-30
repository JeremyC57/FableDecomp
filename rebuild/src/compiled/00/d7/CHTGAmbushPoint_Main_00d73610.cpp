// NScript::CQ_HangingTreeGoodScript::CHTG_AmbushPoint::Main() -- Hanging Tree (good) ambush point script: run script frames until the parent quest's tracked thing comes within 15 units of this point (bail out when the script is done), then have the quest spawn four enemies at the point with the point's data string, pause half a second and remove the point
// Leaf: Main
// Identity (naming wave 11): ?Main@CHTG_AmbushPoint@CQ_HangingTreeGoodScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012D271C slot 1 is stored by the factory 0x00D6FD70 registered as "HTG_AmbushPoint"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D73610, length 146 bytes.
// Callees: IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF (`__fastcall`),
// IsDone @ 0x00F35B30, CQ_HangingTreeGoodScript::SpawnEnemies(const C3DVector&, CCharString, long) @ 0x00D736B0;
// CScriptThing (vtable 0x01238C8C) slots 3 GetDataString (CCharString by value) and 6 GetPos; CGameScriptInterface
// slots 7 NewScriptFrame, 376 Pause(0.5f), 108 RemoveThing(Me, false, true).
// Layout = Ego_r.pdb CHTG_AmbushPoint (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14 (its script thing at +0x68 is the one tracked), PMasterData +0x18.
// Shape: a top-tested `while` (the distance test repeated at the bottom, aligned head) whose body returns on
// IsDone; the by-value CCharString argument is built by GetDataString straight into its argument slot
// (`push ecx ; push esp`), the position fetched last (arguments evaluate right to left).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // (not called: the getter builds the argument in place)
    ~CCharString();
};
class C3DVector { public: float x, y, z; };
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_HangingTreeGoodScript {
public:
    unsigned char _pad[0x68];
    CScriptThing  m_trackedThing;                  // +0x68
    void SpawnEnemies(const C3DVector& position, CCharString name, long count);   // 0x00D736B0
public:
    class CHTG_AmbushPoint;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_HangingTreeGoodScript;
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_HangingTreeGoodScript::CHTG_AmbushPoint : public CEntityScriptBase {
public:
        CQ_HangingTreeGoodScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_AmbushPoint::Main()
{
    while (!IsDistanceBetweenThingsUnder(Me, ParentClass->m_trackedThing, 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->SpawnEnemies(Me.GetPos(), Me.GetDataString(), 4);
    Game->Pause(0.5f);
    Game->RemoveThing(Me, false, true);
}