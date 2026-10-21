// NCameraModes::CStringAndCageBase::UpdateAutoGoBehind(C3DVector& goTo) -- swing the go-to point round the observed thing's focal point (keeping its XY distance) an eleventh of the way towards directly behind the thing each call
// Grade: DIFFER(447v447) -- one x87 operand swap: in the side test `behind . (dir.Y, -dir.X, 0)` retail loads behind.X then multiplies by dir.Y (`fld [esp+18h]; fmul [esp+10h]`), ours loads dir.Y first; the operand order is canonical (source order, a named perpendicular, an inline CTools helper and explicit sums all give the same); everything else matches
// Leaf: UpdateAutoGoBehind
#include <math.h>
#pragma intrinsic(sqrt, acos)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void operator*=(float s) { X *= s; Y *= s; Z *= s; }
    __forceinline float NormaliseReturnLength()
    {
        float z = Z;
        float y = Y;
        float x = X;
        float length = (float)sqrt(x * x + y * y + z * z);
        *this *= 1.0f / length;
        return length;
    }
    C3DVector GetNegatedXY() const { return C3DVector(X * -1.0f, Y * -1.0f, 0.0f); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    C3DVector PerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }
    void Normalise();                                              // 0x00A14440
    void Rotate(const C3DVector& axis, float angle);               // 0x00A13C60
};
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
};
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

class CTCPhysicsBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacing() const;                           // slot 70 (+0x118)
};
class CTCGraphicAppearance {
public:
    C3DVector GetCameraFocalPos(float t) const;                    // 0x004C12D0
};
class CThingGameObject {
public:
    unsigned char         _pad00[0x60];
    CTCPhysicsBase*       PhysicsTC;                               // +0x60 (retail)
    CTCGraphicAppearance* Appearance;                              // +0x64
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    CTCGraphicAppearance* GetAppearance() const { return Appearance; }
};
namespace NCameraModes {
class CTools { unsigned char _body[0x24]; };
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0xa4 - 0x04];
    const CThingGameObject* GetObservedThing() const;              // 0x0084F1B0
    const CTools& GetCamTools() const;                             // 0x0084D080
};
class CStringAndCageBase : public CBase {
public:
    void UpdateAutoGoBehind(C3DVector& goTo);
};
void CStringAndCageBase::UpdateAutoGoBehind(C3DVector& goTo)
{
    C3DVector focal = GetObservedThing()->GetAppearance()->GetCameraFocalPos(1.0f);
    C3DVector centre(focal.X, focal.Y, 0.0f);
    C3DVector direction(goTo.X - focal.X, goTo.Y - focal.Y, 0.0f);
    float length = direction.GetLength();
    direction *= 1.0f / length;
    C3DVector behind = GetObservedThing()->GetPhysicsTC()->GetFacing().GetNegatedXY();
    behind.Normalise();
    float dot = direction.Z * behind.Z + direction.X * behind.X + direction.Y * behind.Y;
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float angle = (float)acos(dot) * 0.15915494f;
    angle *= 0.090909094f;
    GetCamTools();
    if (DotProduct(behind, direction.PerpendicularXY()) < 0.0f)
        angle *= -1.0f;
    direction.Rotate(C3DVector(0.0f, 0.0f, 1.0f), angle);
    goTo.X = length * direction.X + centre.X;
    goTo.Y = length * direction.Y + centre.Y;
}
}
