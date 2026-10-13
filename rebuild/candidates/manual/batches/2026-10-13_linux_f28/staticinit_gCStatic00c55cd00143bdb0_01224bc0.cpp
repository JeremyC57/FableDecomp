// dynamic initializer for CStatic_00c55cd0 g_CStatic_00c55cd0_0143bdb0 @ 0x0143BDB0 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224BC0, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x143bdb0 ; call 0xc55cd0 ; push 0x122ae30 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_00c55cd0
// living at 0x0143BDB0: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122AE30 (`mov ecx,offset object ; jmp CStatic_00c55cd0::~CStatic_00c55cd0`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
struct CStatic_00c55cd0 {
    CStatic_00c55cd0();                 // 0x00C55CD0
    ~CStatic_00c55cd0();                // 0x00C56370, reached through the atexit thunk
};

CStatic_00c55cd0 g_CStatic_00c55cd0_0143bdb0;
