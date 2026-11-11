// CCountedPointer<CAProgressDisplay>::CCountedPointer(T*) -- take ownership of a new pointee: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCAProgressDisplay@@@@QAE@PAVCAProgressDisplay@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403D20, 78 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_SetPItem_00403d20 (stand-in name).
// T = CAProgressDisplay, proven by: CGameInitialiser::SetupProgressDisplay and CWorld::StartAutoSaveProgress wrap a new progress display in it before NProgressDisplay::SetProgressDisplay(CCountedPointer<CAProgressDisplay>&) (FableWin.pdb); 0x0049E7C0 binds it with a CAProgressDisplay* parameter; FableWin.pdb ??0?$CCountedPointer@VCAProgressDisplay@@@@QAE@PAVCAProgressDisplay@@@Z (and the WinMain TU's COMDAT set holds CAProgressDisplay).
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT (`ret 4`); the count
//   block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#include "engine/CCountedPointer.h"
class CAProgressDisplay;
template CCountedPointer<CAProgressDisplay>::CCountedPointer(CAProgressDisplay*);
