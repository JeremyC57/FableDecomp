// NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformPosToMaintainShape(C3DVector& pos, const C3DVector& origStart, const C3DVector& newStart, const C3DVector& origEnd, const C3DVector& newEnd)
// Grade: DIFFER(850v850) -- scheduling/slot residue only: retail keeps the 9 rotation identity stores after the direction normalisation (only the 3 Position zeros sit in the prologue) where VC here hoists all 12 into the prologue whatever the ctor/statement placement (probes 13-17); retail keeps the pos copy in the dead argument slots 0x68..0x70 (ours: the dead newDir slots 0x14..0x1c) and the final TransformPoint multiplies from those slots; 18 probes, the division form (scale = newLen / origLen, operator/= as *this *= 1.0f / s) matched the whole normalisation. Next step: find what delays the identity stores (a may-alias access to the matrix before the cross product, e.g. the rotation set through a reference/pointer), then the pos-copy slot.
// Leaf: ?TransformPosToMaintainShape@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@AAEXAAVC3DVector@@ABV4@111@Z
// Identity (proven): Ego_r.pdb private member (thiscall, this unused, ret 0x14 = five reference args); callers 0x00B218B0
// (landed CSplinePosParamData::TransformFromIndex, which binds this row as an invented "TransformPoint") and 0x00B21850
// (landed as CEngineScreenEffectColourFilter::Render, really CSplinePosParamData::TransformFromPosition) -> NAME_FIXES.
// Callees: C3DVector::AccurateNormalise 0x00A14440, CMatrix3x4::SetFromVectorAndAngle 0x00A55F90
// (PDB ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z; the landed C3DVector::Rotate binds it as a ctor), _CIacos 0x00BFEBA2.
// Constants: 1.0f 0x0122DED8, DELTA_NOTIONAL_ZERO 1e-4f 0x0129BA3C, 1/(2*PI) 0x01237F08.
// Maths: the segment (origStart->origEnd) is mapped onto (newStart->newEnd) by a rotation (axis = newDir x origDir) and a uniform
// scale newLen/origLen about origStart, then translated to newStart; pos is transformed by that matrix.
extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl acos(double);
#pragma intrinsic(sqrt)
#define PI 3.14159265f
extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C (1e-4f; 1,056 references, the engine-wide epsilon global)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector& operator/=(float s) { return *this *= 1.0f / s; }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    C3DVector Cross(const C3DVector& v) const { return C3DVector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X); }
    C3DVector& AccurateNormalise();                                   // 0x00A14440
};
class CMatrix3x4 { public:
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
    void SetFromVectorAndAngle(const C3DVector& axis, float angle);   // 0x00A55F90
    void TransformPoint(const C3DVector& in, C3DVector& out) const
    {
        out.X = XAxis.X * in.X + YAxis.X * in.Y + ZAxis.X * in.Z + Position.X;
        out.Y = XAxis.Y * in.X + YAxis.Y * in.Y + ZAxis.Y * in.Z + Position.Y;
        out.Z = XAxis.Z * in.X + YAxis.Z * in.Y + ZAxis.Z * in.Z + Position.Z;
    }
};
namespace NParticleEngine {
class CPSCSplineBase { public:
    class CSplinePosParamData { public:
        void TransformPosToMaintainShape(C3DVector& pos, const C3DVector& origStart, const C3DVector& newStart,
                                         const C3DVector& origEnd, const C3DVector& newEnd);
    };
};
void CPSCSplineBase::CSplinePosParamData::TransformPosToMaintainShape(C3DVector& pos, const C3DVector& origStart,
    const C3DVector& newStart, const C3DVector& origEnd, const C3DVector& newEnd)
{
    C3DVector newDir = newEnd - newStart;
    C3DVector origDir = origEnd - origStart;
    float newLen = newDir.GetLength();
    float origLen = origDir.GetLength();
    float scale = newLen / origLen;
    newDir /= newLen;
    origDir /= origLen;
    CMatrix3x4 transform;
    C3DVector axis = newDir.Cross(origDir);
    if (axis.GetLengthSquared() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        axis.AccurateNormalise();
        float angle = (float)acos(newDir.Dot(origDir)) * (1.0f / (2.0f * PI));
        transform.SetFromVectorAndAngle(axis, angle);
    }
    transform.XAxis *= scale;
    transform.YAxis *= scale;
    transform.ZAxis *= scale;
    transform.TransformPoint(-origStart, transform.Position);
    transform.Position += newStart;
    C3DVector original(pos);
    transform.TransformPoint(original, pos);
}
}
