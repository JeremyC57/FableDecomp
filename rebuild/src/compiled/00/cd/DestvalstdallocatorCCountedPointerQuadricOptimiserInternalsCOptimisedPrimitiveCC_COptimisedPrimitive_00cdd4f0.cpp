// CCountedPointer<const NScript::CPredicateBase>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDD4F0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the header's destructor body). Retail emits this body under both ~CCountedPointer<T>
// and ReleaseData<T> (0x0041BC90 / 0x0041BB30 for CTexture); no caller names this copy.
// Name: the binding table already binds this PDB name here (a landed caller); the self now spells it (one address, one name).
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
namespace NScript { class CPredicateBase; }
template CCountedPointer<const NScript::CPredicateBase>::~CCountedPointer();
