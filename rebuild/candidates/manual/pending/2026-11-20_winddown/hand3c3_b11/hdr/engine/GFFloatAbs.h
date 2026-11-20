#ifndef FABLE_ENGINE_GFFLOATABS_H
#define FABLE_ENGINE_GFFLOATABS_H
#include "fable_compiler.h"
// FableWin.pdb / Ego_d.pdb ?GFFloatAbs@@YIMM@Z (FableWin 0x01F43D00), inline in retail: the sign bit of the by-value argument
// cleared in place (`mov eax,[n] ; and eax,7FFFFFFFh ; mov [n],eax ; fld [n]`; e.g. 0x00B1646A..0x00B1647C in
// NParticleEngine::CPSCOrbit::COrbitState::Update). Ego_d.pdb: parameter `n`, line table 323..326 = 3 / 11 / 3 / 4 bytes
// (an 11-byte and-assign statement, then a 3-byte `return n;`). This is the debug builds' body, the sign-bit mask, NOT
// `(float)fabs(n)` (that would be one `fld ; fabs` statement, and retail's expansions have no `fabs`); the b7 audit read the same
// mask body in both debug PDBs' builds.
inline float FABLE_FASTCALL GFFloatAbs(float n)
{
    *(unsigned long*)&n &= 0x7fffffff;
    return n;
}
#endif
