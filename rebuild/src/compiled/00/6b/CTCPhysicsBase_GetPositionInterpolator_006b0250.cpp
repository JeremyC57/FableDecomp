// CTCPhysicsBase::GetPositionInterpolator(C3DPositionInterpolator& out) const -- the position interpolator from the last frame's position to the current one; under bullet time for a thing it affects, the pair is instead two positions interpolated between them at the current and next sub-step of the bullet-time cycle
// Leaf: ?GetPositionInterpolator@CTCPhysicsBase@@QBEXAAVC3DPositionInterpolator@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B0250, 524 bytes, `ret 4`.
// Callees: CTCBase::GetWorld() const 0x00686A80 (twice), CThing::IsAffectedByBulletTime 0x004C9920, CBulletTime::GetTimeScale
//   0x004C5FA0, CWorld::GetWorldFrame 0x0049D870 (static), _ftol 0x00BFEA70, CBase::CBase 0x0099A2D0, ~CBase 0x0099A2E0.
//   Global 1.0f 0x0122DED8. Vtable 0x0123981C = C3DPositionInterpolator.
// Layout: as the landed sibling CTCPhysicsBase::GetOrientationInterpolator 0x006B0460 (local model: the generated
//   engine/CTCPhysicsBase.h flattens Position/OldPosition into floats with no C3DVector members). Position +0x0c,
//   OldPosition +0x18 (PDB); C3DPositionInterpolator 0x1c bytes (CBase vptr, StartPosition +0x04, EndPosition +0x10),
//   its constructor and assignment inline here (the landed 0x006AF570 ctor is the out-of-line copy).
class CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline C3DVector Interpolate(const C3DVector& to, float t) const { return (to - *this) * t + *this; }
};
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                        // 0x0099A2D0
    ~CBase();                                       // 0x0099A2E0
}; } using N_0099a2d0::CBase;
struct C3DPositionInterpolator : CBase {            // 0x1c bytes
    C3DVector StartPosition;                        // +0x04
    C3DVector EndPosition;                          // +0x10
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end)
        : CBase(), StartPosition(start), EndPosition(end) { }
    C3DPositionInterpolator& operator=(const C3DPositionInterpolator& other)
    {
        StartPosition = other.StartPosition;
        EndPosition = other.EndPosition;
        return *this;
    }
    virtual void v00();
};

struct CBulletTime {
    unsigned char _pad0[4];
    void*         m_pController;                    // +0x04
    unsigned char _pad1[4];
    bool          m_active;                         // +0x0c
    float GetTimeScale();                           // 0x004C5FA0
    bool IsActive() const { return m_active || m_pController != 0; }
};
struct CWorld {
    unsigned char _pad[0x68];
    CBulletTime*  m_pBulletTime;                    // +0x68
    static long GetWorldFrame();                    // 0x0049D870
};
class CThing {
public:
    bool IsAffectedByBulletTime() const;            // 0x004C9920
};

class CTCPhysicsBase {
public:
    virtual void v000();
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[4];
    C3DVector     Position;                         // +0x0c
    C3DVector     OldPosition;                      // +0x18
    const CWorld* GetWorld() const;                 // 0x00686A80 (CTCBase)
    void GetPositionInterpolator(C3DPositionInterpolator& out) const;
};

void CTCPhysicsBase::GetPositionInterpolator(C3DPositionInterpolator& out) const
{
    if (GetWorld()->m_pBulletTime->IsActive() && ParentThing->IsAffectedByBulletTime()) {
        float timeScale = GetWorld()->m_pBulletTime->GetTimeScale();
        long step = (CWorld::GetWorldFrame() - 1) % (long)timeScale;
        float stepSize = 1.0f / timeScale;
        C3DVector start = OldPosition.Interpolate(Position, step * stepSize);
        ++step;
        C3DVector end = OldPosition.Interpolate(Position, step * stepSize);
        out = C3DPositionInterpolator(start, end);
    } else {
        out = C3DPositionInterpolator(OldPosition, Position);
    }
}