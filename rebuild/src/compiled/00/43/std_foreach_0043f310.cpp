// std::for_each(CGuiControl** first, CGuiControl** last, std::mem_fun_t<void, CGuiControl> f) -- calls one virtual member function (slot 0, through the vcall thunk 0x0043E420) on every pointer of CPlayerGui's control vector and returns the mem_fun_t
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043F310, 58 bytes, `ret 8`. Callers: CPlayerGui::ClearGui 0x004382A3 (and 3 more); 0x0084AD60 is the
// byte-identical CTCTrapBase::UpdateTriggerThings instantiation. The catalog label `_For_each<pair<long,CCharString> const*, CAddDefNamesToComboBox>`
// is propagated across identical bodies: the call site pushes the member pointer 0x0043E420 (a `mov eax,[ecx] ; jmp [eax]` vcall thunk).
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: `mov ecx,[esi] ; call edi` is mem_fun_t::operator() with a 4-byte (single-inheritance) member pointer kept in edi; the empty range
// copies the functor from its argument slot in a second epilogue.
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
class CGuiControl {
public:
    virtual void Clear();                         // slot 0: the call site passes its vcall thunk 0x0043E420
};
template std::mem_fun_t<void, CGuiControl> __fastcall std::for_each(CGuiControl**, CGuiControl**, std::mem_fun_t<void, CGuiControl>);