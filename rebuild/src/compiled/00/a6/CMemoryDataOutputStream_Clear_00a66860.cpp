// CKeyboard::ClearAllMaintainedEvents() -- empty the maintained-event list, then zero the stored-event count
// Leaf: ?ClearAllMaintainedEvents@CKeyboard@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A66860, 26 bytes: push esi; mov esi,ecx; lea ecx,[esi+0x340c]; call 0x9e4710 (_List_base<CMaintainedEvent>::clear);
//   mov [esi+0x3404],0; pop esi; ret (the retail tail is reached by `jmp` from CInputManager::ClearEvents 0x009F55B5).
// Name fix (NF383, 2026-11-24): was the propagated label CMemoryDataOutputStream::Clear (a local class), whose real address is 0x00996B60 (five FableWin
//   callers: CWorld::SaveGameStateInternal, CDefinitionManager::CompressDefinitionBlock, CParticleProgram::OptimiseConstraints / Clear / ReadFromFile;
//   retail has no ICF, so one address carries one name). Identity: the FableWin (debug) CInputManager::ClearEvents calls
//   CJoystick / CMouse / CKeyboard::ClearAllMaintainedEvents in that order, and retail ClearEvents 0x009F5540 calls 0x009E42C0 / 0x00A66C00 /
//   0x00A66860 in that order; Ego_r.pdb lib_keyboard.obj ?ClearAllMaintainedEvents@CKeyboard@@QAEXXZ is 26 bytes (= retail), and 0x00A66860 sits in
//   retail's lib_keyboard.obj block (0x00A667C0 EventScanStart .. 0x00A66990 ~CKeyboard). Model: the shared retail class (engine/CKeyboard.h,
//   FABLE_RETAIL_CKEYBOARD: NoStoredEvents +0x3404, MaintainedEventsList +0x340c).
// NF136 (2026-11-20, q3887): the list clear is STLport's out-of-line _List_base<CMaintainedEvent>::clear (FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE),
//   ?clear@?$_List_base@VCMaintainedEvent@@V?$allocator@VCMaintainedEvent@@@std@@@std@@QAEXXZ (the landed 0x00A66990 / 0x00A66F80 spelling).

#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CKEYBOARD
#include "stlport/fable_containers.h"
#include "engine/CInputEvent.h"
#include "engine/CMaintainedEvent.h"
#include "engine/CKeyboard.h"

void CKeyboard::ClearAllMaintainedEvents()
{
    MaintainedEventsList.clear();
    NoStoredEvents = 0;
}
