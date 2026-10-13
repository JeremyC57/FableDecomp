// dynamic initializer for CStatic_00a38500 g_CStatic_00a38500_013b8a54 @ 0x013B8A54 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A630, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13b8a54 ; call 0xa38500 ; push 0x1228dd0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_00a38500
// living at 0x013B8A54: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228DD0 (`mov ecx,offset object ; jmp CStatic_00a38500::~CStatic_00a38500`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
struct CStatic_00a38500 {
    CStatic_00a38500();                 // 0x00A38500
    ~CStatic_00a38500();                // 0x004D1D50, reached through the atexit thunk
};

CStatic_00a38500 g_CStatic_00a38500_013b8a54;