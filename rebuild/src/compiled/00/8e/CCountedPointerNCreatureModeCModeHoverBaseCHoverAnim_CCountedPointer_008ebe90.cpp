// CCountedPointer::CCountedPointer(T*) -- take ownership of a new pointee: a fresh count block (count 1, DeleteData, Data) when non-null (T = NCreatureMode::CModeHoverBase::CHoverAnim)
// Leaf: ??0?$CCountedPointer@VCHoverAnim@CModeHoverBase@NCreatureMode@@@@QAE@PAVCHoverAnim@CModeHoverBase@NCreatureMode@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EBE90, 78 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _unattributed::CCountedPointer_008ebe90 (stand-in name).
// T = NCreatureMode::CModeHoverBase::CHoverAnim, proven by: NCreatureMode::CModeHoverBase::UpdateHoverAnim 0x008EA9F0 binds ??0?$CCountedPointer@VCHoverAnim@CModeHoverBase@NCreatureMode@@@@QAE@PAVCHoverAnim@...@Z and releases the pointer with ~CCountedPointer<CHoverAnim> 0x0073D630; Ego_r list<CCountedPointer<NCreatureMode::CModeHoverBase::CHoverAnim>>; FableWin.pdb (T*) ctor name.
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT (`ret 4`); the count
//   block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#include "engine/CCountedPointer.h"
namespace NCreatureMode { class CModeHoverBase { public: class CHoverAnim; }; }
template CCountedPointer<NCreatureMode::CModeHoverBase::CHoverAnim>::CCountedPointer(NCreatureMode::CModeHoverBase::CHoverAnim*);
