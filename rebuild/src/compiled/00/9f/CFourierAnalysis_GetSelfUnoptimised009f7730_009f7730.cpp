// CFourierAnalysis::GetSelfUnoptimised_009f7730(long) -- returns `this` (an unoptimised body: frame pointer, `this` spilled to the frame)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7730, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push ebp ; mov ebp, esp ; push ecx ; mov dword ptr [ebp - 4], ecx ; mov eax, dword ptr [ebp - 4] ; mov esp, ebp ; pop ebp ; ret 4
// Tiny-body shape `GetSelfUnoptimised`: `return this` compiled without optimisation (the frame
// pointer is set up and `this` is stored to and reloaded from the frame), so the function sits
// under `#pragma optimize("", off)`; the 1 argument(s) are not read. The manifest label is
// propagated where it is a plain name.
#pragma optimize("", off)
struct CFourierAnalysis {
    CFourierAnalysis* GetSelfUnoptimised_009f7730(long);
};

CFourierAnalysis* CFourierAnalysis::GetSelfUnoptimised_009f7730(long)
{
    return this;
}
#pragma optimize("", on)