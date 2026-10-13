// dynamic initializer for float g_cos_013baf94 @ 0x013BAF94 = (float)cos(const double @ 0x01276E18 = 1.5707963705062866)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F630, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  fld qword ptr [0x1276e18] ; fcos ; fstp dword ptr [0x13baf94] ; ret
// Compiler-generated `??__E` initializer of a static-storage float computed from a double constant
// that lives in .rdata of another translation unit (1.5707963705062866, so it is not folded at compile time)
// with the intrinsic cos() (`#pragma intrinsic`, the retail build's /Oi), stored as float. Both
// objects are named by address; the constant is declared extern const. One of 28 bodies.
#include <math.h>
#pragma intrinsic(cos)

extern const double g_angle_01276e18;     // 0x01276E18 = 1.5707963705062866

float g_cos_013baf94 = (float)cos(g_angle_01276e18);