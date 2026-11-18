// CCountedPointer<CConsoleInputBase>::DeleteData(void* data) -- the counted pointer's per-type deleter: `delete (T*)data`, null-guarded,
// through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCConsoleInputBase@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EDC50, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall
//   member (data in ecx); only its address is used: it is stored as the count block's DeleteFunc.
// Name (NF81, round 7, q10984): FableWin.pdb 0x030B4840 ?DeleteData@?$CCountedPointer@VCConsoleInputBase@@@@SIXPAX@Z; the landed
//   CConsole::AddInput 0x009EC5E0 (NF78, RM) stores this address as the deleter of CCountedPointer<CConsoleInputBase>(input) through
//   engine/CCountedPointer.h, and the landed CCountedPointer<CConsoleInputBase>::Reset 0x009EE540 stores it too. Was the AutoTiny stand-in
//   _global::DeleteViaVtable0_009edc50 (leaf ?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the
//   COMDAT (the 0x0050C210 recipe). CConsoleInputBase has no shared header: modelled with only its virtual destructor (Ego_r.pdb
//   ??1CConsoleInputBase@@UAE@XZ, slot 0), as in 0x009EC5E0.
#include "engine/CCountedPointer.h"
class CConsoleInputBase {
public:
    virtual ~CConsoleInputBase();
};
template void FABLE_FASTCALL CCountedPointer<CConsoleInputBase>::DeleteData(void*);
