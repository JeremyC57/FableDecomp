// CCountedPointer<const C3DAnimationStats>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@$$CBVC3DAnimationStats@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DF9E0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the header's destructor body). Retail emits this body under both ~CCountedPointer<T>
// and ReleaseData<T> (0x0041BC90 / 0x0041BB30 for CTexture); no caller names this copy.
// Name: best guess (owner rule 2026-11-25; one source, listed in rebuild/oracles/descriptive_names.tsv). T from T2: retail TU order v the FableWin.pdb module's pointee types (TU animation_set.obj (LESS: 1 copies v 1 FableWin types, 0 named anchors, 1 alignment(s))). Member: ~CCountedPointer (base rate of the shape).
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class C3DAnimationStats;
template CCountedPointer<const C3DAnimationStats>::~CCountedPointer();
