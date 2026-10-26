// NUISystem::CComponent::UpdatePosition(float) -- advance the position move by dt along a decelerating curve (snap to the target when the move time is up) and derive the render position and the saved (screen-converted when scaled) render position, both scaled by and offset from the parent unless position-independent
// Leaf: UpdatePosition
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052FFD0, 644 bytes (+ int3 padding), `ret 4`.
// Callees: pow(float, int) (VC7.1 math.h overload, inline _Pow_int), ConvertToScreen 0x0052E580 (__fastcall, C2DVector by value), virtual IsPositionScaled (+0x1d4) / IsPositionIndependent (+0x198).
// Note: the saved render position offset goes through a free inline Add(a, b) helper -- the member operator+= / explicit forms load RenderPositionSave.X first (x87 operand order), the helper keeps retail fld [RelativeRenderPosition.X] first.
#include <math.h>
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector operator/(float s) const { float inv = 1.0f / s; return C2DVector(X * inv, Y * inv); }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
};
inline void Add(C2DVector& a, const C2DVector& b) { a.X += b.X; a.Y += b.Y; }
C2DVector __fastcall ConvertToScreen(C2DVector position);   // 0x0052E580

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
namespace NUISystem {
class CComponent { public:
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF10(8) VF10(9)
    VF(100) VF(101)
    virtual bool IsPositionIndependent();               // slot 102 (+0x198)
    VF(103) VF(104) VF(105) VF(106) VF(107) VF(108) VF(109) VF(110) VF(111) VF(112) VF(113) VF(114) VF(115) VF(116)
    virtual bool IsPositionScaled();                    // slot 117 (+0x1d4)
    unsigned char _pad04[0x30 - 0x04];
    float     Time;                                     // +0x30
    C2DVector Position;                                 // +0x34
    C2DVector TargetPosition;                           // +0x3c
    C2DVector InitialPosition;                          // +0x44
    C2DVector ParentPosition;                           // +0x4c
    C2DVector RenderPosition;                           // +0x54
    C2DVector Zoom;                                     // +0x5c
    C2DVector TargetZoom;                               // +0x64
    C2DVector InitialZoom;                              // +0x6c
    C2DVector ParentZoom;                               // +0x74
    C2DVector RenderZoom;                               // +0x7c
    unsigned long Colour, TargetColour, InitialColour, ParentColour, RenderColour;   // +0x84
    float     PositionTimeElapsed;                      // +0x98
    float     PositionTime;                             // +0x9c
    unsigned char _pada0[0xf8 - 0xa0];
    C2DVector RenderPositionSave;                       // +0xf8
    C2DVector RelativeRenderPosition;                   // +0x100
    C2DVector RelativeParentPosition;                   // +0x108
    C2DVector RelativeRenderZoom;                       // +0x110
    void UpdatePosition(float dt);
};

void CComponent::UpdatePosition(float dt)
{
    if (PositionTimeElapsed < PositionTime) {
        PositionTimeElapsed += dt;
        C2DVector vel;
        C2DVector accel;
        accel = (TargetPosition - InitialPosition) * -2.0f / pow(PositionTime, 2);
        vel = (TargetPosition - InitialPosition) * 2.0f / PositionTime;
        Position = accel * 0.5f * pow(PositionTimeElapsed, 2) + vel * PositionTimeElapsed + InitialPosition;
        if (PositionTimeElapsed >= PositionTime) {
            Position = TargetPosition;
            InitialPosition = TargetPosition;
        }
    }
    RenderPosition = Position;
    if (IsPositionScaled())
        RenderPositionSave = ConvertToScreen(Position);
    else
        RenderPositionSave = Position;
    if (!IsPositionIndependent()) {
        RenderPosition.X *= ParentZoom.X;
        RenderPosition.Y *= ParentZoom.Y;
        RenderPosition += ParentPosition;
        RenderPositionSave.X *= RelativeRenderZoom.X;
        RenderPositionSave.Y *= RelativeRenderZoom.Y;
        Add(RenderPositionSave, RelativeRenderPosition);
    }
}
}