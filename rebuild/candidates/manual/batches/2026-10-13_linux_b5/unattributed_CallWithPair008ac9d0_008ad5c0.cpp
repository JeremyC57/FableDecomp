// <unattributed>::CallWithPair_008ac9d0() -- Helper_008ac9d0(First, Second)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AD5C0, oracle length 13 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[ecx+4] ; mov edx,[ecx] ; push eax ; push edx ; call Helper_008ac9d0 ; ret
// The two words of the object (+0x00, +0x04) are passed to a two-argument __thiscall method
// @ 0x008AC9D0 on the same object (first word first) and its result is returned. The manifest's
// `SetStaticCast<NUISystem::CComponent>` label is a propagated template name.
struct CPair_Retail {
    void* First;    // +0x00
    void* Second;   // +0x04
    void* Helper_008ac9d0(void* first, void* second);   // 0x008AC9D0 (__thiscall on this object)
    void* CallWithPair_008ac9d0();
};

void* CPair_Retail::CallWithPair_008ac9d0()
{
    // `this` stays in ecx for the callee (a member call), which is why the first word is loaded into
    // edx rather than reusing ecx.
    return Helper_008ac9d0(this->First, this->Second);
}
