// CEnginePrimitivePrepareInfo::CEnginePrimitivePrepareInfo(const CEnginePrimitivePrepareInfo& other) -- memberwise copy: the interpolation block, the engine pointer, the 2D-edit flag and the camera
// Leaf: ??0CEnginePrimitivePrepareInfo@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00528200, 52 bytes, `ret 4`: `rep movsd` of 13 dwords (+0x00..+0x33), the dword at
//   +0x34, the byte at +0x38, then CCamera's copy constructor on +0x3C; returns this.
// Identity (proven): Ego_r.pdb CEnginePrimitivePrepareInfo is {CInterpolationInfo Interpolate +0x00 (0x34 bytes); CIEngine*
//   Engine +0x34; bool Edit2DEngine +0x38; CCamera Camera +0x3C} and the callee 0x0049D380 is the landed
//   CCamera::CCamera(const CCamera&); the PDB name sets hold ??0CEnginePrimitivePrepareInfo@@QAE@ABV0@@Z. No direct
//   caller (the one xref.py hit, 0x0046BA9C, decodes to no instruction).
// Shape: a memberwise copy constructor: the trivially copyable CInterpolationInfo (six 8-byte CInterpolationInfoSet plus a
//   long) is one `rep movsd` block, the pointer and the bool plain moves, the camera through its own copy constructor.
//   Written as a user copy constructor with the implicit one's member order (the implicit one gives the same code but is
//   only emitted inline).
class CIEngine;

class CInterpolationInfoSet {                   // 8 bytes, trivially copyable
public:
    long  Frame;                                // +0x00 (descriptive)
    float Fraction;                             // +0x04 (descriptive)
};

class CInterpolationInfo {                      // 0x34 bytes (Ego_r.pdb)
public:
    CInterpolationInfoSet Current;              // +0x00
    CInterpolationInfoSet Last;                 // +0x08
    CInterpolationInfoSet PausedCurrent;        // +0x10
    CInterpolationInfoSet PausedLast;           // +0x18
    CInterpolationInfoSet BulletTimeCurrent;    // +0x20
    CInterpolationInfoSet BulletTimeLast;       // +0x28
    long                  WFServerCurrentTime;  // +0x30
};

class C3DVector { public: float X, Y, Z; };

class CCamera {                                 // 0x38 bytes (Ego_r.pdb), 4-aligned
public:
    C3DVector     WorldPosition;                // +0x00
    C3DVector     Up;                           // +0x0C (RHSet)
    C3DVector     Forward;                      // +0x18 (RHSet)
    bool          HeightLocked;                 // +0x24
    bool          ViewVecZLocked;               // +0x25
    unsigned long FOVFlags;                     // +0x28
    float         HorizontalFOV;                // +0x2C
    float         VerticalFOV;                  // +0x30
    float         Zoom;                         // +0x34
    CCamera(const CCamera& other);              // 0x0049D380
};

class CEnginePrimitivePrepareInfo {
public:
    CInterpolationInfo Interpolate;             // +0x00
    CIEngine*          Engine;                  // +0x34
    bool               Edit2DEngine;            // +0x38
    CCamera            Camera;                  // +0x3C
    CEnginePrimitivePrepareInfo(const CEnginePrimitivePrepareInfo& other);
};

CEnginePrimitivePrepareInfo::CEnginePrimitivePrepareInfo(const CEnginePrimitivePrepareInfo& other)
    : Interpolate(other.Interpolate), Engine(other.Engine), Edit2DEngine(other.Edit2DEngine), Camera(other.Camera)
{
}