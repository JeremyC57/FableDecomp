// dynamic initializer for CStatic_00687180 g_CStatic_00687180_013ba89c @ 0x013BA89C -- has nothing to run for the constructor, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121E000, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x1229270 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_00687180
// living at 0x013BA89C: it has nothing to run for the constructor and registers the
// destructor thunk @ 0x01229270 (`mov ecx,offset object ; jmp CStatic_00687180::~CStatic_00687180`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 43 bodies of this shape.
struct CStatic_00687180 {
    // trivial default constructor: nothing observable runs before the atexit registration
    ~CStatic_00687180();                // 0x00687180, reached through the atexit thunk
};

CStatic_00687180 g_CStatic_00687180_013ba89c;