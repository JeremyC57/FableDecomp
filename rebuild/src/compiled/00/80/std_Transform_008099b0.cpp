// std::_Transform(CThing** first, CThing** last, back_insert_iterator<vector<CIntelligentPointer<CThing> > > dest, CToIntelligentPointer func) -- wrap every raw thing pointer of [first, last) in an intelligent pointer and push it onto the destination vector, return the advanced back inserter
// Leaf: _Transform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008099B0, length 181 bytes (+ int3 padding), `ret 0Ch`.
// Callers: CTCTargetingPlayer::PostProcessTargetedThing @ 0x00803B20 (call 0x00803BFD: `ecx` = the returned inserter's
//   slot, `edx` = first, then last, the back inserter and the one-byte functor pushed).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 /
//   ~CBaseIntelligentPointer @ 0x00A01C10; the vector's _M_insert_overflow @ 0x004AC940 (thiscall: position, x, the
//   __false_type tag, 1, true). Vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Identity: Ego_r.pdb names `std::_Transform<CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing> > >,
//   `anonymous namespace'::CToIntelligentPointer>` as `__fastcall` (`YI`), returning the inserter.
// Shape: `for (; _First != _Last; ++_First, ++_Dest) *_Dest = _Func(*_First); return (_Dest);` in a /Gr TU: the hidden
//   result pointer in ecx (spilled), first in edx, last/dest/func on the stack. The functor returns a CIntelligentPointer
//   built straight in the frame temporary (CBaseIntelligentPointer(), the derived vtable store, SetPItem(thing)); the
//   inserter's `container->push_back(temp)` is STLport's inline push_back -- in place through the placement-new null test
//   and the copy constructor (base ctor, vtable, `SetPItem(other.GetPItem())`), `++_M_finish`, else
//   `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` with the tag homed in a frame byte -- then the temporary's
//   destructor. The 8-byte temporary gives VC7.1's `and esp,-8` frame.
#include "fable_compiler.h"
// Name fix (agentNF54): the local CBaseIntelligentPointer / CIntelligentPointer / std::vector / back_insert_iterator stand-ins replaced by
//   the shared engine/CIntelligentPointer.h (retail branch, FABLE_RETAIL_CINTELLIGENTPOINTER_COPY) over engine/CThing.h and
//   stlport/fable_containers.h (STLport 4.6 vector with its allocator, FABLE_RETAIL_STLP_BACK_INSERTER): the overflow call 0x004AC940 binds
//   ?_M_insert_overflow@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@IAEXPAV?$CIntelligentPointer@VCThing@@@@ABV3@ABU__false_type@2@I_N@Z
//   (the landed row's own leaf; was the allocator-less public ...QAEXPAV...ABU__false_type@@K_N@Z).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_BACK_INSERTER
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"

namespace std {
template <class InIt, class OutIt, class Fn1>
OutIt __fastcall _Transform(InIt _First, InIt _Last, OutIt _Dest, Fn1 _Func)
{
    for (; _First != _Last; ++_First, ++_Dest)
        *_Dest = _Func(*_First);
    return (_Dest);
}
}

namespace {
struct CToIntelligentPointer {
    CIntelligentPointer<CThing> operator()(CThing* thing) const { return CIntelligentPointer<CThing>(thing); }
};
}

template std::back_insert_iterator<std::vector<CIntelligentPointer<CThing> > > __fastcall std::_Transform(CThing**, CThing**, std::back_insert_iterator<std::vector<CIntelligentPointer<CThing> > >, CToIntelligentPointer);