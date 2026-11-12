// dynamic initializer for CCharString `anonymous namespace'::EASY_KEY @ 0x013B8AF8 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A790, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x124dcc0 ; mov ecx, 0x13b8af8 ; call 0x99ebf0 ; push 0x1228eb0 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8AF8: it constructs it from the literal and registers the
// destructor thunk @ 0x01228EB0 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::EASY_KEY'' (0x0100E0F0) + FableWin.pdb
//   (0x03FF2400); Ego_r runs ConsoleVarServerScoresAgent -> `anonymous namespace'::EASY_KEY (0x1D bytes, packed) = retail
//   0x0121A770 -> this (0x1D bytes, packed); the literal is a GUID key. Was g_CCharString_013b8af8.
namespace {
CCharString EASY_KEY("4AF92F27-1892-42f1-AA23-AF405325D8E6", -1);    // 0x013B8AF8
}
