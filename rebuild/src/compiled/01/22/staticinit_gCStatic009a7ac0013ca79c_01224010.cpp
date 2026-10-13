// dynamic initializer for CStatic_009a7ac0 g_CStatic_009a7ac0_013ca79c @ 0x013CA79C -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224010, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13ca79c ; call 0x9a7ac0 ; push 0x122a6a0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_009a7ac0
// living at 0x013CA79C: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A6A0 (`mov ecx,offset object ; jmp CStatic_009a7ac0::~CStatic_009a7ac0`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
struct CStatic_009a7ac0 {
    CStatic_009a7ac0();                 // 0x009A7AC0
    ~CStatic_009a7ac0();                // 0x009A7B70, reached through the atexit thunk
};

CStatic_009a7ac0 g_CStatic_009a7ac0_013ca79c;