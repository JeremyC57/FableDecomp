// dynamic initializer for CCharString `anonymous namespace'::STrapUntriggeredAnimName @ 0x013BADF4 -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F01D, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x126b98c ; mov ecx, 0x13badf4 ; call 0x99ebf0 ; push 0x1229b8a ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BADF4: it constructs it from the literal and registers the
// destructor thunk @ 0x01229B8A (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object is the PDB's file-static
// `anonymous namespace'::STrapUntriggeredAnimName (naming wave 15). One of 46 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::STrapUntriggeredAnimName'' (0x0101196D) +
//   FableWin.pdb (0x03FF46FB); Ego_r runs REACTION_MANAGER_DEF_INSTANTIATION_NAME, STrapUntriggeredAnimName, STrapTrigger,
//   STrapReset, STrapCycle = retail 0x0121F000 .. 0x0121F074; the source already spelled it (naming wave 15), only the catalog
//   name was the placeholder. Anniv360 Test PDB (third source): S_LDATA32 CCharString STrapUntriggeredAnimName (internal
//   linkage). Was g_CCharString_013badf4.
namespace {
CCharString STrapUntriggeredAnimName("TRAP_UNTRIGGERED", -1);    // 0x013BADF4
}
