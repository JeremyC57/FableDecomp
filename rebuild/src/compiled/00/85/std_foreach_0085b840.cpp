// std::for_each(SCombatantRing* first, SCombatantRing* last, CSetupHelperRings f) -- applies a 12-byte functor (out-of-line operator() 0x0085B720) to every 8-byte record and returns it
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085B840, 61 bytes, `ret 10h`. No direct callers found (xref). Callee 0x0085B720 (manifest
// "CDefendingCombatantInfo_SetupHelperRings") scales the record's +4 float; element/functor names are descriptive.
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: element by reference, 12-byte implicit copy-out (`mov eax,ebx` first).
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
struct SCombatantRing { long m_index; float m_radius; };
class CSetupHelperRings {
public:
    long m_a, m_b, m_c;                           // three dwords (the callee's state)
    CSetupHelperRings() {}
    void operator()(const SCombatantRing& p);   // 0x0085B720
};
template CSetupHelperRings __fastcall std::for_each(SCombatantRing*, SCombatantRing*, CSetupHelperRings);