// dynamic initializer for CCharString ConsoleVarScoreboardServerIPAddress @ 0x013B8AF0 -- default-constructs it out of line, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A750, oracle length 22 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx, 0x13b8af0 ; call 0x99e4b0 ; push 0x1228e90 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8AF0: it default-constructs it out of line and registers the
// destructor thunk @ 0x01228E90 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 36 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'ConsoleVarScoreboardServerIPAddress'' (0x0100E0B0) + FableWin.pdb
//   (0x03FF23A0); Ego_r order NUISystem::CList::GPreviousSelected, NUISystem::CHoverable::GAlreadyHovered (retail 0x0121A6AC,
//   0x0121A6D0), ConsoleVarScoreboardUserName, ConsoleVarScoreboardPassword, ConsoleVarScoreboardSelectedClan,
//   ConsoleVarScoreboardServerIPAddress, ConsoleVarServerScoresAgent = retail 0x0121A6F0 .. 0x0121A770; default CCharStrings.
//   Anniv360 Test PDB (third source): ?ConsoleVarScoreboardServerIPAddress@@3VCCharString@@A. Was g_CCharString_013b8af0.
CCharString ConsoleVarScoreboardServerIPAddress;        // 0x013B8AF0 ?ConsoleVarScoreboardServerIPAddress@@3VCCharString@@A
