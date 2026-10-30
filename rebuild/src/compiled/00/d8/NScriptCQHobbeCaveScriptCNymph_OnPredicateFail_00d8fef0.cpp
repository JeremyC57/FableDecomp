// NScript::CQ_HobbeCaveScript::CNymph::OnPredicateFail() -- when the nymph is killed: release the brat, record the perpetrator killed, deactivate the Q_HobbeCaveBarrier quest and, unless the innocent was sacrificed (state 2), reset that state to 0
// Leaf: ?OnPredicateFail@CNymph@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8FEF0, 136 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012D5A00, which the entity Alloc 0x00D8FE30 (`NScript::CQ_HobbeCaveScript::CNymph::Alloc`) stores; ?OnPredicateFail@CNymph@CQ_HobbeCaveScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d8fef0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CScriptThing::MsgIsKilledBy (slot 18) with the pooled empty string 0x0122D70E; interface slot 280 DeactivateQuest.
// Data: "Q_HobbeCaveBarrier" @ 0x012D56FC.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent ReleaseBrat +0x4f (Ego_r.pdb +0x63); CQ_SunnyvaleMasterData HobbeCavePerpertratorKilled +0x10d, HobbeCaveInnocentSacrificed +0x110.
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
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb offsets (same in retail)
public:
    unsigned char _pad000[0x10d];
    bool HobbeCavePerpertratorKilled;                   // +0x10d
    unsigned char _pad10e[0x2];
    long HobbeCaveInnocentSacrificed;                   // +0x110
};
class CQ_HobbeCaveScript {
public:
    unsigned char _pad000[0x4f];
    bool ReleaseBrat;                                   // +0x4f (Ego_r.pdb +0x63)
    class CNymph;
};
class CQ_HobbeCaveScript::CNymph : public CEntityScriptBase {
public:
    CQ_HobbeCaveScript*       ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      AttackWarning;              // +0x1c
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_HobbeCaveScript::CNymph::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->ReleaseBrat = true;
        PMasterData->HobbeCavePerpertratorKilled = true;
        Game->DeactivateQuest("Q_HobbeCaveBarrier", 0);
        if (PMasterData->HobbeCaveInnocentSacrificed != 2)
            PMasterData->HobbeCaveInnocentSacrificed = 0;
    }
}