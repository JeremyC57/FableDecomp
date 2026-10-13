// <unattributed>::ForwardWithMember4C(void*) -- Helper_00660034(arg, &this->+0x4C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006590C4, oracle length 16 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  lea eax,[ecx+4Ch] ; push eax ; push dword ptr [esp+8] ; call Helper_00660034 ; ret 4
// Passes its one argument straight through (`push dword ptr [esp+8]`) together with the address of
// the member at +0x4C to a two-argument __stdcall helper @ 0x00660034, which cleans the stack.
// Five sibling bodies (0x00658E90, 0x006590C4..F4) do this for members +0x4C..+0x5C with five sibling
// helpers 46 bytes apart: template instantiations over consecutive list/vector members.
struct CMemberOwner_Retail {
    unsigned char _pad_0x00[0x4C];
    void*         Member;                   // +0x4C
    void ForwardWithMember4C(void* argument);
};

extern void __stdcall Helper_00660034(void* argument, void* member);

void CMemberOwner_Retail::ForwardWithMember4C(void* argument)
{
    Helper_00660034(argument, &this->Member);
}
