// CThingSoundEmitter::WorldUpdate() -- true when the byte state returned by the method @ 0x004C8050 equals 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836780, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  call 0x4c8050 ; dec al ; neg al ; sbb al, al ; inc al ; ret 
// Tiny-body shape `IsStateOne` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. `this` passes through ecx untouched to the callee; the `dec/neg/sbb/inc` tail is VC's `== 1` on a byte.
struct CThingSoundEmitter {
    unsigned char GetState();             // 0x004C8050
    unsigned char WorldUpdate();                   // a byte result: a `bool` return would widen through eax
};

unsigned char CThingSoundEmitter::WorldUpdate()
{
    return GetState() == 1;
}