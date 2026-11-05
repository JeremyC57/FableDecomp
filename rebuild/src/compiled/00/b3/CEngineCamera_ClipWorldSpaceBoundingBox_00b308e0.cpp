// CEngineCamera::ClipWorldSpaceBoundingBox(const C3DBoundingBox& box, float farDistance, bool& fullyInside) const -- frustum-cull a world-space box: false when the box lies wholly outside one of the four side planes or wholly beyond farDistance along the look vector; fullyInside is cleared when the box straddles any plane or the far distance
// Leaf: ClipWorldSpaceBoundingBox
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B308E0, length 616 bytes (`ret 0xc`,
// ?ClipWorldSpaceBoundingBox@CEngineCamera@@QBE_NABVC3DBoundingBox@@MAA_N@Z). Constant 0.0f 0x0122DEDC.
// Layout: Ego_r.pdb CEngineCamera (CameraDesc +0x14 with WorldPosition at desc +0x40 = +0x54, LookVector +0x1b4,
// FrustrumPlanes CPlane[4] +0x1c0 {Normal, Distance}); C3DBoundingBox {MinCoord, MaxCoord}. Modelled locally (the shared
// header's DebugScript is the PDB's 16-byte CArray; nothing past +0x200 is touched here).
#include "engine/C3DVector.h"
class C3DBoundingBox { public: C3DVector MinCoord; C3DVector MaxCoord; };
class CPlane { public: C3DVector Normal; float Distance; };

class CEngineCamera {
public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                        // +0x54 (CameraDesc.WorldPosition)
    unsigned char _pad60[0x1b4 - 0x60];
    C3DVector     LookVector;                           // +0x1b4
    CPlane        FrustrumPlanes[4];                    // +0x1c0
    bool ClipWorldSpaceBoundingBox(const C3DBoundingBox& box, float farDistance, bool& fullyInside) const;
};

bool CEngineCamera::ClipWorldSpaceBoundingBox(const C3DBoundingBox& box, float farDistance, bool& fullyInside) const
{
    fullyInside = true;
    for (long i = 0; i < 4; ++i) {
        C3DVector normal(FrustrumPlanes[i].Normal);
        C3DVector nearPoint;
        nearPoint.X = normal.X > 0.0f ? box.MinCoord.X : box.MaxCoord.X;
        nearPoint.Y = normal.Y > 0.0f ? box.MinCoord.Y : box.MaxCoord.Y;
        nearPoint.Z = normal.Z > 0.0f ? box.MinCoord.Z : box.MaxCoord.Z;
        if (nearPoint.Dot(normal) > FrustrumPlanes[i].Distance) {
            fullyInside = false;
            return false;
        }
        if (fullyInside) {
            C3DVector farPoint;
            farPoint.X = normal.X > 0.0f ? box.MaxCoord.X : box.MinCoord.X;
            farPoint.Y = normal.Y > 0.0f ? box.MaxCoord.Y : box.MinCoord.Y;
            farPoint.Z = normal.Z > 0.0f ? box.MaxCoord.Z : box.MinCoord.Z;
            if (farPoint.Dot(normal) > FrustrumPlanes[i].Distance)
                fullyInside = false;
        }
    }
    C3DVector look = LookVector;
    C3DVector nearPoint;
    nearPoint.X = look.X > 0.0f ? box.MinCoord.X : box.MaxCoord.X;
    nearPoint.Y = look.Y > 0.0f ? box.MinCoord.Y : box.MaxCoord.Y;
    nearPoint.Z = look.Z > 0.0f ? box.MinCoord.Z : box.MaxCoord.Z;
    if ((nearPoint - WorldPosition).Dot(look) > farDistance) {
        fullyInside = false;
        return false;
    }
    if (fullyInside) {
        C3DVector farPoint;
        farPoint.X = look.X > 0.0f ? box.MaxCoord.X : box.MinCoord.X;
        farPoint.Y = look.Y > 0.0f ? box.MaxCoord.Y : box.MinCoord.Y;
        farPoint.Z = look.Z > 0.0f ? box.MaxCoord.Z : box.MinCoord.Z;
        if ((farPoint - WorldPosition).Dot(look) > farDistance)
            fullyInside = false;
    }
    return true;
}