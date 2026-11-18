// CCountedPointer<CConsoleInputBase>::Reset(CConsoleInputBase* data) -- drop the shared count block (destroying the object on last
// reference), then adopt the new object with a fresh count block whose deleter is DeleteData 0x009EDC50
// Leaf: ?Reset@?$CCountedPointer@VCConsoleInputBase@@@@QAEXPAVCConsoleInputBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EE540, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x009EDC50.
// Name (NF81, round 7, q10984 binder): FableWin.pdb 0x030B2950 ?Reset@?$CCountedPointer@VCConsoleInputBase@@@@QAEXPAVCConsoleInputBase@@@Z;
//   the deleter it stores, 0x009EDC50, is CCountedPointer<CConsoleInputBase>::DeleteData (FableWin.pdb 0x030B4840; stored the same way by
//   the landed CConsole::AddInput 0x009EC5E0). Was the stand-in CCountedPointer_009edc50::Reset (local SCountBlock / Destroy_009edc50 model).
// Port: the 0x00834E40 recipe (engine/CCountedPointer.h default Reset, one explicit instantiation; same 103-byte shape). CConsoleInputBase
//   (no shared header) is modelled with its virtual destructor only, as in 0x009EDC50 / 0x009EC5E0, so the DeleteData COMDAT this TU
//   instantiates is the same polymorphic delete as the 0x009EDC50 row (a forward declaration would give an incomplete-type delete).
#include "engine/CCountedPointer.h"
class CConsoleInputBase {
public:
    virtual ~CConsoleInputBase();
};
template void CCountedPointer<CConsoleInputBase>::Reset(CConsoleInputBase*);
