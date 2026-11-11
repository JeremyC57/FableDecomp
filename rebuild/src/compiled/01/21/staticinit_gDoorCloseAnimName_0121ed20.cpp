// dynamic initializer for const CCharString g_DoorCloseAnimName @ 0x013BAD38 -- constructs it from the literal, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121ED20, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x1266e60 ; mov ecx, 0x13bad38 ; call 0x99ebf0 ; push 0x12299f0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAD38: it constructs it from the literal and registers the
// destructor thunk @ 0x012299F0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable (no PDB data name on disk): agentNF13 gave 0x013BAD38 one spelling, the one its
// landed users bind. One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

extern const CCharString g_DoorCloseAnimName("DOOR_CLOSE", -1);