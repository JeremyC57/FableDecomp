// <unattributed>::CallTwo_009d6730() -- Method_009d6730(); Method_009d5a90() (tail)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D68B0, oracle length 16 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push esi ; mov esi,ecx ; call Method_009d6730 ; mov ecx,esi ; pop esi ; jmp Method_009d5a90
// Two no-argument __thiscall methods on the same object, the second as a tail call. The manifest's
// `_Dest_val<...CMusicManager...>` label is a propagated template name (three unrelated siblings
// 0x00788B0A, 0x009C8BF0, 0x009D68B0 carry it).
struct CTwoStep_Retail {
    void Method_009d6730();                     // 0x009D6730
    void Method_009d5a90();                     // 0x009D5A90
    void CallTwo_009d6730();
};

void CTwoStep_Retail::CallTwo_009d6730()
{
    Method_009d6730();
    Method_009d5a90();
}
