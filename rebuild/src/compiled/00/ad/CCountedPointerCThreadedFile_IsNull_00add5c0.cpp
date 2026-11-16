// CCountedPointer<CThreadedFile>::IsNull() const -- true when the counted pointer holds no threaded file
// Leaf: ?IsNull@?$CCountedPointer@VCThreadedFile@@@@QBE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADD5C0, 10 bytes: `mov edx,[ecx] ; xor eax,eax ; test edx,edx ; sete al ; ret` (thiscall, bool).
//   No reference (no call, jump or data cell): an out-of-line COMDAT the linker kept.
// Identity: INFERRED (recorded in NOTES). It sits in the LHA CLHFile translation unit, between the two landed CLHFile constructors
//   0x00ADD380 / 0x00ADD4A0 and the CLHFileCompletionCallback / CLHFile scalar deleting destructors 0x00ADD5D0 / 0x00ADD5F0. It is
//   not a CLHFile member (a CLHFile's first word is its vptr 0x0129F094, never zero); the one object of that TU whose first word is
//   a pointer is the CLHFile's CCountedPointer<CThreadedFile> member (Data +0, Info +4; Reset 0x009A9C80 is called out of line by
//   both constructors), and `return Data == 0;` is the engine's CCountedPointer<T>::IsNull (FableWin.pdb ?IsNull@?$CCountedPointer@
//   ...@@QBE_NXZ for 343 T's; engine/CCountedPointer.h notes no retail out-of-line copy had been found). FableWin.pdb has no
//   CLHFile / LHA objects, so the instance name for T = CThreadedFile cannot be confirmed from a PDB; the TU's member type gives T.
//   The draft's class name CLHFile_00add5c0 is replaced.
// Header: engine/CCountedPointer.h (default branch: `bool IsNull() const { return Data == 0; }`).
#include "engine/CCountedPointer.h"

class CThreadedFile;

template bool CCountedPointer<CThreadedFile>::IsNull() const;   // emits the out-of-line COMDAT