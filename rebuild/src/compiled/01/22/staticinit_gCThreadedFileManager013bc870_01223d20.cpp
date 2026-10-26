// dynamic initializer for CThreadedFileManager g_CThreadedFileManager_013bc870 @ 0x013BC870 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223D20, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13bc870 ; call 0x98f3a0 ; push 0x122a4d0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CThreadedFileManager
// living at 0x013BC870: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A4D0 (`mov ecx,offset object ; jmp CThreadedFileManager::~CThreadedFileManager`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
class CThreadedFileManager { public:
    CThreadedFileManager();             // 0x0098F3A0
    ~CThreadedFileManager();            // 0x0098E690, reached through the atexit thunk
};

CThreadedFileManager g_CThreadedFileManager_013bc870;