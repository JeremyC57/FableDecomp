// CThingFilter_MakesLineWithPosInDirectionOfLine::operator()(CThing* thing) -- true when (thing position - Pos) points along Line (dot product > 0)
// Leaf: ??RCThingFilter_MakesLineWithPosInDirectionOfLine
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7AD0, length 72 bytes (+ int3 padding).
// Callees: CThing::GetPos @ 0x004C73D0 (out of line).
// Globals: the float zero @ 0x0122DEDC (`fcomp dword ptr [g]`; a literal would be `fldz`).
// Layout: PDB CThingFilter_MakesLineWithPosInDirectionOfLine { C3DVector Pos; C3DVector Line; }.
// Shape: the delta is a C3DVector local assigned member by member (three separate float locals are scheduled one
// product at a time); the three deltas are loaded first, then the products are summed z, y, x; the compare materialises an int.
#include "engine/C3DVector.h"

extern const float g_fZero;                         // 0x0122DEDC

#include "engine/CThing.h"

class CThingFilter_MakesLineWithPosInDirectionOfLine { public:
    C3DVector Pos;                                  // +0x00
    C3DVector Line;                                 // +0x0c
    bool operator()(CThing* thing) const;
};

bool CThingFilter_MakesLineWithPosInDirectionOfLine::operator()(CThing* thing) const
{
    const C3DVector& pos = thing->GetPos();
    C3DVector delta;
    delta.X = pos.X - Pos.X;
    delta.Y = pos.Y - Pos.Y;
    delta.Z = pos.Z - Pos.Z;
    return delta.X * Line.X + delta.Y * Line.Y + delta.Z * Line.Z > g_fZero;
}