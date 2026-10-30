// NScript::CPersonalScript_GlobalThingsScript::CClickableDoor::Main() -- shut the door, then every frame until the thread terminates toggle IsDoorOpen and open or close the door whenever the hero uses it
// Leaf: ?Main@CClickableDoor@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDEED0, 179 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the RTTI-less vtable 0x012C32A4, which the entity Alloc 0x00CDEE10 (`NScript::CPersonalScript_GlobalThingsScript::CClickableDoor::Alloc`) stores; ?Main@CClickableDoor@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00cdeed0" is a gap-scan placeholder.
// Callees: CEntityScriptBase::IsActiveThreadTerminating() const 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CScriptThing::MsgIsUsedBy (slot 20); interface slots 7 NewScriptFrame, 425 OpenDoor, 426 CloseDoor.
// Data: "SCRIPT_NAME_HERO" @ 0x0125D1C8.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). CClickableDoor: IsDoorOpen +0x1c.
// Shape: the while loop is rotated (test before the body and after NewScriptFrame); every terminating test inside returns.
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
class CPersonalScript_GlobalThingsScript {
public:
    class CClickableDoor;
};
class CPersonalScript_GlobalThingsScript::CClickableDoor : public CEntityScriptBase {
public:
    CPersonalScript_GlobalThingsScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      IsDoorOpen;                 // +0x1c
private:
    virtual void Main();
};
}

void NScript::CPersonalScript_GlobalThingsScript::CClickableDoor::Main()
{
    Game->CloseDoor(Me);
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            IsDoorOpen = !IsDoorOpen;
            if (IsDoorOpen) {
                if (IsActiveThreadTerminating()) return;
                Game->OpenDoor(Me);
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->CloseDoor(Me);
            }
        }
        Game->NewScriptFrame();
    }
}