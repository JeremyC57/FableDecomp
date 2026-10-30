// NScript::CV_TravellingHeroesScript::CTH_Trigger_NorthernWastes::Main() -- the Northern Wastes trigger: while it is alive, make it persistent, wait until the hero is within 5 units, then play the snow-rose cutscene and remove the trigger
// Leaf: ?Main@CTH_Trigger_NorthernWastes@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE3FC0, 219 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): slot 1 (Main) of the RTTI-less entity vtable 0x012F4438, which the landed
//   NScript::CV_TravellingHeroesScript::CTH_Trigger_NorthernWastes::Alloc 0x00EE1AF0 stores (site 0x00EE1B24); Ego_r.pdb has ?Main@CTH_Trigger_NorthernWastes@CV_TravellingHeroesScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CTH_Trigger_NorthernWastes (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: NScript::CIsThingAlive (vtable 0x012C32F0, inline ctor over CScriptThing copy 0x004ABE90, out-of-line dtor
//   0x00E241B0), SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF
//   (__fastcall), CV_TravellingHeroesScript::PlayCutscene 0x00EE2DB0 ("CS_TRAVHERO_SNOWROSE" 0x012F46A0 built in its by-value
//   slot), CCharString(const char*, long) 0x0099EBF0; interface slots 7 NewScriptFrame, 477 SetThingPersistent, 70 GetHero,
//   108 RemoveThing.
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase {                                  // Ego_r.pdb NScript::CPredicateBase
public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsThingAlive : public CPredicateBase {           // vtable 0x012C32F0
public:
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    virtual ~CIsThingAlive();                           // 0x00E241B0
};
}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
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
class CV_TravellingHeroesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void PlayCutscene(CCharString name);               // 0x00EE2DB0 (landed)
    class CTH_Trigger_NorthernWastes;
};
class CV_TravellingHeroesScript::CTH_Trigger_NorthernWastes : public CEntityScriptBase {
public:
    CV_TravellingHeroesScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Main();
};

void CV_TravellingHeroesScript::CTH_Trigger_NorthernWastes::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingPersistent(Me, true);
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->PlayCutscene("CS_TRAVHERO_SNOWROSE");
    Game->RemoveThing(Me, false, true);
}
}