// dynamic initializer for CStatic_009d4db0 g_CStatic_009d4db0_013ca828 @ 0x013CA828 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012240A0, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13ca828 ; call 0x9d4db0 ; push 0x122a780 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_009d4db0
// living at 0x013CA828: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A780 (`mov ecx,offset object ; jmp CStatic_009d4db0::~CStatic_009d4db0`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
struct CStatic_009d4db0 {
    CStatic_009d4db0();                 // 0x009D4DB0
    ~CStatic_009d4db0();                // 0x009D4E90, reached through the atexit thunk
};

CStatic_009d4db0 g_CStatic_009d4db0_013ca828;