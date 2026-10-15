// CThingFilter_MakesLineWithPosInDirectionOfLine::operator()(CThing* thing) -- true when (thing position - Pos) points along Line (dot product > 0)
// Leaf: ??RCThingFilter_MakesLineWithPosInDirectionOfLine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7AD0, length 72 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0 (out of line).
// Globals: the float zero @ 0x0122DEDC (`fcomp dword ptr [g]`; a literal would be `fldz`).
// Layout: PDB CThingFilter_MakesLineWithPosInDirectionOfLine { C3DVector Pos; C3DVector Line; }.
// Shape: the delta is a C3DVector local assigned member by member (three separate float locals are scheduled one
// product at a time); the three deltas are loaded first, then the products are summed z, y, x; the compare materialises an int.
struct C3DVector {
    float x, y, z;
};

extern const float g_fZero;                         // 0x0122DEDC

struct CThing {
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CThingFilter_MakesLineWithPosInDirectionOfLine {
    C3DVector Pos;                                  // +0x00
    C3DVector Line;                                 // +0x0c
    bool operator()(CThing* thing) const;
};

bool CThingFilter_MakesLineWithPosInDirectionOfLine::operator()(CThing* thing) const
{
    const C3DVector& pos = thing->GetPosition();
    C3DVector delta;
    delta.x = pos.x - Pos.x;
    delta.y = pos.y - Pos.y;
    delta.z = pos.z - Pos.z;
    return delta.x * Line.x + delta.y * Line.y + delta.z * Line.z > g_fZero;
}