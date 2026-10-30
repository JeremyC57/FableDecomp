// NScript::CQ_HobbeCaveScript::CJames::Init() -- set up James: unpushable and not force-pushable, ignores hits, cannot be damaged or killed, no combo multiplier, narrated as JAMES; the shout-for-help timer is reset to 0 and he has not reached granny's house
// Leaf: ?Init@CJames@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D900E0, 198 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D5A1C, which the entity Alloc 0x00D8FFA0 (`NScript::CQ_HobbeCaveScript::CJames::Alloc`) stores; ?Init@CJames@CQ_HobbeCaveScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d900e0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 844 SetIsPushableByHero (by value), 745 SetIsThingForcePushable, 515 EntitySetAsRespondToHit, 516 EntitySetAsDamageable, 517 EntitySetAsKillable, 526 EntitySetAsToAddToComboMultiplierWhenHit, 362 EntitySetWillBeUsingNarrator; CTimer::operator=(long) inline: the global script interface 0x0143E8F8 slot 89 SetTimer.
// Data: "JAMES" @ 0x012D5A38.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). CJames: JamesHealthID +0x1c, ShoutForHelp (CTimer) +0x20, ReachedGrannysHouse +0x24.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase / CGameScriptInterface are the retail script interface from the shared headers
//   (FABLE_RETAIL_NSCRIPT opt-in: CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface, bound name)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeCaveScript {
public:
    class CJames;
};
class CQ_HobbeCaveScript::CJames : public CEntityScriptBase {
public:
    CQ_HobbeCaveScript*       ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      JamesHealthID;              // +0x1c
    CTimer                    ShoutForHelp;               // +0x20
    bool                      ReachedGrannysHouse;        // +0x24
private:
    virtual void Init();
};
}

void NScript::CQ_HobbeCaveScript::CJames::Init()
{
    Game->SetIsPushableByHero(Me, false);
    Game->SetIsThingForcePushable(Me, false);
    Game->EntitySetAsRespondToHit(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetWillBeUsingNarrator(Me, "JAMES");
    ShoutForHelp = 0;
    ReachedGrannysHouse = false;
}