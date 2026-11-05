// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BE10, 17 bytes. Catalog name: std::_Dest_val<...CMusicManager::CRegisteredMusicEntry...>
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// (propagated label). Body: normalise two adjacent C3DVectors in place -- the one at +0x0, then the one at +0xc as a tail
// jump -- through C3DVector::AccurateNormalise @ 0x00A14440 (x,y,z scaled by 1/sqrt(x*x+y*y+z*z), returns *this).
// __thiscall, this in ecx; the enclosing class is not attributed.
#include "engine/C3DVector.h"
struct S { C3DVector a; C3DVector b; void M(); };
void S::M(){ a.AccurateNormalise(); b.AccurateNormalise(); }
