// GFDXGetHermiteSplinePosition(C3DVector& out, const C3DVector& p0, const C3DVector& t0, const C3DVector& p1, const C3DVector& t1, float s) -- Hermite spline position through D3DX: copy the points and tangents into D3DXVECTOR3s, call D3DXVec3Hermite and copy the result back
// Leaf: ?GFDXGetHermiteSplinePosition@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A565C0, 172 bytes (+4 int3), `ret 10h` (PDB
//   ?GFDXGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z: `__fastcall`, out in ecx, p0 in edx, the rest on the stack).
// Callees: D3DXVec3Hermite (d3dx9_25.dll, called through the jmp thunk @ 0x00BFEFDA; __stdcall).
// Shape: four D3DXVECTOR3 frame copies built from the C3DVector components (dword moves), a fifth for the result;
//   the answer comes back as a C3DVector temporary assigned to `out` (retail also stores the dead temporary, in the slot
//   the last input copy used).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct D3DXVECTOR3 {
    float x, y, z;
    D3DXVECTOR3() { }
    D3DXVECTOR3(float fx, float fy, float fz) : x(fx), y(fy), z(fz) { }
};

extern "C" D3DXVECTOR3* __stdcall D3DXVec3Hermite(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pT1,
                                                  const D3DXVECTOR3* pV2, const D3DXVECTOR3* pT2, float s);   // 0x00BFEFDA

void __fastcall GFDXGetHermiteSplinePosition(C3DVector& out, const C3DVector& p0, const C3DVector& t0, const C3DVector& p1, const C3DVector& t1, float s)
{
    D3DXVECTOR3 dxP0(p0.X, p0.Y, p0.Z);
    D3DXVECTOR3 dxT0(t0.X, t0.Y, t0.Z);
    D3DXVECTOR3 dxP1(p1.X, p1.Y, p1.Z);
    D3DXVECTOR3 dxT1(t1.X, t1.Y, t1.Z);
    D3DXVECTOR3 dxOut;
    D3DXVec3Hermite(&dxOut, &dxP0, &dxT0, &dxP1, &dxT1, s);
    out = C3DVector(dxOut.x, dxOut.y, dxOut.z);
}