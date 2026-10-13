// dynamic initializer for CStatic_009a7440 g_CStatic_009a7440_013ca618 @ 0x013CA618 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223FF0, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13ca618 ; call 0x9a7440 ; push 0x122a680 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_009a7440
// living at 0x013CA618: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A680 (`mov ecx,offset object ; jmp CStatic_009a7440::~CStatic_009a7440`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
struct CStatic_009a7440 {
    CStatic_009a7440();                 // 0x009A7440
    ~CStatic_009a7440();                // 0x009A5800, reached through the atexit thunk
};

CStatic_009a7440 g_CStatic_009a7440_013ca618;