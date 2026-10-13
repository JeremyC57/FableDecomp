// <unattributed>::CallTwo_009c8660() -- Method_009c8660(); Method_009ce360() (tail)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8BF0, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push esi ; mov esi,ecx ; call Method_009c8660 ; mov ecx,esi ; pop esi ; jmp Method_009ce360
// Two no-argument __thiscall methods on the same object, the second as a tail call. The manifest's
// `_Dest_val<...CMusicManager...>` label is a propagated template name (three unrelated siblings
// 0x00788B0A, 0x009C8BF0, 0x009D68B0 carry it).
struct CTwoStep_Retail {
    void Method_009c8660();                     // 0x009C8660
    void Method_009ce360();                     // 0x009CE360
    void CallTwo_009c8660();
};

void CTwoStep_Retail::CallTwo_009c8660()
{
    Method_009c8660();
    Method_009ce360();
}
