// dynamic initializer for float g_cos_0143bad4 @ 0x0143BAD4 = (float)cos(const double @ 0x012A1130 = 0.9999999999999999)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224A70, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  fld qword ptr [0x12a1130] ; fcos ; fstp dword ptr [0x143bad4] ; ret
// Compiler-generated `??__E` initializer of a static-storage float computed from a double constant
// that lives in .rdata of another translation unit (0.9999999999999999, so it is not folded at compile time)
// with the intrinsic cos() (`#pragma intrinsic`, the retail build's /Oi), stored as float. Both
// objects are named by address; the constant is declared extern const. One of 28 bodies.
#include <math.h>
#pragma intrinsic(cos)

extern const double g_angle_012a1130;     // 0x012A1130 = 0.9999999999999999

float g_cos_0143bad4 = (float)cos(g_angle_012a1130);
