// _global::_Destroy_Record34(CInputEvent* first, CInputEvent* last) -- call the (empty, out-of-line) element destructor over [first, last) of 52-byte CInputEvent records
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6720, length 33 bytes (padded to 48).
// Leaf: ?_Destroy_Record34@@YIXPAVCInputEvent@@0@Z
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is STLport
// `_Destroy(first, last)` emitted out of line (first in ecx, last in edx, no tag -> plain `ret`) over 0x34-byte
// elements whose destructor is declared out of line: 0x00A03B30 is a bare `ret` (an empty destructor folded with every
// other empty member by /OPT:ICF, landed as "$E2"), so the call survives although it does nothing.
// Callees: CInputEvent::~CInputEvent @ 0x00A03B30 (empty).
// Shape: guarded do-while, `last` in edi, `mov ecx,esi ; call` per element, stride 0x34.
// Name (round 7, NF53): the 52-byte element is CInputEvent (Ego_r.pdb sizeof 52; engine/CInputEvent.h asserts 0x34) and its out-of-line
//   destructor is ??1CInputEvent@@QAE@XZ 0x00A03B30 (the catalog and engine/CInputEvent.h name it so); was the local SRecord34 model. The
//   function's own name stays descriptive (the STLport range-destroy helper; its exact template spelling is not proven).
#include "engine/CInputEvent.h"

void __fastcall _Destroy_Record34(CInputEvent* first, CInputEvent* last)
{
    for (; first != last; ++first)
        first->~CInputEvent();
}
