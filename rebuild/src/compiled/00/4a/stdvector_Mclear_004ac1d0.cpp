// std::vector<CIntelligentPointer<CThing> >::_M_clear() -- STLport 4.6 protected helper: destroy every element (virtual
// CIntelligentPointer destructor through vtable slot 0, flag 0, stride 8), then release the block (free).
// Leaf: ?_M_clear@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AC1D0, 50 bytes, thiscall, no args.
// Callees: free @ 0x00BFEA14 (the inline allocator's deallocate).
// Callers: vector<CIntelligentPointer<CThing> >::reserve 0x0077E580 (landed, names this callee _M_clear), and the other
//   reallocating members of the same instantiation.
// Name fix (NF59, queue q10533): catalogued as ~vector<CConversation>; the landed self row modelled a local struct Vec with a
//   made-up public Destroy (?Destroy@Vec@@QAEXXZ). NF56's 0x0077E580 port onto stlport/fable_containers.h emits this callee as
//   the protected STLport member ?_M_clear@...@IAEXXZ (std::allocator is a class, V); the element is CIntelligentPointer<CThing>
//   (vtable 0x01238C6C, 8 bytes). Ported onto the same shared headers and include block as 0x0077E580.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "stlport/fable_containers.h"
class CThing;
#include "engine/CIntelligentPointer.h"

template void std::vector<CIntelligentPointer<CThing> >::_M_clear();
