// CRightHandedSet::SetDefault() -- reset the orientation basis: Up = (0, 0, 1), Forward = (0, 1, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF07A0, length 96 bytes. Callers: the CParticleEmitter constructor
// (0x00AF0380 @ 0x00AF057D / 0x00AF0585) on OrientationInterpolator.StartOrientation / EndOrientation.
// Leaf: SetDefault
// PDB CRightHandedSet {C3DVector Up +0x00, C3DVector Forward +0x0c}. The catalog label "CVsyncCallbackTimerInfo" is propagated.
// Shape: a 12-byte frame temporary per assignment (`Up = C3DVector(0,0,1)`): each float field is stored as an immediate and
// re-read as a dword for the bitwise copy (VC does not forward a float store into an integer load). It is NOT a constructor:
// eax ends as the last copied word, and only the void form gives retail's register choice (eax = destination of Up,
// `add ecx,0Ch` for Forward); the constructor form keeps eax = this and swaps the two.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRightHandedSet { public:
    C3DVector Up;           // +0x00
    C3DVector Forward;      // +0x0c
    void SetDefault();
};
void CRightHandedSet::SetDefault()
{
    Up = C3DVector(0.0f, 0.0f, 1.0f);
    Forward = C3DVector(0.0f, 1.0f, 0.0f);
}