// NScript::CQ_HerosOldHouseScript::CFishermansWife::Init() -- clear TalkedTo/LeavingHappy, remember Me's facing angle, clear its information flags and, while the hero is helping but has not yet helped, give it the vignette mini-map orb
// Leaf: ?Init@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8A700, 108 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D50DC, which the entity Alloc 0x00D8A650 (`NScript::CQ_HerosOldHouseScript::CFishermansWife::Alloc`) stores; ?Init@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d8a700" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CScriptThing::GetAngleXY (slot 10, +0x28); interface slots 360 SetThingHasInformation, 350 MiniMapAddMarker.
// Data: "HUD_ORB_QUEST_VIGNETTE" @ 0x01258A64.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CFishermansWife: TalkedTo +0x1c, LeavingHappy +0x1d, initial_angle +0x20; parent Helping +0x48, Helped +0x49.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HerosOldHouseScript {
public:
    class CFishermansWife;
    unsigned char _pad000[0x48];
    bool Helping;                                       // +0x48 (retail = Ego_r.pdb - 0x14)
    bool Helped;                                        // +0x49
};
class CQ_HerosOldHouseScript::CFishermansWife : public CEntityScriptBase {
public:
    CQ_HerosOldHouseScript*   ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      TalkedTo;                   // +0x1c
    bool                      LeavingHappy;               // +0x1d
    float                     initial_angle;              // +0x20
private:
    virtual void Init();
};
}

void NScript::CQ_HerosOldHouseScript::CFishermansWife::Init()
{
    TalkedTo = false;
    LeavingHappy = false;
    initial_angle = Me.GetAngleXY();
    Game->SetThingHasInformation(Me, false, false, false);
    if (ParentClass->Helping && !ParentClass->Helped)
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_VIGNETTE");
}