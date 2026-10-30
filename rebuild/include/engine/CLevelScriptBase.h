#ifndef FABLE_ENGINE_CLEVELSCRIPTBASE_H
#define FABLE_ENGINE_CLEVELSCRIPTBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): template <class T> class CLevelScriptBase :
// public CScriptBase (Ego_r.pdb CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>: 0x5C = CScriptBase 0x54 + Game +0x54 +
// MasterScriptData +0x58; retail CScriptBase is 0x40, so Game +0x40, MasterScriptData +0x44, 0x48 bytes: the landed quest
// rows read Game at +0x40). No new virtuals (Ego_r/FableWin: ctor (CScriptDataBase*, CGameScriptInterfaceBase&), virtual
// dtor, GetMasterData).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CLevelScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;

template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual ~CLevelScriptBase();
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CLEVELSCRIPTBASE_H
