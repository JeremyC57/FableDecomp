// NScript::CQ_HobbeCaveScript::CJames::OnPredicateFail() -- James was killed: fail the quest (ReasonForFailure 1, MissionFailed) and remove his health bar
// Leaf: ?OnPredicateFail@CJames@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D90050, 86 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012D5A1C, which the entity Alloc 0x00D8FFA0 (`NScript::CQ_HobbeCaveScript::CJames::Alloc`) stores; ?OnPredicateFail@CJames@CQ_HobbeCaveScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d90050" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::MsgIsKilledBy (slot 18, +0x48; CScriptThing vtable 0x01238C8C); interface slot 338 RemoveQuestInfoElement(long).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CJames: JamesHealthID +0x1c; parent MissionFailed +0x4c, ReasonForFailure +0x70.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
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
};
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
class CQ_HobbeCaveScript {
public:
    class CJames;
    unsigned char _pad000[0x4c];
    bool MissionFailed;                                 // +0x4c (retail = Ego_r.pdb - 0x14)
    unsigned char _pad04d[0x23];
    long ReasonForFailure;                              // +0x70
};
class CQ_HobbeCaveScript::CJames : public CEntityScriptBase {
public:
    CQ_HobbeCaveScript*       ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      JamesHealthID;              // +0x1c
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_HobbeCaveScript::CJames::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->ReasonForFailure = 1;
        ParentClass->MissionFailed = true;
        Game->RemoveQuestInfoElement(JamesHealthID);
    }
}