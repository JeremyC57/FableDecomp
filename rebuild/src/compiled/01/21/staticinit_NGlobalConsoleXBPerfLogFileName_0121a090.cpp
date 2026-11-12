// dynamic initializer for CCharString NGlobalConsole::XBPerfLogFileName @ 0x013B86FC -- default-constructs it out of line, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A090, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13b86fc ; call 0x99e4b0 ; push 0x1228a10 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B86FC: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228A10 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 36 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'NGlobalConsole::XBPerfLogFileName'' (0x0100C480) + FableWin.pdb
//   (0x03FDE0B0); both PDBs run REACTION_MANAGER_DEF_INSTANTIATION_NAME, GDebugPlaySpeechForNarratorName,
//   NGlobalConsole::XBPerfLogFileName, LastSpeechSubString, LastSpeechSymbol, GConsoleDebugString = retail 0x0121A050 ..
//   0x0121A0F0. Anniv360 Test PDB (third source): ?XBPerfLogFileName@NGlobalConsole@@3VCCharString@@A. Was
//   g_CCharString_013b86fc.
namespace NGlobalConsole {
CCharString XBPerfLogFileName;    // 0x013B86FC ?XBPerfLogFileName@NGlobalConsole@@3VCCharString@@A
}
