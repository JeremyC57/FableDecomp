// CObstacleNavigatorExternalInterface::PeekNavigatorPosition2D() const -- the obstacle's position cut to X/Y: the physics component's C3DVector at +0xC converted to a C2DVector returned by hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF434, length 26 bytes (`ret 4`, eax = the result).
// Layout: Obstacle (CThing*) at +4 (PDB); CThing::PhysicsTC at retail +0x60, its position at +0xC (CThing::GetPosition
// @ 0x004C73D0 reads `[this+60h]+0Ch`). Shape: the module is size-optimised (#pragma optimize("s", on)): the converting
// constructor C2DVector(const C3DVector&) copies X through the x87 and Y through ecx; the default (speed) build moves
// both words through integer registers.
// Leaf: PeekNavigatorPosition2D
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#pragma optimize("s", on)
#include "engine/C3DVector.h"

class C2DVector { public:
    float X;
    float Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) {}
    C2DVector* Normalise();                        // 0x00A14540
};

class CTCPhysicsBase {
public:
    virtual void v000();
    unsigned char _pad04[0x8];
    C3DVector     m_position;                      // +0x0C
};

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
};

class CObstacleNavigatorExternalInterface {
public:
    virtual void v000();
    CThing* Obstacle;                              // +0x04
    virtual C2DVector PeekNavigatorOrientation() const;
    virtual C2DVector PeekNavigatorPosition2D() const;
    virtual C3DVector PeekNavigatorPosition3D() const;
};

C2DVector CObstacleNavigatorExternalInterface::PeekNavigatorPosition2D() const
{
    return C2DVector(Obstacle->PhysicsTC->m_position);
}