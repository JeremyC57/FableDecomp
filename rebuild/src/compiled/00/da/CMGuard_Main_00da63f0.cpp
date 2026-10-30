// CM_Guard::Main() -- Minion clifftop-chase guard: wait until the hero is within 20 units, spawn a CREATURE_BS_GUARD scripted "GuardToLeadTheWay" at this marker, tag it MCC4 the first time (remembering that on the quest) or MCC5 afterwards, then remove the marker
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA63F0, length 353 bytes.
// Callees: IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`), IsDone @ 0x00F35B30, CCharString(const char*, long)
//   @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("GuardToLeadTheWay" @ 0x012D71F4, "CREATURE_BS_GUARD" @ 0x012D2548,
//   "MCC4" @ 0x012D764C, "MCC5" @ 0x012D7644), ~CScriptThing @ 0x004AA840; CScriptThing slots 4 SetDataString
//   (0x004AA930, inlined and devirtualised on the local) and 6 GetPos; CGameScriptInterface (vtable 0x01260F0C) slots 7
//   NewScriptFrame, 70 GetHero, 91 CreateCreature (CScriptThing by value), 108 RemoveThing.
// Layout: Ego_r.pdb CM_Guard (28 bytes): entity-script base {vptr, Game +4, CScriptThing Me +8}, ParentClass
//   (CQ_MinionClifftopChaseScript*) +0x14 whose bool at +0x55 records that the first guard was tagged; CScriptThing as
//   engine/CScriptThing.h {vptr, PImp_Data +4, PImp_Info +8}, modelled locally for its virtuals.
// Shape: literals are implicit CCharString conversions (right to left, before the other arguments); each tag is a
//   SetDataString on the by-value guard local; the IsDone exits after the spawn only run the local's destructor.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };
class CCPPointerInfo;
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void SetDataString(const CCharString& text)   // slot 4 (+0x10), 0x004AA930
    {
        if (PImp_Data)
            PImp_Data->SetDataString(text);
    }
    virtual void v005();
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    virtual ~CScriptThing();                               // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_MinionClifftopChaseScript {
    unsigned char _pad[0x55];
    bool          FirstGuardTagged;                // +0x55
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CM_Guard : public CEntityScriptBase {
public:
    void*                         __vftable;       // +0x00
    CGameScriptInterface*         Game;            // +0x04
    CScriptThing                  Me;              // +0x08
    CQ_MinionClifftopChaseScript* ParentClass;     // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;     // +0x18
    void Main();
};

void CM_Guard::Main()
{
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing guard = Game->CreateCreature("CREATURE_BS_GUARD", Me.GetPos(), "GuardToLeadTheWay", false);
    if (!ParentClass->FirstGuardTagged) {
        if (IsActiveThreadTerminating())
            return;
        guard.SetDataString("MCC4");
        ParentClass->FirstGuardTagged = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        guard.SetDataString("MCC5");
    }
    Game->RemoveThing(Me, false, true);
}