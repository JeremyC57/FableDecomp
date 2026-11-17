// NScript::CQ_EndGameScript::CM_UndeadSpawn::Main() -- make the generator spawn GatewayUndead creatures and enable it, wait (frame by frame) until the undead bar reaches the script def's maximum, then remove the generator
// Leaf: ?Main@CM_UndeadSpawn@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D29160, 171 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the RTTI-less vtable 0x012CBC58, which the entity Alloc 0x00D290A0 (`NScript::CQ_EndGameScript::CM_UndeadSpawn::Alloc`) stores; ?Main@CM_UndeadSpawn@CQ_EndGameScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d29160" is a gap-scan placeholder.
// Callees: CEntityScriptBase::IsActiveThreadTerminating() const 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 551 CreatureGeneratorSetGeneratedCreatureScriptName, 557 SetCreatureGeneratorEnabled, 7 NewScriptFrame, 108 RemoveThing.
// Data: "GatewayUndead" @ 0x012CBB24; pGlobalScriptDef 0x0143E90C (+0xbe4 UndeadBarMaximum, as the landed CQ_EndGameScript::CheckRegionLeaving 0x00D27950).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent UndeadBar +0x48 (Ego_r.pdb +0x5c).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a local stand-in)
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
#include "engine/CCountedPointer.h"
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
// agentNF60: the local CScriptDef model named these members descriptively; engine/CScriptDef.h (Ego_r / FableWin names at the
//   retail offsets) names them: UndeadBarMaximum -> EndGame_UpperBarLimit (+0xbe4).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameScript {
public:
    unsigned char _pad000[0x48];
    float UndeadBar;                                    // +0x48 (Ego_r.pdb +0x5c)
    class CM_UndeadSpawn;
};
class CQ_EndGameScript::CM_UndeadSpawn : public CEntityScriptBase {
public:
    CQ_EndGameScript*         ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void Main();
};
}

void NScript::CQ_EndGameScript::CM_UndeadSpawn::Main()
{
    Game->CreatureGeneratorSetGeneratedCreatureScriptName(Me, "GatewayUndead");
    Game->SetCreatureGeneratorEnabled(Me, true);
    while (ParentClass->UndeadBar < pGlobalScriptDef->EndGame_UpperBarLimit) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveThing(Me, false, true);
}