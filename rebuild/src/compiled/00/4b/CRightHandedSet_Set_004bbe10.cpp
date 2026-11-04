// CRightHandedSet::Set(const C3DVector& up, const C3DVector& forward) -- assign the up and forward vectors
// Leaf: Set
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BBE10, length 50 bytes.
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,[eax] ; mov edx,ecx ; mov [edx],esi ; mov esi,[eax+4] ; mov [edx+4],esi ;
//                mov eax,[eax+8] ; mov [edx+8],eax ; mov edx,[esp+0Ch] ; mov eax,[edx] ; add ecx,0Ch ; mov [ecx],eax ;
//                mov eax,[edx+4] ; mov [ecx+4],eax ; mov edx,[edx+8] ; mov [ecx+8],edx ; pop esi ; ret 8
// Manifest label "CRightHandedSet::CRightHandedSet" is propagated and wrong: the body does not return `this` in eax (every
// VC7.1 constructor does), so it is a two-argument member that overwrites both vectors; named from use.
// PDB layout (struct_layouts_egor.tsv, CRightHandedSet = 24 bytes): +0 C3DVector Up, +0xC C3DVector Forward.
// Shape: C3DVector's implicit assignment (a 12-byte bitwise copy) addresses each destination through its own register
// (`mov edx,ecx` for Up, `add ecx,0Ch` for Forward), the floats moving through integer registers.
#include "engine/C3DVector.h"

class CRightHandedSet { public:
    C3DVector Up;                            // +0x00
    C3DVector Forward;                       // +0x0C
    void Set(const C3DVector& up, const C3DVector& forward);
};

void CRightHandedSet::Set(const C3DVector& up, const C3DVector& forward)
{
    Up = up;
    Forward = forward;
}