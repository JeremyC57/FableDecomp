// dynamic initializer for CSimpleMemoryPool g_CSimpleMemoryPool_013d2de8 @ 0x013D2DE8 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224490, oracle length 29 bytes.
// Grade: candidate -- still SRC_FAIL in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push 0x1000 ; push 0 ; mov ecx, 0x13d2de8 ; call 0xa5c570 ; push 0x122ab50 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CSimpleMemoryPool
// living at 0x013D2DE8: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122AB50 (the destructor is inlined into the thunk, so the object's address and type are not recoverable here)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 163 bodies of this shape.
struct CSimpleMemoryPool {
    CSimpleMemoryPool(unsigned long value, unsigned long value);// 0x00A5C570
    ~CSimpleMemoryPool();               // 0x00000000, reached through the atexit thunk
};

CSimpleMemoryPool g_CSimpleMemoryPool_013d2de8(0, 4096);
