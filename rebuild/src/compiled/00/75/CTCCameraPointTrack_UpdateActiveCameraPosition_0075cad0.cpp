// CTCCameraPointTrack::UpdateActiveCameraPosition() -- keep the drag camera between the cage radius and the string length from the hero in the XY plane, pulling it onto the nearer circle when it strays
// Leaf: UpdateActiveCameraPosition
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared2D respelled GetSquaredMagnitude2D (the header / FableWin.pdb method, same computation).
// Retail 0x0075CAD0, 400 bytes, `ret`.
// Callees: the TC's game accessor 0x00686A70, its player-manager accessor 0x0049D9D0, the main hero accessor 0x00487DD0,
// CThing::GetPos 0x004C73D0. Pools: 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout = Ego_r.pdb CTCCameraPointTrack: CameraStringLength +0x40, CameraCageRadius +0x44, DragCamPos +0x48.
// Shape: the hero position is copied into a local; the offset is an XY C3DVector (Z = 0) and its squared XY length is
// `X*X + Y*Y` (VC7.1 squares Y first); the new position is `hero - offset * radius / sqrt(lengthSq)` with C3DVector's
// inline operators (`/` multiplies by the reciprocal).
#include <math.h>
#pragma intrinsic(sqrt)
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;                              // 0x00487DD0
};
class CWorld { public:
    CPlayer* GetPlayerManager();                 // 0x0049D9D0
};
class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCCameraPointDefinitionBase : public CTCBase
{
public:
};
class CTCCameraPointShapeActivationBase : public CTCCameraPointDefinitionBase
{
public:
};

class CTCCameraPointTrack : public CTCCameraPointShapeActivationBase {
public:
    void*         __vftable;                            // +0x00
    unsigned char _pad04[0x40 - 0x04];
    float         CameraStringLength;                   // +0x40
    float         CameraCageRadius;                     // +0x44
    C3DVector     DragCamPos;                           // +0x48
    void UpdateActiveCameraPosition();
};

void CTCCameraPointTrack::UpdateActiveCameraPosition()
{
    C3DVector heroPos = GetWorld().GetPlayerManager()->PeekPControlledCreature()->GetPos();
    const C3DVector& cam = DragCamPos;
    C3DVector offset(heroPos.X - cam.X, heroPos.Y - cam.Y, 0.0f);
    float lengthSq = offset.GetSquaredMagnitude2D();
    if (lengthSq > CameraStringLength * CameraStringLength)
        DragCamPos = heroPos - offset * CameraStringLength / (float)sqrt(lengthSq);
    else if (lengthSq < CameraCageRadius * CameraCageRadius)
        DragCamPos = heroPos - offset * CameraCageRadius / (float)sqrt(lengthSq);
}