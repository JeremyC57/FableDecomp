#ifndef FABLE_ENGINE_CEXPRESSION_STEALSCRIPT_H
#define FABLE_ENGINE_CEXPRESSION_STEALSCRIPT_H
// Retail model (p25B, row 0x00EEBDF0; new header): NScript::CExpression_StealScript, the global "Expression_Steal" script
// (CScriptInfoManager::RegisterScripts 0x00CD52D0 registers its Alloc 0x00EEC390 under that name, site 0x00CD9F16).
// PDB scope: namespace NScript, class key V (Ego_r.pdb ?Main@CExpression_StealScript@NScript@@UAEXXZ, ?RegisterMain@..@@UAEXXZ,
// ?Alloc@CExpression_StealScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z; Ego_d.pdb procs
// Alloc, the ctor, RegisterMain, Init, Main, the dtors).
// Same shape as engine/CExpression_FishScript.h: the ctor takes only the interface, the base is CScriptBase with its own Game
// pointer. Layout: Ego_r.pdb CExpression_StealScript 88 bytes = CScriptBase 0x54 + Game; retail CScriptBase is 0x40, so Game +0x40
// (every Game call of Main 0x00EEBDF0 reads [esi+40h]), sizeof 0x44.
// Retail methods: the script vtable 0x012F5700 (its references 0x00AEBDD1 / 0x00AEC3B1 are the ctor 0x00EEBDC0 and Alloc 0x00EEC390
// stores): slot 0 scalar deleting dtor 0x00EEC3C0, 1 RegisterMain 0x00EEC2D0 (landed), 2 Main 0x00EEBDF0, 3 Init 0x00EEBDE0, 4 OnPersist
// = CScriptBase's 0x00CBD4E0.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CExpression_StealScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;
namespace NScript {
class CExpression_StealScript : public CScriptBase {
public:
    CExpression_StealScript(CGameScriptInterfaceBase&);                  // 0x00EEBDC0
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00EEC390
    virtual void RegisterMain();                                         // 0x00EEC2D0 (slot 1)
    virtual void Main();                                                 // 0x00EEBDF0 (slot 2)
    virtual void Init();                                                 // 0x00EEBDE0 (slot 3)
    CGameScriptInterfaceBase* Game;                                      // +0x40
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CExpression_StealScript) == 0x44);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CEXPRESSION_STEALSCRIPT_H
