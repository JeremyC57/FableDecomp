// C2DBoxF::ClipTo(const C2DBoxF&) -- clamp every corner coordinate of this box into the other box
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066C560, 152 bytes, no callees. `ret 4` = one reference.
// Layout (PDB C2DBoxF): TopLeft {X,Y} +0/+4, BottomRight {X,Y} +8/+0xC. Each coordinate is an
// if/else-if clamp: `test ah,5 ; jp` is the ordered `<` against the low bound, `test ah,41h ; jne`
// the `>` against the high bound; the float copies go through eax (plain float assignment).
class C2DBoxF { public:
    float X0;   // +0x00 TopLeft.X
    float Y0;   // +0x04 TopLeft.Y
    float X1;   // +0x08 BottomRight.X
    float Y1;   // +0x0C BottomRight.Y
    void ClipTo(const C2DBoxF& other);
};

void C2DBoxF::ClipTo(const C2DBoxF& other)
{
    if (X0 < other.X0) X0 = other.X0;
    else if (X0 > other.X1) X0 = other.X1;
    if (Y0 < other.Y0) Y0 = other.Y0;
    else if (Y0 > other.Y1) Y0 = other.Y1;
    if (X1 < other.X0) X1 = other.X0;
    else if (X1 > other.X1) X1 = other.X1;
    if (Y1 < other.Y0) Y1 = other.Y0;
    else if (Y1 > other.Y1) Y1 = other.Y1;
}