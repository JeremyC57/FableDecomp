// <unattributed>::ForwardWithMember60(void*) -- Helper_0076e4d0(arg, &this->+0x60)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E5D0, oracle length 17 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov edx,[esp+4] ; lea eax,[ecx+60h] ; push eax ; push edx ; call Helper_0076e4d0 ; ret 4
// Same family as 0x00658E90: the argument and the address of the member at +0x60 go to a
// two-argument __stdcall helper @ 0x0076E4D0. Here the argument is loaded into edx before the
// member address is formed (the helper's first parameter is a pointer the compiler keeps in a
// register), unlike the pass-through `push dword ptr [esp+8]` of the sibling bodies.
struct CMemberOwner_Retail {
    unsigned char _pad_0x00[0x60];
    void*         Member;                   // +0x60
    void ForwardWithMember60(void* argument);
};

extern void __stdcall Helper_0076e4d0(void* argument, void* member);

void CMemberOwner_Retail::ForwardWithMember60(void* argument)
{
    void* member = &this->Member;
    Helper_0076e4d0(argument, member);
}
