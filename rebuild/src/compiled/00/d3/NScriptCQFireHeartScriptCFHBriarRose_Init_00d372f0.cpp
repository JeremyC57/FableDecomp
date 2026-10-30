// NScript::CQ_FireHeartScript::CFH_BriarRose::Init() -- makes the entity unkillable: Game->EntitySetAsKillable(Me, false, true)
// Leaf: ?Init@CFH_BriarRose@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D372F0, 22 bytes (+ int3 padding to 32), thiscall, plain `ret`.
// Identity (proven): NScript::CQ_FireHeartScript::CFH_BriarRose::Init(); evidence: this body is slot 2 (Init) of the RTTI-less entity-script vtable 0x012CD000 (.rdata 0x012CD008); that vtable is stored, after the CEntityScriptBase vtable 0x012C3224, by the landed factory 0x00D37240 ?Alloc@CFH_BriarRose@CQ_FireHeartScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z (`new` 0x1C bytes); the Ego_r/FableWin.pdb name sets hold ?Init@CFH_BriarRose@CQ_FireHeartScript@NScript@@EAEXXZ
// Label: the manifest's "sub_00d372f0" is a gap-scan placeholder (family J033: six one-call entity-script Main / Init bodies).
// Callee: CGameScriptInterfaceBase virtual slot 517 (+0x814) EntitySetAsKillable(const CScriptThing&, bool, bool) const, the slot
//   ghidra_out/cgsi_slots_aligned.tsv names ?EntitySetAsKillable@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z (implementation 0x0088EB30).
// Layout: Ego_r.pdb CEntityScriptBase (0x14) {vptr, Game (CGameScriptInterfaceBase*) +4, Me (CScriptThing, 0x0C) +8};
//   CFH_BriarRose (0x1C): ParentClass (CQ_FireHeartScript*) +0x14, PMasterData (CQ_SunnyvaleMasterData*) +0x18.
// Shape: `mov eax, ecx`, the interface vptr through Game, the two bools pushed as immediates, `add eax, 8` for &Me.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CScriptThing : public CBaseClass {               // 0x0C bytes, vtable 0x01238C8C
public:
    CCountedPointer<CScriptThing> PImp;                // +0x04
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CEntityScriptBase {                              // vtable 0x012C3224
public:
    virtual ~CEntityScriptBase();                      // slot 0
private:
    virtual void Main();                               // slot 1
    virtual void Init();                               // slot 2
public:
    CGameScriptInterfaceBase* Game;                    // +0x04
    CScriptThing              Me;                      // +0x08
};
class CQ_FireHeartScript {
public:
    class CFH_BriarRose;
};
class CQ_FireHeartScript::CFH_BriarRose : public CEntityScriptBase {   // vtable 0x012CD000
private:
    virtual void Init();                               // slot 2
public:
    CQ_FireHeartScript*            ParentClass;            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
};
void CQ_FireHeartScript::CFH_BriarRose::Init()
{
    Game->EntitySetAsKillable(Me, false, true);
}
}