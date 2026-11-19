// std::vector<pair<long,CIntelligentPointer<CThing> > >::push_back(const value_type& x) -- STLport push_back: copy-construct at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@V?$allocator@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@@2@@std@@QAEXABU?$pair@JV?$CIntelligentPointer@VCThing@@@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3CD0, length 93 bytes (+ int3 padding), `ret 4`.
// Callees: vector::_M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool) @ 0x006A3A70,
//   CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90;
//   CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: the catalog's "Vector_PushBackElement" is a descriptive auto label. 0x006A3A70 (catalog
//   "Vector_ReserveAndFill") takes (position = _M_finish, x, the __false_type tag, 1, true): STLport's
//   `_M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true)`. The vector is the storage of the
//   CVectorMap<long, CIntelligentPointer<CThing>> the neighbouring sort functions order.
// Element: pair<long, CIntelligentPointer<CThing> > with STLport's declared memberwise copy constructor (the landed
//   __uninitialized_copy 0x006A31D0 model: key, then the pointer copy through a second destination pointer).
// Shape: _Construct's placement null test; the __false_type temporary is homed in the dead x-argument slot
//   (`lea edx,[esp+10h]`) after x has been loaded into ecx.
// Shared headers (NF94 q11361): STLport vector / pair of stlport/fable_containers.h and engine/CIntelligentPointer.h with the
//   opt-in set of the callee row 0x006A3A70 (same instantiation); was a local FablePush_006a3cd0 namespace model whose
//   _M_insert_overflow was public and namespaced.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
template void std::vector<std::pair<long, CIntelligentPointer<CThing> > >::push_back(const std::pair<long, CIntelligentPointer<CThing> >&);
