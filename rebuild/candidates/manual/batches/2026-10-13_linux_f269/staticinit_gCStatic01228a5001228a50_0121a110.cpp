// dynamic initializer for CStatic_01228a50 g_CStatic_01228a50_01228a50 @ 0x01228A50 -- has nothing to run for the constructor, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A110, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x1228a50 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_01228a50
// living at 0x01228A50: it has nothing to run for the constructor and registers the
// destructor thunk @ 0x01228A50 (the destructor is inlined into the thunk, so the object's address and type are not recoverable here)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 6 bodies of this shape.
struct CStatic_01228a50 {
    // trivial default constructor: nothing observable runs before the atexit registration
    ~CStatic_01228a50();                // 0x00000000, reached through the atexit thunk
};

CStatic_01228a50 g_CStatic_01228a50_01228a50;
