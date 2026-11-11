// CCountedPointer::CCountedPointer(const CCountedPointer&) -- copy: take Data and Info, then add a reference (T = NCameraModes::CBase)
// Leaf: ??0?$CCountedPointer@VCBase@NCameraModes@@@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056C050, 25 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_CopyCtor_0056c050 (stand-in name).
// T = NCameraModes::CBase, proven by: CGameCameraManager::PushZTargetMode 0x00698E40 copies the manager camera-mode member at +0xA0 by value through it (Ego_r CGameCameraManager CurrentMode / PendingMode are both CCountedPointer<NCameraModes::CBase>); the same TU holds ~CCountedPointer<NCameraModes::CBase> 0x0056C8D0 (landed); FableWin.pdb ??0?$CCountedPointer@VCBase@NCameraModes@@@@QAE@ABV0@@Z.
// Member: the in-class copy constructor of engine/CCountedPointer.h (const&), emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
namespace NCameraModes { class CBase; }
template CCountedPointer<NCameraModes::CBase>::CCountedPointer(const CCountedPointer<NCameraModes::CBase>&);
