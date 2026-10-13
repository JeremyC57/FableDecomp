// dynamic initializer for CShaderRenderManager g_CShaderRenderManager_013bc470 @ 0x013BC470 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223D00, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13bc470 ; call 0x98ab00 ; push 0x122a4c0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CShaderRenderManager
// living at 0x013BC470: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A4C0 (`mov ecx,offset object ; jmp CShaderRenderManager::~CShaderRenderManager`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
struct CShaderRenderManager {
    CShaderRenderManager();             // 0x0098AB00
    ~CShaderRenderManager();            // 0x0098ABF0, reached through the atexit thunk
};

CShaderRenderManager g_CShaderRenderManager_013bc470;