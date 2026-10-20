// Grade: DIFFER(261v261) -- one x87 operand order: retail adds the parent offset to X as `fld RenderPosition.X ; fadd ParentPosition.X` (Y as `fld ParentPosition.Y ; fadd RenderPosition.Y`); ours loads ParentPosition.X first for every X spelling tried (PP.X+RP.X, RP.X+PP.X, member += with named locals); everything else matches
// NUISystem::CScrollingComponent::UpdatePosition(float deltaTime) -- advance the linear scroll from InitialPosition to TargetPosition over PositionTime (snapping at the end), then derive the render position (scaled by the parent zoom and offset by the parent position unless the component ignores its parent)
// Leaf: UpdatePosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00547050, length 259 bytes (`ret 4`), vtable 0x012482CC slot 34.
// FableWin.pdb: ?UpdatePosition@CScrollingComponent@NUISystem@@UAEXM@Z (the override of CComponent::UpdatePosition 0x0052FFD0).
// Callees: the component's virtual slot 101 (+0x194, a bool query that skips the parent transform; descriptive name).
// Float pool: 0x0122DED8 = 1.0f.
// Layout: Ego_r.pdb CComponent members 8 bytes lower in retail: Position +0x34, TargetPosition +0x3c, InitialPosition +0x44,
// ParentPosition +0x4c, RenderPosition +0x54, ParentZoom +0x74, PositionTimeElapsed +0x98, PositionTime +0x9c.
// Shape: the elapsed time stays on the FPU (`fst`) for the interpolation and the end test; the velocity is a C2DVector local
// built from `(Target - Initial) / time` through the reciprocal and a frame temporary (user copy ctor); the position is
// `velocity * elapsed + Initial`; the snap is the chained `InitialPosition = Position = TargetPosition`; the parent
// transform is written per component (the operand order differs between the X and Y terms).
namespace NUISystem {

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
    C2DVector& operator*=(const C2DVector& other) { X *= other.X; Y *= other.Y; return *this; }
    C2DVector operator-(const C2DVector& other) const { return C2DVector(X - other.X, Y - other.Y); }
    C2DVector operator+(const C2DVector& other) const { return C2DVector(X + other.X, Y + other.Y); }
    C2DVector operator*(float scale) const { return C2DVector(X * scale, Y * scale); }
    C2DVector operator/(float divisor) const { float inverse = 1.0f / divisor; return C2DVector(X * inverse, Y * inverse); }
};

struct CScrollingComponent {
    virtual void v000();
    unsigned char _pad004[0x30];
    C2DVector     Position;                      // +0x34
    C2DVector     TargetPosition;                // +0x3c
    C2DVector     InitialPosition;               // +0x44
    C2DVector     ParentPosition;                // +0x4c
    C2DVector     RenderPosition;                // +0x54
    C2DVector     Zoom;                          // +0x5c
    C2DVector     TargetZoom;                    // +0x64
    C2DVector     InitialZoom;                   // +0x6c
    C2DVector     ParentZoom;                    // +0x74
    unsigned char _pad07c[0x1c];
    float         PositionTimeElapsed;           // +0x98
    float         PositionTime;                  // +0x9c
    void UpdatePosition(float deltaTime);
#define V(n) virtual void v##n();
    V(001) V(002) V(003) V(004) V(005) V(006) V(007) V(008) V(009) V(010) V(011) V(012) V(013) V(014) V(015) V(016) V(017)
    V(018) V(019) V(020) V(021) V(022) V(023) V(024) V(025) V(026) V(027) V(028) V(029) V(030) V(031) V(032) V(033) V(034)
    V(035) V(036) V(037) V(038) V(039) V(040) V(041) V(042) V(043) V(044) V(045) V(046) V(047) V(048) V(049) V(050) V(051)
    V(052) V(053) V(054) V(055) V(056) V(057) V(058) V(059) V(060) V(061) V(062) V(063) V(064) V(065) V(066) V(067) V(068)
    V(069) V(070) V(071) V(072) V(073) V(074) V(075) V(076) V(077) V(078) V(079) V(080) V(081) V(082) V(083) V(084) V(085)
    V(086) V(087) V(088) V(089) V(090) V(091) V(092) V(093) V(094) V(095) V(096) V(097) V(098) V(099) V(100)
#undef V
    virtual bool IgnoresParentTransform();       // slot 101 (+0x194) descriptive
};

void CScrollingComponent::UpdatePosition(float deltaTime)
{
    if (PositionTimeElapsed < PositionTime) {
        PositionTimeElapsed += deltaTime;
        C2DVector velocity;
        velocity = (TargetPosition - InitialPosition) / PositionTime;
        Position = velocity * PositionTimeElapsed + InitialPosition;
        if (PositionTimeElapsed >= PositionTime)
            InitialPosition = Position = TargetPosition;
    }
    RenderPosition = Position;
    if (!IgnoresParentTransform()) {
        RenderPosition *= ParentZoom;
        RenderPosition.X = RenderPosition.X + ParentPosition.X;
        RenderPosition.Y = RenderPosition.Y + ParentPosition.Y;
    }
}

}
