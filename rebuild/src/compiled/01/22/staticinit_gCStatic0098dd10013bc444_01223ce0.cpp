// dynamic initializer for CStatic_0098dd10 g_CStatic_0098dd10_013bc444 @ 0x013BC444 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223CE0, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13bc444 ; call 0x98dd10 ; push 0x122a4b0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_0098dd10
// living at 0x013BC444: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A4B0 (`mov ecx,offset object ; jmp CStatic_0098dd10::~CStatic_0098dd10`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
struct CStatic_0098dd10 {
    CStatic_0098dd10();                 // 0x0098DD10
    ~CStatic_0098dd10();                // 0x0098DD90, reached through the atexit thunk
};

CStatic_0098dd10 g_CStatic_0098dd10_013bc444;