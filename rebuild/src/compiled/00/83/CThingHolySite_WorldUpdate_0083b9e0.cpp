// CThingHolySite::vfunc_19() -- true when the byte state returned by the method @ 0x00838BA0 equals 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083B9E0, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  call 0x838ba0 ; dec al ; neg al ; sbb al, al ; inc al ; ret 
// Tiny-body shape `IsStateOne` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. `this` passes through ecx untouched to the callee; the `dec/neg/sbb/inc` tail is VC's `== 1` on a byte.
struct CThingHolySite {
    unsigned char GetState();             // 0x00838BA0
    unsigned char vfunc_19();                   // a byte result: a `bool` return would widen through eax
};

unsigned char CThingHolySite::vfunc_19()
{
    return GetState() == 1;
}