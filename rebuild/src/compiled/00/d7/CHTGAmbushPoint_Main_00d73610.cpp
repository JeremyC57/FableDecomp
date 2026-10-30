// CHTG_AmbushPoint::Main() -- Hanging Tree (good) ambush point script: run script frames until the parent quest's tracked thing comes within 15 units of this point (bail out when the script is done), then have the quest spawn four enemies at the point with the point's data string, pause half a second and remove the point
// Leaf: Main
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
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CCharString GetDataString() const;     // slot 3 (+0x0c)
    virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    unsigned char _body[8];
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_HangingTreeGoodScript {
    unsigned char _pad[0x68];
    CScriptThing  m_trackedThing;                  // +0x68
    void SpawnEnemies(const C3DVector& position, CCharString name, long count);   // 0x00D736B0
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CHTG_AmbushPoint : public CEntityScriptBase {
public:
    void*                     __vftable;           // +0x00
    CGameScriptInterface*     Game;                // +0x04
    CScriptThing              Me;                  // +0x08
    CQ_HangingTreeGoodScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
    void Main();
};

void CHTG_AmbushPoint::Main()
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