// C2DVector::ToC2DCoordI() const -- float point to integer point by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C46ED0, length 37 bytes (one of three byte-identical copies: 0x004347D0, 0x00C46ED0, 0x00405F40).
// Callee: __ftol2 @ 0x00BFEA70 (the CRT float->long conversion, called once per component).
// The manifest label "C2DCoordI::operator+=" is rejected: `this` (ecx) holds two floats and the
// stores go through the hidden result pointer at [esp+4], which is returned -- a by-value
// C2DCoordI return (the user constructor forces the hidden-pointer return). Constructor
// arguments are evaluated right to left (Y converted first, kept in edi, then X), and the
// Y store precedes the X store.
class C2DCoordI { public:
    long X;     // +0x00
    long Y;     // +0x04
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};

class C2DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    C2DCoordI ToC2DCoordI() const;
};

C2DCoordI C2DVector::ToC2DCoordI() const
{
    return C2DCoordI((long)X, (long)Y);
}