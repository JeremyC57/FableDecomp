// CCountedPointer<const C3DSkeletalMorph>::ShareData(T* data, CCPPointerInfo* info) -- share another pointer's count block
// Leaf: ?ShareData@?$CCountedPointer@$$CBVC3DSkeletalMorph@@@@QAEXPBVC3DSkeletalMorph@@PAVCCPPointerInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3C840, 67 bytes. Callees: the old block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// When the block differs from the current one: release the current block (RefCount drop; at zero DeleteFunc on its Data
// and the block freed), then adopt data / info and take a reference on the new block (engine/CCountedPointer.h).
// Name: best guess (owner rule 2026-11-25; one source, listed in rebuild/oracles/descriptive_names.tsv). T from T2: retail TU order v the FableWin.pdb module's pointee types (TU lib_3d_animation_state.obj (EQ: 1 copies v 1 FableWin types, 0 named anchors, 1 alignment(s))). Member: ShareData (the shape).
// The release ShareData expands is the ReleaseData<T> body retail emits (Data zeroed before Info); its zero stores are
// overwritten by the adoption, so the leaf is the same, and the pick-any ReleaseData<T> copy this TU emits is retail's body.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class C3DSkeletalMorph;
template void CCountedPointer<const C3DSkeletalMorph>::ShareData(const C3DSkeletalMorph*, CCPPointerInfo*);
