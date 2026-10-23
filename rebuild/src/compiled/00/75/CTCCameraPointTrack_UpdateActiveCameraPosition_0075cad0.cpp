// CTCCameraPointTrack::UpdateActiveCameraPosition() -- keep the drag camera between the cage radius and the string length from the hero in the XY plane, pulling it onto the nearer circle when it strays
// Leaf: UpdateActiveCameraPosition
// Retail 0x0075CAD0, 400 bytes, `ret`.
// Callees: the TC's game accessor 0x00686A70, its player-manager accessor 0x0049D9D0, the main hero accessor 0x00487DD0,
// CThing::GetPosition 0x004C73D0. Pools: 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout = Ego_r.pdb CTCCameraPointTrack: CameraStringLength +0x40, CameraCageRadius +0x44, DragCamPos +0x48.
// Shape: the hero position is copied into a local; the offset is an XY C3DVector (Z = 0) and its squared XY length is
// `X*X + Y*Y` (VC7.1 squares Y first); the new position is `hero - offset * radius / sqrt(lengthSq)` with C3DVector's
// inline operators (`/` multiplies by the reciprocal).
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    float GetLengthSquared2D() const { return X * X + Y * Y; }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator/(float s) const { float inv = 1.0f / s; return C3DVector(X * inv, Y * inv, Z * inv); }
};
struct CThing {
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CPlayerManager {
    CThing* GetMainHero();                              // 0x00487DD0
};
struct CGame {
    CPlayerManager* GetPlayerManager();                 // 0x0049D9D0
};
class CTCCameraPointTrack {
public:
    void*         __vftable;                            // +0x00
    unsigned char _pad04[0x40 - 0x04];
    float         CameraStringLength;                   // +0x40
    float         CameraCageRadius;                     // +0x44
    C3DVector     DragCamPos;                           // +0x48
    CGame* GetGame();                                   // 0x00686A70
    void UpdateActiveCameraPosition();
};

void CTCCameraPointTrack::UpdateActiveCameraPosition()
{
    C3DVector heroPos = GetGame()->GetPlayerManager()->GetMainHero()->GetPosition();
    const C3DVector& cam = DragCamPos;
    C3DVector offset(heroPos.X - cam.X, heroPos.Y - cam.Y, 0.0f);
    float lengthSq = offset.GetLengthSquared2D();
    if (lengthSq > CameraStringLength * CameraStringLength)
        DragCamPos = heroPos - offset * CameraStringLength / (float)sqrt(lengthSq);
    else if (lengthSq < CameraCageRadius * CameraCageRadius)
        DragCamPos = heroPos - offset * CameraCageRadius / (float)sqrt(lengthSq);
}