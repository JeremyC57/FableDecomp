// dynamic initializer for CStatic_0099ec30 g_CStatic_0099ec30_013bad94 @ 0x013BAD94 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EE70, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x13bad8c ; mov ecx, 0x13bad94 ; call 0x99ec30 ; push 0x1229aa0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_0099ec30
// living at 0x013BAD94: it default-constructs it out of line and registers the
// destructor thunk @ 0x01229AA0 (`mov ecx,offset object ; jmp CStatic_0099ec30::~CStatic_0099ec30`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 12 bodies of this shape.
struct CStatic_0099ec30 {
    CStatic_0099ec30(unsigned long value);// 0x0099EC30
    ~CStatic_0099ec30();                // 0x0099EAE0, reached through the atexit thunk
};

CStatic_0099ec30 g_CStatic_0099ec30_013bad94(20688268);