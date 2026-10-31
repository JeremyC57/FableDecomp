// NThingShot::CTCShotDefinitionThrow::InitMovement() -- launch a thrown shot on a 45-degree ballistic arc: from the flat distance and height difference to the target solve for the launch speed under the shot gravity (0.12), switch the physics on and, when the solution is positive, set the velocity along the flat direction tilted up by the angle
// Grade: DIFFER(295v299) -- retail keeps a second register copy of sin(A) (`fst qword [s] ; fld st(0)`): the division uses one copy and s*s the other, so it pops four x87 slots, ours three; and the velocity product order differs (retail Y = dirY*s, Z = s*dirZ). Agent611: 94 -> 28 diff lines by dropping the float casts on s/c (s and c stay double; frame 0x34 now matches) and computing s*s*2 in double. Names re-proven (InitMovement protected virtual MAE, GetShootAtPos, GetPos, CTCPhysicsBase::SetVelocity). Tried: repeated sin()/cos(), inline double Square, split t local, named-result / f*X operator*, `float sf = (float)s` for the square (gives the copy, 299v299, but the float conversion CSEs with the velocity one and spills s as a dword: 92 lines).
#pragma optimize("s", on)
// Leaf: ?InitMovement@CTCShotDefinitionThrow@NThingShot@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BBC9F, length 299 bytes (thiscall, plain `ret`, ebp frame + `leave`;
//   the NThingShot TU is size-optimised: unaligned entry, fld1).
// Identity: CTCShotDefinitionThrow vtable 0x0123CDC4 slot 33 (+0x84 = 0x0123CE48); Ego_r.pdb
//   ?InitMovement@CTCShotDefinitionThrow@NThingShot@@MAEXXZ. The catalog label "CTCPhysicsRigidBody_ApplyForce" is wrong.
// Callees: NThingShot::CTCShotDefinitionBase::GetShootAtPos() const 0x007BEDD0 (Ego_r.pdb IBE?AVC3DVector@@XZ, by hidden
//   pointer; the packed member at +0x51), CThing::GetPos 0x004C73D0, the thing's physics (+0x60, CTCPhysicsBase) vtable
//   slot 38 (+0x98) with 1 (CTCPhysicsBase 0x006B10D0 is `ret 4`; name unproven), CTCPhysicsBase::SetVelocity 0x0066F9B0
//   (landed, Ego_r.pdb ?SetVelocity@CTCPhysicsBase@@QAEXABVC3DVector@@@Z).
// Globals: (double)(float)(PI/4) 0x01261E50 (sin and cos at run time), shot gravity 0.12f 0x01265DF0, 1e-4f 0x0129BA3C,
//   0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Shape: `target = GetTargetPos(); pos = PThing->GetPos(); dir = (target.X - pos.X, target.Y - pos.Y, 0);
//   distance = dir.Normalise(); double s = sin(A), c = cos(A); k = (distance / s * c - (target.Z - pos.Z)) * (s * s * 2)
//   / (distance * distance * G); physics->slot38(1); if (k > 1e-4) { speed = 1 / sqrt(k); physics->SetVelocity((dir * s
//   + C3DVector(0, 0, 1) * c) * speed); }`
extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
#pragma intrinsic(sqrt, sin, cos)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    __forceinline const float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    __forceinline float GetLength() const { return (float)sqrt(Dot(*this)); }
    __forceinline float Normalise()
    {
        float length = GetLength();
        float inverse = 1.0f / length;
        X *= inverse;
        Y *= inverse;
        Z *= inverse;
        return length;
    }
};

__forceinline float Square(float x) { return x * x; }

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(bool on);                          // slot 38 (+0x98; name unproven)
    void SetVelocity(const C3DVector& velocity);        // 0x0066F9B0
};

class CThing {
public:
    unsigned char        _pad00[0x60];
    CTCPhysicsBase*      PhysicsTC;                     // +0x60
    const C3DVector& GetPos() const;                    // 0x004C73D0
};

namespace NThingShot {

const double THROW_ANGLE = 0.78539818525314331;     // (double)(float)(PI / 4), 0x01261E50

class CTCShotDefinitionBase {
public:
    virtual ~CTCShotDefinitionBase();
    CThing* PThing;                                     // +0x04 (CTCBase)
protected:
    C3DVector GetShootAtPos() const;                    // 0x007BEDD0
    virtual void InitMovement();
};
class CTCShotDefinitionThrow : public CTCShotDefinitionBase {
protected:
    virtual void InitMovement();                        // vtable 0x0123CDC4 slot 33
};

void CTCShotDefinitionThrow::InitMovement()
{
    C3DVector target = GetShootAtPos();
    const C3DVector& pos = PThing->GetPos();
    C3DVector direction(target.X - pos.X, target.Y - pos.Y, 0.0f);
    float distance = direction.Normalise();
    double s = sin(THROW_ANGLE);
    double c = cos(THROW_ANGLE);
    float k = (float)((distance / s * c - (target.Z - pos.Z)) * (s * s * 2.0f) / (distance * distance * 0.12f));
    CTCPhysicsBase* physics = PThing->PhysicsTC;
    physics->v38(true);
    if (k > 0.0001f) {
        float speed = 1.0f / (float)sqrt(k);
        physics->SetVelocity((direction * s + C3DVector(0.0f, 0.0f, 1.0f) * c) * speed);
    }
}

}
