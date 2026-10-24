// NUISystem::CZoomingComponent::Update(float time) -- the base update, then clears the Panning flag once the position has reached its pan target (both axes within 1e-4)
// Leaf: ?Update@CZoomingComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546A50, 66 bytes (+ int3 padding), virtual (CZoomingComponent vtable slot 1), `ret 4`.
// Ego_r.pdb: `virtual void CZoomingComponent::Update(float)`; Panning is the bool at PDB +0x1a8 (retail +0x178,
//   the CComponent body is 0x30 smaller in retail, see the landed CZoomingComponent::ChangeState 0x005469E0).
// Callees: CChangingStateComponent::Update 0x0052C7E0 (base call, `dt` forwarded).
// Floats: 0x0129BA3C (1e-4f, shared with the landed CHoverable::Update 0x0055B890).
// Shape: `fld target; fsub position; fabs; fcomp eps; test ah,41h; jp` per axis is `fabs(TargetPosition - Position) <= eps`;
//   the `(float)` cast keeps the compare in single precision. Base layout as in the landed CHoverable::Update.
// Layout (retail CComponent): Position +0x34, TargetPosition +0x3c (Ego_r.pdb +0x64 / +0x6c).
extern const float g_positionEpsilon;              // 0x0129BA3C (1e-4f)
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
namespace NUISystem {
struct C2DVector {
    float X;
    float Y;
};
struct IComponentA {                               // the interface base at +0x04 (vtable, 0x10 bytes of state)
    virtual void va00();
    unsigned char _state[0x10];
};
struct IComponentB {                               // the interface base at +0x18 (vtable only)
    virtual void vb00();
};
struct CComponentRoot {                            // the primary base: vptr at +0
    virtual void v00();
};
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body1c[0x34 - 0x1c];
    C2DVector Position;                            // +0x34
    C2DVector TargetPosition;                      // +0x3c
    unsigned char _body44[0x15c - 0x44];
    void Update(float time);                       // 0x0052C7E0
};
struct CZoomingComponent : CChangingStateComponent {
    unsigned char _body15c[0x178 - 0x15c];
    bool Panning;                                  // +0x178
    void Update(float time);
};

void CZoomingComponent::Update(float time)
{
    CChangingStateComponent::Update(time);
    if ((float)fabs(TargetPosition.X - Position.X) <= g_positionEpsilon &&
        (float)fabs(TargetPosition.Y - Position.Y) <= g_positionEpsilon)
        Panning = false;
}
}