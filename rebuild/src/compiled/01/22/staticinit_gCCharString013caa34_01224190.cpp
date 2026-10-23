// dynamic initializer for CCharString g_CCharString_013caa34 @ 0x013CAA34 -- default-constructs it out of line, then registers its destructor with atexit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224190, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13caa34 ; call 0x99aed0 ; push 0x122a880 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013CAA34: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122A880 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
// Identity: the default constructor called here is 0x0099AED0, CWideString::CWideString() (labels, chunks 237/247/265/284), so the object is a CWideString.

CWideString g_CCharString_013caa34;