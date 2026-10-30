// NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Init() -- set up the tortured mother: brain state 1, not following the hero, cannot be damaged, a quest-core marker, only the minion follow-thing state group, no follow/wait responses, persistent
// Leaf: ?Init@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD6950, 175 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012DC108, which the entity Alloc 0x00DD68A0 (`NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Alloc`) stores; ?Init@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00dd6950" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 235 EntitySetAsAllowedToFollowHero, 516 EntitySetAsDamageable, 350 MiniMapAddMarker, 591 EntitySetAllStategroupsEnabled, 590 EntitySetStategroupEnabled, 237 EntitySetAsRespondingToFollowAndWaitExpressions, 477 SetThingPersistent.
// Data: "HUD_ORB_QUEST_CORE" @ 0x01258AB0, "SG_MINION_FOLLOW_THING" @ 0x0128A814.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). CMotherTorture: BrainState +0x1c.
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
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonEscapeRescueMotherScript {
public:
    class CMotherTorture;
};
class CQ_PrisonEscapeRescueMotherScript::CMotherTorture : public CEntityScriptBase {
public:
    CQ_PrisonEscapeRescueMotherScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      BrainState;                 // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Init()
{
    BrainState = 1;
    Game->EntitySetAsAllowedToFollowHero(Me, false);
    Game->EntitySetAsDamageable(Me, false);
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->EntitySetAllStategroupsEnabled(Me, false);
    Game->EntitySetStategroupEnabled(Me, "SG_MINION_FOLLOW_THING", true);
    Game->EntitySetAsRespondingToFollowAndWaitExpressions(Me, false);
    Game->SetThingPersistent(Me, true);
}