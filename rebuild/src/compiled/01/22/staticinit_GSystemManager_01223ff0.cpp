// dynamic initializer for 'GSystemManager' @ 0x013CA618 -- default-constructs the CSystemManager out of line, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223FF0, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13ca618 ; call 0x9a7440 ; push 0x122a680 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for the static-storage CSystemManager GSystemManager (Ego_r / FableWin
// `dynamic initializer for 'GSystemManager'`; GFGetSystemManager 0x009A4EC0 returns its address) living at 0x013CA618: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A680 (`mov ecx,offset object ; jmp CSystemManager::~CSystemManager`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table (naming wave 13: the object and class carry their PDB names).
// One of 36 bodies of this shape.
class CSystemManager { public:
    CSystemManager();                   // 0x009A7440 ??0CSystemManager@@QAE@XZ
    virtual ~CSystemManager();          // 0x009A5800 ??1CSystemManager@@UAE@XZ, reached through the atexit thunk
};

CSystemManager GSystemManager;