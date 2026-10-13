// <unattributed>::CallTwo_00788555() -- Method_00788555(); Method_00788aef() (tail)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788B0A, oracle length 16 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push esi ; mov esi,ecx ; call Method_00788555 ; mov ecx,esi ; pop esi ; jmp Method_00788aef
// Two no-argument __thiscall methods on the same object, the second as a tail call. The manifest's
// `_Dest_val<...CMusicManager...>` label is a propagated template name (three unrelated siblings
// 0x00788B0A, 0x009C8BF0, 0x009D68B0 carry it).
struct CTwoStep_Retail {
    void Method_00788555();                     // 0x00788555
    void Method_00788aef();                     // 0x00788AEF
    void CallTwo_00788555();
};

void CTwoStep_Retail::CallTwo_00788555()
{
    Method_00788555();
    Method_00788aef();
}
