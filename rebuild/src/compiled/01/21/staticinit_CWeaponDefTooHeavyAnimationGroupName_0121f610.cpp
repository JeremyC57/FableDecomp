// dynamic initializer for const CCharString CWeaponDef::TooHeavyAnimationGroupName @ 0x013BAF5C -- constructs it from the literal, then registers its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F610, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push 0x12759b8 ; mov ecx, 0x13baf5c ; call 0x99ebf0 ; push 0x122a070 ; call 0x4012bc ; pop ecx ; ret 
// Compiler-generated `??__E` dynamic initializer for one static-storage object of type CCharString
// living at 0x013BAF5C: it constructs it from the literal and registers the
// destructor thunk @ 0x0122A070 (`mov ecx,offset object ; jmp CCharString::~CCharString`, already landed)
// with _atexit @ 0x004012BC. Reconstructing the global definition below makes VC7.1 emit this
// exact function (and the thunk) from the .CRT$XCU table; the object's name is its PDB name
// (see Names below). One of 163 bodies of this shape.
#include "fable_string.h"      // CCharString: ctor @ 0x0099EBF0 / default @ 0x0099AED0, ~CCharString @ 0x0099EAE0
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWEAPONDEF_TOOHEAVY
#include "engine/CWeaponDef.h"   // opt-in: public static const TooHeavyAnimationGroupName (agentFAM09 hdr overlay)

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'CWeaponDef::TooHeavyAnimationGroupName'' (0x01011FD0) + FableWin.pdb
//   (0x03FFA350); both PDBs name CWeaponDef::TooHeavyAnimationGroupName = retail this ("TOO_HEAVY", after its TU's
//   REACTION_MANAGER_DEF_INSTANTIATION_NAME 0x0121F5F0). Anniv360 Test PDB (third source):
//   ?TooHeavyAnimationGroupName@CWeaponDef@@2VCCharString@@B. Was g_CCharString_013baf5c.
const CCharString CWeaponDef::TooHeavyAnimationGroupName("TOO_HEAVY", -1);    // 0x013BAF5C ?TooHeavyAnimationGroupName@CWeaponDef@@2VCCharString@@B
