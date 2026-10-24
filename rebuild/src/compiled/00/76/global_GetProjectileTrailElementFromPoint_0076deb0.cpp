// GetProjectileTrailElementFromPoint(const C3DVector& point) -- (file-local, anonymous namespace) the two edge points of a projectile's weapon-trail element at a point: half of an eighth of the render camera's up vector below and above the point
// Leaf: ?GetProjectileTrailElementFromPoint@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076DEB0, 216 bytes (+8 int3), `__fastcall` (hidden pair result in ecx,
//   point in edx), `ret`.
// Name: FableWin.pdb `?GetProjectileTrailElementFromPoint@?A0x22d77832@@YI?AU?$pair@VC3DVector@@V1@@std@@ABVC3DVector@@@Z`
//   (a file-local helper of the CTCWeaponTrail TU: three callers at 0x0076E2AB, 0x0076F0C8, 0x0076F118). The catalog
//   name is right.
// Callees: the camera manager's virtual GetRenderCamera(float) const (slot 1, +0x04: CCamera by hidden pointer) through the
//   engine singleton 0x013B86A0 (+0x24 world, +0x34 camera manager; the landed CTCBoastUI::Deactivate 0x0056A880 path).
// Constants: 1.0f (pushed), 0.125f 0x0123095C, 0.5f 0x0122F59C.
// Layout: CCamera (56 bytes, Ego_r.pdb): WorldPosition +0x00, RHSet (CRightHandedSet: Up +0x0c, Forward +0x18), ...
// Shape: inline C3DVector operators with the memberwise copy constructor (as in this TU's list / vector rows 0x0076FD40,
//   0x007700D0); `offset * 0.5f` is written in BOTH pair arguments, `point - offset * 0.5f` and `offset * 0.5f + point`,
//   and VC7.1 CSEs the halves (the `fld st(0)` duplicate and the scattered spills); a named `half` local gives a 0x50 frame,
//   the copy constructor without the CSE a 0x5c one. The second argument is evaluated first.
// The file-local helper is kept emitted by the exported caller below (not part of the graded body).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}

class CRightHandedSet {
public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0c
};

class CCamera {
public:
    C3DVector WorldPosition;                            // +0x00
    CRightHandedSet RHSet;                              // +0x0c
    bool HeightLocked;                                  // +0x24
    bool ViewVecZLocked;                                // +0x25
    unsigned long FOVFlags;                             // +0x28
    float HorizontalFOV;                                // +0x2c
    float VerticalFOV;                                  // +0x30
    float Zoom;                                         // +0x34
};

class CGameCameraManager {
public:
    virtual void Update();                              // slot 0
    virtual CCamera GetRenderCamera(float t) const;     // slot 1 (+0x04)
};
class CGameWorldCalls {
public:
    unsigned char _pad_0x00[0x34];
    CGameCameraManager* m_pCameraManager;               // +0x34
};
class CEngineCalls {
public:
    unsigned char _pad_0x00[0x24];
    CGameWorldCalls* m_pWorld;                          // +0x24
};
extern CEngineCalls* g_pEngine;                         // 0x013B86A0

namespace {
std::pair<C3DVector, C3DVector> __fastcall GetProjectileTrailElementFromPoint(const C3DVector& point)
{
    CCamera camera = g_pEngine->m_pWorld->m_pCameraManager->GetRenderCamera(1.0f);
    C3DVector offset = camera.RHSet.Up * 0.125f;
    return std::pair<C3DVector, C3DVector>(point - offset * 0.5f, offset * 0.5f + point);
}
}

std::pair<C3DVector, C3DVector> __fastcall CallGetProjectileTrailElementFromPoint(const C3DVector& point)   // keeps the helper emitted
{
    return GetProjectileTrailElementFromPoint(point);
}