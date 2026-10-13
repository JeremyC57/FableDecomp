// <unattributed>::CallWithValueAndAddress(long) -- Helper_004b8e40(value, &value)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B9E20, oracle length 18 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  lea eax,[esp+4] ; push eax ; mov eax,[esp+8] ; push eax ; call Helper_004b8e40 ; ret 4
// The argument is handed to a two-parameter __stdcall helper @ 0x004B8E40 both by value and by
// address (the helper may update it in place). `this` is unused.
struct CValueForwarder_Retail {
    void CallWithValueAndAddress(long value);
};

extern void __stdcall Helper_004b8e40(long value, long* slot);

void CValueForwarder_Retail::CallWithValueAndAddress(long value)
{
    Helper_004b8e40(value, &value);
}
