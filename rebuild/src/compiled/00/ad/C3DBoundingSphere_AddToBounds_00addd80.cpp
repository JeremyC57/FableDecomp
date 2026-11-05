// C3DBoundingSphere::AddToBounds(const C3DVector& point, float radius) -- grow the sphere to enclose another sphere: adopt it when empty or when it swallows this one, else span both
// Leaf: ?AddToBounds@C3DBoundingSphere@@QAEXABVC3DVector@@M@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local Length respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 35c: the exact PDB symbol (Ego_r.pdb + FableWin.pdb, the (const C3DVector&, float) overload the landed CPSCLight::AddToBoundingSphere 0x00B18AF0 binds); body unchanged.
#include <math.h>

#include "engine/C3DVector.h"

class C3DBoundingSphere {
public:
    C3DVector Center;   // +0x00
    float     Radius;   // +0x0c
    void AddToBounds(const C3DVector& point, float radius);
};

void C3DBoundingSphere::AddToBounds(const C3DVector& point, float radius)
{
    if (Radius == 0.0f) {
        Center = point;
        Radius = radius;
    } else if (radius != 0.0f) {
        C3DVector offset = Center - point;
        float distance = offset.GetMagnitude();
        if (Radius < radius) {
            if (radius - Radius > distance) {
                Center = point;
                Radius = radius;
                return;
            }
        } else if (Radius - radius > distance) {
            return;
        }
        Center = (point + Center + offset * (Radius - radius) / distance) * 0.5f;
        Radius = (distance + radius + Radius) * 0.5f;
    }
}