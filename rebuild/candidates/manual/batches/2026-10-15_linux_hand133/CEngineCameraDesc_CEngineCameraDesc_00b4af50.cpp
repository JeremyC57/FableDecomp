// CEngineCameraDesc::CEngineCameraDesc(const CEngineCameraDesc& other) -- copy constructor: member-wise copy of the 0x6c-byte camera description
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4AF50, length 114 bytes.
// Callees: none. Layout = the PDB CEngineCameraDesc (0x6c bytes): Window C2DBoxF +0x00, RotationMatrix CMatrix3x4 +0x10, WorldPosition
// C3DVector +0x40, HorizontalFOV +0x4c, VerticalFOV +0x50, Use2DFOV +0x54, NearZPlane +0x58, FarZPlane +0x5c, ZBufferStart +0x60,
// ZBufferEnd +0x64, StateValid +0x68.
// Shape: `mov eax,ecx` (returns `this`), the window's four dwords through ecx alone (C2DBoxF has a user member-wise copy constructor: a POD
// member copies through re-based esi/edi pointers instead), the 48-byte matrix as a `rep movsd` block (a POD member), the position's three
// dwords through ecx (C3DVector's user copy constructor), then every scalar dword / byte through ecx/cl -- a member-wise copy constructor.
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
};

struct CMatrix3x4 {
    float m_elements[12];
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct CEngineCameraDesc {
    C2DBoxF    Window;               // +0x00
    CMatrix3x4 RotationMatrix;       // +0x10
    C3DVector  WorldPosition;        // +0x40
    float      HorizontalFOV;        // +0x4c
    float      VerticalFOV;          // +0x50
    bool       Use2DFOV;             // +0x54
    float      NearZPlane;           // +0x58
    float      FarZPlane;            // +0x5c
    float      ZBufferStart;         // +0x60
    float      ZBufferEnd;           // +0x64
    bool       StateValid;           // +0x68
    CEngineCameraDesc(const CEngineCameraDesc& other);
};

CEngineCameraDesc::CEngineCameraDesc(const CEngineCameraDesc& other)
    : Window(other.Window),
      RotationMatrix(other.RotationMatrix),
      WorldPosition(other.WorldPosition),
      HorizontalFOV(other.HorizontalFOV),
      VerticalFOV(other.VerticalFOV),
      Use2DFOV(other.Use2DFOV),
      NearZPlane(other.NearZPlane),
      FarZPlane(other.FarZPlane),
      ZBufferStart(other.ZBufferStart),
      ZBufferEnd(other.ZBufferEnd),
      StateValid(other.StateValid)
{
}
