// dynamic initializer for const float NCreatureAI::COS_OF_THE_ANGLE_NEEDED_TO_FACE_GUARD @ 0x013BAF94 = (float)cos(const double @ 0x01276E18 = 1.5707963705062866)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F630, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  fld qword ptr [0x1276e18] ; fcos ; fstp dword ptr [0x13baf94] ; ret
// Compiler-generated `??__E` initializer of a static-storage float computed from a double constant
// that lives in .rdata of another translation unit (1.5707963705062866, so it is not folded at compile time)
// with the intrinsic cos() (`#pragma intrinsic`, the retail build's /Oi), stored as float. The
// object carries its PDB name (see Names below); the angle constant is declared extern const. One of 28 bodies.
#include <math.h>
#pragma intrinsic(cos)

extern const double g_angle_01276e18;     // 0x01276E18 = 1.5707963705062866

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'NCreatureAI::COS_OF_THE_ANGLE_NEEDED_TO_FACE_GUARD'' (0x01011FB0) +
//   FableWin.pdb (0x03FF72D0); both PDBs name NCreatureAI::COS_OF_THE_ANGLE_NEEDED_TO_FACE_GUARD (a float initializer) = retail
//   this, cos of the pi/2 constant 0x01276E18; Anniv360 Test PDB (third source): S_LDATA32 const float. Was g_cos_013baf94.
namespace NCreatureAI {
const float COS_OF_THE_ANGLE_NEEDED_TO_FACE_GUARD = (float)cos(g_angle_01276e18);    // 0x013BAF94 (internal linkage)
}
