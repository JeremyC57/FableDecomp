// CGameCameraManager::IsPosInViewingFrustrum(const C3DVector& pos) const -- the frustum test (CTools @ 0x00875120 on the helper at +0x78) over the view record at +0x98 (its matrix at +0x10 and plane set at +0x54) with the camera's field of view (+4 -> +0x2c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697F50, length 48 bytes.
// Callees: the field-of-view getter @ 0x00A0BEB0 (`fld [ecx+2Ch]` on the camera object at +4; labelled "CMap::DrawGetSeaLevel", propagated), CTools::
// IsPosInViewingFrustrum @ 0x00875120 (on +0x78: pos, &view+0x10, &view+0x54, fov). Shape: the float pushed through `push ecx ; fstp [esp]`, the view record
// in edi, the bool returned in al, `ret 4`. Virtual const (the mangled name in the manifest).
// Leaf: ?IsPosInViewingFrustrum@CGameCameraManager@@UBE_NABVC3DVector@@@Z
struct C3DVector;
struct CCameraObject_00697f50 {
    unsigned char _body[0x74];
    float GetFieldOfView() const;                  // 0x00A0BEB0
};
struct SViewRecord_00697f50 {
    unsigned char _pad[0x10];
    unsigned char m_matrix[0x44];                  // +0x10
    unsigned char m_planes[0x40];                  // +0x54
};
struct CTools_00697f50 {
    unsigned char _body[0x20];
    bool IsPosInViewingFrustrum(const C3DVector& pos, const void* matrix, const void* planes, float fieldOfView);   // 0x00875120
};

struct CGameCameraManager {
    virtual void v00();
    CCameraObject_00697f50 m_camera;               // +0x04
    CTools_00697f50        m_tools;                // +0x78
    SViewRecord_00697f50*  m_pView;                // +0x98
    virtual bool IsPosInViewingFrustrum(const C3DVector& pos) const;
};

bool CGameCameraManager::IsPosInViewingFrustrum(const C3DVector& pos) const
{
    return const_cast<CTools_00697f50&>(m_tools).IsPosInViewingFrustrum(pos, m_pView->m_matrix, m_pView->m_planes, m_camera.GetFieldOfView());
}