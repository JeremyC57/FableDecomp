// std::count_if(const CIntelligentPointer<CThing>* first, const CIntelligentPointer<CThing>* last, CThingFilter_IsNotInMode pred) -- counts the intelligent pointers the (out-of-line) thing filter 0x008E8A10 accepts
// Leaf: count_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E8A90, 46 bytes, `ret 4`. No direct callers found (xref). Callee: the filter's operator() @
// 0x008E8A10 (manifest "FindChargeEvent" is wrong: it tests the thing's interface 0x31 mode 0x3F and returns the negation).
// The catalog's `_Find_if<CTargetPoint*, CFilter_Bind2<...>>` label is propagated; the body counts (`xor ebx,ebx ; ... inc ebx`).
// STLport count_if: `n = 0; for (; first != last; ++first) if (pred(*first)) ++n; return n;`, /Gr `__fastcall` (first ecx, last edx).
// 0x008E8B80 is the not1() instantiation of the same filter.
namespace std {
template <class Arg, class Result> struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
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
template <class T> class CIntelligentPointer {
public:
    void* m_vtable;
    T* m_pItem;
};
class CThingFilter_IsNotInMode : public std::unary_function<CIntelligentPointer<CThing>, bool> {
public:
    CThingFilter_IsNotInMode() {}
    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x008E8A10
};
template int __fastcall std::count_if(const CIntelligentPointer<CThing>*, const CIntelligentPointer<CThing>*, CThingFilter_IsNotInMode);