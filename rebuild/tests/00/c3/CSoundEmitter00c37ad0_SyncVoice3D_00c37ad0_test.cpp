// Behaviour test for 00c37ad0: CSoundEmitter::SyncVoice3D -- hands the flagged cached parameters to the device update and copies the device's adjusted values back
#include <cstdio>

struct CVector3 {
    float x, y, z;
    CVector3() : x(0), y(0), z(0) { }
    CVector3& operator=(const CVector3& v) { x = v.x; y = v.y; z = v.z; return *this; }
};

struct CVoiceDevice {
    int m_calls;
    long m_handle;
    bool m_gotScalar, m_gotPosition;
    void Update(long handle, float* pScalar, void*, CVector3* pPosition, void*)
    {
        ++m_calls; m_handle = handle;
        m_gotScalar = pScalar != 0; m_gotPosition = pPosition != 0;
        if (pScalar) *pScalar *= 2.0f;
        if (pPosition) pPosition->z += 1.0f;
    }
};
struct COwner {
    CVoiceDevice* m_pDevice;
    CVoiceDevice* GetDevice() { return m_pDevice; }
};

struct CSoundEmitter {
    COwner* m_pOwner;
    long m_handle;
    float m_scalar;
    CVector3 m_position;
    long m_flags;
    void SyncVoice3D()
    {
        if (m_flags == 0)
            return;
        if (!m_pOwner->GetDevice())
            return;
        CVector3 position;
        float scalar;
        CVector3* pPosition = (m_flags & 2) ? &position : 0;
        float* pScalar = (m_flags & 1) ? &scalar : 0;
        if (pPosition)
            *pPosition = m_position;
        if (pScalar)
            *pScalar = m_scalar;
        m_pOwner->GetDevice()->Update(m_handle, pScalar, 0, pPosition, 0);
        if (pPosition)
            m_position = *pPosition;
        if (pScalar)
            m_scalar = *pScalar;
    }
};

static int fail() { std::printf("00c37ad0_TEST FAIL\n"); return 1; }

int main()
{
    CVoiceDevice device = { 0, 0, false, false };
    COwner owner = { 0 };
    CSoundEmitter e;
    e.m_pOwner = &owner; e.m_handle = 12; e.m_scalar = 0.5f;
    e.m_position.x = 1.0f; e.m_position.y = 2.0f; e.m_position.z = 3.0f;
    e.m_flags = 3;
    e.SyncVoice3D();                                                 // no device
    if (device.m_calls != 0) return fail();
    owner.m_pDevice = &device;
    e.m_flags = 0;
    e.SyncVoice3D();                                                 // nothing flagged
    if (device.m_calls != 0) return fail();
    e.m_flags = 1;
    e.SyncVoice3D();
    if (device.m_calls != 1 || !device.m_gotScalar || device.m_gotPosition || e.m_scalar != 1.0f || e.m_position.z != 3.0f) return fail();
    e.m_flags = 2;
    e.SyncVoice3D();
    if (device.m_calls != 2 || device.m_gotScalar || !device.m_gotPosition || e.m_position.z != 4.0f || e.m_position.x != 1.0f || device.m_handle != 12) return fail();
    std::printf("00c37ad0_TEST PASS\n");
    return 0;
}