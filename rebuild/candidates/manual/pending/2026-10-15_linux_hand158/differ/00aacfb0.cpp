// CShaderRenderManager::UpdateFogColour() -- when the owner's device exists, copy three colour vectors (+4, +0x1c, +0x10) into locals and hand them to the device's fog target (slot 12)
// Grade: DIFFER(108v108) -- the last copy stores z ([esp+8]) before y ([esp+4]) around the device reload; retail stores y then z (scheduling only)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACFB0, length 108 bytes. Callees: device virtual slot 6 (+0x18) returns the fog target, its slot 12 (+0x30).
// Shape: memberwise copies need a user copy ctor on the vector; the owner pointer is a named local (kept in eax).
struct C3DVector { float x, y, z; C3DVector(const C3DVector& v) : x(v.x), y(v.y), z(v.z) {} };
struct CFogTarget {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11();
    virtual void SetFog(const C3DVector& a, const C3DVector& b, const C3DVector& c);   // slot 12
};
struct CDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual CFogTarget* GetFogTarget();   // slot 6
};
struct COwner { unsigned char _pad[0x40]; CDevice* m_pDevice; };
struct CShaderRenderManager {
    void*     _vf;
    C3DVector m_a;      // +0x04
    C3DVector m_c;      // +0x10
    C3DVector m_b;      // +0x1c
    COwner*   m_pOwner; // +0x28
    void UpdateFogColour();
};
void CShaderRenderManager::UpdateFogColour()
{
    COwner* owner = m_pOwner;
    if (owner->m_pDevice) {
        C3DVector a = m_a;
        C3DVector b = m_b;
        C3DVector c = m_c;
        owner->m_pDevice->GetFogTarget()->SetFog(a, b, c);
    }
}
