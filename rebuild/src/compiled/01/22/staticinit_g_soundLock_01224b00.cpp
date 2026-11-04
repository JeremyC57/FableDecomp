// dynamic initializer for PKCCriticalSection g_soundLock @ 0x0143BAFC -- default-constructs it out of line, then registers its destructor with atexit
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224B00, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x143bafc ; call 0xc1c810 ; push 0x122add0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CStatic_00c1c810
// living at 0x0143BAFC: it default-constructs it out of line and registers the
// destructor thunk @ 0x0122ADD0 (`mov ecx,offset object ; jmp CStatic_00c1c810::~CStatic_00c1c810`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is descriptive, its
// real identifier is not recoverable. One of 36 bodies of this shape.
#include "libpk/PKCProtectionInstance.h"

PKCCriticalSection g_soundLock;            // 0x0143BAFC the audio library's module lock (chunk 586)