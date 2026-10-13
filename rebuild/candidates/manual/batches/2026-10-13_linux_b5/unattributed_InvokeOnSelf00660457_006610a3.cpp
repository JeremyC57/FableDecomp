// <unattributed>::InvokeOnSelf_00660457(T*) -- target->Method(target), unoptimised body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006610A3, oracle length 18 bytes.
// Grade: candidate -- still DIFFER under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push [ebp+8] ; mov ecx,[ebp+8] ; call Method_00660457 ; pop ebp ; ret 4
// A frame-pointer body (this translation unit shipped unoptimised): the argument object is both the
// receiver and the parameter of a one-argument __thiscall method @ 0x00660457. Five siblings
// (0x0066106D..B5) differ only in the callee. `this` is unused.
struct CSelfTarget_00660457 {
    void Method_00660457(CSelfTarget_00660457* argument);   // 0x00660457
};

struct CInvoker_Retail {
    void InvokeOnSelf_00660457(CSelfTarget_00660457* target);
};

void CInvoker_Retail::InvokeOnSelf_00660457(CSelfTarget_00660457* target)
{
    target->Method_00660457(target);
}
