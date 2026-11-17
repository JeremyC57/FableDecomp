// std::__copy(CThing** first, CThing** last, back_insert_iterator<vector<CIntelligentPointer<CThing> > > result, const random_access_iterator_tag&, int*) -- STLport random-access copy of a thing-pointer array into a vector of intelligent pointers through a back inserter: each pointer is wrapped in a temporary CIntelligentPointer<CThing> and pushed back; returns the inserter
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CE1F0, 201 bytes (+7 int3), `ret 10h`, `and esp,-8` frame: /Gr __fastcall
//   returning the 4-byte back_insert_iterator through the hidden pointer in ecx (it has a user constructor), `first`
//   in edx, then last, the inserter by value, the tag reference and the distance pointer on the stack.
// Identity: called by the CTCBurntEffect ctor 0x007CDFE0 (landed; "STLport __copy into a back_insert_iterator
//   0x007CE1F0", copy(things.begin(), things.end(), back_inserter(TintList)) over a vector<CThing*>), the two
//   copy() wrappers 0x007CE2C0 / 0x007CE2F0, CTCElectrocutedEffect 0x007CEB00 and 0x007CF110. Catalog label
//   "Vector_InsertElements_SmartPtr" is descriptive.
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), vector<CIntelligentPointer<CThing> >::_M_insert_overflow(pointer,
//   const T&, const __false_type&, size_type, bool) @ 0x004AC940.
// Shape: STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first;
//   ++__result; } return __result;` -- the assignment converts the CThing* into a temporary intelligent pointer and
//   runs the inserter's operator= (vector::push_back inline: placement-new null test, copy constructor, `add [edi+4],8`,
//   else _M_insert_overflow with its own __false_type byte); 1-aligned intelligent pointer (the landed GiveItems
//   0x007CA9E0 model).
// Name fix (agentNF54): the local CBaseIntelligentPointer / CIntelligentPointer / std::vector / back_insert_iterator / __copy stand-ins
//   replaced by the shared engine/CIntelligentPointer.h (retail branch; FABLE_RETAIL_CINTELLIGENTPOINTER_COPY: the GetPItem / SetPItem copy)
//   over engine/CThing.h, and stlport/fable_containers.h (STLport 4.6 vector with its allocator argument, FABLE_RETAIL_STLP_BACK_INSERTER,
//   the random-access __copy): the overflow call 0x004AC940 now binds the PDB-shaped protected STLport member
//   ?_M_insert_overflow@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@IAEXPAV?$CIntelligentPointer@VCThing@@@@ABV3@ABU__false_type@2@I_N@Z
//   (the landed row's own leaf; was the allocator-less public ...QAEXPAV...@Z), and the row's own symbol gains the allocator argument
//   and the class key of back_insert_iterator (NAME_FIXES: its landed callers bind stand-ins).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_BACK_INSERTER
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"

namespace std {
template back_insert_iterator<vector<CIntelligentPointer<CThing> > > __fastcall
__copy<CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing> > >, int>(
    CThing**, CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing> > >,
    const random_access_iterator_tag&, int*);
}
