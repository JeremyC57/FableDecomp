// dynamic initializer for static CCharString GNullName @ 0x013D27E4 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x012242D0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x12393e4 ; mov ecx, 0x13d27e4 ; call 0x99ebf0 ; push 0x122aa60 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013D27E4: it constructs it from the literal and registers the
// destructor thunk @ 0x0122AA60 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'GNullName'' (0x0101AD10/0x0101AE80) + FableWin.pdb
//   (0x0400AF10/0x0400B510); both PDBs list GNullName twice (Ego_r: before CSymbolMap::CurrentFilename and before
//   CCRCSymbolMap::CurrentFilename); retail 0x01224170 -> 0x01224190 and 0x012242D0 -> 0x012242F0 run the same pairs; literal
//   "NULL"; Anniv360 Test PDB (third source): two S_LDATA32 CCharString (internal linkage). Was g_CCharString_013d27e4.
static CCharString GNullName("NULL", -1);    // 0x013D27E4 (internal linkage)
