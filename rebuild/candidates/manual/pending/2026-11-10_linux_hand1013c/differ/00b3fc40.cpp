// CEngineMap::PeekInterpolatedMapNormal(float x, float y) const -- the landscape normal at a fractional cell position: the four corner cells' map normals blended bilinearly, then normalised
// Grade: DIFFER(528v528) -- frame slots only (od.py against the real 528-byte body; q.py grades the chunk1013c oracle row, which is cut wrong: 816 bytes running on into 0x00B3FE50 and 0x00B3FEE0, so it shows DIFFER(528v813)). Retail frame 0x60, ours 0x54: c2's packer (ebp frame, slots in creation order) makes the n11 slot first (-0x24) where retail makes n01's first, and retail gives the n11 and n01 weighted terms new slots (-0x60, -0x54) where ours lets the n01 term join n11's dead slot. Inert (fr.sh): all 24 orders of the four sum terms (none keeps retail's instruction order except the current one), corner declaration order, n11 declared before n01, separate assignment (worse), corner names (packer order is not name or declaration order)
// Leaf: ?PeekInterpolatedMapNormal@CEngineMap@@QBE?AVC3DVector@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3FC40, 528 bytes, thiscall (ebp frame), `ret 0Ch` (hidden C3DVector return + two floats).
// Identity: Ego_r.pdb / FableWin.pdb ?PeekInterpolatedMapNormal@CEngineMap@@QBE?AVC3DVector@@MM@Z; callee
//   ?PeekMapNormal@CEngineMap@@QBE?AVC3DVector@@JJ@Z 0x00B3F5E0 (four calls: (ix,iy) (ix+1,iy) (ix,iy+1) (ix+1,iy+1)); the inline
//   C3DVector::Normalise (GFFastOneOverSqrt table 0x013CE558) expanded at the tail.
// Shape: the four terms evaluate right to left (n11 first: the inline operator+ arguments), so the sum is one expression.
//   Inert (probes): weight operand orders, fx declared after fy, `return normal.Normalise()`, const corners, assignment to a
//   declared normal (554), a two-pair parenthesised sum (540).
// Retail-asm: 0x00B3FC5F..0x00B3FC79 `fld [ebp-10h] ; fsub [ebp-4] ; fistp dword ptr [ebp-8] ; fild [ebp-8] ; fadd [ebp-0Ch] ; fstp [ebp-14h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; inc dword ptr [ebp-8]` (and again 0x00B3FC90..0x00B3FCAA for y) -- the shared engine/GFFloatToLongFloor.h helper expanded twice.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEKNORMAL
#include "engine/C3DVector.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/CEngineMap.h"

C3DVector CEngineMap::PeekInterpolatedMapNormal(float x, float y) const
{
    long ix = GFFloatToLongFloor(x);
    long iy = GFFloatToLongFloor(y);
    float fx = x - ix;
    float fy = y - iy;
    C3DVector n00 = PeekMapNormal(ix, iy);
    C3DVector n10 = PeekMapNormal(ix + 1, iy);
    C3DVector n01 = PeekMapNormal(ix, iy + 1);
    C3DVector n11 = PeekMapNormal(ix + 1, iy + 1);
    C3DVector normal = n00 * ((1.0f - fx) * (1.0f - fy)) + n10 * (fx * (1.0f - fy)) + n01 * ((1.0f - fx) * fy) + n11 * (fx * fy);
    normal.Normalise();
    return normal;
}
