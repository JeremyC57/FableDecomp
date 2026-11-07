#ifndef FABLE_ENGINE_CEXPRESSION_PICKPOCKETSCRIPT_H
#define FABLE_ENGINE_CEXPRESSION_PICKPOCKETSCRIPT_H
// New header (chunk 841), modelled on engine/CExpression_FishScript.h: the global "Expression_Pickpocket" script. Ego_r.pdb
// CExpression_PickpocketScript (struct_layouts_egor.tsv, 88 bytes there, no own members; retail's CScriptBase is 0x40, so retail
// is 0x44): Game +0x40 (the landed ctor 0x00EEAEC0 stores it). Script vtable 0x012F56A0 (landed Alloc 0x00EEB730): slot 0 scalar
// deleting dtor 0x00EEB760, 1 RegisterMain 0x00EEB670, 2 Main 0x00EEAEF0, 3 Init 0x00EEAEE0; Ego_r.pdb + FableWin.pdb spell each
// ?<Name>@CExpression_PickpocketScript@NScript@@UAEXXZ (ctor ??0CExpression_PickpocketScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CExpression_PickpocketScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_PickpocketScript : public CScriptBase {
public:
    CExpression_PickpocketScript(CGameScriptInterfaceBase&);             // 0x00EEAEC0
    virtual void RegisterMain();                                         // 0x00EEB670 (slot 1)
    virtual void Main();                                                 // 0x00EEAEF0 (slot 2)
    virtual void Init();                                                 // 0x00EEAEE0 (slot 3)
    CGameScriptInterfaceBase* Game;                                      // +0x40
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CExpression_PickpocketScript) == 0x44);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CEXPRESSION_PICKPOCKETSCRIPT_H
