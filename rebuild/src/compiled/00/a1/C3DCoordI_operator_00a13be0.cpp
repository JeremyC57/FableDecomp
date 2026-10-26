// C3DCoordI::operator+=(const C3DVector&) -- adds a float vector to the integer coordinate component-wise, truncating each sum back to long (__ftol2)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A13BE0, length 54 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (CRT float-to-long on the x87 stack, result in eax).
// PDB layout: C3DCoordI = {long X, long Y, long Z}; C3DVector = {float X, Y, Z}. Each component
// is `fild` + `fadd [v]` + ftol, and the next component's `fild` is scheduled before the store
// of the previous result. Returns *this (ret 4).
class C3DVector { public:
    float X, Y, Z;
};

class C3DCoordI { public:
    long X, Y, Z;
    C3DCoordI& operator+=(const C3DVector& v);
};

C3DCoordI& C3DCoordI::operator+=(const C3DVector& v)
{
    X = (long)((float)X + v.X);
    Y = (long)((float)Y + v.Y);
    Z = (long)((float)Z + v.Z);
    return *this;
}