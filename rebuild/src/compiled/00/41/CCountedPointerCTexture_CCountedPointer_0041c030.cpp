// CCountedPointer<CTexture>::CCountedPointer(T*) -- take ownership of a new pointee: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCTexture@@@@QAE@PAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C030, 78 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _unattributed::CCountedPointer_0041c030 (stand-in name).
// T = CTexture, proven by: NUISystem::CManager::Action 0x0041E6D3 binds ??0?$CCountedPointer@VCTexture@@@@QAE@PAVCTexture@@@Z (engine/CCountedPointer.h: the CTexture ctor COMDAT 0x0041C030 in LOAD_PHOTO); the same function assigns it through operator=<CTexture> 0x00429A62 and releases it through ReleaseData<CTexture> 0x0041BB30; FableWin.pdb (T*) ctor name.
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT (`ret 4`); the count
//   block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#include "engine/CCountedPointer.h"
class CTexture;
template CCountedPointer<CTexture>::CCountedPointer(CTexture*);
