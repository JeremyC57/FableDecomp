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
// Retail opt-in (chunk 713): define FABLE_RETAIL_CLEVELSCRIPTBASE before the include for retail's compiler-generated
// ~CLevelScriptBase<T>. Evidence (two sources): (1) every implicit level-script dtor tail-jumps straight to ~CScriptBase
// 0x00CBD510 (0x00E32310 CV_AmbushScamScript, 0x00E398A0 CV_AssassinAttacksScript, 0x00E542B0 CV_BeardyBaldyScript,
// 0x00ED39F0 CV_SingingStonesScript, 0x00ED7640 CV_SwordInTheStoneScript, 0x00D005E0 ...), where the out-of-line declaration
// below makes VC7.1 jump to ??1?$CLevelScriptBase@... instead, and an inline `{}` adds a vtable store (chunks 664/665);
// (2) the six retail out-of-line copies 0x00CDD100..0x00CDD150 are bare `jmp ~CScriptBase` thunks (the PDBs' ??1?$CLevelScriptBase
// COMDATs, e.g. Ego_r 0x00CC2A70), i.e. an implicit dtor emitted out of line that no derived dtor calls. The default text is kept
// for the 1,062 landed includers; flipping the default is header debt (chunk 713 NOTES.md).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CLevelScriptBase.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;

template <class T> class CLevelScriptBase : public CScriptBase {
public:
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_BODYGUARD)
    CLevelScriptBase(CScriptDataBase* data,CGameScriptInterfaceBase& game):Game(game),MasterScriptData(*(T*)data) {}
#else
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
#endif
#ifndef FABLE_RETAIL_CLEVELSCRIPTBASE
    virtual ~CLevelScriptBase();
#endif
    T* GetMasterData();
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_BODYGUARD)
    CGameScriptInterfaceBase& Game;
    T& MasterScriptData;
#else
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
#endif
};

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CLEVELSCRIPTBASE_H
