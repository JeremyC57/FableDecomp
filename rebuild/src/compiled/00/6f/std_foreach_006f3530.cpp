// std::for_each(SAttackerSlot* first, SAttackerSlot* last, CRotateThing f) -- applies a 32-byte functor (out-of-line operator() 0x006F2C00) to every 28-byte record and returns it
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F3530, 54 bytes, `ret 24h`. No direct callers found (xref). Callee 0x006F2C00 (manifest
// CAttackerPositionEvaluator::EvaluateAttacker) reads an intelligent pointer at record+0x14; names are descriptive.
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: a POD-like 32-byte functor with an implicit copy ctor is copied out with `rep movsd` (8 dwords).
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
struct SAttackerSlot { long m_words[7]; };
class CRotateThing {
public:
    long m_words[8];
    CRotateThing() {}
    void operator()(const SAttackerSlot& r);   // 0x006F2C00
};
template CRotateThing __fastcall std::for_each(SAttackerSlot*, SAttackerSlot*, CRotateThing);