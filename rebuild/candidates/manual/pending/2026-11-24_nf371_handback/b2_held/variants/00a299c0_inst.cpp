// CCountedPointer::Reset(T* data) -- (CCountedPointer<C3DMesh2>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VC3DMesh2@@@@QAEXPAVC3DMesh2@@@Z
// Variant (not adopted): the sibling explicit-instantiation form (0x00A29FC0), with DeleteData declaration-only.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class C3DMesh2;
template void CCountedPointer<C3DMesh2>::Reset(C3DMesh2*);
