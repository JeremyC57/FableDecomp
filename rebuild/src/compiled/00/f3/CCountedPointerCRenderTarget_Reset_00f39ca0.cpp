// CCountedPointer::Reset(T* data) -- (CCountedPointer<CRenderTarget>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCRenderTarget@@@@QAEXPAVCRenderTarget@@@Z
// Variant (not adopted): the sibling explicit-instantiation form (0x00A29FC0), with DeleteData declaration-only.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CRenderTarget;
template void CCountedPointer<CRenderTarget>::Reset(CRenderTarget*);
