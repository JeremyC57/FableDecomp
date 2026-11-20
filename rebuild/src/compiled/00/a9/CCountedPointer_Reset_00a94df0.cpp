// CCountedPointer::Reset(T* data) -- (CCountedPointer<C3DClothPrimitive>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VC3DClothPrimitive@@@@QAEXPAVC3DClothPrimitive@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A94DF0, 103 bytes, thiscall, `ret 4`. Stores DeleteData 0x00A93A60.
// Names (NF197, 2026-11-20; was the generated stand-in CCountedPointer_00a93a60::Reset with a local count-block model calling
//   ?Destroy_00a93a60@@YIXPAX@Z): the body stores CCountedPointer<C3DClothPrimitive>::DeleteData 0x00A93A60 (FableWin.pdb
//   ?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@SIXPAX@Z) as the count block's DeleteFunc, so this is the same instance's
//   Reset (engine/CCountedPointer.h; the shape of 0x00834E40 CCountedPointer<CAIBrain>::Reset). The class is complete and polymorphic
//   (FABLE_RETAIL_C3DCLOTHPRIMITIVE_VIRTUAL_DTOR) so the DeleteData COMDAT this TU instantiates is the owner row's body.
// Shape: engine/CCountedPointer.h Reset: ReleaseData expanded (its trailing Data store is dead), Data = data, then the count block
//   for a non-null data re-reads Data through `this`; a failed new leaves Info 0.
#define FABLE_RETAIL_C3DCLOTHPRIMITIVE_VIRTUAL_DTOR
#include "engine/C3DClothPrimitive.h"

#include "engine/CCountedPointer.h"

template void CCountedPointer<C3DClothPrimitive>::Reset(C3DClothPrimitive*);
