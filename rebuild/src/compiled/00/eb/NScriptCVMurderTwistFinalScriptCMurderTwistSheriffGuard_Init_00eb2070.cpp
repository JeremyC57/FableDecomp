// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Init() -- the sheriff's guard: cannot be force-pushed, pushed, killed or damaged, no movement in actions, looks at the hero; its ID comes from the thing's data string and picks the _TOUGH or _WEAK speech suffix
// Leaf: ?Init@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB2070, 206 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012EFEE8, which the landed
//   constructor 0x00EB2000 (landed under the stand-in name CVtable_012efee8::CVtable_012efee8) of the
//   "MurderTwistSheriffGuard" binding's Alloc 0x00EB1F80 (bound at 0x00EAEAD0, name 0x012EF9D8) stores (site 0x00EB202B); Ego_r.pdb has ?Init@CMurderTwistSheriffGuard@CV_MurderTwistFinalScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMurderTwistSheriffGuard (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 745 SetIsThingForcePushable, 844 SetIsPushableByHero (Me by value), 529
//   EntitySetAsUseMovementInActions, 517 EntitySetAsKillable, 516 EntitySetAsDamageable, 70 GetHero, 498
//   EntityForceToLookAtThing; CScriptThing slot 3 GetDataString (CCharString by value), GFCharStringToInt 0x0099E7F0,
//   ~CCharString 0x0099EAE0, CCharString::operator=(const char*) 0x0099EFE0 ("_TOUGH" 0x012EFF04, "_WEAK" 0x012EFF0C).
class CCharString {
public:
    void* PData;
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);     // 0x0099EFE0
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0, implicit copy
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
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // slot 0
    virtual void t01(); virtual void t02();
    virtual CCharString GetDataString() const;          // slot 3 (+0xc)
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CScriptGameResourceObjectScriptedThingBase {      // Ego_r.pdb, 16 bytes: vptr, base word, PImp
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    long _base;                                         // +0x04 (unnamed in the PDB layout)
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_MurderTwistFinalScript {
public:
    class CMurderTwistSheriffGuard;
};
class CV_MurderTwistFinalScript::CMurderTwistSheriffGuard : public CEntityScriptBase {
public:
    CV_MurderTwistFinalScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    unsigned long NextLine;                             // +0x2c
    unsigned long ID;                                   // +0x30
    CCharString Name;                                   // +0x34
private:
    virtual void Init();
};

void CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Init()
{
    Game->SetIsThingForcePushable(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetAsUseMovementInActions(Me, false);
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntityForceToLookAtThing(Me, Game->GetHero());
    ID = GFCharStringToInt(Me.GetDataString());
    Name = ID == 0 ? "_WEAK" : "_TOUGH";
    NextLine = 1;
}
}