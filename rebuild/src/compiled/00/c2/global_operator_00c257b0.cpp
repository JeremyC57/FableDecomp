// operator-(const C3DVector& a, const C3DVector& b) -- component-wise difference, returned by value (__fastcall under /Gr: the hidden result in ecx, `a` in edx, `b` on the stack)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C257B0, length 48 bytes.
// Callees: none. Shape: the three differences computed Z, Y, X onto the FPU stack (the constructor's arguments right to left) and stored X, Y, Z into the hidden result, `ret 4`.
// The chunk label "CGuiWindow::OnBeginDrag" is propagated (0x00C2xxxx module).
// Leaf: ??G
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
#include "engine/C3DVector.h"

C3DVector __fastcall operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}