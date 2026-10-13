// dynamic initializer for CStatic_009e5200 g_CStatic_009e5200_013badb4 @ 0x013BADB4 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EF30, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13badb4 ; call 0x9e5200 ; push 0x1229b20 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_009e5200
// living at 0x013BADB4: it default-constructs it out of line and registers the
// destructor thunk @ 0x01229B20 (`mov ecx,offset object ; jmp CStatic_009e5200::~CStatic_009e5200`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
struct CStatic_009e5200 {
    CStatic_009e5200();                 // 0x009E5200
    ~CStatic_009e5200();                // 0x006FD040, reached through the atexit thunk
};

CStatic_009e5200 g_CStatic_009e5200_013badb4;