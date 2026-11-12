// dynamic initializer for CCharString `anonymous namespace'::CrimeTimeLeftText @ 0x013B8B3C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A898, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x124fea0 ; mov ecx, 0x13b8b3c ; call 0x99ebf0 ; push 0x1228ece ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013B8B3C: it constructs it from the literal and registers the
// destructor thunk @ 0x01228ECE (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::CrimeTimeLeftText'' (0x0100DE61) +
//   FableWin.pdb (0x03FE0CB4); both PDBs run DEFAULT_DEATH_RECOVERY_MARKER_NAME, CrimeText, CrimeTotalText, CrimeTimeLeftText =
//   retail 0x0121A7AD, 0x0121A7CA (landed CrimeText), 0x0121A87B, 0x0121A898 (HERO_GUILD_RECOVERY_MARKER /
//   TEXT_GUI_CRIME_TOTAL_FINE / TEXT_GUI_CRIME_TIME_UNTIL_FORGIVEN). Anniv360 Test PDB (third source): S_LDATA32 CCharString
//   CrimeTimeLeftText (internal linkage). Was g_CCharString_013b8b3c.
namespace {
CCharString CrimeTimeLeftText("TEXT_GUI_CRIME_TIME_UNTIL_FORGIVEN", -1);    // 0x013B8B3C
}
