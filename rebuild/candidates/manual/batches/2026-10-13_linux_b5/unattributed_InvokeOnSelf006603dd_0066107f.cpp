// <unattributed>::InvokeOnSelf_006603dd(T*) -- target->Method(target), unoptimised body
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066107F, oracle length 18 bytes.
// Grade: candidate -- still DIFFER under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push [ebp+8] ; mov ecx,[ebp+8] ; call Method_006603dd ; pop ebp ; ret 4
// A frame-pointer body (this translation unit shipped unoptimised): the argument object is both the
// receiver and the parameter of a one-argument __thiscall method @ 0x006603DD. Five siblings
// (0x0066106D..B5) differ only in the callee. `this` is unused.
struct CSelfTarget_006603dd {
    void Method_006603dd(CSelfTarget_006603dd* argument);   // 0x006603DD
};

struct CInvoker_Retail {
    void InvokeOnSelf_006603dd(CSelfTarget_006603dd* target);
};

void CInvoker_Retail::InvokeOnSelf_006603dd(CSelfTarget_006603dd* target)
{
    target->Method_006603dd(target);
}
