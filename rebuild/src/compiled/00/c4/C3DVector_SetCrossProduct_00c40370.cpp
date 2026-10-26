// C3DVector::SetCrossProduct(const C3DVector& a, const C3DVector& b) -- this = b x a, each component stored as soon as it is computed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40370, length 57 bytes.
// Leaf: ?SetCrossProduct@C3DVector
// Retail bytes: mov edx,[esp+8] ; mov eax,[esp+4] ; fld [edx+4] ; fmul [eax+8] ; fld [eax+4] ; fmul [edx+8] ; fsubp ; fstp [ecx] ; (Y) ; fstp [ecx+4] ; (Z) ; fstp [ecx+8] ; ret 8
// The manifest label "CRightHandedSet::GetRight" is propagated: the body writes `this` as a bare C3DVector from
// two vector arguments (`ret 8`, no hidden result, eax not `this`). Its caller 0x00C40590 passes (edi, ebx) to the
// member at +0x3c of its owner. The three statements store X, Y, Z in turn.
class C3DVector { public:
    float X, Y, Z;
    void SetCrossProduct(const C3DVector& a, const C3DVector& pos);
};

void C3DVector::SetCrossProduct(const C3DVector& a, const C3DVector& pos)
{
    X = pos.Y * a.Z - a.Y * pos.Z;
    Y = a.X * pos.Z - pos.X * a.Z;
    Z = a.Y * pos.X - a.X * pos.Y;
}