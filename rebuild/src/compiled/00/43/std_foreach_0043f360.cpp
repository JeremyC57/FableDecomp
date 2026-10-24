// std::for_each(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, std::mem_fun_ref_t<void, CEnginePrimitiveHandle> f) -- calls CEnginePrimitiveHandle::Clear on every 8-byte handle of a vector range and returns the mem_fun_ref_t
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043F360, 58 bytes (+ padding), `ret 8`. 36 callers, e.g. CPlayerGui::DrawDebugSafeAreas 0x0043B55D
// pushing the member pointer 0x0041BE70 (CEnginePrimitiveHandle::Clear). Catalog label `_For_each<pair<long,CCharString>...>` is propagated.
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: mem_fun_ref_t::operator() = `mov ecx,esi ; call edi`, stride 8.
namespace std {
template <class Result, class T> class mem_fun_ref_t {
    Result (T::*m_pf)();
public:
    explicit mem_fun_ref_t(Result (T::*pf)()) : m_pf(pf) {}
    Result operator()(T& r) const { return (r.*m_pf)(); }
};
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CEnginePrimitiveHandle {
public:
    void* m_pVTableOrOwner;
    void* m_pData;
    void Clear();                          // 0x0041BE70
};
template std::mem_fun_ref_t<void, CEnginePrimitiveHandle> __fastcall std::for_each(CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, std::mem_fun_ref_t<void, CEnginePrimitiveHandle>);