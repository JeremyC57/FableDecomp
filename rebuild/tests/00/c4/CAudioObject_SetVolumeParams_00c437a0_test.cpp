// Behaviour test for 00c437a0: CAudioObject::SetVolumeParams -- forwards (&m_pVolumeParams, &m_volumeParamsStorage, value) to the set-or-clear helper
#include <cstdio>

struct SVolumeParams {
    long  m_mode;
    float m_volume;
    float m_fadeTime;
    bool operator==(const SVolumeParams& o) const { return m_mode == o.m_mode && m_volume == o.m_volume && m_fadeTime == o.m_fadeTime; }
};

class CAudioObject {
public:
    CAudioObject() : m_dirtyFlags(0), m_pVolumeParams(0) { m_storage.m_mode = 0; m_storage.m_volume = 0.0f; m_storage.m_fadeTime = 0.0f; }
    virtual ~CAudioObject() {}
    virtual void SetVolumeParams(const SVolumeParams* value) { UpdateVolumeParams(&m_pVolumeParams, &m_storage, value); }
    // the landed helper 0x00C434A0
    void UpdateVolumeParams(SVolumeParams** current, SVolumeParams* storage, const SVolumeParams* value)
    {
        if (!value) { if (*current) { *current = 0; m_dirtyFlags |= 0x100; } return; }
        if (!*current) { *current = storage; *storage = *value; m_dirtyFlags |= 0x100; return; }
        if (!(*storage == *value)) { *storage = *value; m_dirtyFlags |= 0x100; }
    }
    unsigned int   m_dirtyFlags;
    SVolumeParams* m_pVolumeParams;
    SVolumeParams  m_storage;
};

int main()
{
    CAudioObject o;
    SVolumeParams v; v.m_mode = 1; v.m_volume = 0.5f; v.m_fadeTime = 2.0f;
    o.SetVolumeParams(&v);
    if (o.m_pVolumeParams != &o.m_storage || o.m_storage.m_mode != 1 || o.m_dirtyFlags != 0x100) {
        std::printf("00c437a0_TEST FAIL\n"); return 1;
    }
    o.m_dirtyFlags = 0;
    v.m_volume = 0.75f;
    o.SetVolumeParams(&v);                     // changed: copied and dirty
    if (o.m_dirtyFlags != 0x100 || o.m_storage.m_volume != 0.75f) { std::printf("00c437a0_TEST FAIL\n"); return 1; }
    o.m_dirtyFlags = 0;
    o.SetVolumeParams(0);
    if (o.m_pVolumeParams != 0 || o.m_dirtyFlags != 0x100) { std::printf("00c437a0_TEST FAIL\n"); return 1; }
    std::printf("00c437a0_TEST PASS\n");
    return 0;
}