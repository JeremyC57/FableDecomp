// CEngineInternalPrimitiveSprite::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere) -- a sphere centred on the sprite position whose radius is the length of the anchored half-extents (|anchor*size| + size per axis); always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACF00, length 103 bytes (`ret 4`); vtable 0x012A5444 slot 18.
// Callees: none (fabs and fsqrt inline: `#pragma intrinsic(fabs)`, sqrtf from <math.h>).
// Layout: no PDB layout for the sprite here; position C3DVector +0x48, width/height +0x60/+0x64, anchor factors +0x68/+0x6C,
// modelled locally.
// Shape: the sphere is a constructed temporary `C3DBoundingSphere(position, radius)` assigned to the argument: C3DVector's
// memberwise USER copy constructor makes the centre a real copy into the 16-byte frame temporary (`mov [esp],eax ...`, then the
// implicit operator= copies the four words out); without it VC7.1 forwards the position straight into the argument.
#include <math.h>
#pragma intrinsic(fabs)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() { }
    C3DBoundingSphere(const C3DVector& centre, float radius) : Center(centre), Radius(radius) { }
};

class CEngineInternalPrimitiveSprite {
public:
    virtual void v000();
    unsigned char _pad04[0x44];
    C3DVector     m_position;                      // +0x48
    unsigned char _pad54[0xc];
    float         m_width;                         // +0x60
    float         m_height;                        // +0x64
    float         m_anchorX;                       // +0x68
    float         m_anchorY;                       // +0x6c
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere);
};

bool CEngineInternalPrimitiveSprite::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere)
{
    float w = (float)fabs(m_anchorX * m_width) + m_width;
    float h = (float)fabs(m_anchorY * m_height) + m_height;
    sphere = C3DBoundingSphere(m_position, sqrtf(w * w + h * h));
    return true;
}
