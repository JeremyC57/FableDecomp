// <unattributed>::ForwardToInterface_00445920(void*) -> Interface(+0x24)->Method_00445920
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687D40, oracle length 16 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[ecx+24h] ; push eax ; call Method_00445920 ; ret 4
// Same shape as 0x00687D10: the argument is forwarded to the object at +0x24 through a one-argument
// __thiscall method @ 0x00445920 and the result is returned untouched.
struct CInterfaceCalls_00445920 {
    void* Method_00445920(void* argument);  // 0x00445920 (landed; its label is a propagated template name)
};

struct CInterfaceOwner_Retail {
    unsigned char             _pad_0x00[0x24];
    CInterfaceCalls_00445920* Interface;    // +0x24
    void* ForwardToInterface_00445920(void* argument);
};

void* CInterfaceOwner_Retail::ForwardToInterface_00445920(void* argument)
{
    void* result = this->Interface->Method_00445920(argument);
    return result;
}
