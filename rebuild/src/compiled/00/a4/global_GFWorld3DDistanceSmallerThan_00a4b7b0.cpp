// GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance) -- true when the two positions are strictly closer than the distance (squared lengths compared)
// Leaf: ?GFWorld3DDistanceSmallerThan@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4B7B0, length 68 bytes (+12 int3 pad), `__fastcall` (a ecx, b edx, the
//   distance on the stack, `ret 4`), bool in eax.
// Name: Ego_r.pdb / FableWin.pdb `?GFWorld3DDistanceSmallerThan@@YI_NABVC3DVector@@0M@Z` (name set).
// Shape: `return (a - b).GetLengthSquared() < distance * distance;` (the difference kept on the x87 stack, VC's own
//   X, Z, Y sum order for X*X + Y*Y + Z*Z, as in the landed vector rows; the float compare materialised as
//   `mov eax,1` / `xor eax,eax` returns).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance)
{
    return (a - b).GetLengthSquared() < distance * distance;
}