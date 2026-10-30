// NScript::CQ_RansomVictimScript::CRansomHelper::Init() -- makes the entity unpushable by the hero: Game->SetIsPushableByHero(Me, false), Me passed by value
// Leaf: ?Init@CRansomHelper@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE1E20, 49 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (proven): NScript::CQ_RansomVictimScript::CRansomHelper::Init(); evidence: this body is slot 2 (Init) of the RTTI-less entity-script vtable 0x012DD1E4 (.rdata 0x012DD1EC); that vtable is stored, after the CEntityScriptBase vtable 0x012C3224, by the landed factory 0x00DE1D00 ?Alloc@CRansomHelper@CQ_RansomVictimScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z (`new` 0x1C bytes); the Ego_r/FableWin.pdb name sets hold ?Init@CRansomHelper@CQ_RansomVictimScript@NScript@@EAEXXZ
// Label: the manifest's "sub_00de1e20" is a gap-scan placeholder (family J038: six one-call entity-script Init bodies).
// Callee: CGameScriptInterfaceBase virtual slot 844 (+0xD30) SetIsPushableByHero(CScriptThing, bool) (ghidra_out/cgsi_slots_aligned.tsv:
//   ?SetIsPushableByHero@CGameScriptInterface@@UAEXVCScriptThing@@_N@Z, implementation 0x008A6DD0); the by-value CScriptThing is
//   copy-constructed inline in the argument slot (CBaseClass vtable 0x01231710, then CScriptThing 0x01238C8C, PImp shared with
//   the count bumped) and destroyed by the callee.
// Layout: Ego_r.pdb CEntityScriptBase (0x14) {vptr, Game (CGameScriptInterfaceBase*) +4, Me (CScriptThing, 0x0C) +8};
//   CRansomHelper (0x1C): ParentClass (CQ_RansomVictimScript*) +0x14, PMasterData (CQ_SunnyvaleMasterData*) +0x18.
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
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
class CQ_RansomVictimScript {
public:
    class CRansomHelper;
};
class CQ_RansomVictimScript::CRansomHelper : public CEntityScriptBase {   // vtable 0x012DD1E4
private:
    virtual void Init();                               // slot 2
public:
    CQ_RansomVictimScript*            ParentClass;            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
};
void CQ_RansomVictimScript::CRansomHelper::Init()
{
    Game->SetIsPushableByHero(Me, false);
}
}