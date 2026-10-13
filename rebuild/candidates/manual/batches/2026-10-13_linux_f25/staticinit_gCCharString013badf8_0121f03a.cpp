// dynamic initializer for CCharString g_CCharString_013badf8 @ 0x013BADF8 -- constructs it from the literal, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F03A, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x126b9a0 ; mov ecx, 0x13badf8 ; call 0x99ebf0 ; push 0x1229b94 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BADF8: it constructs it from the literal and registers the
// destructor thunk @ 0x01229B94 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

CCharString g_CCharString_013badf8("TRAP_TRIGGER", -1);
