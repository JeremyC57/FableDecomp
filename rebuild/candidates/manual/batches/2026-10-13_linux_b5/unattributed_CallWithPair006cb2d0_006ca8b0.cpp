// <unattributed>::CallWithPair_006cb2d0() -- Helper_006cb2d0(First, Second)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA8B0, oracle length 13 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[ecx+4] ; mov edx,[ecx] ; push eax ; push edx ; call Helper_006cb2d0 ; ret
// The two words of the object (+0x00, +0x04) are passed to a two-argument __thiscall method
// @ 0x006CB2D0 on the same object (first word first) and its result is returned. The manifest's
// `SetStaticCast<NUISystem::CComponent>` label is a propagated template name.
struct CPair_Retail {
    void* First;    // +0x00
    void* Second;   // +0x04
    void* Helper_006cb2d0(void* first, void* second);   // 0x006CB2D0 (__thiscall on this object)
    void* CallWithPair_006cb2d0();
};

void* CPair_Retail::CallWithPair_006cb2d0()
{
    // `this` stays in ecx for the callee (a member call), which is why the first word is loaded into
    // edx rather than reusing ecx.
    return Helper_006cb2d0(this->First, this->Second);
}
