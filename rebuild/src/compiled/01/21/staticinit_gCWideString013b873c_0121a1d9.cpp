// dynamic initializer for CWideString g_CWideString_013b873c @ 0x013B873C -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A1D9, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push 0x12310d8 ; mov ecx, 0x13b873c ; call 0x99b6b0 ; push 0x1228a97 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CWideString
// living at 0x013B873C: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228A97 (`mov ecx,offset object ; jmp CWideString::~CWideString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 9 bodies of this shape.
#include "fable_string.h"      // CWideString: CWideString(const wchar_t*) @ 0x0099B6B0 (landed), ~CWideString

CWideString g_CWideString_013b873c(L"E3TownHSP");