// dynamic initializer for const float `anonymous namespace'::COS_45 @ 0x013BAD6C = (float)cos(const double @ 0x01261E50 = 0.7853981852531433)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EE10, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  fld qword ptr [0x1261e50] ; fcos ; fstp dword ptr [0x13bad6c] ; ret
// Compiler-generated `??__E` initializer of a static-storage float computed from a double constant
// that lives in .rdata of another translation unit (0.7853981852531433, so it is not folded at compile time)
// with the intrinsic cos() (`#pragma intrinsic`, the retail build's /Oi), stored as float. The
// object carries its PDB name (see Names below); the angle constant is declared extern const. One of 28 bodies.
#include <math.h>
#pragma intrinsic(cos)

extern const double g_angle_01261e50;     // 0x01261E50 = 0.7853981852531433

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for '`anonymous namespace'::COS_45'' (0x010117C0) + FableWin.pdb
//   (0x03FF3F10); both PDBs run CLOCKWISE_90, ANTI_CLOCKWISE_90, CLOCKWISE_180, ANTI_CLOCKWISE_180, COS_45,
//   CTCShapeManager::RegisteredInstantiations = retail 0x0121ED90 .. 0x0121EE20 (TURNING_C90 .. TURNING_AC180, then cos of the
//   pi/4 constant 0x01261E50). Anniv360 Test PDB (third source): S_LDATA32 const float COS_45 (internal linkage). Was
//   g_cos_013bad6c.
namespace {
const float COS_45 = (float)cos(g_angle_01261e50);    // 0x013BAD6C
}
