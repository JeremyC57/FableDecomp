// std::for_each(CThing** first, CThing** last, CDisplayRentFunctor f) -- applies CWorld's rent-display functor (out-of-line operator()(CThing*) 0x004AD3D0) to every pointer of a vector range and returns it
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AD890, 63 bytes, `ret 8`; 0x00726660 is the same shape for another 4-byte functor.
// Caller: CWorld::DisplayCollectableRentInformation 0x004A0566. Callee: the functor's operator()(CThing*) @ 0x004AD3D0 (manifest
// "Physics_CheckNavigability" is wrong; it reads thing+0x60). Functor and element names are descriptive (catalog label propagated).
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: the element is pushed by value (`mov eax,[esi] ; push eax`) and the functor's address is its argument slot (`lea ecx,[esp+18h]`).
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CThing;
class CDisplayRentFunctor {
public:
    void* m_pOwner;
    CDisplayRentFunctor(void* owner) : m_pOwner(owner) {}
    void operator()(CThing* thing);        // 0x004AD3D0
};
template CDisplayRentFunctor __fastcall std::for_each(CThing**, CThing**, CDisplayRentFunctor);