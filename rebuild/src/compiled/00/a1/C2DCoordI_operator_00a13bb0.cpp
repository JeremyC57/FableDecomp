// C2DCoordI::operator-=(const C2DVector& v) -- subtract a float offset from an integer point, truncating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A13BB0, length 48 bytes.
// Callee: __ftol2 @ 0x00BFEA70 (float->long, once per component).
// Layout from the PDB (C2DCoordI: X +0, Y +4; C2DVector: X +0, Y +4). Each component is
// `fild member ; fsub v.member ; __ftol2 ; store`, X then Y; returns *this. The manifest's
// "operator+=" label is the propagated name of the add twin @ 0x00A13B80; this body subtracts.
class C2DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
};

class C2DCoordI { public:
    long X;     // +0x00
    long Y;     // +0x04
    C2DCoordI& operator-=(const C2DVector& v);
};

C2DCoordI& C2DCoordI::operator-=(const C2DVector& v)
{
    X = (long)((float)X - v.X);
    Y = (long)((float)Y - v.Y);
    return *this;
}