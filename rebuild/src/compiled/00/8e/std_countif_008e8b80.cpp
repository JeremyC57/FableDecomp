// std::count_if(const CIntelligentPointer<CThing>* first, const CIntelligentPointer<CThing>* last, std::unary_negate<CIsImposterHit> pred) -- counts the intelligent pointers the (out-of-line) thing filter 0x008E8A10 rejects
// Leaf: count_if
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E8B80, 46 bytes, `ret 4`. No direct callers found (xref). Callee: the wrapped filter's
// operator() @ 0x008E8A10. Same as 0x008E8A90 with the predicate wrapped in std::unary_negate (`jne` instead of `je` before `inc ebx`);
// unary_negate holds the empty filter at offset 0, so the call still addresses the argument slot (`lea ecx,[esp+14h]`).
namespace std {
template <class Arg, class Result> struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};
template <class Predicate> class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
protected:
    Predicate m_pred;
public:
    explicit unary_negate(const Predicate& x) : m_pred(x) {}
    bool operator()(const typename Predicate::argument_type& x) const { return !m_pred(x); }
};
template <class InputIter, class Predicate>
int __fastcall count_if(InputIter first, InputIter last, Predicate pred)
{
    int n = 0;
    for ( ; first != last; ++first)
        if (pred(*first))
            ++n;
    return n;
}
}
class CThing;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
namespace {
class CIsImposterHit : public std::unary_function<CIntelligentPointer<CThing>, bool> {   // Ego_r.pdb anonymous namespace
public:
    CIsImposterHit() {}
    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x008E8A10
};
}
template int __fastcall std::count_if(const CIntelligentPointer<CThing>*, const CIntelligentPointer<CThing>*, std::unary_negate<CIsImposterHit>);