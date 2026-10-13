// dynamic initializer for float g_cos_013bad6c @ 0x013BAD6C = (float)cos(const double @ 0x01261E50 = 0.7853981852531433)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121EE10, oracle length 15 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  fld qword ptr [0x1261e50] ; fcos ; fstp dword ptr [0x13bad6c] ; ret
// Compiler-generated `??__E` initializer of a static-storage float computed from a double constant
// that lives in .rdata of another translation unit (0.7853981852531433, so it is not folded at compile time)
// with the intrinsic cos() (`#pragma intrinsic`, the retail build's /Oi), stored as float. Both
// objects are named by address; the constant is declared extern const. One of 28 bodies.
#include <math.h>
#pragma intrinsic(cos)

extern const double g_angle_01261e50;     // 0x01261E50 = 0.7853981852531433

float g_cos_013bad6c = (float)cos(g_angle_01261e50);
