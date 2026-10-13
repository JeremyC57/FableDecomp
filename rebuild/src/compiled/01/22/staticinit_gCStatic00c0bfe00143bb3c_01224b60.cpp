// dynamic initializer for CStatic_00c0bfe0 g_CStatic_00c0bfe0_0143bb3c @ 0x0143BB3C -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224B60, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x122d70e ; mov ecx, 0x143bb3c ; call 0xc0bfe0 ; push 0x122ae00 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_00c0bfe0
// living at 0x0143BB3C: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122AE00 (`mov ecx,offset object ; jmp CStatic_00c0bfe0::~CStatic_00c0bfe0`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 12 bodies of this shape.
struct CStatic_00c0bfe0 {
    CStatic_00c0bfe0(const char* text); // 0x00C0BFE0
    ~CStatic_00c0bfe0();                // 0x00C0BA40, reached through the atexit thunk
};

CStatic_00c0bfe0 g_CStatic_00c0bfe0_0143bb3c("");