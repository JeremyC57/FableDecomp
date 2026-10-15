// CLocalDetailPrimitiveBase::ClipPrimitive(CLocalDetailObjectCollectionType* type, CEngineCamera* camera, bool skipDistanceTest, unsigned long clipFlags) -- false when the primitive's bounding sphere lies beyond the collection's fade-out distance from the camera (unless the distance test is skipped), else the camera's sphere clip: visible unless the "outside" clip bit (bit 6) comes back set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF9AA0, length 160 bytes.
// Callees: CLocalDetailObjectCollectionType::PeekFadeEnd() @ 0x00BE2740, CEngineCamera::ClipSphere(const C3DVector*, float, unsigned long*) @ 0x00B30480.
// Shape: the camera-to-centre delta is a 3-float frame local (camera position +0x54, sphere centre +0x20, radius
// +0x2c); `range * range < z*z + y*y + x*x` squares the range first and keeps it on the FPU; the clip flags are
// the by-value argument passed by address, read back as an `unsigned char` bit field; `Outside ? false : true`
// gives the 8-bit `shr al,6 ; not al ; and al,1` (`!Outside` widens to eax).
struct C3DVector {
    float x, y, z;
};
struct SClipFlags {                                // the camera clip result bits
    unsigned char Planes  : 6;
    unsigned char Outside : 1;                     // bit 6
};
struct CLocalDetailObjectCollectionType {
    float PeekFadeEnd() const;                     // 0x00BE2740
};
struct CEngineCamera {
    unsigned char _pad00[0x54];
    C3DVector     m_position;                      // +0x54
    bool ClipSphere(const C3DVector* centre, float radius, unsigned long* clipFlags);   // 0x00B30480
};
struct CLocalDetailPrimitiveBase {
    unsigned char _pad00[0x20];
    C3DVector     m_centre;                        // +0x20
    float         m_radius;                        // +0x2c
    bool ClipPrimitive(CLocalDetailObjectCollectionType* type, CEngineCamera* camera, bool skipDistanceTest, unsigned long clipFlags);
};

bool CLocalDetailPrimitiveBase::ClipPrimitive(CLocalDetailObjectCollectionType* type, CEngineCamera* camera, bool skipDistanceTest, unsigned long clipFlags)
{
    if (!skipDistanceTest)
    {
        C3DVector d;
        d.x = camera->m_position.x - m_centre.x;
        d.y = camera->m_position.y - m_centre.y;
        d.z = camera->m_position.z - m_centre.z;
        float range = type->PeekFadeEnd() + m_radius;
        if (range * range < d.z * d.z + d.y * d.y + d.x * d.x)
            return false;
    }
    camera->ClipSphere(&m_centre, m_radius, &clipFlags);
    return ((SClipFlags*)&clipFlags)->Outside ? false : true;
}