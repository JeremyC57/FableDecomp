// Behaviour test for 00875990: src/00875990.cpp copied verbatim; a switch thing with a settable position and radius so
// both conditions (radius beyond the ground distance, on the camera's side) are exercised.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CIsSwitchValid::operator()(CThing* thing) const -- true for a camera-switch thing whose radius (virtual getter) exceeds its ground-plane distance from the focus and which lies on the camera's side of the focus (positive XY dot product)
// Retry 10b (B-12 sweep): the ground length is a const REFERENCE to the float temporary (`const float& distance = (float)sqrt(...)`): computed BEFORE the virtual radius call and homed in the dead argument slot (`fstp [esp+28h] ; call ; fcomp [esp+28h]`); a value local is computed after the call.
// Leaf: ??RCIsSwitchValid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875990, length 157 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0, the thing's virtual float getter (slot 62, +0xf8). Float constant 0x0122DEDC = 0.0f.
// Model: CIsSwitchValid from Ego_r.pdb (CameraPos +0, FocusPos +4, both C3DVector*). The two offsets are inline
// `C3DVector operator-` results (right-to-left: FocusPos is read before the position call; the dead Z differences are
// dropped but keep their frame words); the ground length is a const-reference float temporary
// that VC homes in the dead `thing` argument slot across the virtual call.
// Shape: `x*x + y*y` squares y first; `a.X*b.X + a.Y*b.Y` likewise; both tests exit through `test ah,41h ; jne`.
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
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
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
class CIsSwitchValid {
public:
    C3DVector* CameraPos;                          // +0x00
    C3DVector* FocusPos;                           // +0x04
    bool operator()(CThing* thing) const;
};

bool CIsSwitchValid::operator()(CThing* thing) const
{
    C3DVector toThing = thing->GetPosition() - *FocusPos;
    C3DVector toCamera = *CameraPos - *FocusPos;
    const float& distance = (float)sqrt(toThing.X * toThing.X + toThing.Y * toThing.Y);
    if (thing->GetSwitchRadius() > distance && toThing.X * toCamera.X + toThing.Y * toCamera.Y > 0.0f)
        return true;
    return false;
}
// ---- end source ----
static C3DVector g_pos; static float g_radius = 0.0f;
const C3DVector& CThing::GetPosition() const { return g_pos; }
#define V(n) void CThing::v##n() {}
V(000) V(001) V(002) V(003) V(004) V(005) V(006) V(007) V(008) V(009) V(010) V(011) V(012) V(013) V(014) V(015)
V(016) V(017) V(018) V(019) V(020) V(021) V(022) V(023) V(024) V(025) V(026) V(027) V(028) V(029) V(030) V(031)
V(032) V(033) V(034) V(035) V(036) V(037) V(038) V(039) V(040) V(041) V(042) V(043) V(044) V(045) V(046) V(047)
V(048) V(049) V(050) V(051) V(052) V(053) V(054) V(055) V(056) V(057) V(058) V(059) V(060) V(061)
float CThing::GetSwitchRadius() const { return g_radius; }
static C3DVector Make(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
int main()
{
    C3DVector camera = Make(10, 0, 5), focus = Make(0, 0, 0);
    CIsSwitchValid test; test.CameraPos = &camera; test.FocusPos = &focus;
    CThing thing;
    g_pos = Make(3, 4, 100); g_radius = 6.0f;            // distance 5 < 6, dot = 30 > 0
    CHECK(test(&thing) == true);
    g_radius = 4.0f;                                     // radius below the distance
    CHECK(test(&thing) == false);
    g_radius = 6.0f; g_pos = Make(-3, 4, 0);             // behind the focus (dot -30)
    CHECK(test(&thing) == false);
    g_pos = Make(0, 2, 0);                               // dot exactly 0
    CHECK(test(&thing) == false);
    if (fails == 0) printf("00875990_TEST PASS\n"); else printf("00875990_TEST FAIL\n");
    return fails;
}