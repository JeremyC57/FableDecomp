#include <cstdio>
struct SLoopPoints {
    long m_start; float m_length;
    bool operator==(const SLoopPoints& o) const { return m_start == o.m_start && m_length == o.m_length; }
};
struct CMusicManager {
    unsigned int m_dirtyFlags;
    void SetLoopPoints(SLoopPoints** current, SLoopPoints* storage, const SLoopPoints* value)
    {
        if (!value) { if (*current) { *current = 0; m_dirtyFlags |= 0x80; } return; }
        if (!*current) { *current = storage; *storage = *value; m_dirtyFlags |= 0x80; return; }
        if (!(*storage == *value)) { *storage = *value; m_dirtyFlags |= 0x80; }
    }
};
int main()
{
    CMusicManager m; m.m_dirtyFlags = 0;
    SLoopPoints store = { 0, 0.0f }; SLoopPoints* cur = 0; SLoopPoints v = { 3, 1.5f };
    bool fail = false;
    m.SetLoopPoints(&cur, &store, 0); if (m.m_dirtyFlags != 0) fail = true;
    m.SetLoopPoints(&cur, &store, &v); if (cur != &store || store.m_start != 3 || m.m_dirtyFlags != 0x80) fail = true;
    m.m_dirtyFlags = 0; m.SetLoopPoints(&cur, &store, &v); if (m.m_dirtyFlags != 0) fail = true;
    v.m_length = 2.0f; m.SetLoopPoints(&cur, &store, &v); if (store.m_length != 2.0f || m.m_dirtyFlags != 0x80) fail = true;
    m.m_dirtyFlags = 0; m.SetLoopPoints(&cur, &store, 0); if (cur != 0 || m.m_dirtyFlags != 0x80) fail = true;
    if (fail) { std::printf("00c43420_TEST FAIL\n"); return 1; }
    std::printf("00c43420_TEST PASS\n");
    return 0;
}
