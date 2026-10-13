// <unattributed>::ForwardWithMember58(void*) -- Helper_00660090(arg, &this->+0x58)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006590E4, oracle length 16 bytes.
// Grade: candidate -- still DIFFER under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  lea eax,[ecx+58h] ; push eax ; push dword ptr [esp+8] ; call Helper_00660090 ; ret 4
// Passes its one argument straight through (`push dword ptr [esp+8]`) together with the address of
// the member at +0x58 to a two-argument __stdcall helper @ 0x00660090, which cleans the stack.
// Five sibling bodies (0x00658E90, 0x006590C4..F4) do this for members +0x4C..+0x5C with five sibling
// helpers 46 bytes apart: template instantiations over consecutive list/vector members.
struct CMemberOwner_Retail {
    unsigned char _pad_0x00[0x58];
    void*         Member;                   // +0x58
    void ForwardWithMember58(void* argument);
};

extern void __stdcall Helper_00660090(void* argument, void* member);

void CMemberOwner_Retail::ForwardWithMember58(void* argument)
{
    Helper_00660090(argument, &this->Member);
}
