// CIsSwitchValid::operator()(CThing* thing) const -- true for a camera-switch thing whose radius (virtual getter) exceeds its ground-plane distance from the focus and which lies on the camera's side of the focus (positive XY dot product)
// Retry 10b (B-12 sweep): the ground length is a const REFERENCE to the float temporary (`const float& distance = (float)sqrt(...)`): computed BEFORE the virtual radius call and homed in the dead argument slot (`fstp [esp+28h] ; call ; fcomp [esp+28h]`); a value local is computed after the call.
// Leaf: ??RCIsSwitchValid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875990, length 157 bytes.
// Callees: CThing::GetPos @ 0x004C73D0, the thing's virtual float getter (slot 62, +0xf8). Float constant 0x0122DEDC = 0.0f.
// Model: CIsSwitchValid from Ego_r.pdb (CameraPos +0, FocusPos +4, both C3DVector*). The two offsets are inline
// `C3DVector operator-` results (right-to-left: FocusPos is read before the position call; the dead Z differences are
// dropped but keep their frame words); the ground length is a const-reference float temporary
// that VC homes in the dead `thing` argument slot across the virtual call.
// Shape: `x*x + y*y` squares y first; `a.X*b.X + a.Y*b.Y` likewise; both tests exit through `test ah,41h ; jne`.
#include <math.h>
#pragma intrinsic(sqrt)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    C3DVector d;
    d.X = a.X - b.X;
    d.Y = a.Y - b.Y;
    d.Z = a.Z - b.Z;
    return d;
}
class CThing {
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
    virtual void v060(); virtual void v061();
    virtual float GetSwitchRadius() const;         // slot 62 (+0xf8)
    const C3DVector& GetPos() const;          // 0x004C73D0
};
class CIsSwitchValid {
public:
    C3DVector* CameraPos;                          // +0x00
    C3DVector* FocusPos;                           // +0x04
    bool operator()(CThing* thing) const;
};

bool CIsSwitchValid::operator()(CThing* thing) const
{
    C3DVector toThing = thing->GetPos() - *FocusPos;
    C3DVector toCamera = *CameraPos - *FocusPos;
    const float& distance = (float)sqrt(toThing.X * toThing.X + toThing.Y * toThing.Y);
    if (thing->GetSwitchRadius() > distance && toThing.X * toCamera.X + toThing.Y * toCamera.Y > 0.0f)
        return true;
    return false;
}