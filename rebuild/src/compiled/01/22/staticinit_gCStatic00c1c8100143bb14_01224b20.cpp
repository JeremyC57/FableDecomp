// dynamic initializer for PKCCriticalSection g_lock_0143bb14 @ 0x0143BB14 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224B20, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x143bb14 ; call 0xc1c810 ; push 0x122ade0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_00c1c810
// living at 0x0143BB14: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122ADE0 (`mov ecx,offset object ; jmp CStatic_00c1c810::~CStatic_00c1c810`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
class PKCCriticalSection {                // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    PKCCriticalSection();               // 0x00C1C810
    ~PKCCriticalSection();              // 0x00C1C820, reached through the atexit thunk
    unsigned char m_section[0x18];
};

PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14 the other module lock (the landed PKC rows' spelling)