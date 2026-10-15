#include <cstdio>
struct SVolumeParams {
    long m_mode; float m_volume; float m_fadeTime;
    bool operator==(const SVolumeParams& o) const { return m_mode == o.m_mode && m_volume == o.m_volume && m_fadeTime == o.m_fadeTime; }
};
struct CMusicManager {
    unsigned int m_dirtyFlags;
    void SetVolumeParams(SVolumeParams** current, SVolumeParams* storage, const SVolumeParams* value)
    {
        if (!value) { if (*current) { *current = 0; m_dirtyFlags |= 0x100; } return; }
        if (!*current) { *current = storage; *storage = *value; m_dirtyFlags |= 0x100; return; }
        if (!(*storage == *value)) { *storage = *value; m_dirtyFlags |= 0x100; }
    }
};
int main()
{
    CMusicManager m; m.m_dirtyFlags = 0;
    SVolumeParams store = { 0, 0.0f, 0.0f }; SVolumeParams* cur = 0; SVolumeParams v = { 1, 0.5f, 2.0f };
    bool fail = false;
    m.SetVolumeParams(&cur, &store, &v); if (cur != &store || store.m_fadeTime != 2.0f || m.m_dirtyFlags != 0x100) fail = true;
    m.m_dirtyFlags = 0; m.SetVolumeParams(&cur, &store, &v); if (m.m_dirtyFlags != 0) fail = true;
    v.m_fadeTime = 3.0f; m.SetVolumeParams(&cur, &store, &v); if (store.m_fadeTime != 3.0f || m.m_dirtyFlags != 0x100) fail = true;
    m.m_dirtyFlags = 0; m.SetVolumeParams(&cur, &store, 0); if (cur != 0 || m.m_dirtyFlags != 0x100) fail = true;
    if (fail) { std::printf("00c434a0_TEST FAIL\n"); return 1; }
    std::printf("00c434a0_TEST PASS\n");
    return 0;
}
