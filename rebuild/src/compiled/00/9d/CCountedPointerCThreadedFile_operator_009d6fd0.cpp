// CCountedPointer<CThreadedFile>::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this
// Leaf: ??4?$CCountedPointer@VCThreadedFile@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D6FD0, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_009d6fd0 (stand-in name).
// T = CThreadedFile, proven by: CBankFileAsync::OpenReadOnly 0x009D56C0 binds CCountedPointer<CThreadedFile>::operator= (assigning ThreadedFile: Ego_r CBankFileAsync +0x174 CCountedPointer<CThreadedFile>); FableWin.pdb ??4 name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CThreadedFile;
template CCountedPointer<CThreadedFile>& CCountedPointer<CThreadedFile>::operator=(const CCountedPointer<CThreadedFile>&);
