// std::count_if(const CIntelligentPointer<CThing>* first, const CIntelligentPointer<CThing>* last, std::unary_negate<CThingFilter_IsNotInMode> pred) -- counts the intelligent pointers the (out-of-line) thing filter 0x008E8A10 rejects
// Leaf: count_if
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
template int __fastcall std::count_if(const CIntelligentPointer<CThing>*, const CIntelligentPointer<CThing>*, std::unary_negate<CThingFilter_IsNotInMode>);