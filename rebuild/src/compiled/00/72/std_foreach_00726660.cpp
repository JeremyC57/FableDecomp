// std::for_each(CThing** first, CThing** last, CApplyAdditionalImpulseIfInCombatAndTooClose f) -- applies the impulse functor (out-of-line operator()(CThing*) 0x00726160) to every nearby thing and returns it
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x00726660, 63 bytes, `ret 8`. Caller: CTCPhysicsControlled::UpdateAdditionalImpulse 0x00725C20.
// Callee: CApplyAdditionalImpulseIfInCombatAndTooClose::operator()(CThing*) @ 0x00726160 (manifest). Catalog label is propagated.
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: element pushed by value, functor addressed in its argument slot.
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
class CTCPhysicsControlled;
class CApplyAdditionalImpulseIfInCombatAndTooClose {
public:
    CTCPhysicsControlled* m_pPhysics;
    CApplyAdditionalImpulseIfInCombatAndTooClose(CTCPhysicsControlled* physics) : m_pPhysics(physics) {}
    void operator()(CThing* thing);        // 0x00726160
};
template CApplyAdditionalImpulseIfInCombatAndTooClose __fastcall std::for_each(CThing**, CThing**, CApplyAdditionalImpulseIfInCombatAndTooClose);