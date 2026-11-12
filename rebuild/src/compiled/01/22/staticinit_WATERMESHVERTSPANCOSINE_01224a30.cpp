// dynamic initializer for const float WATER_MESH_VERT_SPAN_COSINE @ 0x0143BAC0 = (float)cos(const double @ 0x012A1130 = 0.9999999999999999)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224A30, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  fld qword ptr [0x12a1130] ; fcos ; fstp dword ptr [0x143bac0] ; ret
// Compiler-generated `??__E` initializer of a static-storage float computed from a double constant
// that lives in .rdata of another translation unit (0.9999999999999999, so it is not folded at compile time)
// with the intrinsic cos() (`#pragma intrinsic`, the retail build's /Oi), stored as float. The
// object carries its PDB name (see Names below); the angle constant is declared extern const. One of 28 bodies.
#include <math.h>
#pragma intrinsic(cos)

extern const double g_angle_012a1130;     // 0x012A1130 = 0.9999999999999999

// Names (agentFAM09): Ego_r.pdb `dynamic initializer for 'WATER_MESH_VERT_SPAN_COSINE'' (32 copies) + FableWin.pdb (32 copies);
//   the header constants WATER_MESH_VERT_SPAN_SINE / WATER_MESH_VERT_SPAN_COSINE (+ WATER_BOUNDING_SPHERE_RADIUS): both PDBs
//   list them as a sine, cosine, radius run in every including TU; retail runs sin then cos of the 0.9999999999999999 constant
//   0x012A1130 per TU (the radius row, cos(0.15*pi)*13000, where it survived); Anniv360 Test PDB (third source): S_LDATA32
//   const float. Was g_cos_0143bac0.
const float WATER_MESH_VERT_SPAN_COSINE = (float)cos(g_angle_012a1130);    // 0x0143BAC0 (internal linkage)
