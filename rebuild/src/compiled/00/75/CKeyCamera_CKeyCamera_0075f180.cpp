// CKeyCamera::CKeyCamera(const C3DVector& pos, const C3DVector& lookDirection, float fov, float roll, float shuttleSpeed, float duration, float pauseTime) -- value constructor of one camera-path key (the manifest's CManagerShadowedSpotLight label is a byte-twin propagation)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F180, length 107 bytes. Callee: CCharString::CCharString() @ 0x0099E4B0.
// Leaf: ??0CKeyCamera
// Layout (PDB CKeyCamera, 0x38, no vtable): DeltaIndex +0x00 (zeroed first), Pos +0x04, LookDirection +0x10, FOV +0x1c,
//   Roll +0x20, ShuttleSpeed +0x24, Duration +0x28, PauseTime +0x2c, EventName (CCharString) +0x30, SpeedFraction +0x34.
// Shape: C3DVector copies are its inline memberwise copy constructor (direct [esi+N] stores); the five floats are
//   init-list copies (the scheduler interleaves them), then the default CCharString constructor, then SpeedFraction = 0.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};

struct CKeyCamera {
    float       DeltaIndex;                        // +0x00
    C3DVector   Pos;                               // +0x04
    C3DVector   LookDirection;                     // +0x10
    float       FOV;                               // +0x1c
    float       Roll;                              // +0x20
    float       ShuttleSpeed;                      // +0x24
    float       Duration;                          // +0x28
    float       PauseTime;                         // +0x2c
    CCharString EventName;                         // +0x30
    float       SpeedFraction;                     // +0x34
    CKeyCamera(const C3DVector& pos, const C3DVector& lookDirection, float fov, float roll, float shuttleSpeed, float duration, float pauseTime);
};

CKeyCamera::CKeyCamera(const C3DVector& pos, const C3DVector& lookDirection, float fov, float roll, float shuttleSpeed, float duration, float pauseTime)
    : DeltaIndex(0), Pos(pos), LookDirection(lookDirection), FOV(fov), Roll(roll), ShuttleSpeed(shuttleSpeed), Duration(duration),
      PauseTime(pauseTime), SpeedFraction(0)
{
}