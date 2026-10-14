// g_global_0121a2dc() -- registers the destructor thunk @ 0x01228AF1 with _atexit @ 0x004012BC (a dynamic initialiser of a global with a trivial constructor)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A2DC, oracle length 12 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 0x1228af1 ; call 0x4012bc ; pop ecx ; ret 
// Tiny-body shape `RegisterAtExit` synthesised from the instruction pattern; the manifest label is
// propagated where it is a plain name, the other names are descriptive.
struct CGlobal_0121a2dc {                       // trivially constructed, destroyed at exit through the thunk @ 0x01228AF1
    ~CGlobal_0121a2dc();
};

CGlobal_0121a2dc g_global_0121a2dc;