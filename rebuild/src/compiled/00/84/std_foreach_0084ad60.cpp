// std::for_each(CTCPerceiveThing** first, CTCPerceiveThing** last, std::mem_fun_t<void, CTCPerceiveThing> f) -- calls CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing on every pointer of a vector range and returns the mem_fun_t
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084AD60, 58 bytes, `ret 8`; byte-identical to 0x0043F310. Caller: CTCTrapBase::UpdateTriggerThings
// 0x0084A7C2, which pushes the member pointer 0x007449E0 (CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing). Catalog label propagated.
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: mem_fun_t::operator() = `mov ecx,[esi] ; call edi` (4-byte member pointer in edi).
namespace std {
template <class Result, class T> class mem_fun_t {
    Result (T::*m_pf)();
public:
    explicit mem_fun_t(Result (T::*pf)()) : m_pf(pf) {}
    Result operator()(T* p) const { return (p->*m_pf)(); }
};
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CTCPerceiveThing {
public:
    void SetAsIndirectlyAwareOfPerceivedThing();  // 0x007449E0 (the bound member pointer)
};
template std::mem_fun_t<void, CTCPerceiveThing> __fastcall std::for_each(CTCPerceiveThing**, CTCPerceiveThing**, std::mem_fun_t<void, CTCPerceiveThing>);