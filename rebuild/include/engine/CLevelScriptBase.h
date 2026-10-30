#ifndef FABLE_ENGINE_CLEVELSCRIPTBASE_H
#define FABLE_ENGINE_CLEVELSCRIPTBASE_H
// Retail model (hand-written 2026-10-29, wave 1 class-model migration): template <class T> class CLevelScriptBase :
// public CScriptBase (Ego_r.pdb CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>: 0x5C = CScriptBase 0x54 + Game +0x54 +
// MasterScriptData +0x58; retail CScriptBase is 0x40, so Game +0x40, MasterScriptData +0x44, 0x48 bytes: the landed quest
// rows read Game at +0x40). No new virtuals (Ego_r/FableWin: ctor (CScriptDataBase*, CGameScriptInterfaceBase&), virtual
// dtor, GetMasterData).
// The ctor is inline (wave 7): every level-script ctor and Alloc expands it -- `call 0x00CB8110` (CScriptBase()) followed
// at once by the two member stores, Game +0x40 then MasterScriptData +0x44, with the dead CLevelScriptBase<T> vtable store
// dropped (e.g. 0x00D61C30, the Alloc family 0x00EED220); Ego_r.pdb (release) has no ??0?$CLevelScriptBase COMDAT, while
// retail keeps five unreferenced out-of-line copies 0x00CDD160..0x00CDD220 (one per T, vtables 0x012C2EEC..0x012C2F3C),
// probably an explicit instantiation of the inline member (unproven: no caller, no TU evidence). The parameter is CScriptDataBase*,
// the member T* (T derives from CScriptDataBase at offset 0, so the conversion emits no code).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CLevelScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;

template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    virtual ~CLevelScriptBase();
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CLEVELSCRIPTBASE_H
