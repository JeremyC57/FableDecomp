// <unattributed>::CallWithSelf(CTarget*, void*) -- target->Method(this, value)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00996970, oracle length 18 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[esp+8] ; push eax ; push ecx ; mov ecx,[esp+0Ch] ; call Method_009964e0 ; ret 8
// The receiver hands itself and its second argument to a two-argument __thiscall method
// @ 0x009964E0 on its first argument.
struct CCallWithSelf_Retail;

struct CCallTarget_009964e0 {
    void Method_009964e0(CCallWithSelf_Retail* caller, void* value);   // 0x009964E0
};

struct CCallWithSelf_Retail {
    void CallWithSelf(CCallTarget_009964e0* target, void* value);
};

void CCallWithSelf_Retail::CallWithSelf(CCallTarget_009964e0* target, void* value)
{
    target->Method_009964e0(this, value);
}
