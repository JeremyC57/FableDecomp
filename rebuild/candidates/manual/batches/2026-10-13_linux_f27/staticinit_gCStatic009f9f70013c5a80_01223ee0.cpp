// dynamic initializer for CStatic_009f9f70 g_CStatic_009f9f70_013c5a80 @ 0x013C5A80 -- has nothing to run for the constructor, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223EE0, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x122a600 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_009f9f70
// living at 0x013C5A80: it has nothing to run for the constructor and registers the
// destructor thunk @ 0x0122A600 (`mov ecx,offset object ; jmp CStatic_009f9f70::~CStatic_009f9f70`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 43 bodies of this shape.
struct CStatic_009f9f70 {
    // trivial default constructor: nothing observable runs before the atexit registration
    ~CStatic_009f9f70();                // 0x009F9F70, reached through the atexit thunk
};

CStatic_009f9f70 g_CStatic_009f9f70_013c5a80;
